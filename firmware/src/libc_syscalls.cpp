/*
 * This defines the stubs described in the newlib documentation:
 * https://sourceware.org/newlib/libc.html#Stubs
 */
#include <errno.h>
#include <libbase/uart.h>
#include <sys/stat.h>

#include <cstddef>

extern "C" {

// Handle macro and variable definitions
#undef errno
extern int errno;

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

int _close(int file) {
  // We cannot close any file
  return -1;
}

int _fstat(int file, struct stat* st) {
  // Make all files character special devices
  st->st_mode = S_IFCHR;
  return 0;
}

int _getpid() {
  // We don't have processes
  return 1;
}

int _isatty(int file) {
  // Only files are streams
  return 1;
}

int _kill(int pid, int sig) {
  // We don't have processes, so we cannot send signals
  errno = EINVAL;
  return -1;
}

int _lseek(int file, int ptr, int dir) {
  // We cannot seek in io streams
  return 0;
}

int _read(int file, char* ptr, int len) {
  // We don't currently support reading from streams
  return -1;
}

int _write(int file, char* ptr, int len) {
  // Write through UART
  for (int i = 0; i < len; ++i) {
    uart_write(*ptr++);
  }

  return len;
}
}
