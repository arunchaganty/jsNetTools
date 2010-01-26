/**
 * @file jsNetTools.cpp
 * @author Arun Tejasvi Chaganty <arunchaganty@gmail.com>
 * @version 0.1
 * 
 * @section Description
 * Entry point for jsNetTools plugin
 * Contains all functions required to setup the plugin and interface with the
 * browser
 */

#include "jsNetTools.h"
#include "jsInterface.h"
#include "NPN.h"
#include "util.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define PLUGIN_NAME         "jsNetTools"
#define PLUGIN_DESCRIPTION  PLUGIN_NAME " (Mozilla SDK)"
#define PLUGIN_VERSION      "1.0.0.0"

NPNetscapeFuncs* gBrowserFuncs = NULL;

/**
 * \struct InstanceData
 *
 * Keeps track of a handle to the browser.
 */
typedef struct InstanceData {
    NPP npp;
    NPWindow window;
} InstanceData;

/**
 * Populates the browsers plugin function table.
 *
 * @param bFuncs - A handle to the browsers functions
 * @param pFuncs - The browser's handle to the plugins functions
 */
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

/**
 * Clean up when the plugin is shutdown
 */
NP_EXPORT(NPError)
NP_Shutdown()
{
    return NPERR_NO_ERROR;
}

/**
 * Plugin version string
 */
NP_EXPORT(char*)
NP_GetPluginVersion()
{
  return (char*)PLUGIN_VERSION;
}

/**
 * Plugin's handled mimetypes
 */
NP_EXPORT(char*)
NP_GetMIMEDescription()
{
  return (char*)"application/nettool::NetTool";
}

/**
 * Allows the browser to get attributes of the plugin
 *
 * @param instance - Plugin instance
 * @param variable - Attribute being requested
 * @param retValue - Value of attribute
 */
NP_EXPORT(NPError)
NP_GetValue(void* instance, NPPVariable variable, void* retValue) 
{
    switch (variable) {
        case NPPVpluginNameString:
            *((char**)retValue) = (char*)PLUGIN_NAME;
            break;
        case NPPVpluginDescriptionString:
            *((char**)retValue) = (char*)PLUGIN_DESCRIPTION;
            break;
        // Whether the plugin is scriptable. Should return a NPObject - which is also a "Javascript object"
        case NPPVpluginScriptableNPObject:
            {
                if (instance)
                {
                    *((NPObject**)retValue) = jsInterface_New((NPP)instance);
                }
                else
                {
                    return NPERR_INVALID_PARAM;
                }
            }
            break;
        default:
            return NPERR_INVALID_PARAM;
            break;
    }
    return NPERR_NO_ERROR;
}

/**
 * Creates a new instance of the plugin.
 *
 * @param pluginType - Actual mimetype
 * @param instance - Browsers handle to plugin instance
 * @param mode - ?
 * @param argc - Number of input key/value pairs for the object constructor
 * @param argn - keys for the object constructor
 * @param argv - values for the object constructor
 * @param saved - data that can be used across multiple plugin instances
 */
NPError
NPP_New(NPMIMEType pluginType, NPP instance, uint16_t mode, 
        int16_t argc, char* argn[], char* argv[], 
        NPSavedData* saved) 
{
    if(instance == NULL)
        return NPERR_INVALID_INSTANCE_ERROR;

    instance->pdata = NULL;
    return NPERR_NO_ERROR;
}

/**
 * Clean up for a plugin instance
 */
NPError
NPP_Destroy(NPP instance, NPSavedData** save)
{
    return NPERR_NO_ERROR;
}

/**
 * Set up for a window - in case the plugin requires to draw on the browser.
 */
NPError
NPP_SetWindow(NPP instance, NPWindow* window) 
{
  return NPERR_GENERIC_ERROR;
}

/**
 * Set up a stream for the plugin
 */
NPError
NPP_NewStream(NPP instance, NPMIMEType type, NPStream* stream, NPBool seekable, uint16_t* stype) 
{
  return NPERR_GENERIC_ERROR;
}

/**
 * Destroy a stream to the plugin
 */
NPError
NPP_DestroyStream(NPP instance, NPStream* stream, NPReason reason) 
{
  return NPERR_GENERIC_ERROR;
}

/**
 * Query if the plugin is ready to be written to. (Stream function)
 */
int32_t
NPP_WriteReady(NPP instance, NPStream* stream) 
{
  return 0;
}

/**
 * Write to the plugin. (Stream function)
 */
int32_t
NPP_Write(NPP instance, NPStream* stream, int32_t offset, int32_t len, void* buffer) 
{
  return 0;
}

/**
 * Construct the stream as a file
 */
void
NPP_StreamAsFile(NPP instance, NPStream* stream, const char* fname) 
{

}

/**
 * Print the object (?)
 */
void
NPP_Print(NPP instance, NPPrint* platformPrint) 
{

}

/**
 * Handle browser events
 */
int16_t
NPP_HandleEvent(NPP instance, void* event) 
{
    return 0;
}

/**
 * ?
 */
void
NPP_URLNotify(NPP instance, const char* URL, NPReason reason, void* notifyData) 
{
}

/**
 * Allows the browser to get attributes of the plugin
 *
 * @param instance - Plugin instance
 * @param variable - Attribute being requested
 * @param value - Value of attribute
 */
NPError
NPP_GetValue(NPP instance, NPPVariable variable, void *value) 
{
    switch (variable) {
        // Whether the plugin is scriptable. Should return a NPObject - which is also a "Javascript object"
        case NPPVpluginScriptableNPObject:
            {
                if (instance)
                {
                    *((NPObject**)value) = jsInterface_New(instance);
                }
                else
                {
                    return NPERR_INVALID_PARAM;
                }
            }
            break;
        default:
            return NPERR_INVALID_PARAM;
            break;
    }
    return NPERR_NO_ERROR;
}

/**
 * Allows the browser to set attributes of the plugin
 *
 * @param instance - Plugin instance
 * @param variable - Attribute being set
 * @param value - Value of attribute
 */
NPError
NPP_SetValue(NPP instance, NPNVariable variable, void *value) 
{
  return NPERR_GENERIC_ERROR;
}

