#ifndef C_MINILIB_INIT_H
#define C_MINILIB_INIT_H

#include <c_minilib_error.h>
#include <stdbool.h>
#include <stdint.h>

typedef struct cme_Error *cmi_error_t;

cmi_error_t cmi_init(void);
void cmi_close(void);

void cmi_register(char *id, void *init_func, void *close_func,
                  uint32_t dependencies_length, char **dependencies);

#define CMI_REGITSER(id, init_func, close_func, dependencies,                  \
                     dependencies_length)                                      \
  void __attribute__((constructor)) id##registration(void) {                   \
    cmi_register(id, init_func, close_func, dependencies,                      \
                 dependencies_length);                                         \
  }

#endif // C_MINILIB_INIT_H
