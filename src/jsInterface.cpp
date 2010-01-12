
#include "jsInterface.h"
#include "NPN.h"
#include <assert.h>
#include <string.h>
#include <stdlib.h>

#define JS_INTERFACE_METHOD_COUNT 2
#define DBG_PRINTV(fmt, ...)     (fprintf(stderr, "%s:" fmt "\n", __FUNCTION__, __VA_ARGS__))
#define DBG_PRINT(fmt)     (fprintf(stderr, "%s:" fmt "\n", __FUNCTION__))

extern NPNetscapeFuncs* gBrowserFuncs;

static const NPUTF8* jsInterfaceMethodNames[JS_INTERFACE_METHOD_COUNT] = 
{
    "Ping",
    "Nslookup"
};

#define JS_INTERFACE_PROPERTY_COUNT 0

static const NPUTF8* jsInterfacePropertyNames[JS_INTERFACE_PROPERTY_COUNT] = 
{
};

static NPClass jsNetToolsClass =
{
    NP_CLASS_STRUCT_VERSION_ENUM, // structVersion
    NULL, // allocate
    NULL, // deallocate
    jsInterface_Invalidate, // invalidate
    jsInterface_HasMethod, // hasMethod
    jsInterface_Invoke, // invoke
    jsInterface_InvokeDefault, // invokeDefault
    jsInterface_HasProperty, // hasProperty
    jsInterface_GetProperty, // getProperty
    jsInterface_SetProperty, // setProperty
    jsInterface_RemoveProperty, // removeProperty
    jsInterface_Enumeration, // enumerate
    NULL // construct
};

NPObject*
jsInterface_New(NPP plugin)
{
    NPObject* object = NULL;
    DBG_PRINT("");
    object = gBrowserFuncs->createobject(plugin, &jsNetToolsClass);

    return object;
}

NPObject* 
jsInterface_Allocate (NPP npp, NPClass *aClass)
{
    DBG_PRINT("");
    assert(false);
    return NULL;
}

void 
jsInterface_Deallocate (NPObject *npobj)
{
    DBG_PRINT("");
    assert(false);
}

void 
jsInterface_Invalidate (NPObject *npobj)
{
    DBG_PRINT("");
}

bool 
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

    DBG_PRINTV("methodName: %s: %d", methodName, retVal);
    free(methodName);
    return retVal;
}

bool jsInterface_Invoke (NPObject *npobj, NPIdentifier name,
                            const NPVariant *args, uint32_t argCount,
                            NPVariant *result)
{
    bool retVal = false;
    NPUTF8* methodName;

    methodName = NPN_UTF8FromIdentifier(name);
    if (!methodName)
        return false;
    DBG_PRINTV("methodName: %s: %d", methodName, retVal);

    return true;
}

bool jsInterface_InvokeDefault (NPObject *npobj,
                                   const NPVariant *args,
                                   uint32_t argCount,
                                   NPVariant *result)
{
    DBG_PRINT("");
    return false;
}

bool jsInterface_HasProperty (NPObject *npobj, NPIdentifier name)
{
    int i;
    bool retVal = false;
    NPUTF8* propertyName;

    propertyName = NPN_UTF8FromIdentifier(name);
    DBG_PRINTV("propertyName: %s", propertyName);
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

    free(propertyName);
    return retVal;
}

bool jsInterface_GetProperty (NPObject *npobj, NPIdentifier name,
                                 NPVariant *result)
{
    DBG_PRINT("");
    return false;
}
bool jsInterface_SetProperty (NPObject *npobj, NPIdentifier name,
                                 const NPVariant *value)
{
    DBG_PRINT("");
    return false;
}
bool jsInterface_RemoveProperty (NPObject *npobj,
                                    NPIdentifier name)
{
    DBG_PRINT("");
    return false;
}

bool jsInterface_Enumeration (NPObject *npobj, NPIdentifier **value,
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

bool jsInterface_Construct (NPObject *npobj,
                                       const NPVariant *args,
                                       uint32_t argCount,
                                       NPVariant *result)
{
    DBG_PRINT("");
    return false;
}
