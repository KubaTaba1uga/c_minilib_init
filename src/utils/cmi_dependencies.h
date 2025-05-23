/*
 * Copyright (c) 2025 Jakub Buczynski <KubaTaba1uga>
 * SPDX-License-Identifier: MIT
 * See LICENSE file in the project root for full license information.
 */

#ifndef C_MINILIB_INIT_DEPENDENCIES_H
#define C_MINILIB_INIT_DEPENDENCIES_H

#include <stdint.h>
#include <stdlib.h>

#include "c_minilib_init.h"

struct cmi_Dependencies {
  uint32_t dependencies_length;
  char **dependencies;
};

cme_error_t cmi_dependencies_init(const uint32_t dependencies_length,
                                  const char **dependencies,
                                  struct cmi_Dependencies *output);

void cmi_dependencies_destroy(struct cmi_Dependencies *output);

#endif // C_MINILIB_INIT_DEPENDENCIES_H
