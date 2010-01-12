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
#include "NPN.h"

extern NPNetscapeFuncs *gBrowserFuncs;

void NPN_Version(int* plugin_major, int* plugin_minor, int* netscape_major, int* netscape_minor)
{
  *plugin_major   = NP_VERSION_MAJOR;
  *plugin_minor   = NP_VERSION_MINOR;
  *netscape_major = HIBYTE(gBrowserFuncs->version);
  *netscape_minor = LOBYTE(gBrowserFuncs->version);
}

NPError NPN_GetURLNotify(NPP instance, const char *url, const char *target, void* notifyData)
{
	int navMinorVers = gBrowserFuncs->version & 0xFF;
  NPError rv = NPERR_NO_ERROR;

  if( navMinorVers >= NPVERS_HAS_NOTIFICATION )
		rv = gBrowserFuncs->geturlnotify(instance, url, target, notifyData);
	else
		rv = NPERR_INCOMPATIBLE_VERSION_ERROR;

  return rv;
}

NPError NPN_GetURL(NPP instance, const char *url, const char *target)
{
  NPError rv = gBrowserFuncs->geturl(instance, url, target);
  return rv;
}

NPError NPN_PostURLNotify(NPP instance, const char* url, const char* window, uint32_t len, const char* buf, NPBool file, void* notifyData)
{
	int navMinorVers = gBrowserFuncs->version & 0xFF;
  NPError rv = NPERR_NO_ERROR;

	if( navMinorVers >= NPVERS_HAS_NOTIFICATION )
		rv = gBrowserFuncs->posturlnotify(instance, url, window, len, buf, file, notifyData);
	else
		rv = NPERR_INCOMPATIBLE_VERSION_ERROR;

  return rv;
}

NPError NPN_PostURL(NPP instance, const char* url, const char* window, uint32_t len, const char* buf, NPBool file)
{
  NPError rv = gBrowserFuncs->posturl(instance, url, window, len, buf, file);
  return rv;
} 

NPError NPN_RequestRead(NPStream* stream, NPByteRange* rangeList)
{
  NPError rv = gBrowserFuncs->requestread(stream, rangeList);
  return rv;
}

NPError NPN_NewStream(NPP instance, NPMIMEType type, const char* target, NPStream** stream)
{
	int navMinorVersion = gBrowserFuncs->version & 0xFF;

  NPError rv = NPERR_NO_ERROR;

	if( navMinorVersion >= NPVERS_HAS_STREAMOUTPUT )
		rv = gBrowserFuncs->newstream(instance, type, target, stream);
	else
		rv = NPERR_INCOMPATIBLE_VERSION_ERROR;

  return rv;
}

int32_t NPN_Write(NPP instance, NPStream *stream, int32_t len, void *buffer)
{
	int navMinorVersion = gBrowserFuncs->version & 0xFF;
  int32_t rv = 0;

  if( navMinorVersion >= NPVERS_HAS_STREAMOUTPUT )
		rv = gBrowserFuncs->write(instance, stream, len, buffer);
	else
		rv = -1;

  return rv;
}

NPError NPN_DestroyStream(NPP instance, NPStream* stream, NPError reason)
{
	int navMinorVersion = gBrowserFuncs->version & 0xFF;
  NPError rv = NPERR_NO_ERROR;

  if( navMinorVersion >= NPVERS_HAS_STREAMOUTPUT )
		rv = gBrowserFuncs->destroystream(instance, stream, reason);
	else
		rv = NPERR_INCOMPATIBLE_VERSION_ERROR;

  return rv;
}

void NPN_Status(NPP instance, const char *message)
{
  gBrowserFuncs->status(instance, message);
}

const char* NPN_UserAgent(NPP instance)
{
  const char * rv = NULL;
  rv = gBrowserFuncs->uagent(instance);
  return rv;
}

void* NPN_MemAlloc(uint32_t size)
{
  void * rv = NULL;
  rv = gBrowserFuncs->memalloc(size);
  return rv;
}

void NPN_MemFree(void* ptr)
{
  gBrowserFuncs->memfree(ptr);
}

uint32_t NPN_MemFlush(uint32_t size)
{
  uint32_t rv = gBrowserFuncs->memflush(size);
  return rv;
}

void NPN_ReloadPlugins(NPBool reloadPages)
{
  gBrowserFuncs->reloadplugins(reloadPages);
}

NPError NPN_GetValue(NPP instance, NPNVariable variable, void *value)
{
  NPError rv = gBrowserFuncs->getvalue(instance, variable, value);
  return rv;
}

NPError NPN_SetValue(NPP instance, NPPVariable variable, void *value)
{
  NPError rv = gBrowserFuncs->setvalue(instance, variable, value);
  return rv;
}

void NPN_InvalidateRect(NPP instance, NPRect *invalidRect)
{
  gBrowserFuncs->invalidaterect(instance, invalidRect);
}

void NPN_InvalidateRegion(NPP instance, NPRegion invalidRegion)
{
  gBrowserFuncs->invalidateregion(instance, invalidRegion);
}

void NPN_ForceRedraw(NPP instance)
{
  gBrowserFuncs->forceredraw(instance);
}

NPIdentifier NPN_GetStringIdentifier(const NPUTF8 *name)
{
  return gBrowserFuncs->getstringidentifier(name);
}

void NPN_GetStringIdentifiers(const NPUTF8 **names, uint32_t nameCount,
                              NPIdentifier *identifiers)
{
  return gBrowserFuncs->getstringidentifiers(names, nameCount, identifiers);
}

NPIdentifier NPN_GetStringIdentifier(int32_t intid)
{
  return gBrowserFuncs->getintidentifier(intid);
}

bool NPN_IdentifierIsString(NPIdentifier identifier)
{
  return gBrowserFuncs->identifierisstring(identifier);
}

NPUTF8 *NPN_UTF8FromIdentifier(NPIdentifier identifier)
{
  return gBrowserFuncs->utf8fromidentifier(identifier);
}

int32_t NPN_IntFromIdentifier(NPIdentifier identifier)
{
  return gBrowserFuncs->intfromidentifier(identifier);
}

NPObject *NPN_CreateObject(NPP npp, NPClass *aClass)
{
  return gBrowserFuncs->createobject(npp, aClass);
}

NPObject *NPN_RetainObject(NPObject *obj)
{
  return gBrowserFuncs->retainobject(obj);
}

void NPN_ReleaseObject(NPObject *obj)
{
  return gBrowserFuncs->releaseobject(obj);
}

bool NPN_Invoke(NPP npp, NPObject* obj, NPIdentifier methodName,
                const NPVariant *args, uint32_t argCount, NPVariant *result)
{
  return gBrowserFuncs->invoke(npp, obj, methodName, args, argCount, result);
}

bool NPN_InvokeDefault(NPP npp, NPObject* obj, const NPVariant *args,
                       uint32_t argCount, NPVariant *result)
{
  return gBrowserFuncs->invokeDefault(npp, obj, args, argCount, result);
}

bool NPN_Evaluate(NPP npp, NPObject* obj, NPString *script,
                  NPVariant *result)
{
  return gBrowserFuncs->evaluate(npp, obj, script, result);
}

bool NPN_GetProperty(NPP npp, NPObject* obj, NPIdentifier propertyName,
                     NPVariant *result)
{
  return gBrowserFuncs->getproperty(npp, obj, propertyName, result);
}

bool NPN_SetProperty(NPP npp, NPObject* obj, NPIdentifier propertyName,
                     const NPVariant *value)
{
  return gBrowserFuncs->setproperty(npp, obj, propertyName, value);
}

bool NPN_RemoveProperty(NPP npp, NPObject* obj, NPIdentifier propertyName)
{
  return gBrowserFuncs->removeproperty(npp, obj, propertyName);
}

bool NPN_Enumerate(NPP npp, NPObject *obj, NPIdentifier **identifier,
                   uint32_t *count)
{
  return gBrowserFuncs->enumerate(npp, obj, identifier, count);
}

bool NPN_Construct(NPP npp, NPObject *obj, const NPVariant *args,
                   uint32_t argCount, NPVariant *result)
{
  return gBrowserFuncs->construct(npp, obj, args, argCount, result);
}

bool NPN_HasProperty(NPP npp, NPObject* obj, NPIdentifier propertyName)
{
  return gBrowserFuncs->hasproperty(npp, obj, propertyName);
}

bool NPN_HasMethod(NPP npp, NPObject* obj, NPIdentifier methodName)
{
  return gBrowserFuncs->hasmethod(npp, obj, methodName);
}

void NPN_ReleaseVariantValue(NPVariant *variant)
{
  gBrowserFuncs->releasevariantvalue(variant);
}

void NPN_SetException(NPObject* obj, const NPUTF8 *message)
{
  gBrowserFuncs->setexception(obj, message);
}

