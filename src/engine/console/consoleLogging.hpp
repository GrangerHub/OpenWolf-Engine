////////////////////////////////////////////////////////////////////////////////////////
// Copyright(C) 2018 - 2021 Dusan Jocic <dusanjocic@msn.com>
//
// This file is part of the OpenWolf GPL Source Code.
// OpenWolf Source Code is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// OpenWolf Source Code is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with OpenWolf Source Code.  If not, see <http://www.gnu.org/licenses/>.
//
// In addition, the OpenWolf Source Code is also subject to certain additional terms.
// You should have received a copy of these additional terms immediately following the
// terms and conditions of the GNU General Public License which accompanied the
// OpenWolf Source Code. If not, please request a copy in writing from id Software
// at the address below.
//
// If you have questions concerning this license or the applicable additional terms,
// you may contact in writing id Software LLC, c/o ZeniMax Media Inc.,
// Suite 120, Rockville, Maryland 20850 USA.
//
// -------------------------------------------------------------------------------------
// File name:   consoleLogging.hpp
// Created:
// Compilers:   Microsoft (R) C/C++ Optimizing Compiler Version 19.26.28806 for x64,
//              gcc (Ubuntu 9.3.0-10ubuntu2) 9.3.0
// Description:
// -------------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////

#ifndef __CONSOLE_LOGGING_H__
#define __CONSOLE_LOGGING_H__

#define MAX_LOG 32768u

static valueType consoleLog[MAX_LOG];
static uint64 writePos = 0;
static uint64 readPos = 0;

//
// idConsoleLoggingSystemLocal
//
class idConsoleLoggingSystemLocal : public idConsoleLoggingSystem {
public:
    idConsoleLoggingSystemLocal();
    ~idConsoleLoggingSystemLocal();

    virtual uint64 LogWrite(pointer in);
    virtual uint64 LogRead(valueType *out, uint64 outSize);

    static uint64 LogSize(void);
    static uint64 LogFree(void);
private:
};

extern idConsoleLoggingSystemLocal consoleLoggingLocal;

#endif //!__CONSOLE_LOGGING_H__
