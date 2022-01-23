#include <irq.h>
#include <libbase/uart.h>

#include <cstdio>

class Test {
 public:
  Test(char c) { uart_write(c); }
  ~Test() { uart_write('\n'); }

 private:
  char cur_c_;
};

extern "C" void* _sbrk(int incr) { return 0; }

extern "C" int main(int argc, char* argv[]) {
  irq_setmask(0);
  irq_setie(1);

  uart_init();

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
