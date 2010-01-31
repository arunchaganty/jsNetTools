/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* ***** BEGIN LICENSE BLOCK *****
 * Version: NPL 1.1/GPL 2.0/LGPL 2.1
 *
 * The contents of this file are subject to the Netscape Public License
 * Version 1.1 (the "License"); you may not use this file except in
 * compliance with the License. You may obtain a copy of the License at
 * http://www.mozilla.org/NPL/
 *
 * Software distributed under the License is distributed on an "AS IS" basis,
 * WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License
 * for the specific language governing rights and limitations under the
 * License.
 *
 * The Original Code is mozilla.org code.
 *
 * The Initial Developer of the Original Code is 
 * Netscape Communications Corporation.
 * Portions created by the Initial Developer are Copyright (C) 1998
 * the Initial Developer. All Rights Reserved.
 *
 * Contributor(s):
 *
 * Alternatively, the contents of this file may be used under the terms of
 * either the GNU General Public License Version 2 or later (the "GPL"), or 
 * the GNU Lesser General Public License Version 2.1 or later (the "LGPL"),
 * in which case the provisions of the GPL or the LGPL are applicable instead
 * of those above. If you wish to allow use of your version of this file only
 * under the terms of either the GPL or the LGPL, and not to allow others to
 * use your version of this file under the terms of the NPL, indicate your
 * decision by deleting the provisions above and replace them with the notice
 * and other provisions required by the GPL or the LGPL. If you do not delete
 * the provisions above, a recipient may use your version of this file under
 * the terms of any one of the NPL, the GPL or the LGPL.
 *
 * ***** END LICENSE BLOCK ***** */

////////////////////////////////////////////////////////////
//
// Implementation of Netscape entry points (NPN_*)
//
#include <npapi.h>
#include <npfunctions.h>

#ifndef HIBYTE
#define HIBYTE(x) ((((uint32_t)(x)) & 0xff00) >> 8)
#endif

#ifndef LOBYTE
#define LOBYTE(W) ((W) & 0xFF)
#endif

void NPN_Version(int* plugin_major, int* plugin_minor, int* netscape_major, int* netscape_minor);
NPError NPN_GetURLNotify(NPP instance, const char *url, const char *target, void* notifyData);
NPError NPN_GetURL(NPP instance, const char *url, const char *target);
NPError NPN_PostURLNotify(NPP instance, const char* url, const char* window, uint32_t len, const char* buf, NPBool file, void* notifyData);
NPError NPN_PostURL(NPP instance, const char* url, const char* window, uint32_t len, const char* buf, NPBool file);
NPError NPN_RequestRead(NPStream* stream, NPByteRange* rangeList);
NPError NPN_NewStream(NPP instance, NPMIMEType type, const char* target, NPStream** stream);
int32_t NPN_Write(NPP instance, NPStream *stream, int32_t len, void *buffer);
NPError NPN_DestroyStream(NPP instance, NPStream* stream, NPError reason);
void NPN_Status(NPP instance, const char *message);
const char* NPN_UserAgent(NPP instance);
void* NPN_MemAlloc(uint32_t size);
void NPN_MemFree(void* ptr);
uint32_t NPN_MemFlush(uint32_t size);
void NPN_ReloadPlugins(NPBool reloadPages);
NPError NPN_GetValue(NPP instance, NPNVariable variable, void *value);
NPError NPN_SetValue(NPP instance, NPPVariable variable, void *value);
void NPN_InvalidateRect(NPP instance, NPRect *invalidRect);
void NPN_InvalidateRegion(NPP instance, NPRegion invalidRegion);
void NPN_ForceRedraw(NPP instance);
NPIdentifier NPN_GetStringIdentifier(const NPUTF8 *name);
void NPN_GetStringIdentifiers(const NPUTF8 **names, uint32_t nameCount, NPIdentifier *identifiers);
NPIdentifier NPN_GetStringIdentifier(int32_t intid);
bool NPN_IdentifierIsString(NPIdentifier identifier);
NPUTF8 *NPN_UTF8FromIdentifier(NPIdentifier identifier);
int32_t NPN_IntFromIdentifier(NPIdentifier identifier);
NPObject *NPN_CreateObject(NPP npp, NPClass *aClass);
NPObject *NPN_RetainObject(NPObject *obj);
void NPN_ReleaseObject(NPObject *obj);
bool NPN_Invoke(NPP npp, NPObject* obj, NPIdentifier methodName, const NPVariant *args, uint32_t argCount, NPVariant *result);
bool NPN_InvokeDefault(NPP npp, NPObject* obj, const NPVariant *args, uint32_t argCount, NPVariant *result);
bool NPN_Evaluate(NPP npp, NPObject* obj, NPString *script, NPVariant *result);
bool NPN_GetProperty(NPP npp, NPObject* obj, NPIdentifier propertyName, NPVariant *result);
bool NPN_SetProperty(NPP npp, NPObject* obj, NPIdentifier propertyName, const NPVariant *value);
bool NPN_RemoveProperty(NPP npp, NPObject* obj, NPIdentifier propertyName);
bool NPN_Enumerate(NPP npp, NPObject *obj, NPIdentifier **identifier, uint32_t *count);
bool NPN_Construct(NPP npp, NPObject *obj, const NPVariant *args, uint32_t argCount, NPVariant *result);
bool NPN_HasProperty(NPP npp, NPObject* obj, NPIdentifier propertyName);
bool NPN_HasMethod(NPP npp, NPObject* obj, NPIdentifier methodName);
void NPN_ReleaseVariantValue(NPVariant *variant);
void NPN_SetException(NPObject* obj, const NPUTF8 *message);
void NPN_PluginThreadAsyncCall(NPP instance, void (*func) (void *), void *userData);

