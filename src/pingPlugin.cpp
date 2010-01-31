/**
 * @file pingPlugin.cpp
 * @author Arun Tejasvi Chaganty <arunchaganty@gmail.com>
 * @version 0.1
 * 
 * @section Description
 * Implementation ping utility
 */

#include "pingPlugin.h"
#include "NPN.h"
#include <npruntime.h>
#include <npfunctions.h>
#include "util.h"
#include <regex.h>
#include <pthread.h>

#define CMD_BUF_SIZE    500

static void pingWorker(PingArgs* args);

/** 
 * Execute a ping 
 * Expects arguments of the form (string hostIP, [int count], [float interval])
 * 
 * @param obj  - The calling object - required to determine associated plugin
 * @param args - Arguments to function : Should contain an object with suitable properties
 * @param argCount - Number of arguments
 * @param result - Result of function to be stored here
 *
 * @return True if method correctly executed. False otherwise
 */
bool  
jsNetTool_Ping (const jsInterfaceObject *obj, const NPVariant *args, uint32_t argCount, NPVariant *result)
{
    NPP plugin = obj->plugin;
    NPObject* params_obj;

    // Validate function call
    DBG_PRINT("");
    if (argCount != 1)
        return NPERR_INVALID_PARAM;

    if ((!NPVARIANT_IS_OBJECT(args[0])))
        return NPERR_INVALID_PARAM;

    params_obj = NPVARIANT_TO_OBJECT(args[0]);

    // Extract parameters
    NPVariant param;

    // Host (Required)
    const char* host;
    if ((NPN_HasProperty(plugin, params_obj, NPN_GetStringIdentifier("host"))))
    {
        if ((!NPN_GetProperty(plugin, params_obj, NPN_GetStringIdentifier("host"), &param)) ||
            (!NPVARIANT_IS_STRING(param)))
            return NPERR_INVALID_PARAM;
        // IMPORTANT: host should be freed after use
        host = strndup(NPVARIANT_TO_STRING(param).utf8characters, CMD_BUF_SIZE);
        NPN_ReleaseVariantValue(&param);
    }
    else
        return NPERR_INVALID_PARAM;

    // Callback (Required)
    NPObject* callb;
    if (NPN_HasMethod(plugin, params_obj, NPN_GetStringIdentifier("callback")))
    {
        callb = params_obj;
        // Keep the object - it will be need to use the callback
        NPN_RetainObject(params_obj);
    }
    else
        return NPERR_INVALID_PARAM;

    // Interval (optional)
    double interval;
    if ((NPN_HasProperty(plugin, params_obj, NPN_GetStringIdentifier("interval"))))
    {
        if (!(NPN_GetProperty(plugin, params_obj, NPN_GetStringIdentifier("interval"), &param)) ||
             (!NPVARIANT_IS_DOUBLE(param)))
            return NPERR_INVALID_PARAM;
        interval = NPVARIANT_TO_DOUBLE(param);
        NPN_ReleaseVariantValue(&param);
    } 
    else
        interval = 0.2;

    // Count (optional)
    int count;
    if ((NPN_HasProperty(plugin, params_obj, NPN_GetStringIdentifier("count"))))
    {
        if (!(NPN_GetProperty(plugin, params_obj, NPN_GetStringIdentifier("count"), &param)) ||
             (!NPVARIANT_IS_INT32(param)))
            return NPERR_INVALID_PARAM;
        count = NPVARIANT_TO_INT32(param);
        NPN_ReleaseVariantValue(&param);
    } 
    else
        count = 3;

    // Spawn thread for ping
    {
        pthread_t thread;
        pthread_attr_t attrs;
        PingArgs *args;


        args = (PingArgs*) malloc(sizeof(PingArgs));
        args->obj = obj;

        args->host = (char*)host;
        args->count = count;
        args->interval = interval;

        args->callb = callb;

        // Create the worker thread.
        // NOTE: The worker thread must free host, args and release reference on the
        // object
        pthread_create(&thread, &attrs, (void* (*)(void*))pingWorker, args);
    }

    BOOLEAN_TO_NPVARIANT(true, (*result));

    return true;
}

/**
 * Worker thread function. This function runs in _a seperate context_ than the
 * brower thread, and hence _no browser functions_ should directly be called.
 * Only exception is NPN_PluginThreadAsyncCall.
 *
 * @param args - arguments to ping
 */
static void pingWorker(PingArgs* args)
{
    DBG_PRINT("");
    char* host = args->host;
    int count = args->count;
    double interval = args->interval;

    FILE* pingIO = NULL;
    char *cmd_str;
    char buf[CMD_BUF_SIZE];

    NPUTF8 *str_;

    // Construct a regex matcher for the ping output
    regex_t rtt_re;
    regmatch_t match[2];
    regcomp(&rtt_re, "rtt.*= ([0-9]+\\.[0-9]+/[0-9]+\\.[0-9]+/[0-9]+\\.[0-9]+/[0-9]+\\.[0-9]+).*", REG_EXTENDED);

    cmd_str = (char*)malloc(CMD_BUF_SIZE*sizeof(char));
    snprintf(cmd_str, CMD_BUF_SIZE, "ping -q -c%d -i%.2f %s", count, interval, host);
    // Free the host string
    free(host);

    // Parse output for results
    pingIO = popen(cmd_str, "r");
    if (pingIO != NULL)
    {
        while(fgets(buf, CMD_BUF_SIZE, pingIO))
        {
            if(regexec(&rtt_re, buf, 2, match, 0) == 0)
            {
                int len;

                // Copy across the results
                len = match[1].rm_eo - match[1].rm_so + 1;
                str_ = (NPUTF8*)NPN_MemAlloc(len*sizeof(NPUTF8));
                strncpy((char*)str_, buf+match[1].rm_so, len-1);
                str_[len-1] = '\0';

                break;
            }
        }
        fclose(pingIO);

        // Construct a "reply" object for the callback
        ScriptReply* reply;
        reply = (ScriptReply*) malloc (sizeof(ScriptReply));
        reply->callb = (NPObject*) args->callb;
        reply->result = (NPVariant*) NPN_MemAlloc(sizeof(NPVariant));
        STRINGZ_TO_NPVARIANT(str_, *(reply->result));
        // Return result using callback
        // IMPORTANT: Remember to free callb, reply and reply->result
        jsInvokeCallback((jsInterfaceObject*) args->obj, reply);
    }
    free(cmd_str);
    regfree(&rtt_re);

    free(args);
    pthread_exit(NULL);
}


