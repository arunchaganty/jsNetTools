/*
 * jsNetTools.h - Entry point for jsNetTools plugin
 * Last Modified: January 26, 2010
 * Copyright (C) Arun Tejasvi Chaganty 2009 <arunchaganty@gmail.com>
 */

#ifndef JS_NET_TOOLS_H
#define JS_NET_TOOLS_H

#include <npapi.h>
#include <npfunctions.h>
#include <npruntime.h>

NPError NP_Initialize(NPNetscapeFuncs* bFuncs, NPPluginFuncs* pFuncs);
NPError NP_Shutdown();

NPError NPP_New(NPMIMEType pluginType, NPP instance, uint16_t mode, int16_t argc, char* argn[], char* argv[], NPSavedData* saved);
NPError NPP_Destroy(NPP instance, NPSavedData** save);
NPError NPP_SetWindow(NPP instance, NPWindow* window);
NPError NPP_NewStream(NPP instance, NPMIMEType type, NPStream* stream, NPBool seekable, uint16_t* stype);
NPError NPP_DestroyStream(NPP instance, NPStream* stream, NPReason reason);
int32_t NPP_WriteReady(NPP instance, NPStream* stream);
int32_t NPP_Write(NPP instance, NPStream* stream, int32_t offset, int32_t len, void* buffer);
void    NPP_StreamAsFile(NPP instance, NPStream* stream, const char* fname);
void    NPP_Print(NPP instance, NPPrint* platformPrint);
int16_t NPP_HandleEvent(NPP instance, void* event);
void    NPP_URLNotify(NPP instance, const char* URL, NPReason reason, void* notifyData);
NPError NPP_GetValue(NPP instance, NPPVariable variable, void *value);
NPError NPP_SetValue(NPP instance, NPNVariable variable, void *value);

#endif //JS_NET_TOOLS_H
