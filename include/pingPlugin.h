/*
 * pingPlugin.h - Exposes functions for ping utility
 * Last Modified: January 26, 2010
 * Copyright (C) Arun Tejasvi Chaganty 2009 <arunchaganty@gmail.com>
 */

#ifndef PING_PLUGIN_H
#define PING_PLUGIN_H

#include "NPN.h"
#include "jsInterface.h"
#include "util.h"

/**
 * \struct PingArgs
 * Encapsulates all the arguments to ping.
 */
struct PingArgs
{
    char* host;
    int count;
    double interval;

    const jsInterfaceObject* obj;
    const NPObject* callb;
};

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
bool jsNetTool_Ping (const jsInterfaceObject* obj, const NPVariant *args, uint32_t argCount, NPVariant *result);

#endif  // PING_PLUGIN_H

