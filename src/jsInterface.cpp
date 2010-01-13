
#include "jsInterface.h"
#include "NPN.h"
#include "util.h"
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <regex.h>

#define DBG_PRINTV(fmt, ...)     (fprintf(stderr, "%s:" fmt "\n", __FUNCTION__, __VA_ARGS__))
#define DBG_PRINT(fmt)     (fprintf(stderr, "%s:" fmt "\n", __FUNCTION__))

extern NPNetscapeFuncs* gBrowserFuncs;

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


#define JS_INTERFACE_METHOD_COUNT 2
typedef bool (*ScriptableFunc) (const NPVariant *args, uint32_t argCount, NPVariant *result);
static bool jsNetTool_Ping (const NPVariant *args, uint32_t argCount, NPVariant *result);
static bool jsNetTool_Nslookup (const NPVariant *args, uint32_t argCount, NPVariant *result);

static const NPUTF8* jsInterfaceMethodNames[JS_INTERFACE_METHOD_COUNT] = 
{
    "Ping",
    "Nslookup"
};

static const ScriptableFunc
jsInterfaceMethodImpl[JS_INTERFACE_METHOD_COUNT] = 
{
    jsNetTool_Ping,
    jsNetTool_Nslookup
};

#define JS_INTERFACE_PROPERTY_COUNT 0
static const NPUTF8* 
jsInterfacePropertyNames[JS_INTERFACE_PROPERTY_COUNT] = 
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
    object = gBrowserFuncs->createobject(plugin, &jsNetToolsClass);

    return object;
}

static NPObject* 
jsInterface_Allocate (NPP npp, NPClass *aClass)
{
    assert(false);
    return NULL;
}

static void 
jsInterface_Deallocate (NPObject *npobj)
{
    assert(false);
}

static void 
jsInterface_Invalidate (NPObject *npobj)
{
}

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

static bool 
jsInterface_InvokeDefault (NPObject *npobj,
                                   const NPVariant *args,
                                   uint32_t argCount,
                                   NPVariant *result)
{
    return false;
}

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

static bool 
jsInterface_GetProperty (NPObject *npobj, NPIdentifier name,
                                 NPVariant *result)
{
    return false;

}
static bool 
jsInterface_SetProperty (NPObject *npobj, NPIdentifier name,
                                 const NPVariant *value)
{
    return false;
}

static bool 
jsInterface_RemoveProperty (NPObject *npobj,
                                    NPIdentifier name)
{
    return false;
}

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

static bool 
jsInterface_Construct (NPObject *npobj,
                                       const NPVariant *args,
                                       uint32_t argCount,
                                       NPVariant *result)
{
    return false;
}

// Expects arguments of the form (string hostIP, [int count], [float interval])
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

    // Execute the fork
    {
        FILE* pingIO;
        char *cmd_str;
        char buf[100];

        regex_t rtt_re;
        regmatch_t match[2];

        regcomp(&rtt_re, "rtt.*= ([0-9]+\\.[0-9]+/[0-9]+\\.[0-9]+/[0-9]+\\.[0-9]+/[0-9]+\\.[0-9]+).*", REG_EXTENDED);

        cmd_str = (char*)NPN_MemAlloc(200*sizeof(char));
        snprintf(cmd_str, 200, "ping -q -c%d -i%f %s", count, interval, host);

        // Parse output for results
        pingIO = popen(cmd_str, "r");
        if (pingIO)
        {
            while(fgets(buf, 100, pingIO))
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

static bool 
jsNetTool_Nslookup (const NPVariant *args, uint32_t argCount, NPVariant *result)
{
    return false;
}

