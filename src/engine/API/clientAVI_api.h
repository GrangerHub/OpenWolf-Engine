////////////////////////////////////////////////////////////////////////////////////////
// Copyright(C) 2019 Dusan Jocic <dusanjocic@msn.com>
//
// This file is part of OpenWolf.
//
// OpenWolf is free software; you can redistribute it
// and / or modify it under the terms of the GNU General Public License as
// published by the Free Software Foundation; either version 2 of the License,
// or (at your option) any later version.
//
// OpenWolf is distributed in the hope that it will be
// useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with OpenWolf; if not, write to the Free Software
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110 - 1301  USA
//
// -------------------------------------------------------------------------------------
// File name:   clientAVI_api.h
// Version:     v1.00
// Created:
// Compilers:   Visual Studio 2019, gcc 7.3.0
// Description:
// -------------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////

#ifndef __CLIENTAVI_API_H__
#define __CLIENTAVI_API_H__

//
// idClientAVISystemAPI
//
class idClientAVISystemAPI
{
public:
    virtual void WriteAVIVideoFrame( const U8* imageBuffer, S32 size ) = 0;
    virtual void WriteAVIAudioFrame( const U8* pcmBuffer, S32 size ) = 0;
    virtual void TakeVideoFrame( void ) = 0;
    virtual bool CloseAVI( void ) = 0;
    virtual bool VideoRecording( void ) = 0;
    virtual bool OpenAVIForWriting( StringEntry fileName ) = 0;
};

extern idClientAVISystemAPI* clientAVISystem;

#endif // !__CLIENTAVI_API_H__
