//
// Copyright 2016 Pixar
//
// Licensed under the Apache License, Version 2.0 (the "Apache License")
// with the following modification; you may not use this file except in
// compliance with the Apache License and the following modification to it:
// Section 6. Trademarks. is deleted and replaced with:
//
// 6. Trademarks. This License does not grant permission to use the trade
//    names, trademarks, service marks, or product names of the Licensor
//    and its affiliates, except as required to comply with Section 4(c) of
//    the License and to reproduce the content of the NOTICE file.
//
// You may obtain a copy of the Apache License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the Apache License with the above modification is
// distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
// KIND, either express or implied. See the Apache License for the specific
// language governing permissions and limitations under the Apache License.
//
#include "usdMaya/stageCache.h"

#include <maya/MGlobal.h>
#include <maya/MSceneMessage.h>


static
void
_OnMayaSceneUpdateCallback(void* clientData)
{
    MGlobal::displayInfo("Clearing USD Stage Cache");
    UsdMayaStageCache::Clear();
}

/* static */
UsdStageCache&
UsdMayaStageCache::Get(bool forcePopulate)
{
    static UsdStageCache theCacheForcePopulate;
    static UsdStageCache theCache;

    // IMPORTANT: At every NEW scene in Maya we clear the USD stage cache.
    static MCallbackId sceneUpdateCallbackId = 0;
    if (sceneUpdateCallbackId == 0) {
        sceneUpdateCallbackId = 
            MSceneMessage::addCallback(MSceneMessage::kSceneUpdate,
                                       _OnMayaSceneUpdateCallback);
    }

    return forcePopulate ? theCacheForcePopulate : theCache;
}

/* static */
void 
UsdMayaStageCache::Clear()
{
    UsdMayaStageCache::Get(true).Clear();
    UsdMayaStageCache::Get(false).Clear();
}
