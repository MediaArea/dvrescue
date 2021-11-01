/*  Copyright (c) MIPoPS. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-3-Clause license that can
 *  be found in the LICENSE.txt file in the root of the source tree.
 */

#include "Common/ProcessFileWrapper.h"
#include "Common/ProcessFile.h"
#include "iostream"
#include "iomanip"

using namespace std;

FileWrapper::FileWrapper(file* File)
{
    Files.push_back(File);
}

void FileWrapper::Parse_Buffer(const uint8_t* Buffer, size_t Buffer_Size)
{
  for (size_t Buffer_Offset = 0; Buffer_Offset < Buffer_Size; Buffer_Offset++) {
    int A = 0;
    
    switch (Buffer[Buffer_Offset] & 0xE0) {
      case 0x20:
        for (size_t Pos = 0; Pos < 48; Pos += 8) {
          auto PackType = Buffer[Buffer_Offset + 3 + Pos + 3];

          // dv_timecode
          if (PackType == 0x13)  // Pack type=0x13 (dv_timecode)
          {
            bool DropFrame =
                (Buffer[Buffer_Offset + 3 + Pos + 3 + 1] & 0x40) ? true : false;
            auto Frames =
                ((Buffer[Buffer_Offset + 3 + Pos + 3 + 1] & 0x30) >> 4) * 10 +
                ((Buffer[Buffer_Offset + 3 + Pos + 3 + 1] & 0x0F));
            auto Seconds =
                ((Buffer[Buffer_Offset + 3 + Pos + 3 + 2] & 0x70) >> 4) * 10 +
                ((Buffer[Buffer_Offset + 3 + Pos + 3 + 2] & 0x0F));
            auto Minutes =
                ((Buffer[Buffer_Offset + 3 + Pos + 3 + 3] & 0x70) >> 4) * 10 +
                ((Buffer[Buffer_Offset + 3 + Pos + 3 + 3] & 0x0F));
            auto Hours =
                ((Buffer[Buffer_Offset + 3 + Pos + 3 + 4] & 0x30) >> 4) * 10 +
                ((Buffer[Buffer_Offset + 3 + Pos + 3 + 4] & 0x0F));

            TimeCode TC(Hours, Minutes, Seconds, Frames, 30, DropFrame);
            /* cerr
                << "\nDV " << dec << Buffer_Size << " " << File_Seek_IsUsed
                << " " << File_Pos << " " << hex 
              << TC.ToString()  << "\n";*/
            A = 1;
            break;
          }
        }
        if (A)
          break;
    }
    if (A)
      break;
  }

    if (File_Seek_IsUsed)
        File_Seek->Parse_Buffer(Buffer, Buffer_Size);
    else
        Files[File_Pos]->Parse_Buffer(Buffer, Buffer_Size);
}
