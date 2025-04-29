/*
 * Copyright (c) 2025 Jakub Buczynski <KubaTaba1uga>
 * SPDX-License-Identifier: MIT
 * See LICENSE file in the project root for full license information.
 */

#include <errno.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "utils/cmi_dependencies.h"
#include "utils/cmi_error.h"

cmi_error_t cmi_dependencies_init(const uint32_t dependencies_length,
                                  const char **dependencies,
                                  struct cmi_Dependencies *output) {
  char **local_dependencies;
  cmi_error_t err;
  local_dependencies = malloc(sizeof(char *) * dependencies_length);
  if (!local_dependencies) {
    err = cmi_errorf(ENOMEM, "Cannot allocate memory for `dependencies`\n");
    goto error_out;
  }
  uint32_t i;
  for (i = 0; i < dependencies_length; i++) {
    local_dependencies[i] = strdup(dependencies[i]);
    if (!local_dependencies[i]) {
      err = cmi_errorf(
          ENOMEM, "Cannot allocate memory for `local_dependencies[%d]`\n", i);
      goto error_dependencies_cleanup;
    }
  }

  output->dependencies = local_dependencies;
  output->dependencies_length = dependencies_length;

  return NULL;

error_dependencies_cleanup:
  while (i > 0) {
    free(local_dependencies[i--]);
  };
  free(local_dependencies);
error_out:
  return err;
}

void cmi_dependencies_destroy(struct cmi_Dependencies *output) {
  while (output->dependencies_length > 0) {
    output->dependencies_length--;
    free(output->dependencies[output->dependencies_length]);
  };

  free(output->dependencies);
  output->dependencies = NULL;
}
