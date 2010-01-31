/*
 * jsInterface.h - Javascript API for jsNetTools
 * Last Modified: January 26, 2010
 * Copyright (C) Arun Tejasvi Chaganty 2009 <arunchaganty@gmail.com>
 */

#ifndef JS_INTERFACE_H
#define JS_INTERFACE_H

#include "NPN.h"
#include <npfunctions.h>
#include <npruntime.h>


/**
 * \struct jsInterfaceObject
 *
 * jsNetTools object - contains some private members
 */
struct jsInterfaceObject
{
    NPObject obj;
    NPP plugin;
};

/** 
 * Create a new jsNetToolsClass instance
 * 
 * @param plugin - Parent plugin
 * @return JS Object
 */
jsInterfaceObject* jsInterface_New(NPP plugin);


/**
 * \struct ScriptReply
 *
 * Contains information required to callback a function on browser
 */
struct ScriptReply
{
    NPP plugin;
    NPObject* callb;
    NPVariant* result;
};

/**
 * Handles callbacks for async functions with a reply
 * 
 * @param obj Object that contains the plugin
 * @param reply Reply structure that contains which callback should be called, and with what args
 */
void 
jsInvokeCallback(jsInterfaceObject* obj, ScriptReply* reply);

/**
 * Type for callbacks to NPN_PluginThreadAsyncCall
 */
typedef void (*NPCallback)(void*);

#endif  // JS_INTERFACE_H

