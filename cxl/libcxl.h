/* SPDX-License-Identifier: LGPL-2.1 */
/* Copyright (C) 2020-2021, Intel Corporation. All rights reserved. */
#ifndef _LIBCXL_H_
#define _LIBCXL_H_

#include <stdarg.h>
#include <unistd.h>

#ifdef HAVE_UUID
#include <uuid/uuid.h>
#else
typedef unsigned char uuid_t[16];
#endif

#ifdef __cplusplus
extern "C" {
#endif

struct cxl_ctx;
struct cxl_ctx *cxl_ref(struct cxl_ctx *ctx);
void cxl_unref(struct cxl_ctx *ctx);
int cxl_new(struct cxl_ctx **ctx);
void cxl_set_log_fn(struct cxl_ctx *ctx,
		void (*log_fn)(struct cxl_ctx *ctx, int priority,
			const char *file, int line, const char *fn,
			const char *format, va_list args));
int cxl_get_log_priority(struct cxl_ctx *ctx);
void cxl_set_log_priority(struct cxl_ctx *ctx, int priority);
void cxl_set_userdata(struct cxl_ctx *ctx, void *userdata);
void *cxl_get_userdata(struct cxl_ctx *ctx);

struct cxl_memdev;
struct cxl_memdev *cxl_memdev_get_first(struct cxl_ctx *ctx);
struct cxl_memdev *cxl_memdev_get_next(struct cxl_memdev *memdev);
int cxl_memdev_get_id(struct cxl_memdev *memdev);
const char *cxl_memdev_get_devname(struct cxl_memdev *memdev);
int cxl_memdev_get_major(struct cxl_memdev *memdev);
int cxl_memdev_get_minor(struct cxl_memdev *memdev);
struct cxl_ctx *cxl_memdev_get_ctx(struct cxl_memdev *memdev);
unsigned long long cxl_memdev_get_pmem_size(struct cxl_memdev *memdev);
unsigned long long cxl_memdev_get_ram_size(struct cxl_memdev *memdev);

#define cxl_memdev_foreach(ctx, memdev) \
        for (memdev = cxl_memdev_get_first(ctx); \
             memdev != NULL; \
             memdev = cxl_memdev_get_next(memdev))

struct cxl_cmd;
struct cxl_cmd *cxl_cmd_new_identify(struct cxl_memdev *memdev);
struct cxl_cmd *cxl_cmd_new_raw(struct cxl_memdev *memdev);
struct cxl_cmd *cxl_cmd_new_get_supported_logs(struct cxl_memdev *memdev);
struct cxl_cmd *cxl_cmd_new_get_log(struct cxl_memdev *memdev);
int cxl_cmd_attach_payloads(struct cxl_cmd *cmd,
		void *in, int size_in, void *out, int size_out);
void cxl_cmd_ref(struct cxl_cmd *cmd);
void cxl_cmd_unref(struct cxl_cmd *cmd);
int cxl_cmd_submit(struct cxl_cmd *cmd);
int cxl_cmd_identify_get_fw_rev(struct cxl_cmd *cmd, char *fw_rev, int fw_len);
unsigned long long cxl_cmd_identify_get_partition_align(struct cxl_cmd *cmd);
unsigned int cxl_cmd_identify_get_lsa_size(struct cxl_cmd *cmd);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif
