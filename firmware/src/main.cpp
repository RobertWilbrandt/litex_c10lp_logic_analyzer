#include <irq.h>
#include <libbase/uart.h>

#include <cstddef>
#include <cstdio>

class Test {
 public:
  Test(char c) { uart_write(c); }
  ~Test() { uart_write('\n'); }

 private:
  char cur_c_;
};

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

  char* c1 = new char{'x'};
  char* c2 = new char{'y'};
  uart_write(*c1);
  uart_write(*c2);

  char c = 'a';
  while (1) {
    Test test{c};

    if (c == 'z') {
      c = 'a';
    } else {
      c++;
    }

    busy_wait(1000);
  }
  return 0;
}
