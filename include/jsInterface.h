
#ifndef JS_INTERFACE_H
#define JS_INTERFACE_H

#include <npapi.h>
#include <npfunctions.h>
#include <npruntime.h>

NPObject* jsInterface_New(NPP plugin);

NPObject* jsInterface_Allocate (NPP npp, NPClass *aClass);
void jsInterface_Deallocate (NPObject *npobj);
void jsInterface_Invalidate (NPObject *npobj);
bool jsInterface_HasMethod (NPObject *npobj, NPIdentifier name);
bool jsInterface_Invoke (NPObject *npobj, NPIdentifier name,
                            const NPVariant *args, uint32_t argCount,
                            NPVariant *result);
bool jsInterface_InvokeDefault (NPObject *npobj,
                                   const NPVariant *args,
                                   uint32_t argCount,
                                   NPVariant *result);
bool jsInterface_HasProperty (NPObject *npobj, NPIdentifier name);
bool jsInterface_GetProperty (NPObject *npobj, NPIdentifier name,
                                 NPVariant *result);
bool jsInterface_SetProperty (NPObject *npobj, NPIdentifier name,
                                 const NPVariant *value);
bool jsInterface_RemoveProperty (NPObject *npobj,
                                    NPIdentifier name);
bool jsInterface_Enumeration (NPObject *npobj, NPIdentifier **value,
                                 uint32_t *count);
bool jsInterface_Construct (NPObject *npobj,
                                       const NPVariant *args,
                                       uint32_t argCount,
                                       NPVariant *result);


#endif  // JS_INTERFACE_H

