/**
 * @file pingPlugin.cpp
 * @author Arun Tejasvi Chaganty <arunchaganty@gmail.com>
 * @version 0.1
 * 
 * @section Description
 * Implementation ping utility
 */

#include "NPN.h"
#include "npruntime.h"
#include "pingPlugin.h"
#include "util.h"
#include <regex.h>
#include <pthread.h>

#define CMD_BUF_SIZE    500

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
// TODO: Figure out how to store a dictionary mapping. This is getting painful.
bool  
jsNetTool_Ping (const jsInterfaceObject *obj, const NPVariant *args, uint32_t argCount, NPVariant *result)
{
    const char* host;
    int count = 3;
    double interval = 0.2;
    NPObject* callb;

    if (argCount < 1 || argCount > 4)
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

    /* Spawn thread for ping */
    {
        pthread_t thread;
        pthread_attr_t attrs;
        PingArgs *args;

        args = (PingArgs*) NPN_MemAlloc(sizeof(PingArgs));
        args->host = (char*)host;
        args->count = count;
        args->interval = interval;

        args->obj = obj;
        args->callb = callb;
    }

    BOOLEAN_TO_NPVARIANT(true, (*result));

    return true;
}

static void pingWorker(PingArgs* args)
{
    const char* host = args->host;
    int count = args->count;
    double interval = args->interval;

    FILE* pingIO;
    char *cmd_str;
    char buf[CMD_BUF_SIZE];

    NPUTF8 *str_;

    regex_t rtt_re;
    regmatch_t match[2];
    regcomp(&rtt_re, "rtt.*= ([0-9]+\\.[0-9]+/[0-9]+\\.[0-9]+/[0-9]+\\.[0-9]+/[0-9]+\\.[0-9]+).*", REG_EXTENDED);

    cmd_str = (char*)malloc(CMD_BUF_SIZE*sizeof(char));
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

                len = match[1].rm_eo - match[1].rm_so + 1;
                str_ = (NPUTF8*)malloc(len*sizeof(NPUTF8));
                strncpy((char*)str_, buf+match[1].rm_so, len-1);
                str_[len-1] = '\0';

                break;
            }
        }
        fclose(pingIO);
    }
    free(cmd_str);
    regfree(&rtt_re);

    {
        ScriptReply* reply;

        reply = (ScriptReply*) malloc (sizeof(ScriptReply));
        reply->callb = (NPObject*) args->callb;
        STRINGZ_TO_NPVARIANT(str_, *(reply->result));
        // Store result
        jsInvokeCallback((jsInterfaceObject*) args->obj, reply);
    }
    free(args);
}


