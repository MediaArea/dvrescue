/*  Copyright (c) MIPoPS. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-3-Clause license that can
 *  be found in the LICENSE.txt file in the root of the source tree.
 */

//---------------------------------------------------------------------------
#pragma once

#include <cstddef> //for std::size_t, native size_t isn't avaiable in obj-c++ mode
#include <string>
#include <vector>
#ifdef MEDIAINFO_DLL
#include "MediaInfoDLL/MediaInfoDLL.h"
#define MediaInfoNameSpace MediaInfoDLL
#elif defined MEDIAINFO_STATIC
#include "MediaInfoDLL/MediaInfoDLL_Static.h"
#define MediaInfoNameSpace MediaInfoDLL
#else
#include "MediaInfo/MediaInfoList.h"
#define MediaInfoNameSpace MediaInfoLib
#endif

class file;

//***************************************************************************
// Class FileWrapper
//***************************************************************************

class Thread_Frames
{
public:
    Thread_Frames();
    void Open(const MediaInfoNameSpace::String& FileName);
    void operator()();
    void Parse_Buffer(const std::uint8_t* Buffer, std::size_t Buffer_Size);
};

class FileWrapper {
public:
    FileWrapper(file* File);
    FileWrapper(Thread_Frames* Thr)
    {
        Th = Thr;
    }
    void Parse_Buffer(const std::uint8_t* Buffer, std::size_t Buffer_Size);

    file* File_Seek = nullptr;
    bool File_Seek_IsUsed = false;
    size_t File_Pos = 0;
    std::vector<file*> Files;

    uint8_t* Buffer_LastFrame = nullptr;
    Thread_Frames* Th = nullptr;

 private:
};
