#include <cstddef>

extern "C" {

// Defined in linker script
extern char _end;

void* _sbrk(int incr) {
  // Set up heap pointer
  static std::byte* heap = nullptr;
  if (heap == nullptr) {
    heap = reinterpret_cast<std::byte*>(&_end);
  }

  // Perform allocation
  std::byte* const cur_heap = heap;
  heap += incr;

  return cur_heap;
}
}
