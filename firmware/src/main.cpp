#include <irq.h>
#include <libbase/uart.h>

#include <cstdio>
#include <vector>

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
