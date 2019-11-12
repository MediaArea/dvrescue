/*  Copyright (c) MIPoPS. All Rights Reserved.
 *
 *  Use of this source code is governed by a Unlicense license that can
 *  be found in the LICENSE file in the root of the source tree.
 */

//---------------------------------------------------------------------------
#pragma once
#include "Common/Config.h"
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
#include "MediaInfo/MediaInfo_Events.h"
#include <vector>
using namespace MediaInfoNameSpace;
using namespace std;
//---------------------------------------------------------------------------

//***************************************************************************
// Class file
//***************************************************************************

class file
{
public:
    MediaInfo MI;
    double FrameRate;

    file(const String& FileName);
    ~file();
};
