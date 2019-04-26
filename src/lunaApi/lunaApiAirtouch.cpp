// Copyright (c) 2018-2019 LG Electronics, Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
// SPDX-License-Identifier: Apache-2.0

#include <string.h>
#include <json-c/json.h>

#include "logging.h"
#include "lunaApiAirtouch.h"

// using namespace speechRecognition;

lunaApiAirtouch* lunaApiAirtouch::pInstance = NULL;

const LSMethod lunaApiAirtouch::rootCategory[] = {
    { "start",                  start,                  LUNA_METHOD_FLAGS_NONE},
    { "stop",                   stop,                   LUNA_METHOD_FLAGS_NONE},
    { "getState",               getState,               LUNA_METHOD_FLAGS_NONE},
    { "getResponse",            getResponse,            LUNA_METHOD_FLAGS_NONE},
    { NULL, NULL },
};

const lunaApiAirtouch::serviceApi lunaApiAirtouch::airtouchApis[] = {
    { "/", (LSMethod *)rootCategory },
    { NULL, NULL },
};

lunaApiAirtouch::lunaApiAirtouch() {
    // static const char *airtouchServiceId = "com.webos.service.ai.voice";
    static const char *airtouchServiceId = "com.lg.app.signage.dev.service";

    pApis       = (serviceApi *)airtouchApis;
    serviceId   = airtouchServiceId;

    // TO-DO : register callback function
    // mSrw.register_event_Callback((pfnEventCB)postEvent);
}

lunaApiAirtouch::~lunaApiAirtouch() {
}

// API implementations.
bool lunaApiAirtouch::start(LSHandle *sh, LSMessage *msg, void *data) {
    json_object *object = json_tokener_parse(LSMessageGetPayload(msg));
    if (!object) {
        Instance()->LSMessageReplyErrorBadJSON(sh, msg);
        return true;
    }

    // json_object *v;

    // const char *mode        = json_object_object_get_ex(object, "mode", &v) ? json_object_get_string(v) : NULL;
    // json_object *keyword    = json_object_object_get_ex(object, "keywordDetect", &v) ? v : NULL;
    // const bool useKeyword   = keyword ? json_object_get_boolean(keyword) : false;
    const char *clientName  = LSMessageGetSenderServiceName(msg);
    const char *clientId    = LSMessageGetUniqueToken(msg);
    const char *appId       = LSMessageGetApplicationID(msg);

    AI_LOG_INFO(MSGID_LUNASERVICE, 0, "[ %s : %d ] %s( ... ) clientName = %s, applicationId = %s, clientId = %s, mode = %s, useKeyword = %d", __FILE__, __LINE__, __FUNCTION__, clientName, appId, clientId, mode, useKeyword);

    ERROR_CODE ret = INVALID_PARAM;
    // TO-DO : start function
    // if (mode != NULL && keyword != NULL) ret = Instance()->mSrw.start((char *)mode, useKeyword);

    char *payload = NULL;
    if (ret == NONE) payload = g_strdup_printf("{\"returnValue\":true}");
    else payload = g_strdup_printf("{\"returnValue\":false,\"errorCode\":%d,\"errorText\":\"%s\"}", ret, errorStr(ret));

    Instance()->LSMessageReplySuccess(sh, msg, payload);
    g_free(payload);

    return true;
}

bool lunaApiAirtouch::stop(LSHandle *sh, LSMessage *msg, void *data) {
    json_object *object = json_tokener_parse(LSMessageGetPayload(msg));
    if (!object) {
        Instance()->LSMessageReplyErrorBadJSON(sh, msg);
        return true;
    }

    const char *clientName  = LSMessageGetSenderServiceName(msg);
    const char *clientId    = LSMessageGetUniqueToken(msg);
    const char *appId       = LSMessageGetApplicationID(msg);

    AI_LOG_INFO(MSGID_LUNASERVICE, 0, "[ %s : %d ] %s( ... ) clientName = %s, applicationId = %s, clientId = %s", __FILE__, __LINE__, __FUNCTION__, clientName, appId, clientId);

    // TO-DO: stop function
    // ERROR_CODE ret = Instance()->mSrw.stop();

    char *payload = NULL;
    if (ret == NONE) payload = g_strdup_printf("{\"returnValue\":true}");
    else payload = g_strdup_printf("{\"returnValue\":false,\"errorCode\":%d,\"errorText\":\"%s\"}", ret, errorStr(ret));

    Instance()->LSMessageReplySuccess(sh, msg, payload);
    g_free(payload);

    return true;
}

bool lunaApiAirtouch::getResponse(LSHandle *sh, LSMessage *msg, void *data) {
    AI_LOG_INFO(MSGID_LUNASERVICE, 0, "[ %s : %d ] %s( ... ) , payload = %s" , __FILE__ , __LINE__ , __FUNCTION__ , LSMessageGetPayload(msg));

    json_object *object = json_tokener_parse(LSMessageGetPayload(msg));
    if (!object) {
        Instance()->LSMessageReplyErrorBadJSON(sh, msg);
        return true;
    }

    json_object *v;

    const bool subscribe    = json_object_object_get_ex(object, "subscribe", &v) ? json_object_get_boolean(v) : false;
    const char *clientName  = LSMessageGetSenderServiceName(msg);
    const char *clientId    = LSMessageGetUniqueToken(msg);
    const char *appId       = LSMessageGetApplicationID(msg);

    AI_LOG_INFO(MSGID_LUNASERVICE, 0, "[ %s : %d ] %s( ... ) clientName = %s, applicationId = %s, clientId = %s", __FILE__, __LINE__, __FUNCTION__, clientName, appId, clientId);

    if (subscribe) {
        LSError lserror;
        LSErrorInit(&lserror);

        if (!LSSubscriptionAdd(sh, subscription_key_response, msg, &lserror)) {
            LSErrorPrint(&lserror, stderr);
            LSErrorFree(&lserror);
            return true;
        }
    }

    const char *subscribed = subscribe ? "true" : "false";
    char *payload    = g_strdup_printf("{\"returnValue\":true,\"subscribed\":%s}", subscribed);

    AI_LOG_INFO(MSGID_LUNASERVICE, 0, "[ %s : %d ] %s( ... ), payload = %s" , __FILE__, __LINE__, __FUNCTION__, payload);

    Instance()->LSMessageReplySuccess(sh, msg, payload);

    g_free(payload);

    return true;
}

void lunaApiAirtouch::postEvent(void *subscribeKey, void *payload) {
    lunaApiBase::postEvent(Instance()->pLSHandle, subscribeKey, payload);
}
