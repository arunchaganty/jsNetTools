/*
 * jsNetTools.h - Entry point for jsNetTools plugin
 * Last Modified: January 26, 2010
 * Copyright (C) Arun Tejasvi Chaganty 2009 <arunchaganty@gmail.com>
 */

#ifndef JS_NET_TOOLS_H
#define JS_NET_TOOLS_H

#include "NPN.h"
#include <npfunctions.h>
#include <npruntime.h>

/**
 * Populates the browsers plugin function table.
 *
 * @param bFuncs - A handle to the browsers functions
 * @param pFuncs - The browser's handle to the plugins functions
 */
NPError NP_Initialize(NPNetscapeFuncs* bFuncs, NPPluginFuncs* pFuncs);

/**
 * Clean up when the plugin is shutdown
 */
NPError NP_Shutdown();

#endif //JS_NET_TOOLS_H
