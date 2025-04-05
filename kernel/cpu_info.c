#include "cpu_info.h"

#include "terminal.h"

void cpuid(uint32_t eax, uint32_t *a, uint32_t *b, uint32_t *c, uint32_t *d) {
  __asm__ volatile("cpuid" : "=a"(*a), "=b"(*b), "=c"(*c), "=d"(*d) : "a"(eax));
}

void detect_cpu() {
  uint32_t eax, ebx, ecx, edx;

  // Vendor ID
  char vendor_id[13] = {0};
  cpuid(0, &eax, &ebx, &ecx, &edx);
  *(uint32_t *)&vendor_id[0] = ebx;
  *(uint32_t *)&vendor_id[4] = edx;
  *(uint32_t *)&vendor_id[8] = ecx;
  terminal_printf("Vendor ID: %s\n", vendor_id);

  // Brand String (if supported)
  uint32_t max_extended;
  cpuid(0x80000000, &max_extended, &ebx, &ecx, &edx);
  if (max_extended >= 0x80000004) {
    char brand[49] = {0};
    cpuid(0x80000002, (uint32_t *)&brand[0], (uint32_t *)&brand[4],
          (uint32_t *)&brand[8], (uint32_t *)&brand[12]);
    cpuid(0x80000003, (uint32_t *)&brand[16], (uint32_t *)&brand[20],
          (uint32_t *)&brand[24], (uint32_t *)&brand[28]);
    cpuid(0x80000004, (uint32_t *)&brand[32], (uint32_t *)&brand[36],
          (uint32_t *)&brand[40], (uint32_t *)&brand[44]);
    terminal_printf("Brand: %s\n", brand);
  }

  // Family/Model/Stepping
  cpuid(1, &eax, &ebx, &ecx, &edx);
  uint8_t stepping = eax & 0xF;
  uint8_t model = (eax >> 4) & 0xF;
  uint8_t family = (eax >> 8) & 0xF;
  if (family == 0xF) {
    family += (eax >> 20) & 0xFF;
    model |= (eax >> 12) & 0xF0;
  }
  terminal_printf("Family: 0x%x, Model: 0x%x, Stepping: 0x%x\n", family, model,
                  stepping);
}
