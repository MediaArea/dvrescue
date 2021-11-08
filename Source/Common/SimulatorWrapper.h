/*  Copyright (c) MIPoPS. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-3-Clause license that can
 *  be found in the LICENSE.txt file in the root of the source tree.
 */

//---------------------------------------------------------------------------
#pragma once

#include <string>

#include "Common/ProcessFileWrapper.h"
#include "ZenLib/Ztring.h"

//***************************************************************************
// Enums
//***************************************************************************

enum playback_mode {
    Playback_Mode_NotPlaying=0,
    Playback_Mode_Playing
};

//***************************************************************************
// Class SimulatorWrapper
//***************************************************************************

class SimulatorWrapper {
 public:
    // Constructor/Destructor
    SimulatorWrapper(const ZenLib::Ztring& FileName);
    ~SimulatorWrapper();

    // Functions
    void CreateCaptureSession(FileWrapper* Wrapper);
    void StartCaptureSession();
    void StopCaptureSession();
    void SetPlaybackMode(playback_mode Mode, float Speed);
    void WaitForSessionEnd();

private:
    void* Priv;
};
