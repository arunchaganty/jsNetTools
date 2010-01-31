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

#include "pingPlugin.h"

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

typedef bool (*ScriptableFunc) (const jsInterfaceObject* obj, const NPVariant* args, uint32_t argCount, NPVariant* result);

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
    jsInterface_Allocate,           // allocate
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
jsInterfaceObject* 
jsInterface_New(NPP plugin)
{
    jsInterfaceObject* object = NULL;
    object = (jsInterfaceObject*) NPN_CreateObject(plugin, &jsNetToolsClass);
    object->plugin = plugin;

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
    jsInterfaceObject *object;
    object = (jsInterfaceObject*) NPN_MemAlloc(sizeof(jsInterfaceObject));
    return (NPObject*) object;
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
    jsInterfaceObject* obj = (jsInterfaceObject*)npobj;

    methodName = NPN_UTF8FromIdentifier(name);
    if (!methodName)
        return false;

    for (i=0; i<JS_INTERFACE_METHOD_COUNT; i++)
    {
        if (strcmp(methodName, jsInterfaceMethodNames[i]) == 0)
        {
            retVal = jsInterfaceMethodImpl[i](obj, args, argCount, result);
            break;
        }
    }

    return retVal;
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
 * Calls the function in the reply with the reply of the function. This has to
 * be done in the browser's main thread, and hence this functions
 *
 * @param reply - Struct containing the callback object, and the reply param.
 */
static void 
jsReply (ScriptReply* reply)
{
    if (reply)
    {
        NPVariant result;
        NPN_Invoke(reply->plugin, reply->callb, NPN_GetStringIdentifier("callback"), reply->result, 1, &result);

        NPN_ReleaseObject(reply->callb);
        NPN_MemFree(reply->result);
        free(reply);
    }
}

/**
 * Handles callbacks for async functions with a reply
 * 
 * @param obj Object that contains the plugin
 * @param reply Reply structure that contains which callback should be called, and with what args
 */
void 
jsInvokeCallback(jsInterfaceObject* obj, ScriptReply* reply)
{
    NPP plugin;

    plugin = obj->plugin;
    assert(plugin);
    if (reply) 
    {
        // Populate the reply structure
        reply->plugin = plugin;
        NPN_PluginThreadAsyncCall(plugin, (NPCallback)jsReply, reply);
    }
}

