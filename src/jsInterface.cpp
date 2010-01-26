/*
 * jsInterface.cpp - 
 * Last Modified: January 26, 2010
 * Copyright (C) Arun Tejasvi Chaganty 2009 <arunchaganty@gmail.com>
 */
/**
 * @file jsInterface.cpp
 * @author Arun Tejasvi Chaganty <arunchaganty@gmail.com>
 * @version 0.1
 * 
 * @section Description
 * Javascript API for jsNetTools
 * Contains all Javascript API functions exposed by the plugin.
 */

#include "jsInterface.h"
#include "NPN.h"
#include "util.h"
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <regex.h>

#define CMD_BUF_SIZE    500

extern NPNetscapeFuncs* gBrowserFuncs;

/* Internal functions for the JS object */
static NPObject* jsInterface_Allocate (NPP npp, NPClass *aClass);
static void jsInterface_Deallocate (NPObject *npobj);
static void jsInterface_Invalidate (NPObject *npobj);
static bool jsInterface_HasMethod (NPObject *npobj, NPIdentifier name);
static bool jsInterface_Invoke (NPObject *npobj, NPIdentifier name,
        const NPVariant *args, uint32_t argCount,
        NPVariant *result);
static bool jsInterface_InvokeDefault (NPObject *npobj,
        const NPVariant *args,
        uint32_t argCount,
        NPVariant *result);
static bool jsInterface_HasProperty (NPObject *npobj, NPIdentifier name);
static bool jsInterface_GetProperty (NPObject *npobj, NPIdentifier name,
        NPVariant *result);
static bool jsInterface_SetProperty (NPObject *npobj, NPIdentifier name,
        const NPVariant *value);
static bool jsInterface_RemoveProperty (NPObject *npobj,
        NPIdentifier name);
static bool jsInterface_Enumeration (NPObject *npobj, NPIdentifier **value,
        uint32_t *count);
static bool jsInterface_Construct (NPObject *npobj,
        const NPVariant *args,
        uint32_t argCount,
        NPVariant *result);

/* jsNetTools API functions */
/*
 * To add a new API function, add the name and function to the
 * jsInterfaceMethodNames and jsInterfaceMethodImpl arrays respectively.
 */

typedef bool (*ScriptableFunc) (const NPVariant *args, uint32_t argCount, NPVariant *result);
static bool jsNetTool_Ping (const NPVariant *args, uint32_t argCount, NPVariant *result);

static const NPUTF8* jsInterfaceMethodNames[] = 
{
    "Ping",
};

static const ScriptableFunc
jsInterfaceMethodImpl[] = 
{
    jsNetTool_Ping,
};
#define JS_INTERFACE_METHOD_COUNT ARR_LEN(jsInterfaceMethodImpl)

static const NPUTF8* 
jsInterfacePropertyNames[] = 
{
};
#define JS_INTERFACE_PROPERTY_COUNT ARR_LEN(jsInterfacePropertyNames)

/**
 * Definition of the jsNetToolsClass
 * This object contains function pointers to all the JS object properties
 */
static NPClass jsNetToolsClass =
{
    NP_CLASS_STRUCT_VERSION_ENUM,   // structVersion
    NULL,                           // allocate
    NULL,                           // deallocate
    jsInterface_Invalidate,         // invalidate
    jsInterface_HasMethod,          // hasMethod
    jsInterface_Invoke,             // invoke
    jsInterface_InvokeDefault,      // invokeDefault
    jsInterface_HasProperty,        // hasProperty
    jsInterface_GetProperty,        // getProperty
    jsInterface_SetProperty,        // setProperty
    jsInterface_RemoveProperty,     // removeProperty
    jsInterface_Enumeration,        // enumerate
    NULL                            // construct
};

/* JS Object implementations */

/** 
 * Create a new jsNetToolsClass instance
 * 
 * @param plugin - Parent plugin
 * @return JS Object
 */
NPObject*
jsInterface_New(NPP plugin)
{
    NPObject* object = NULL;
    object = NPN_CreateObject(plugin, &jsNetToolsClass);

    return object;
}

/** 
 * Custom allocation of memory.
 * Stub for future use.
 * 
 * @param plugin - Parent plugin
 * @param aClass - Plugin class
 * @return JS Object
 */
static NPObject* 
jsInterface_Allocate (NPP plugin, NPClass *aClass)
{
    assert(false);
    return NULL;
}

/** 
 * Custom freeing of memory.
 * Stub for future use.
 * 
 * @param npobj - Instance
 */
static void 
jsInterface_Deallocate (NPObject *npobj)
{
    assert(false);
}

/** 
 * Invalidate the object so that it can not be used again
 * Stub for future use.
 * 
 * @param npobj - Instance
 */
static void 
jsInterface_Invalidate (NPObject *npobj)
{
}

/** 
 * Query if the object has a method
 * 
 * @param npobj - Instance
 * @param name - Method name (as a browser-specific identifier) 
 *
 * @return True if method correctly executed. False otherwise
 */
static bool 
jsInterface_HasMethod (NPObject *npobj, NPIdentifier name)
{
    int i;
    bool retVal = false;
    NPUTF8* methodName;

    methodName = NPN_UTF8FromIdentifier(name);
    if (!methodName)
        return false;

    for (i=0; i<JS_INTERFACE_METHOD_COUNT; i++)
    {
        if (strcmp(methodName, jsInterfaceMethodNames[i]) == 0)
        {
            retVal = true;
            break;
        }
    }

    free(methodName);
    return retVal;
}

/** 
 * Invoke a method
 * 
 * @param npobj - Instance
 * @param name - Method name (as a browser-specific identifier) 
 * @param args - Array of arguments
 * @param argCount - Number of arguments
 * @param result - Result of the method should be stored here
 *
 * @return True if method correctly executed. False otherwise
 */
static bool 
jsInterface_Invoke (NPObject *npobj, NPIdentifier name,
                            const NPVariant *args, uint32_t argCount,
                            NPVariant *result)
{
    int i;
    bool retVal = false;
    NPUTF8* methodName;

    methodName = NPN_UTF8FromIdentifier(name);
    if (!methodName)
        return false;

    for (i=0; i<JS_INTERFACE_METHOD_COUNT; i++)
    {
        if (strcmp(methodName, jsInterfaceMethodNames[i]) == 0)
        {
            retVal = jsInterfaceMethodImpl[i](args, argCount, result);
            break;
        }
    }

    return true;
}

/** 
 * Invoke the default method
 * Stub for future use.
 * 
 * @param npobj - Instance
 * @param args - Array of arguments
 * @param argCount - Number of arguments
 * @param result - Result of the method should be stored here
 *
 * @return True if method correctly executed. False otherwise
 */
static bool 
jsInterface_InvokeDefault (NPObject *npobj,
                                   const NPVariant *args,
                                   uint32_t argCount,
                                   NPVariant *result)
{
    return false;
}

/** 
 * Query if the object has a property
 * 
 * @param npobj - Instance
 * @param name - Property name (as a browser-specific identifier) 
 *
 * @return True if method correctly executed. False otherwise
 */
static bool 
jsInterface_HasProperty (NPObject *npobj, NPIdentifier name)
{
    int i;
    bool retVal = false;
    NPUTF8* propertyName;

    propertyName = NPN_UTF8FromIdentifier(name);
    if (!propertyName)
        return false;

    for (i=0; i<JS_INTERFACE_PROPERTY_COUNT; i++)
    {
        if (strcmp(propertyName, jsInterfacePropertyNames[i]) == 0)
        {
            retVal = true;
            break;
        }
    }

    return retVal;
}


/** 
 * Get a property value
 * 
 * @param npobj - Instance
 * @param name - Property name (as a browser-specific identifier) 
 * @param result - Value of the property should be stored here
 *
 * @return True if method correctly executed. False otherwise
 */
static bool 
jsInterface_GetProperty (NPObject *npobj, NPIdentifier name,
                                 NPVariant *result)
{
    return false;

}

/** 
 * Set a property value
 * 
 * @param npobj - Instance
 * @param name - Property name (as a browser-specific identifier) 
 * @param value - Value of the property should be taken from here
 *
 * @return True if method correctly executed. False otherwise
 */
static bool 
jsInterface_SetProperty (NPObject *npobj, NPIdentifier name,
                                 const NPVariant *value)
{
    return false;
}

/** 
 * Remove a property
 * 
 * @param npobj - Instance
 * @param name - Property name (as a browser-specific identifier) 
 *
 * @return True if method correctly executed. False otherwise
 */
static bool 
jsInterface_RemoveProperty (NPObject *npobj,
                                    NPIdentifier name)
{
    return false;
}

/** 
 * Enumerate all the methods and properties the object has.
 * 
 * @param npobj - Instance
 * @param value - Array where the method identifiers should be stored.
 * @param count - Number of such identifiers
 *
 * @return True if method correctly executed. False otherwise
 */
static bool 
jsInterface_Enumeration (NPObject *npobj, NPIdentifier **value,
                                 uint32_t *count)
{
    return false;
//    DBG_PRINT("");
//    *count = (JS_INTERFACE_METHOD_COUNT + JS_INTERFACE_PROPERTY_COUNT);
//    *value = (NPIdentifier*)NPN_MemAlloc((*count) * sizeof(NPIdentifier));
//
//    NPN_GetStringIdentifiers(jsInterfaceMethodNames, JS_INTERFACE_METHOD_COUNT, (*value));
//    NPN_GetStringIdentifiers(jsInterfacePropertyNames, JS_INTERFACE_PROPERTY_COUNT, ((*value)+JS_INTERFACE_METHOD_COUNT));
//
    return true;
}

/** 
 * Construct an object
 * 
 * @param npobj - Instance
 * @param args - Arguments to constructor
 * @param argCount - Number of arguments
 * @param result - (?)
 *
 * @return True if method correctly executed. False otherwise
 */
static bool 
jsInterface_Construct (NPObject *npobj,
                                       const NPVariant *args,
                                       uint32_t argCount,
                                       NPVariant *result)
{
    return false;
}

/** 
 * Execute a ping 
 * Expects arguments of the form (string hostIP, [int count], [float interval])
 * 
 * @param args - Arguments to function
 * @param argCount - Number of arguments
 * @param result - Result of function to be stored here
 *
 * @return True if method correctly executed. False otherwise
 */
static bool  
jsNetTool_Ping (const NPVariant *args, uint32_t argCount, NPVariant *result)
{
    const char* host;
    int count = 3;
    double interval = 0.2;

    if (argCount < 1 || argCount > 3)
        return NPERR_INVALID_PARAM;

    if (args[0].type != NPVariantType_String)
        return NPERR_INVALID_PARAM;

    host = args[0].value.stringValue.utf8characters;
    if (argCount == 2)
    {
        if (args[1].type != NPVariantType_Int32)
            return NPERR_INVALID_PARAM;
        count = args[1].value.intValue;
    }
    if (argCount == 3)
    {
        if (args[2].type != NPVariantType_Double)
            return NPERR_INVALID_PARAM;
        interval = args[2].value.doubleValue;
    }

    {
        FILE* pingIO;
        char *cmd_str;
        char buf[CMD_BUF_SIZE];

        regex_t rtt_re;
        regmatch_t match[2];

        regcomp(&rtt_re, "rtt.*= ([0-9]+\\.[0-9]+/[0-9]+\\.[0-9]+/[0-9]+\\.[0-9]+/[0-9]+\\.[0-9]+).*", REG_EXTENDED);

        cmd_str = (char*)NPN_MemAlloc(CMD_BUF_SIZE*sizeof(char));
        snprintf(cmd_str, CMD_BUF_SIZE, "ping -q -c%d -i%f %s", count, interval, host);

        // Parse output for results
        pingIO = popen(cmd_str, "r");
        if (pingIO)
        {
            while(fgets(buf, CMD_BUF_SIZE, pingIO))
            {
                if(regexec(&rtt_re, buf, 2, match, 0) == 0)
                {
                    int len;
                    NPUTF8 *str_;

                    len = match[1].rm_eo - match[1].rm_so + 1;
                    str_ = (NPUTF8*)NPN_MemAlloc(len*sizeof(NPUTF8));
                    strncpy((char*)str_, buf+match[1].rm_so, len-1);
                    str_[len-1] = '\0';

                    STRINGZ_TO_NPVARIANT(str_, *result);
                    break;
                }
            }
            fclose(pingIO);
        }
        NPN_MemFree(cmd_str);

        regfree(&rtt_re);
    }

    return true;
}


