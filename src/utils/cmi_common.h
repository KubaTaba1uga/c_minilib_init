#ifndef C_MINILIB_INIT_CMI_COMMON_H
#define C_MINILIB_INIT_CMI_COMMON_H

#include <stdbool.h>

#include "c_minilib_init.h"
#include "utils/cmi_dependencies.h"

struct cmi_Registration {
  char *id;
  bool is_initiated;
  int (*init_func)(void);
  void (*close_func)(void);
  struct cmi_Dependencies dependencies;
  struct cmi_Registration *next_reg;
};

#define CMI_FOREACH_REGISTRATION(var, reg)                                     \
  for (struct cmi_Registration *var = reg; var != NULL; var = var->next_reg)

struct cmi_Settings {
  void (*log_func)(enum cmi_LogLevelEnum log_level, char *msg);
};

#define CMI_LOG(settings, level, fmt, ...)                                     \
  do {                                                                         \
    if ((settings).log_func) {                                                 \
      char _log_buf[1024];                                                     \
      int _written =                                                           \
          snprintf(_log_buf, sizeof(_log_buf), (fmt), ##__VA_ARGS__);          \
      if (_written < 0 || _written >= (int)sizeof(_log_buf)) {                 \
        strncpy(_log_buf, "[log message truncated]", sizeof(_log_buf) - 1);    \
        _log_buf[sizeof(_log_buf) - 1] = '\0';                                 \
      }                                                                        \
      (settings).log_func(level, _log_buf);                                    \
    }                                                                          \
  } while (0)

#endif // C_MINILIB_INIT_CMI_COMMON_H
