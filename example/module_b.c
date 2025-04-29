#include <c_minilib_init.h>
#include <stdio.h>

static int module_b_init(void) {
  puts("Module B initialized");
  return 0;
}

static void module_b_close(void) { puts("Module B closed"); }

CMI_REGISTER(module_b, module_b_init, module_b_close,
             (const char *[]){"module_a"}, 1);
