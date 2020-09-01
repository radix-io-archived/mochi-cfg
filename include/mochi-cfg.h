/*
 * (C) 2020 The University of Chicago
 *
 * See COPYRIGHT in top-level directory.
 */

#ifndef __MOCHI_CFG
#define __MOCHI_CFG

#ifdef __cplusplus
extern "C" {
#endif

#include <jansson.h>

char* mochi_cfg_emit(json_t *cfg, const char *component_name);

int mochi_cfg_get_value_int(json_t *component, const char* in_key, int* out_value);

void mochi_cfg_release_component(json_t* component);

#ifdef __cplusplus
}
#endif

#endif /* __MOCHI_CFG */
