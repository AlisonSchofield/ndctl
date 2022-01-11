// SPDX-License-Identifier: GPL-2.0
// Copyright (C) 2015-2020 Intel Corporation. All rights reserved.
#include <limits.h>
#include <util/json.h>
#include <uuid/uuid.h>
#include <cxl/libcxl.h>
#include <json-c/json.h>
#include <json-c/printbuf.h>
#include <ccan/short_types/short_types.h>

#include "json.h"

static struct json_object *util_cxl_memdev_health_to_json(
		struct cxl_memdev *memdev, unsigned long flags)
{
	struct json_object *jhealth;
	struct json_object *jobj;
	struct cxl_cmd *cmd;
	u32 field;
	int rc;

	jhealth = json_object_new_object();
	if (!jhealth)
		return NULL;
	if (!memdev)
		goto err_jobj;

	cmd = cxl_cmd_new_get_health_info(memdev);
	if (!cmd)
		goto err_jobj;

	rc = cxl_cmd_submit(cmd);
	if (rc < 0)
		goto err_cmd;
	rc = cxl_cmd_get_mbox_status(cmd);
	if (rc != 0)
		goto err_cmd;

	/* health_status fields */
	rc = cxl_cmd_health_info_get_maintenance_needed(cmd);
	jobj = json_object_new_boolean(rc);
	if (jobj)
		json_object_object_add(jhealth, "maintenance_needed", jobj);

	rc = cxl_cmd_health_info_get_performance_degraded(cmd);
	jobj = json_object_new_boolean(rc);
	if (jobj)
		json_object_object_add(jhealth, "performance_degraded", jobj);

	rc = cxl_cmd_health_info_get_hw_replacement_needed(cmd);
	jobj = json_object_new_boolean(rc);
	if (jobj)
		json_object_object_add(jhealth, "hw_replacement_needed", jobj);

	/* media_status fields */
	rc = cxl_cmd_health_info_get_media_normal(cmd);
	jobj = json_object_new_boolean(rc);
	if (jobj)
		json_object_object_add(jhealth, "media_normal", jobj);

	rc = cxl_cmd_health_info_get_media_not_ready(cmd);
	jobj = json_object_new_boolean(rc);
	if (jobj)
		json_object_object_add(jhealth, "media_not_ready", jobj);

	rc = cxl_cmd_health_info_get_media_persistence_lost(cmd);
	jobj = json_object_new_boolean(rc);
	if (jobj)
		json_object_object_add(jhealth, "media_persistence_lost", jobj);

	rc = cxl_cmd_health_info_get_media_data_lost(cmd);
	jobj = json_object_new_boolean(rc);
	if (jobj)
		json_object_object_add(jhealth, "media_data_lost", jobj);

	rc = cxl_cmd_health_info_get_media_powerloss_persistence_loss(cmd);
	jobj = json_object_new_boolean(rc);
	if (jobj)
		json_object_object_add(jhealth, "media_powerloss_persistence_loss", jobj);

	rc = cxl_cmd_health_info_get_media_shutdown_persistence_loss(cmd);
	jobj = json_object_new_boolean(rc);
	if (jobj)
		json_object_object_add(jhealth, "media_shutdown_persistence_loss", jobj);

	rc = cxl_cmd_health_info_get_media_persistence_loss_imminent(cmd);
	jobj = json_object_new_boolean(rc);
	if (jobj)
		json_object_object_add(jhealth, "media_persistence_loss_imminent", jobj);

	rc = cxl_cmd_health_info_get_media_powerloss_data_loss(cmd);
	jobj = json_object_new_boolean(rc);
	if (jobj)
		json_object_object_add(jhealth, "media_powerloss_data_loss", jobj);

	rc = cxl_cmd_health_info_get_media_shutdown_data_loss(cmd);
	jobj = json_object_new_boolean(rc);
	if (jobj)
		json_object_object_add(jhealth, "media_shutdown_data_loss", jobj);

	rc = cxl_cmd_health_info_get_media_data_loss_imminent(cmd);
	jobj = json_object_new_boolean(rc);
	if (jobj)
		json_object_object_add(jhealth, "media_data_loss_imminent", jobj);

	/* ext_status fields */
	if (cxl_cmd_health_info_get_ext_life_used_normal(cmd))
		jobj = json_object_new_string("normal");
	else if (cxl_cmd_health_info_get_ext_life_used_warning(cmd))
		jobj = json_object_new_string("warning");
	else if (cxl_cmd_health_info_get_ext_life_used_critical(cmd))
		jobj = json_object_new_string("critical");
	else
		jobj = json_object_new_string("unknown");
	if (jobj)
		json_object_object_add(jhealth, "ext_life_used", jobj);

	if (cxl_cmd_health_info_get_ext_temperature_normal(cmd))
		jobj = json_object_new_string("normal");
	else if (cxl_cmd_health_info_get_ext_temperature_warning(cmd))
		jobj = json_object_new_string("warning");
	else if (cxl_cmd_health_info_get_ext_temperature_critical(cmd))
		jobj = json_object_new_string("critical");
	else
		jobj = json_object_new_string("unknown");
	if (jobj)
		json_object_object_add(jhealth, "ext_temperature", jobj);

	if (cxl_cmd_health_info_get_ext_corrected_volatile_normal(cmd))
		jobj = json_object_new_string("normal");
	else if (cxl_cmd_health_info_get_ext_corrected_volatile_warning(cmd))
		jobj = json_object_new_string("warning");
	else
		jobj = json_object_new_string("unknown");
	if (jobj)
		json_object_object_add(jhealth, "ext_corrected_volatile", jobj);

	if (cxl_cmd_health_info_get_ext_corrected_persistent_normal(cmd))
		jobj = json_object_new_string("normal");
	else if (cxl_cmd_health_info_get_ext_corrected_persistent_warning(cmd))
		jobj = json_object_new_string("warning");
	else
		jobj = json_object_new_string("unknown");
	if (jobj)
		json_object_object_add(jhealth, "ext_corrected_persistent", jobj);

	/* other fields */
	field = cxl_cmd_health_info_get_life_used(cmd);
	if (field != 0xff) {
		jobj = json_object_new_int(field);
		if (jobj)
			json_object_object_add(jhealth, "life_used_percent", jobj);
	}

	field = cxl_cmd_health_info_get_temperature(cmd);
	if (field != 0xffff) {
		jobj = json_object_new_int(field);
		if (jobj)
			json_object_object_add(jhealth, "temperature", jobj);
	}

	field = cxl_cmd_health_info_get_dirty_shutdowns(cmd);
	jobj = json_object_new_int64(field);
	if (jobj)
		json_object_object_add(jhealth, "dirty_shutdowns", jobj);

	field = cxl_cmd_health_info_get_volatile_errors(cmd);
	jobj = json_object_new_int64(field);
	if (jobj)
		json_object_object_add(jhealth, "volatile_errors", jobj);

	field = cxl_cmd_health_info_get_pmem_errors(cmd);
	jobj = json_object_new_int64(field);
	if (jobj)
		json_object_object_add(jhealth, "pmem_errors", jobj);

	cxl_cmd_unref(cmd);
	return jhealth;

err_cmd:
	cxl_cmd_unref(cmd);
err_jobj:
	json_object_put(jhealth);
	return NULL;
}

/*
 * Present complete view of memdev partition by presenting fields from
 * both GET_PARTITION_INFO and IDENTIFY mailbox commands.
 */
static struct json_object *util_cxl_memdev_partition_to_json(struct cxl_memdev *memdev,
		unsigned long flags)
{
	struct json_object *jobj = NULL;
	struct json_object *jpart;
	unsigned long long cap;
	struct cxl_cmd *cmd;
	int rc;

	jpart = json_object_new_object();
	if (!jpart)
		return NULL;
	if (!memdev)
		goto err_jobj;

	/* Retrieve partition info in GET_PARTITION_INFO mbox cmd */
	cmd = cxl_cmd_new_get_partition_info(memdev);
	if (!cmd)
		goto err_jobj;

	rc = cxl_cmd_submit(cmd);
	if (rc < 0)
		goto err_cmd;
	rc = cxl_cmd_get_mbox_status(cmd);
	if (rc != 0)
		goto err_cmd;

	cap = cxl_cmd_partition_info_get_active_volatile_bytes(cmd);
	if (cap != ULLONG_MAX) {
		jobj = util_json_object_size(cap, flags);
		if (jobj)
			json_object_object_add(jpart,
					"active_volatile_bytes", jobj);
	}
	cap = cxl_cmd_partition_info_get_active_persistent_bytes(cmd);
	if (cap != ULLONG_MAX) {
		jobj = util_json_object_size(cap, flags);
		if (jobj)
			json_object_object_add(jpart,
					"active_persistent_bytes", jobj);
	}
	cap = cxl_cmd_partition_info_get_next_volatile_bytes(cmd);
	if (cap != ULLONG_MAX) {
		jobj = util_json_object_size(cap, flags);
		if (jobj)
			json_object_object_add(jpart,
					"next_volatile_bytes", jobj);
	}
	cap = cxl_cmd_partition_info_get_next_persistent_bytes(cmd);
	if (cap != ULLONG_MAX) {
		jobj = util_json_object_size(cap, flags);
		if (jobj)
			json_object_object_add(jpart,
					"next_persistent_bytes", jobj);
	}
	cxl_cmd_unref(cmd);

	/* Retrieve partition info in the IDENTIFY mbox cmd */
	cmd = cxl_cmd_new_identify(memdev);
	if (!cmd)
		goto err_jobj;

	rc = cxl_cmd_submit(cmd);
	if (rc < 0)
		goto err_cmd;
	rc = cxl_cmd_get_mbox_status(cmd);
	if (rc != 0)
		goto err_cmd;

	cap = cxl_cmd_identify_get_total_bytes(cmd);
	if (cap != ULLONG_MAX) {
		jobj = util_json_object_size(cap, flags);
		if (jobj)
			json_object_object_add(jpart, "total_bytes", jobj);
	}
	cap = cxl_cmd_identify_get_volatile_only_bytes(cmd);
	if (cap != ULLONG_MAX) {
		jobj = util_json_object_size(cap, flags);
		if (jobj)
			json_object_object_add(jpart,
					"volatile_only_bytes", jobj);
	}
	cap = cxl_cmd_identify_get_persistent_only_bytes(cmd);
	if (cap != ULLONG_MAX) {
		jobj = util_json_object_size(cap, flags);
		if (jobj)
			json_object_object_add(jpart,
					"persistent_only_bytes", jobj);
	}
	cap = cxl_cmd_identify_get_partition_align_bytes(cmd);
	jobj = util_json_object_size(cap, flags);
	if (jobj)
		json_object_object_add(jpart, "partition_alignment_bytes", jobj);

	cxl_cmd_unref(cmd);
	return jpart;

err_cmd:
	cxl_cmd_unref(cmd);
err_jobj:
	json_object_put(jpart);
	return NULL;
}

struct json_object *util_cxl_memdev_to_json(struct cxl_memdev *memdev,
		unsigned long flags)
{
	const char *devname = cxl_memdev_get_devname(memdev);
	struct json_object *jdev, *jobj;

	jdev = json_object_new_object();
	if (!devname || !jdev)
		return NULL;

	jobj = json_object_new_string(devname);
	if (jobj)
		json_object_object_add(jdev, "memdev", jobj);

	jobj = util_json_object_size(cxl_memdev_get_pmem_size(memdev), flags);
	if (jobj)
		json_object_object_add(jdev, "pmem_size", jobj);

	jobj = util_json_object_size(cxl_memdev_get_ram_size(memdev), flags);
	if (jobj)
		json_object_object_add(jdev, "ram_size", jobj);

	if (flags & UTIL_JSON_HEALTH) {
		jobj = util_cxl_memdev_health_to_json(memdev, flags);
		if (jobj)
			json_object_object_add(jdev, "health", jobj);
	}
	if (flags & UTIL_JSON_PARTITION) {
		jobj = util_cxl_memdev_partition_to_json(memdev, flags);
		if (jobj)
			json_object_object_add(jdev, "partition_info", jobj);
	}
	return jdev;
}
