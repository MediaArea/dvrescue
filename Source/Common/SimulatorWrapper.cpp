/*  Copyright (c) MIPoPS. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-3-Clause license that can
 *  be found in the LICENSE.txt file in the root of the source tree.
 */

#include "Common/SimulatorWrapper.h"
#include "ZenLib/File.h"
#include <vector>
#include <list>
#include <iostream> //TEMP

using namespace std;
using namespace ZenLib;

struct ctl
{
    playback_mode Mode = Playback_Mode_Playing;
    float Speed = FLT_MAX;
    list<float> NextSpeed;
    int NextSpeed_CountDown = 0;
    size_t Pos = 0;
    vector<File*> F;
    size_t MaxParsed = 0;
    FileWrapper* Wrapper = nullptr;
    bool Stop = false;
};

SimulatorWrapper::SimulatorWrapper()
{
}

ctl* Ctl = nullptr;

void SimulatorWrapper::CreateCaptureSession(const ZenLib::Ztring &FileName, FileWrapper* Wrapper)
{
    if (!Ctl)
    {
        Ctl = new ctl;

        for (size_t i = 0;; i++)
        {
            auto FileNameExt = FileName + __T('.') + Ztring::ToZtring(i);
            if (!File::Exists(FileNameExt))
                break;
            Ctl->F.push_back(new File(FileNameExt));
        }
    }
    Ctl->Wrapper = Wrapper;
}

SimulatorWrapper::~SimulatorWrapper()
{
}

void SimulatorWrapper::StartCaptureSession()
{
}

void SimulatorWrapper::StopCaptureSession()
{

}

void SimulatorWrapper::SetPlaybackMode(playback_mode Mode, float Speed)
{
    Ctl->Mode = Mode;
    if (Speed && Ctl->Speed && Ctl->Speed != FLT_MAX)
    {
        Ctl->NextSpeed.push_back(Speed);
        if (!Ctl->NextSpeed_CountDown)
            Ctl->NextSpeed_CountDown = 2;
    }
    else if (!Speed)
    {
        Ctl->NextSpeed.push_back(Ctl->Speed);
        if (!Ctl->NextSpeed_CountDown)
            Ctl->NextSpeed_CountDown = 2;
        Ctl->Stop = true;
    }
    else
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

void SimulatorWrapper::WaitForSessionEnd()
{
    int8u* Buffer = new int8u[120000];
    for (;;)
    {
        if (Ctl->Stop)
        {
            Ctl->Stop = false;
            break;
        }
        if (Ctl->NextSpeed_CountDown)
        {
            Ctl->NextSpeed_CountDown--;
            if (!Ctl->NextSpeed_CountDown)
            {
                Ctl->Speed = Ctl->NextSpeed.front();
                Ctl->NextSpeed.pop_front();
                if (!Ctl->NextSpeed.empty())
                {
                    Ctl->NextSpeed_CountDown = 2;
                }
            }
        }
        if (Ctl->Speed < 0)
        {
            Ctl->F[Ctl->Pos]->GoTo(-120000 * 2, File::FromCurrent);
            Ctl->F[Ctl->Pos]->Position_Get();
        }
        if (Ctl->F[Ctl->Pos]->Read(Buffer, 120000) != 120000)
            break;
        if (!Ctl->NextSpeed_CountDown)
        {
            auto SeekPos = Ctl->F[Ctl->Pos]->Position_Get();
            if (Ctl->Pos && SeekPos > Ctl->MaxParsed)
                Ctl->Pos = 0;
            if (!Ctl->Pos && Ctl->MaxParsed < SeekPos)
                Ctl->MaxParsed = SeekPos;
        }
        Ctl->Wrapper->Parse_Buffer(Buffer, 120000);
    }
}

