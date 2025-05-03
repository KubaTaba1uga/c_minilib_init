/*
 * Copyright (c) 2025 Jakub Buczynski <KubaTaba1uga>
 * SPDX-License-Identifier: MIT
 * See LICENSE file in the project root for full license information.
 */

#include "c_minilib_error.h"
#include <c_minilib_init.h>
#include <stdio.h>

int main(void) {
  cme_error_t err;

  if ((err = cmi_init())) {
    printf("Error dumped to error.txt: %s\n", err->msg);
    cme_error_dump_to_file(err, "error.txt");
    cmi_destroy();
    return 1;
  }

  puts("App is working...");

  cmi_destroy();

  return 0;
}
