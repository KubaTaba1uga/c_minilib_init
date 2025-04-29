#include <errno.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <c_minilib_mock.h>

#include "c_minilib_init.h"
#include "utils/cmi_common.h"
#include "utils/cmi_dependencies.h"
#include "utils/cmi_error.h"

static void cmi_destroy_registration(char *id);
static cmi_error_t cmi_init_registration(char *id);
MOCKABLE_STATIC(
    cmi_error_t cmi_append_registration(struct cmi_Registration *registration));

static struct cmi_Registration *cmi_registrations = NULL;
static struct cmi_Settings cmi_settings = {0};
static cmi_error_t cmi_error = NULL;

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

  struct cmi_Registration *local_registration =
      malloc(sizeof(struct cmi_Registration));
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
  local_registration->next_reg = NULL;

  if ((cmi_error = cmi_dependencies_init(dependencies_length, dependencies,
                                         &local_registration->dependencies))) {
    return;
  }

  if ((cmi_error = cmi_append_registration(local_registration))) {
    return;
  };
};

cmi_error_t cmi_init(void) {
  cmi_error_t err;
  if (cmi_error) {
    err = cmi_error;
    cmi_error = NULL;
    return err;
  }

  struct cmi_Registration *local_registration = cmi_registrations;
  while (local_registration) {
    if ((err = cmi_init_registration(local_registration->id))) {
      return err;
    }
    local_registration = local_registration->next_reg;
  }

  return NULL;
};

void cmi_destroy(void) {
  CMI_FOREACH_REGISTRATION(local_registration, cmi_registrations) {
    cmi_destroy_registration(local_registration->id);
  }

  struct cmi_Registration *local_registration = cmi_registrations;
  cmi_registrations = NULL;
  while (local_registration) {
    struct cmi_Registration *next = local_registration->next_reg;
    cmi_dependencies_destroy(&local_registration->dependencies);
    free(local_registration->id);
    free(local_registration);
    local_registration = next;
  }

  if (cmi_error) {
    cmi_error_destroy(&cmi_error);
  }
};

void cmi_configure(void (*log_func)(enum cmi_LogLevelEnum log_level,
                                    char *msg)) {
  cmi_settings.log_func = log_func;
}

cmi_error_t cmi_init_registration(char *id) {
  cmi_error_t err;
  int _err;

  CMI_FOREACH_REGISTRATION(local_registration, cmi_registrations) {
    if (strcmp(id, local_registration->id) == 0) {
      if (local_registration->is_initiated) {
        break;
      }

      for (uint32_t i = 0;
           i < local_registration->dependencies.dependencies_length; i++) {
        if ((err = cmi_init_registration(
                 local_registration->dependencies.dependencies[i]))) {
          return err;
        }
      }

      if (local_registration->init_func &&
          (_err = local_registration->init_func())) {
        return cmi_errorf(_err,
                          "Init func failed for `local_registration->id=%s`\n",
                          local_registration->id);
      }

      local_registration->is_initiated = true;
      break;
    }
  }

  // If registration is missing this means the package doesn't have any
  // dependencies so we can just return NULL as it was succesfully initiated.
  return NULL;
};

void cmi_destroy_registration(char *id) {
  // How to destroy registration?
  // 1. Find every registration that is using module that needs to be destroyed
  // 2. Destroy every registration from step 1.
  // 3. Destroy this module
  // 4. destroy every modules child
  CMI_LOG(cmi_settings, cmi_LogLevelEnum_DEBUG, "Closing %s\n", id);

  CMI_FOREACH_REGISTRATION(local_registration, cmi_registrations) {
    for (uint32_t i = 0;
         i < local_registration->dependencies.dependencies_length; i++) {
      if (strcmp(id, local_registration->dependencies.dependencies[i]) == 0 &&
          local_registration->is_initiated) {
        cmi_destroy_registration(local_registration->id);
        break;
      }
    }
  }

  CMI_FOREACH_REGISTRATION(local_registration, cmi_registrations) {
    if (strcmp(id, local_registration->id) == 0) {
      if (!local_registration->is_initiated) {
        break;
      }

      if (local_registration->close_func) {
        local_registration->close_func();
      }

      local_registration->is_initiated = false;
      break;
    }
  }

  CMI_LOG(cmi_settings, cmi_LogLevelEnum_DEBUG, "Closed %s\n", id);
}

MOCKABLE_STATIC(cmi_error_t cmi_append_registration(
    struct cmi_Registration *registration)) {
  cmi_error_t err;
  if (!registration) {
    err =
        cmi_errorf(EINVAL, "`registration=%p` cannot be NULL\n", registration);
    goto error_out;
  }

  if (!cmi_registrations) {
    cmi_registrations = registration;

  } else {
    struct cmi_Registration *last_registration = cmi_registrations;
    CMI_FOREACH_REGISTRATION(tmp, last_registration) {
      last_registration = tmp;
    }

    last_registration->next_reg = registration;
  }

  return NULL;

error_out:
  return err;
}
