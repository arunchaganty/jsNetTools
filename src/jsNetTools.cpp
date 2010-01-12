
#include "jsNetTools.h"
#include "jsInterface.h"
#include "NPN.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define PLUGIN_NAME         "jsNetTools"
#define PLUGIN_DESCRIPTION  PLUGIN_NAME " (Mozilla SDK)"
#define PLUGIN_VERSION      "1.0.0.0"
#define DBG_PRINTV(fmt, ...)     (fprintf(stderr, "%s:" fmt "\n", __FUNCTION__, __VA_ARGS__))
#define DBG_PRINT(fmt)     (fprintf(stderr, "%s:" fmt "\n", __FUNCTION__))

NPNetscapeFuncs* gBrowserFuncs = NULL;

typedef struct InstanceData {
    NPP npp;
    NPWindow window;
} InstanceData;

NP_EXPORT(NPError)
NP_Initialize(NPNetscapeFuncs *bFuncs, NPPluginFuncs *pFuncs)
{
    gBrowserFuncs = bFuncs;

    // Populate the pFuncs table
    memset(pFuncs, 0, sizeof(pFuncs));
    pFuncs->version = 11;
    pFuncs->size = sizeof(*pFuncs);
    pFuncs->newp = NPP_New;
    pFuncs->destroy = NPP_Destroy;
    pFuncs->setwindow = NPP_SetWindow;
    pFuncs->newstream = NPP_NewStream;
    pFuncs->destroystream = NPP_DestroyStream;
    pFuncs->asfile = NPP_StreamAsFile;
    pFuncs->writeready = NPP_WriteReady;
    pFuncs->write = NPP_Write;
    pFuncs->print = NPP_Print;
    pFuncs->event = NPP_HandleEvent;
    pFuncs->urlnotify = NPP_URLNotify;
    pFuncs->getvalue = NPP_GetValue;
    pFuncs->setvalue = NPP_SetValue;

    return NPERR_NO_ERROR;
}

NP_EXPORT(NPError)
NP_Shutdown()
{
    return NPERR_NO_ERROR;
}

NP_EXPORT(char*)
NP_GetPluginVersion()
{
  return (char*)PLUGIN_VERSION;
}

NP_EXPORT(char*)
NP_GetMIMEDescription()
{
  return (char*)"application/nettool:ntl:NetTool";
}

NP_EXPORT(NPError)
NP_GetValue(void* instance, NPPVariable variable, void* aValue) 
{
    DBG_PRINTV("Variable: %d", variable);
    switch (variable) {
        case NPPVpluginNameString:
            *((char**)aValue) = (char*)PLUGIN_NAME;
            break;
        case NPPVpluginDescriptionString:
            *((char**)aValue) = (char*)PLUGIN_DESCRIPTION;
            break;
        case NPPVpluginScriptableNPObject:
            {
                if (instance)
                {
                    *((NPObject**)aValue) = jsInterface_New((NPP)instance);
                }
                else
                {
                    return NPERR_INVALID_PARAM;
                }
                DBG_PRINTV("%p", *((NPObject**)aValue));
            }
            break;
        default:
            return NPERR_INVALID_PARAM;
            break;
    }
    return NPERR_NO_ERROR;
}

NPError
NPP_New(NPMIMEType pluginType, NPP instance, uint16_t mode, 
        int16_t argc, char* argn[], char* argv[], 
        NPSavedData* saved) 
{
    DBG_PRINT("");
    if(instance == NULL)
        return NPERR_INVALID_INSTANCE_ERROR;

    instance->pdata = NULL;
    return NPERR_NO_ERROR;
}

NPError
NPP_Destroy(NPP instance, NPSavedData** save)
{
    return NPERR_NO_ERROR;
}

NPError
NPP_SetWindow(NPP instance, NPWindow* window) 
{
  return NPERR_GENERIC_ERROR;
}

NPError
NPP_NewStream(NPP instance, NPMIMEType type, NPStream* stream, NPBool seekable, uint16_t* stype) 
{
  return NPERR_GENERIC_ERROR;
}

NPError
NPP_DestroyStream(NPP instance, NPStream* stream, NPReason reason) 
{
  return NPERR_GENERIC_ERROR;
}

int32_t
NPP_WriteReady(NPP instance, NPStream* stream) 
{
  return 0;
}

int32_t
NPP_Write(NPP instance, NPStream* stream, int32_t offset, int32_t len, void* buffer) 
{
  return 0;
}

void
NPP_StreamAsFile(NPP instance, NPStream* stream, const char* fname) 
{

}

void
NPP_Print(NPP instance, NPPrint* platformPrint) 
{

}

int16_t
NPP_HandleEvent(NPP instance, void* event) 
{
    return 0;
}

void
NPP_URLNotify(NPP instance, const char* URL, NPReason reason, void* notifyData) 
{
}

NPError
NPP_GetValue(NPP instance, NPPVariable variable, void *value) 
{
    DBG_PRINTV("Variable: %d", variable);
    switch (variable) {
        case NPPVpluginScriptableNPObject:
            {
                //{
                //    // nsITestPlugin interface object should be associated with the plugin
                //    // instance itself. For the purpose of this example to keep things simple
                //    // we just assign it to instance->pdata after we create it.

                //    nsITestPlugin *scriptablePeer = (nsITestPlugin *)instance->pdata;

                //    // see if this is the first time and we haven't created it yet
                //    if (!scriptablePeer) {
                //        scriptablePeer = new nsScriptablePeer();
                //        if (scriptablePeer)
                //            NS_ADDREF(scriptablePeer); // addref for ourself,
                //        // don't forget to release on
                //        // shutdown to trigger its destruction
                //    }
                //    // add reference for the caller requesting the object
                //    NS_ADDREF(scriptablePeer);
                //    *(nsISupports **)value = scriptablePeer;
                //}

                if (instance)
                {
                    *((NPObject**)value) = jsInterface_New(instance);
                }
                else
                {
                    return NPERR_INVALID_PARAM;
                }
                DBG_PRINTV("%p", *((NPObject**)value));
            }
            break;
            //case NPPVpluginScriptableIID:
            //    {
            //        nsIID* ptr = (nsIID*) NPN_MemAlloc(sizeof(nsIID));
            //        *ptr = scriptableIID;
            //        *(nsIID **)value = ptr;
            //    }
            //    break;
        default:
            return NPERR_INVALID_PARAM;
            break;
    }
    return NPERR_NO_ERROR;
}

NPError
NPP_SetValue(NPP instance, NPNVariable variable, void *value) 
{
  return NPERR_GENERIC_ERROR;
}

