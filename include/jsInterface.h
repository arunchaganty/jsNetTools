/*
 * jsInterface.h - Javascript API for jsNetTools
 * Last Modified: January 26, 2010
 * Copyright (C) Arun Tejasvi Chaganty 2009 <arunchaganty@gmail.com>
 */

#ifndef JS_INTERFACE_H
#define JS_INTERFACE_H

#include <npapi.h>
#include <npfunctions.h>
#include <npruntime.h>

NPObject* jsInterface_New(NPP plugin);

#endif  // JS_INTERFACE_H

