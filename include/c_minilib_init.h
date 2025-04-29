/*
 * Copyright (c) 2025 Jakub Buczynski <KubaTaba1uga>
 * SPDX-License-Identifier: MIT
 * See LICENSE file in the project root for full license information.
 */

#ifndef C_MINILIB_INIT_H
#define C_MINILIB_INIT_H

#include <c_minilib_error.h>
#include <stdbool.h>
#include <stdint.h>

typedef struct cme_Error *cmi_error_t;

static inline void cmi_error_destroy(cmi_error_t *error) {
  cme_error_destroy((struct cme_Error *)*error);
}

cmi_error_t cmi_init(void);
void cmi_destroy(void);

void cmi_register(const char *id, void *init_func, void *close_func,
                  const uint32_t dependencies_length,
                  const char **dependencies);

#define CMI_REGISTER(id, init_func, close_func, dependencies,                  \
                     dependencies_length)                                      \
  void __attribute__((constructor)) id##_registration(void) {                  \
    cmi_register(#id, init_func, close_func, dependencies_length,              \
                 dependencies);                                                \
  }

/**
 * Logging verbosity levels.
 */
enum cmi_LogLevelEnum {
  cmi_LogLevelEnum_ERROR,
  cmi_LogLevelEnum_WARNING,
  cmi_LogLevelEnum_INFO,
  cmi_LogLevelEnum_DEBUG,
};

// Configuration function runs always first
void cmi_configure(void (*log_func)(enum cmi_LogLevelEnum log_level,
                                    char *msg));
#define CMI_CONFIGURE(log_func)                                                \
  void __attribute__((constructor(0))) configure_cmi(void) {                   \
    cmi_configure(log_func);                                                   \
  }

#endif // C_MINILIB_INIT_H
