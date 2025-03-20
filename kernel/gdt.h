#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// Define a GDT entry (8 bytes)
struct gdt_entry {
    uint16_t limit_low;     // Lower 16 bits of limit
    uint16_t base_low;      // Lower 16 bits of base
    uint8_t base_middle;    // Next 8 bits of base
    uint8_t access;         // Access flags (e.g., executable, ring level)
    uint8_t flags_limit_hi; // Flags (4 bits) + Upper 4 bits of limit
    uint8_t base_high;      // Upper 8 bits of base
} __attribute__((packed));

// Define the GDT with 3 entries: null, code, data
struct gdt_entry gdt[3] = {
    // Null descriptor (mandatory)
    {0, 0, 0, 0, 0, 0},

    // Code segment descriptor (0x08)
    {.limit_low = 0xFFFF,
     .base_low = 0x0000,
     .base_middle = 0x00,
     .access = 0b10011010,         // P=1, DPL=0, code, readable
     .flags_limit_hi = 0b11001111, // G=1, D/B=1, limit_hi=0xF
     .base_high = 0x00},

    // Data segment descriptor (0x10)
    {.limit_low = 0xFFFF,
     .base_low = 0x0000,
     .base_middle = 0x00,
     .access = 0b10010010,         // P=1, DPL=0, data, writable
     .flags_limit_hi = 0b11001111, // G=1, D/B=1, limit_hi=0xF
     .base_high = 0x00}};

// GDT descriptor (for LGDT instruction)
struct gdt_descriptor {
    uint16_t size;
    uint32_t offset;
} __attribute__((packed));

struct gdt_descriptor gdt_desc = {.size = sizeof(gdt) - 1,
                                  .offset = (uint32_t)&gdt};

void gdt_load() {
    // Load the GDT descriptor
    asm volatile("lgdt %0" : : "m"(gdt_desc));

    // Reload code segment register (CS) with a far jump
    asm volatile("ljmp $0x08, $reload_cs\n\t" // 0x08 = code segment selector
                 "reload_cs:\n\t"
                 "mov $0x10, %%eax\n\t" // 0x10 = data segment selector
                 "mov %%eax, %%ds\n\t"
                 "mov %%eax, %%es\n\t"
                 "mov %%eax, %%fs\n\t"
                 "mov %%eax, %%gs\n\t"
                 "mov %%eax, %%ss\n\t" ::
                     : "eax");
}

#ifdef __cplusplus
}
#endif
