/*
 * Copyright (c) 2025 Jakub Buczynski <KubaTaba1uga>
 * SPDX-License-Identifier: MIT
 * See LICENSE file in the project root for full license information.
 */

#include <c_minilib_init.h>
#include <stdio.h>

static int module_a_init(void) {
  puts("Module A initialized");
  return 0;
}

static void module_a_close(void) { puts("Module A closed"); }

CMI_REGISTER(module_a, module_a_init, module_a_close, NULL, 0);
