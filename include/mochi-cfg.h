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

/**
 * Parse configuration for a specific component (possibly out a broader
 * json string) and set defaults for any unspecified parameters
 *
 * @param [in] json_cfg_string   json string containing an object with
 *                               parameters for the specified component
 * @param [in] component_name    name of component (i.e. json object)
 * @param [in] defaults          json string specifying default values for
 *                               component
 *
 * @returns jansson reference with configuration for component
 */
json_t* mochi_cfg_get_component(const char *json_cfg_string,
    const char* component_name,
    const char* defaults);

/**
 * Retrieve integer value for specified parameter within component configuration
 *
 * @param [in] component         jannson reference for component
 *                               configuration, as retrieved with
 *                               mochi_cfg_get_component()
 * @param [in] in_key            name of parameter
 * @param [out] out_value        current value of parameter
 *
 * @returns 0 on success, -1 on failure
 */
int mochi_cfg_get_value_int(json_t *component, const char* in_key, int* out_value);

/**
 * counterpart to mochi_cfg_get_component() that releases the jansson
 * reference for the configuration
 *
 * @param [in] component         component configuration reference
 *
 * no return value
 */
void mochi_cfg_release_component(json_t* component);

/**
 * emits json string encoding of component configuration
 *
 * @param [in] cfg               jansson reference to component configuration
 * @param [in] component_name    component name
 *
 * @returns string that must be free'd by caller
 */
char* mochi_cfg_emit(json_t *cfg, const char *component_name);

#ifdef __cplusplus
}
#endif

#endif /* __MOCHI_CFG */
