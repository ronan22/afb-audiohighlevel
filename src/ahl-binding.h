/*
 * Copyright (C) 2017 "Audiokinetic Inc"
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef AHL_BINDING_INCLUDE
#define AHL_BINDING_INCLUDE

#define AFB_BINDING_VERSION 2

//#define AHL_DISCONNECT_POLICY // define for debugging HLB in standalone only

#include <json-c/json.h>
#include <glib.h>

#include <afb/afb-binding.h>
#include "ahl-interface.h"
#include "ahl-policy-utils.h"


#ifndef PUBLIC
  #define PUBLIC
#endif

#define AHL_SUCCESS 0
#define AHL_FAIL 1

#define AHL_ACCESS_CONTROL_GRANTED 1
#define AHL_ACCESS_CONTROL_DENIED 0

#define AHL_UNDEFINED -1
#define AHL_STR_MAX_LENGTH 256


typedef struct RoleInfo {
    char *              pRoleName;          // Role string identifier
    int                 iPriority;          // Role normalized priority (0-100)
    InterruptBehaviorT  eInterruptBehavior; // Role behavior when interrupting lower priority streams
    GPtrArray *         pActionList;        // List of supported actions for the role (gchar*)
    GPtrArray *         pSourceEndpoints;   // Source endpoints info (EndpointInfoT*)
    GPtrArray *         pSinkEndpoints;     // Sink endpoints info (EndpointInfoT*)
} RoleInfoT;

// Parts of the context that are visible to the policy (for state based decisions)
typedef struct AHLPolicyCtx {
    GHashTable *        pRoleInfo;         // Hash table of role information structure (RoleInfoT*) accessed by role name 
    GHashTable *        pStreams;          // List of active streams (StreamInfoT*) accessed by streamID
    GPtrArray *         pHALList;          // List of HAL dependencies
    // TODO: Events need to be sent directly by HLB when separation with policy complete
    struct afb_event    propertyEvent;     // AGL event used when property changes 
    struct afb_event    volumeEvent;       // AGL event used when volume changes
    struct afb_event    postActionEvent;   // AGL event used on post action call
} AHLPolicyCtxT;

// Global binding context
typedef struct AHLCtx {
    AHLPolicyCtxT   policyCtx;
    endpointID_t    nextSourceEndpointID;       // Counter to assign new ID
    endpointID_t    nextSinkEndpointID;         // Counter to assign new ID
    endpointID_t    nextStreamID;               // Counter to assign new ID
} AHLCtxT;

// Client specific binding context
typedef struct AHLClientCtx {
     GArray *        pStreamAccessList;           // List of streams that client has control over
} AHLClientCtxT;

// ahl-binding.c
PUBLIC int AhlBindingInit();
PUBLIC void AhlOnEvent(const char *evtname, json_object *eventJ);

// ahl-deviceenum.c
int  EnumerateDevices(json_object * in_jDeviceArray, char * in_pAudioRole, EndpointTypeT in_deviceType, GPtrArray * out_pEndpointArray);
EndpointInfoT * InitEndpointInfo();
void TermEndpointInfo( EndpointInfoT * out_pEndpointInfo );
// ahl-config.c
int  ParseHLBConfig();
// ahl-policy.c
#ifndef AHL_DISCONNECT_POLICY
PUBLIC void audiohlapi_raise_event(json_object *EventDataJ);
#endif

#endif // AHL_BINDING_INCLUDE
