#include <generated/csr.h>
#include <generated/soc.h>
#include <irq.h>
#include <libbase/uart.h>

extern "C" void isr(void) {
  unsigned int irqs;

  irqs = irq_pending() & irq_getmask();

  if (irqs & (1 << UART_INTERRUPT)) uart_isr();
}
