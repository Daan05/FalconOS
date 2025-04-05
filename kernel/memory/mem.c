#include "mem.h"
#include "../terminal.h"
#include <limine.h>

__attribute__((
    used,
    section(".limine_requests"))) static volatile struct limine_memmap_request
    memmap_request = {.id = LIMINE_MEMMAP_REQUEST, .revision = 0};

uint64_t get_total_usable_memory() {

  // Ensure the memory map response is available
  if (memmap_request.response == NULL) {
    terminal_printf("memmap response == NULL\n");
    return 0; // Memory map not available
  }

  uint64_t total_usable_memory = 0;
  struct limine_memmap_response *memmap_response = memmap_request.response;

  // Iterate over each memory map entry
  terminal_printf("number of memmap entries: %d\n",
                  memmap_response->entry_count);
  for (size_t i = 0; i < memmap_response->entry_count; i++) {
    struct limine_memmap_entry *entry = memmap_response->entries[i];

    // Check if the entry is of type usable
    if (entry->type == LIMINE_MEMMAP_USABLE) {
      total_usable_memory += entry->length;
    }
  }

  return total_usable_memory;
}
