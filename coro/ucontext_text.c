#include <ucontext.h>
#include <stdio.h>
#include <string.h>

struct ucontext_t test;
char stack[102400];
int n = 0;

void testv() {
  printf("Hello World!\n");
}

int test4() {
  getcontext(&test);
  printf("Test\n");
  if (n == 0) {
    test.uc_stack.ss_sp = stack;
    test.uc_stack.ss_size = 102400;
    makecontext(&test, testv, 0);
    n = 1;
    setcontext(&test);
  }
  return 0;
}

int main() {
  test4();
  return 0;
}
