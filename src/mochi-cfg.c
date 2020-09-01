/*
 * (C) 2020 The University of Chicago
 *
 * See COPYRIGHT in top-level directory.
 */

#include "mochi-cfg-config.h"
#include <assert.h>
#include <jansson.h>
#include "mochi-cfg.h"

char* mochi_cfg_emit(json_t *cfg, const char *component_name)
{
    json_t *emit_cfg;
    char* ret;

    assert(component_name);

    emit_cfg = json_object();
    json_object_set(emit_cfg, component_name, cfg);
    ret = json_dumps(emit_cfg, JSON_INDENT(4));
    json_decref(emit_cfg);

    return(ret);
}

int mochi_cfg_get_value_int(json_t *component, const char* in_key, int* out_value)
{
    json_t *val_obj;

    val_obj = json_object_get(component, in_key);
    if(!val_obj)
        return(-1);

    *out_value = json_integer_value(val_obj);

    return(0);
}

void mochi_cfg_release_component(json_t* component)
{
    json_decref(component);
    return;
}

json_t* mochi_cfg_get_component(const char *json_cfg_string,
    const char* component_name,
    const char* defaults)
{
    json_t *cfg_default;
    json_t *cfg;
    json_t *component_default;
    json_t *component;
    json_error_t error;
    const char *key_default;
    json_t *value_default;
    json_t *value;

    cfg = json_loads(json_cfg_string, 0, &error);
    if(!cfg)
    {
        fprintf(stderr, "Error: config line %d: %s\n", error.line, error.text);
        return(NULL);
    }
    cfg_default = json_loads(defaults, 0, &error);
    if(!cfg_default)
    {
        json_decref(cfg);
        fprintf(stderr, "Error: default config line %d: %s\n", error.line, error.text);
        return(NULL);
    }
    component_default = json_object_get(cfg_default, "abt-io");
    if(!component_default)
    {
        json_decref(cfg_default);
        json_decref(cfg);
        fprintf(stderr, "Error: default config lacks abt-io object.\n");
        return(NULL);
    }
    component = json_object_get(cfg, "abt-io");
    if(!component)
    {
        json_decref(cfg);
        json_decref(cfg_default);
        fprintf(stderr, "Error: config lacks abt-io object.\n");
        return(NULL);
    }

    /* loop through and set any pairs that aren't already present in config */
    json_object_foreach(component_default, key_default, value_default)
    {
        value = json_object_get(component, key_default);
        if(!value)
        {
            /* set default in cfg */
            json_object_set(component, key_default, value_default);
        }
    }

    /* keep a reference to component, since we are going to pass it out to
     * caller; drop the references to the rest */
    json_incref(component);
    json_decref(cfg);
    json_decref(cfg_default);
    return(component);
}
