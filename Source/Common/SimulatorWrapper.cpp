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
    playback_mode Mode = Playback_Mode_Playing;
    float Speed = 1.0;
    size_t Pos = 0;
    vector<File*> F;
    size_t MaxParsed = 0;
};

SimulatorWrapper::SimulatorWrapper()
{
}

void SimulatorWrapper::CreateCaptureSession(const ZenLib::Ztring &FileName, FileWrapper* Wrapper)
{
    auto Ctl = new ctl;
    Priv = Ctl;

    for(size_t i=0;;i++)
    {
        auto FileNameExt=FileName+__T('.')+Ztring::ToZtring(i);
        if (!File::Exists(FileNameExt))
            break;
        Ctl->F.push_back(new File(FileNameExt));
    }

    int8u* Buffer = new int8u[120000];
    for (;;)
    {
        if (Ctl->Speed == 0)
            break;
        if (Ctl->Speed < 0)
        {
            Ctl->F[Ctl->Pos]->GoTo(-120000 * 2, File::FromCurrent);
            Ctl->F[Ctl->Pos]->Position_Get();
        }
        if (Ctl->F[Ctl->Pos]->Read(Buffer, 120000)!=120000)
            break;
        auto SeekPos = Ctl->F[Ctl->Pos]->Position_Get();
        if (Ctl->Pos && SeekPos > Ctl->MaxParsed)
            Ctl->Pos = 0;
        if (!Ctl->Pos && Ctl->MaxParsed < SeekPos)
            Ctl->MaxParsed = SeekPos;
        Wrapper->Parse_Buffer(Buffer, 120000);
    }
}

SimulatorWrapper::~SimulatorWrapper()
{
}

void SimulatorWrapper::SetPlaybackMode(playback_mode Mode, float Speed)
{
    auto Ctl = (ctl*)Priv;
    Ctl->Mode = Mode;
    Ctl->Speed = Speed;
    if (Speed < 0)
    {
        auto SeekPos = Ctl->F[Ctl->Pos]->Position_Get();
        if (Ctl->Pos + 1 < Ctl->F.size())
            Ctl->Pos++;
        Ctl->F[Ctl->Pos]->GoTo(SeekPos);
        Ctl->F[Ctl->Pos]->Position_Get();
    }
}
