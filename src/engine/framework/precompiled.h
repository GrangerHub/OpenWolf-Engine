////////////////////////////////////////////////////////////////////////////////////////
// Copyright(C) 2018 - 2019 Dusan Jocic <dusanjocic@msn.com>
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
// File name:   precompiled.h
// Version:     v1.02
// Created:
// Compilers:   Visual Studio 2019, gcc 7.3.0
// Description:
// -------------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////

#ifndef __PRECOMPILED_H__
#define __PRECOMPILED_H__

//Dushan
//FIX ALL THIS

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <setjmp.h>
#include <iostream>
#include <mutex>
#include <queue>

#ifndef _WIN32
#include <sys/ioctl.h>
#endif

#include <fcntl.h>
#include <algorithm>

#include <signal.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <math.h>

#ifdef _WIN32
#include <SDL_syswm.h>
#include <io.h>
#include <shellapi.h>
#include <timeapi.h>
#include <windows.h>
#include <direct.h>
#include <lmerr.h>
#include <lmcons.h>
#include <lmwksta.h>
#include <conio.h>
#include <wincrypt.h>
#include <shlobj.h>
#include <psapi.h>
#include <float.h>
#include <Shobjidl.h>
#include <platform/windows/resource.h>
#pragma fenv_access (on)
#else
#include <sys/types.h>
#include <dirent.h>
#include <libgen.h>
#include <fenv.h>
#include <pwd.h>
#include <unistd.h>
#endif

#include <GPURenderer/qgl.h>

#ifdef _WIN32
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_opengl_glext.h>
#include <SDL_thread.h>
#include <platform/Windows/resource.h>
#else
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <SDL2/SDL_opengl_glext.h>
#include <SDL2/SDL_thread.h>
#endif
#include <curl/curl.h>

#include <framework/appConfig.h>
#include <framework/types.h>
#include <qcommon/q_shared.h>
#include <qcommon/qcommon.h>
#include <API/clientAVI_api.h>
#include <client/clientAVI.h>
#include <API/clientGame_api.h>
#include <client/clientGame.h>
#include <API/sgame_api.h>
#include <audio/s_local.h>
#include <audio/s_codec.h>
#include <audio/s_dmahd.h>

#include <API/download_api.h>
#include <download/downloadLocal.h>
#include <GPURenderer/r_splash.h>
#include <GPURenderer/r_local.h>
#include <GPURenderer/r_fbo.h>
#include <GPURenderer/r_dsa.h>
#include <GPURenderer/r_common.h>
#include <client/client.h>
#include <client/keys.h>
#include <framework/keycodes.h>

#include <framework/Unzip.h>
#include <framework/Puff.h>
#include <framework/SurfaceFlags_Tech3.h>
#include <API/system_api.h>
#include <platform/systemLocal.h>

#include <API/gui_api.h>
#include <cm/cm_local.h>
#include <cm/cm_patch.h>

#include <API/cgame_api.h>

#ifdef _WIN32
#include <freetype/ft2build.h>
#else
#include <freetype2/ft2build.h>
#endif
#undef getch

#ifdef __LINUX_
#include <stdint.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <stdio.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/time.h>
#include <pwd.h>
#include <libgen.h>
#include <fcntl.h>
#include <fenv.h>
#include <CL/cl_gl.h>
#endif

#ifdef _MSC_VER
// MSVC users must install the OpenAL SDK which doesn't use the AL/*.h scheme.
#include <al.h>
#include <alc.h>
#elif defined (MACOS_X)
#include <OpenAL/al.h>
#include <OpenAL/alc.h>
#else
#include <AL/al.h>
#include <AL/alc.h>
#endif

#include <API/FileSystem_api.h>
#include <framework/FileSystem.h>
#include <API/CVarSystem_api.h>
#include <framework/CVarSystem.h>
#include <API/bgame_api.h>
#include <server/serverCcmds.h>
#include <API/serverClient_api.h>
#include <server/serverClient.h>
#include <API/serverGame_api.h>
#include <server/serverGame.h>
#include <API/serverWorld_api.h>
#include <server/serverWorld.h>
#include <API/serverSnapshot_api.h>
#include <server/serverSnapshot.h>
#include <API/serverNetChan_api.h>
#include <server/serverNetChan.h>
#include <API/serverInit_api.h>
#include <server/serverInit.h>
#include <server/server.h>
#include <API/serverMain_api.h>
#include <server/serverMain.h>
#include <API/CmdSystem_api.h>
#include <framework/CmdSystem.h>
#include <API/CmdBuffer_api.h>
#include <framework/CmdBuffer.h>
#include <API/CmdDelay_api.h>
#include <framework/CmdDelay.h>
#include <API/MD4_api.h>
#include <framework/MD4.h>
#include <API/serverDemo_api.h>
#include <server/serverDemo.h>
#include <API/serverCrypto_api.h>
#include <server/serverCrypto.h>
#include <server/serverOACS.h>
#include <server/serverWallhack.h>

// includes for the OGG codec
#include <errno.h>
#define OV_EXCLUDE_STATIC_CALLBACKS
#include <vorbis/vorbisfile.h>

#include <sodium.h>

// curses.h defines COLOR_*, which are already defined in q_shared.h
#undef COLOR_BLACK
#undef COLOR_RED
#undef COLOR_GREEN
#undef COLOR_YELLOW
#undef COLOR_BLUE
#undef COLOR_MAGENTA
#undef COLOR_CYAN
#undef COLOR_WHITE

#include <curses.h>

#include <API/consoleCurses_api.h>
#include <console/consoleCurses.h>
#include <API/consoleLogging_api.h>
#include <console/consoleLogging.h>

#include <framework/Huffman.h>

#include <client/clientBrowser.h>
#include <API/clientLAN_api.h>
#include <client/clientLAN.h>
#include <API/clientGUI_api.h>
#include <client/clientGUI.h>

#endif //!__PRECOMPILED_H__