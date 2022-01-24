#include <irq.h>
#include <libbase/uart.h>

#include <cstddef>
#include <cstdio>
#include <vector>

extern char _end;

extern "C" void* _sbrk(int incr) {
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

extern "C" int main(int argc, char* argv[]) {
  irq_setmask(0);
  irq_setie(1);

  uart_init();

  std::vector<char> test_vector;
  for (char c = 'a'; c <= 'z'; ++c) {
    test_vector.push_back(c);
  }

  while (1) {
    for (char c : test_vector) {
      uart_write(c);
      busy_wait(1000);
    }
    uart_write('\n');
  }
  return 0;
}
