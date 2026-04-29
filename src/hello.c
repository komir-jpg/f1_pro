#include "hello.h"

#include <stdio.h>

int sum(int a, int b) { return a + b; }

void hello(const char* name) {
  printf("Welcome %s this was built with scikit\n", name);
}
