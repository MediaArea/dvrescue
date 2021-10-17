/*  Copyright (c) MIPoPS. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-3-Clause license that can
 *  be found in the LICENSE.txt file in the root of the source tree.
 */

#include "Common/SimulatorWrapper.h"
#include "ZenLib/File.h"
#include <vector>
#include <iostream> //TEMP

using namespace std;
using namespace ZenLib;

struct ctl
{
    playback_mode Mode;
    float Speed;
};

SimulatorWrapper::SimulatorWrapper()
{
}

void SimulatorWrapper::CreateCaptureSession(const ZenLib::Ztring &FileName, FileWrapper* Wrapper)
{
    Ctl = new ctl;
    ((ctl*)Ctl)->Mode = Playback_Mode_Playing;
    ((ctl*)Ctl)->Speed = 1.0;

    vector<File*> F;
    if (File::Exists(FileName))
      F.push_back(new File(FileName));
    //for (;;)
    {
        
    }

    int8u* Buffer = new int8u[120000];
    for (;;)
    {
        if (((ctl*)Ctl)->Speed < 0)
            F[0]->GoTo(-120000*2, File::FromCurrent);
        if (F[0]->Read(Buffer, 120000)!=120000)
            break;
        Wrapper->Parse_Buffer(Buffer, 120000);
    }
}

SimulatorWrapper::~SimulatorWrapper()
{
}

void SimulatorWrapper::SetPlaybackMode(playback_mode Mode, float Speed)
{
    ((ctl*)Ctl)->Mode = Mode;
    ((ctl*)Ctl)->Speed = Speed;
}
