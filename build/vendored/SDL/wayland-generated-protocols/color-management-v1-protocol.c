/* Generated by wayland-scanner 1.22.0 */

/*
 * Copyright 2019 Sebastian Wick
 * Copyright 2019 Erwin Burema
 * Copyright 2020 AMD
 * Copyright 2020-2024 Collabora, Ltd.
 * Copyright 2024 Xaver Hugl
 * Copyright 2022-2025 Red Hat, Inc.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice (including the next
 * paragraph) shall be included in all copies or substantial portions of the
 * Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

#include <stdlib.h>
#include <stdint.h>
#include "wayland-util.h"

#ifndef __has_attribute
# define __has_attribute(x) 0  /* Compatibility with non-clang compilers. */
#endif

#if (__has_attribute(visibility) || defined(__GNUC__) && __GNUC__ >= 4)
#define WL_PRIVATE __attribute__ ((visibility("hidden")))
#else
#define WL_PRIVATE
#endif

extern const struct wl_interface wl_output_interface;
extern const struct wl_interface wl_surface_interface;
extern const struct wl_interface wp_color_management_output_v1_interface;
extern const struct wl_interface wp_color_management_surface_feedback_v1_interface;
extern const struct wl_interface wp_color_management_surface_v1_interface;
extern const struct wl_interface wp_image_description_creator_icc_v1_interface;
extern const struct wl_interface wp_image_description_creator_params_v1_interface;
extern const struct wl_interface wp_image_description_info_v1_interface;
extern const struct wl_interface wp_image_description_v1_interface;

static const struct wl_interface *color_management_v1_types[] = {
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	&wp_color_management_output_v1_interface,
	&wl_output_interface,
	&wp_color_management_surface_v1_interface,
	&wl_surface_interface,
	&wp_color_management_surface_feedback_v1_interface,
	&wl_surface_interface,
	&wp_image_description_creator_icc_v1_interface,
	&wp_image_description_creator_params_v1_interface,
	&wp_image_description_v1_interface,
	&wp_image_description_v1_interface,
	&wp_image_description_v1_interface,
	NULL,
	&wp_image_description_v1_interface,
	&wp_image_description_v1_interface,
	&wp_image_description_v1_interface,
	&wp_image_description_v1_interface,
	&wp_image_description_info_v1_interface,
};

static const struct wl_message wp_color_manager_v1_requests[] = {
	{ "destroy", "", color_management_v1_types + 0 },
	{ "get_output", "no", color_management_v1_types + 8 },
	{ "get_surface", "no", color_management_v1_types + 10 },
	{ "get_surface_feedback", "no", color_management_v1_types + 12 },
	{ "create_icc_creator", "n", color_management_v1_types + 14 },
	{ "create_parametric_creator", "n", color_management_v1_types + 15 },
	{ "create_windows_scrgb", "n", color_management_v1_types + 16 },
};

static const struct wl_message wp_color_manager_v1_events[] = {
	{ "supported_intent", "u", color_management_v1_types + 0 },
	{ "supported_feature", "u", color_management_v1_types + 0 },
	{ "supported_tf_named", "u", color_management_v1_types + 0 },
	{ "supported_primaries_named", "u", color_management_v1_types + 0 },
	{ "done", "", color_management_v1_types + 0 },
};

WL_PRIVATE const struct wl_interface wp_color_manager_v1_interface = {
	"wp_color_manager_v1", 1,
	7, wp_color_manager_v1_requests,
	5, wp_color_manager_v1_events,
};

static const struct wl_message wp_color_management_output_v1_requests[] = {
	{ "destroy", "", color_management_v1_types + 0 },
	{ "get_image_description", "n", color_management_v1_types + 17 },
};

static const struct wl_message wp_color_management_output_v1_events[] = {
	{ "image_description_changed", "", color_management_v1_types + 0 },
};

WL_PRIVATE const struct wl_interface wp_color_management_output_v1_interface = {
	"wp_color_management_output_v1", 1,
	2, wp_color_management_output_v1_requests,
	1, wp_color_management_output_v1_events,
};

static const struct wl_message wp_color_management_surface_v1_requests[] = {
	{ "destroy", "", color_management_v1_types + 0 },
	{ "set_image_description", "ou", color_management_v1_types + 18 },
	{ "unset_image_description", "", color_management_v1_types + 0 },
};

WL_PRIVATE const struct wl_interface wp_color_management_surface_v1_interface = {
	"wp_color_management_surface_v1", 1,
	3, wp_color_management_surface_v1_requests,
	0, NULL,
};

static const struct wl_message wp_color_management_surface_feedback_v1_requests[] = {
	{ "destroy", "", color_management_v1_types + 0 },
	{ "get_preferred", "n", color_management_v1_types + 20 },
	{ "get_preferred_parametric", "n", color_management_v1_types + 21 },
};

static const struct wl_message wp_color_management_surface_feedback_v1_events[] = {
	{ "preferred_changed", "u", color_management_v1_types + 0 },
};

WL_PRIVATE const struct wl_interface wp_color_management_surface_feedback_v1_interface = {
	"wp_color_management_surface_feedback_v1", 1,
	3, wp_color_management_surface_feedback_v1_requests,
	1, wp_color_management_surface_feedback_v1_events,
};

static const struct wl_message wp_image_description_creator_icc_v1_requests[] = {
	{ "create", "n", color_management_v1_types + 22 },
	{ "set_icc_file", "huu", color_management_v1_types + 0 },
};

WL_PRIVATE const struct wl_interface wp_image_description_creator_icc_v1_interface = {
	"wp_image_description_creator_icc_v1", 1,
	2, wp_image_description_creator_icc_v1_requests,
	0, NULL,
};

static const struct wl_message wp_image_description_creator_params_v1_requests[] = {
	{ "create", "n", color_management_v1_types + 23 },
	{ "set_tf_named", "u", color_management_v1_types + 0 },
	{ "set_tf_power", "u", color_management_v1_types + 0 },
	{ "set_primaries_named", "u", color_management_v1_types + 0 },
	{ "set_primaries", "iiiiiiii", color_management_v1_types + 0 },
	{ "set_luminances", "uuu", color_management_v1_types + 0 },
	{ "set_mastering_display_primaries", "iiiiiiii", color_management_v1_types + 0 },
	{ "set_mastering_luminance", "uu", color_management_v1_types + 0 },
	{ "set_max_cll", "u", color_management_v1_types + 0 },
	{ "set_max_fall", "u", color_management_v1_types + 0 },
};

WL_PRIVATE const struct wl_interface wp_image_description_creator_params_v1_interface = {
	"wp_image_description_creator_params_v1", 1,
	10, wp_image_description_creator_params_v1_requests,
	0, NULL,
};

static const struct wl_message wp_image_description_v1_requests[] = {
	{ "destroy", "", color_management_v1_types + 0 },
	{ "get_information", "n", color_management_v1_types + 24 },
};

static const struct wl_message wp_image_description_v1_events[] = {
	{ "failed", "us", color_management_v1_types + 0 },
	{ "ready", "u", color_management_v1_types + 0 },
};

WL_PRIVATE const struct wl_interface wp_image_description_v1_interface = {
	"wp_image_description_v1", 1,
	2, wp_image_description_v1_requests,
	2, wp_image_description_v1_events,
};

static const struct wl_message wp_image_description_info_v1_events[] = {
	{ "done", "", color_management_v1_types + 0 },
	{ "icc_file", "hu", color_management_v1_types + 0 },
	{ "primaries", "iiiiiiii", color_management_v1_types + 0 },
	{ "primaries_named", "u", color_management_v1_types + 0 },
	{ "tf_power", "u", color_management_v1_types + 0 },
	{ "tf_named", "u", color_management_v1_types + 0 },
	{ "luminances", "uuu", color_management_v1_types + 0 },
	{ "target_primaries", "iiiiiiii", color_management_v1_types + 0 },
	{ "target_luminance", "uu", color_management_v1_types + 0 },
	{ "target_max_cll", "u", color_management_v1_types + 0 },
	{ "target_max_fall", "u", color_management_v1_types + 0 },
};

WL_PRIVATE const struct wl_interface wp_image_description_info_v1_interface = {
	"wp_image_description_info_v1", 1,
	0, NULL,
	11, wp_image_description_info_v1_events,
};

