/*
 * Copyright (c) 2025 Jakub Buczynski <KubaTaba1uga>
 * SPDX-License-Identifier: MIT
 * See LICENSE file in the project root for full license information.
 */

#include <c_minilib_init.h>
#include <stdio.h>

static int module_b_init(void) {
  puts("Module B initialized");
  return 0;
}

static void module_b_close(void) { puts("Module B closed"); }

CMI_REGISTER(module_b, module_b_init, module_b_close,
             (const char *[]){"module_a"}, 1);
