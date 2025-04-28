#include <errno.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include <c_minilib_mock.h>

#include "c_minilib_init.h"
#include "utils/cmi_common.h"
#include "utils/cmi_dependencies.h"
#include "utils/cmi_error.h"

static struct cmi_Regitrsation *cmi_registrations = NULL;
static cmi_error_t cmi_error = NULL;

static cmi_error_t cmi_init_registration(char *id);
MOCKABLE_STATIC(cmi_error_t cmi_append_registration(
    struct cmi_Regitrsation *registration)) {
  cmi_error_t err;
  if (!registration) {
    err =
        cmi_errorf(EINVAL, "`registration=%p` cannot be NULL\n", registration);
    goto error_out;
  }

  if (!cmi_registrations) {
    cmi_registrations = registration;

  } else {
    registration->next_reg = cmi_registrations;
    cmi_registrations = registration;
  }

  return NULL;

error_out:
  return err;
}

void cmi_register(const char *id, void *init_func, void *close_func,
                  const uint32_t dependencies_length,
                  const char **dependencies) {
  if (cmi_error) {
    return;
  }

  if (!id) {
    cmi_error = cmi_errorf(EINVAL, "`id=%p` cannot be NULL\n", id);
    return;
  }

  struct cmi_Regitrsation *local_registration =
      malloc(sizeof(struct cmi_Regitrsation));
  if (!local_registration) {
    cmi_error =
        cmi_errorf(ENOMEM, "Cannot allocate memory for `local_registration`\n");
    return;
  }

  if (!(local_registration->id = strdup(id))) {
    cmi_error = cmi_errorf(
        ENOMEM, "Cannot allocate memory for `local_registration->id`\n");
    return;
  }

  local_registration->init_func = init_func;
  local_registration->close_func = close_func;
  local_registration->is_initiated = false;

  if ((cmi_error = cmi_dependencies_init(dependencies_length, dependencies,
                                         &local_registration->dependencies))) {
    return;
  }

  if ((cmi_error = cmi_append_registration(local_registration))) {
    return;
  };
};

cmi_error_t cmi_init(void) {
  if (cmi_error) {
    return cmi_error;
  }
  cmi_error_t err;

  struct cmi_Regitrsation *local_registration = cmi_registrations;
  while (local_registration) {
    if ((err = cmi_init_registration(local_registration->id))) {
      return err;
    }
    local_registration = local_registration->next_reg;
  }

  return NULL;
};

void cmi_close(void) {
  if (cmi_error) {
    cmi_error_destroy(&cmi_error);
  }
};

cmi_error_t cmi_init_registration(char *id) {
  struct cmi_Regitrsation *local_registration = cmi_registrations;
  cmi_error_t err;
  int _err;

  while (local_registration) {
    if (strcmp(id, local_registration->id)) {
      if (local_registration->is_initiated) {
        goto stop_loop;
      }

      for (uint32_t i = 0;
           i < local_registration->dependencies.dependencies_length; i++) {
        if ((err = cmi_init_registration(
                 local_registration->dependencies.dependencies[i]))) {
          return err;
        }
      }

      if ((_err = local_registration->init_func())) {
        return cmi_errorf(_err,
                          "Init func failed for `local_registration->id=%s`\n",
                          local_registration->id);
      }

      local_registration->is_initiated = true;
    stop_loop:
      local_registration = NULL;
    } else {
      local_registration = local_registration->next_reg;
    }
  }

  // If registration is missing this means it doesn't have any dependencies
  //    so we can just return NULL as it was succesfully initiated.
  return NULL;
};
