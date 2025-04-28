/*
 * Copyright (c) 2025 Jakub Buczynski <KubaTaba1uga>
 * SPDX-License-Identifier: MIT
 * See LICENSE file in the project root for full license information.
 */

#ifndef C_MINILIB_INIT_CMC_ERROR_H
#define C_MINILIB_INIT_CMC_ERROR_H

#include <c_minilib_error.h>
#include <c_minilib_init.h>

#define cmi_errorf(...) (cmi_error_t) cme_errorf(__VA_ARGS__)

#endif // C_MINILIB_INIT_CMC_ERROR_H
