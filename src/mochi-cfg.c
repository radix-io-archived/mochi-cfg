/*
 * (C) 2020 The University of Chicago
 *
 * See COPYRIGHT in top-level directory.
 */

#include "mochi-cfg-config.h"
#include <assert.h>
#include <jansson.h>
#include "mochi-cfg.h"

static json_t* __mochi_cfg_get_component(json_t* input_json,
    const char* component_name,
    const char* defaults);

char* mochi_cfg_emit(json_t *cfg, const char *component_name)
{
    json_t *emit_cfg;
    char* ret;

    if(component_name)
    {
        emit_cfg = json_object();
        json_object_set(emit_cfg, component_name, cfg);
        ret = json_dumps(emit_cfg, JSON_INDENT(4));
        json_decref(emit_cfg);
    }
    else
    {
        ret = json_dumps(cfg, JSON_INDENT(4));
    }

    return(ret);
}

int mochi_cfg_get_value_string(json_t *component, const char* in_key, const char** out_value)
{
    json_t *val_obj;

    val_obj = json_object_get(component, in_key);
    if(!val_obj)
        return(-1);

    *out_value = json_string_value(val_obj);

    return(0);
}

int mochi_cfg_set_value_string(json_t *component, const char* in_key, const char* in_value)
{
    json_t *val_obj;
    int ret;

    val_obj = json_object_get(component, in_key);
    if(!val_obj)
        return(-1);

    ret = json_string_set(val_obj, in_value);

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

int mochi_cfg_set_value_int(json_t *component, const char* in_key, int in_value)
{
    json_t *val_obj;
    int ret;

    val_obj = json_object_get(component, in_key);
    if(!val_obj)
        return(-1);

    ret = json_integer_set(val_obj, in_value);

    return(ret);
}

int mochi_cfg_get_object(json_t *component, const char* in_key, json_t** out_object)
{
    *out_object = json_object_get(component, in_key);
    if(!*out_object)
        return(-1);

    return(0);
}

int mochi_cfg_append_array_by_string(json_t *component, const char* in_key, const char* obj_string)
{
    json_t *new_json;
    json_t *array;
    json_error_t error;
    int ret;

    new_json = json_loads(obj_string, 0, &error);
    if(!new_json)
    {
        fprintf(stderr, "mochi-cfg error: config line %d: %s\n", error.line, error.text);
        return(-1);
    }
    array = json_object_get(component, in_key);
    if(!array)
    {
        json_decref(new_json);
        fprintf(stderr, "mochi-cfg error: config line %d: %s\n", error.line, error.text);
        return(-1);
    }

    ret = json_array_append(array, new_json);
    return(ret);
}


int mochi_cfg_set_object_by_string(json_t *component, const char* in_key, const char* obj_string)
{
    json_t *new_json;
    json_t *new_object;
    json_error_t error;
    int ret;

    new_json = json_loads(obj_string, 0, &error);
    if(!new_json)
    {
        json_decref(new_json);
        fprintf(stderr, "mochi-cfg error: config line %d: %s\n", error.line, error.text);
        return(-1);
    }

    new_object = json_object_get(new_json, in_key);
    if(!new_object)
    {
        fprintf(stderr, "mochi-cfg error: could not find %s in %s\n", in_key, obj_string);
        return(-1);
    }

    /* update relevant part of component */
    ret = json_object_set(component, in_key, new_object);

    return(ret);
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
    json_t *cfg;
    json_error_t error;

    cfg = json_loads(json_cfg_string, 0, &error);
    if(!cfg)
    {
        fprintf(stderr, "mochi-cfg error: config line %d: %s\n", error.line, error.text);
        return(NULL);
    }

    return(__mochi_cfg_get_component(cfg, component_name, defaults));
}

json_t* mochi_cfg_get_component_file(const char *json_file,
    const char* component_name,
    const char* defaults)
{
    json_t *cfg;
    json_error_t error;

    cfg = json_load_file(json_file, 0, &error);
    if(!cfg)
    {
        fprintf(stderr, "mochi-cfg error: config line %d: %s\n", error.line, error.text);
        return(NULL);
    }

    return(__mochi_cfg_get_component(cfg, component_name, defaults));
}

static json_t* __mochi_cfg_get_component(json_t* cfg,
    const char* component_name,
    const char* defaults)
{
    json_t *cfg_default;
    json_t *component_default;
    json_t *component;
    json_error_t error;
    const char *key_default;
    json_t *value_default;
    json_t *value;

    cfg_default = json_loads(defaults, 0, &error);
    if(!cfg_default)
    {
        json_decref(cfg);
        fprintf(stderr, "mochi-cfg error: default config line %d: %s\n", error.line, error.text);
        return(NULL);
    }
    if(component_name)
    {
        component_default = json_object_get(cfg_default, component_name);
        if(!component_default)
        {
            json_decref(cfg_default);
            json_decref(cfg);
            fprintf(stderr, "mochi-cfg error: default config lacks %s object.\n", component_name);
            return(NULL);
        }
        component = json_object_get(cfg, component_name);
        if(!component)
        {
            json_decref(cfg);
            json_decref(cfg_default);
            fprintf(stderr, "mochi-cfg error: config lacks %s object.\n", component_name);
            return(NULL);
        }
    }
    else
    {
        component_default = cfg_default;
        component = cfg;
    }

    /* TODO: are we going to have to generalize this to arbitrary nesting at
     * some point?  For now just go one nesting level in objects
     */
    /* loop through and set any pairs that aren't already present in config */
    json_object_foreach(component_default, key_default, value_default)
    {
        /* support one level of nesting */
        if(json_is_object(value_default))
        {
            json_t *component_default_nest =
                json_object_get(component_default, key_default);
            json_t *component_nest;
            json_t *value_default_nest;
            json_t *value_nest;
            const char *key_default_nest;
            component_nest = json_object_get(component, key_default);
            if(!component_nest)
            {
                component_nest = json_object();
                json_object_set(component, key_default, component_nest);
                json_decref(component_nest);
            }
            json_object_foreach(component_default_nest, key_default_nest, value_default_nest)
            {
                value_nest = json_object_get(component_nest, key_default_nest);
                if(!value_nest)
                {
                    /* set default in cfg */
                    json_object_set(component_nest, key_default_nest, value_default_nest);
                }
            }
        }
        else
        {
            value = json_object_get(component, key_default);
            if(!value)
            {
                /* set default in cfg */
                json_object_set(component, key_default, value_default);
            }
        }
    }

    /* keep a reference to component, since we are going to pass it out to
     * caller; drop the references to the rest */
    json_incref(component);
    json_decref(cfg);
    json_decref(cfg_default);
    return(component);
}
