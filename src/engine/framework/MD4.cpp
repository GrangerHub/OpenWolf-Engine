﻿/*
	mdfour.c

	An implementation of MD4 designed for use in the samba SMB
	authentication protocol

	Copyright (C) 1997-1998  Andrew Tridgell

	This program is free software; you can redistribute it and/or
	modify it under the terms of the GNU General Public License
	as published by the Free Software Foundation; either version 2
	of the License, or (at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

	See the GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program; if not, write to:

		Free Software Foundation, Inc.
		59 Temple Place - Suite 330
		Boston, MA  02111-1307, USA

	$Id: mdfour.c,v 1.1 2002/08/23 22:03:27 abster Exp $
*/

#ifdef UPDATE_SERVER
#include <null/null_autoprecompiled.h>
#elif DEDICATED
#include <null/null_serverprecompiled.h>
#else
#include <framework/precompiled.h>
#endif

idMD4SystemLocal MD4SystemLocal;
idMD4System* MD4System = &MD4SystemLocal;

/*
===============
idMD4SystemLocal::idMD4SystemLocal
===============
*/
idMD4SystemLocal::idMD4SystemLocal( void )
{
}

/*
===============
idMD4SystemLocal::~idMD4SystemLocal
===============
*/
idMD4SystemLocal::~idMD4SystemLocal( void )
{
}

/*
===============
idMD4SystemLocal::mdfour64

this applies md4 to 64 U8 chunks
===============
*/
void idMD4SystemLocal::mdfour64( U32* M )
{
    S32 j;
    U32 AA, BB, CC, DD;
    U32 X[16];
    U32 A, B, C, D;
    
    for( j = 0; j < 16; j++ )
    {
        X[j] = M[j];
    }
    
    A = m->A;
    B = m->B;
    C = m->C;
    D = m->D;
    AA = A;
    BB = B;
    CC = C;
    DD = D;
    
    ROUND1( A, B, C, D,  0,  3 );
    ROUND1( D, A, B, C,  1,  7 );
    ROUND1( C, D, A, B,  2, 11 );
    ROUND1( B, C, D, A,  3, 19 );
    ROUND1( A, B, C, D,  4,  3 );
    ROUND1( D, A, B, C,  5,  7 );
    ROUND1( C, D, A, B,  6, 11 );
    ROUND1( B, C, D, A,  7, 19 );
    ROUND1( A, B, C, D,  8,  3 );
    ROUND1( D, A, B, C,  9,  7 );
    ROUND1( C, D, A, B, 10, 11 );
    ROUND1( B, C, D, A, 11, 19 );
    ROUND1( A, B, C, D, 12,  3 );
    ROUND1( D, A, B, C, 13,  7 );
    ROUND1( C, D, A, B, 14, 11 );
    ROUND1( B, C, D, A, 15, 19 );
    
    ROUND2( A, B, C, D,  0,  3 );
    ROUND2( D, A, B, C,  4,  5 );
    ROUND2( C, D, A, B,  8,  9 );
    ROUND2( B, C, D, A, 12, 13 );
    ROUND2( A, B, C, D,  1,  3 );
    ROUND2( D, A, B, C,  5,  5 );
    ROUND2( C, D, A, B,  9,  9 );
    ROUND2( B, C, D, A, 13, 13 );
    ROUND2( A, B, C, D,  2,  3 );
    ROUND2( D, A, B, C,  6,  5 );
    ROUND2( C, D, A, B, 10,  9 );
    ROUND2( B, C, D, A, 14, 13 );
    ROUND2( A, B, C, D,  3,  3 );
    ROUND2( D, A, B, C,  7,  5 );
    ROUND2( C, D, A, B, 11,  9 );
    ROUND2( B, C, D, A, 15, 13 );
    
    ROUND3( A, B, C, D,  0,  3 );
    ROUND3( D, A, B, C,  8,  9 );
    ROUND3( C, D, A, B,  4, 11 );
    ROUND3( B, C, D, A, 12, 15 );
    ROUND3( A, B, C, D,  2,  3 );
    ROUND3( D, A, B, C, 10,  9 );
    ROUND3( C, D, A, B,  6, 11 );
    ROUND3( B, C, D, A, 14, 15 );
    ROUND3( A, B, C, D,  1,  3 );
    ROUND3( D, A, B, C,  9,  9 );
    ROUND3( C, D, A, B,  5, 11 );
    ROUND3( B, C, D, A, 13, 15 );
    ROUND3( A, B, C, D,  3,  3 );
    ROUND3( D, A, B, C, 11,  9 );
    ROUND3( C, D, A, B,  7, 11 );
    ROUND3( B, C, D, A, 15, 15 );
    
    A += AA;
    B += BB;
    C += CC;
    D += DD;
    
    for( j = 0; j < 16; j++ )
    {
        X[j] = 0;
    }
    
    m->A = A;
    m->B = B;
    m->C = C;
    m->D = D;
}

/*
===============
idMD4SystemLocal::copy64
===============
*/
void idMD4SystemLocal::copy64( U32* M, U8* in )
{
    S32 i;
    
    for( i = 0; i < 16; i++ )
    {
        M[i] = ( in[i * 4 + 3] << 24 ) | ( in[i * 4 + 2] << 16 ) |
               ( in[i * 4 + 1] << 8 ) | ( in[i * 4 + 0] << 0 );
    }
}

/*
===============
idMD4SystemLocal::copy4
===============
*/
void idMD4SystemLocal::copy4( U8* out, U32 x )
{
    out[0] = x & 0xFF;
    out[1] = ( x >> 8 ) & 0xFF;
    out[2] = ( x >> 16 ) & 0xFF;
    out[3] = ( x >> 24 ) & 0xFF;
}

/*
===============
idMD4SystemLocal::mdfour_begin
===============
*/
void idMD4SystemLocal::mdfour_begin( struct mdfour* md )
{
    md->A = 0x67452301;
    md->B = 0xefcdab89;
    md->C = 0x98badcfe;
    md->D = 0x10325476;
    md->totalN = 0;
}

/*
===============
idMD4SystemLocal::mdfour_tail
===============
*/
void idMD4SystemLocal::mdfour_tail( U8* in, S32 n )
{
    U8 buf[128];
    U32 M[16];
    U32 b;
    
    m->totalN += n;
    
    b = m->totalN * 8;
    
    ::memset( buf, 0, 128 );
    if( n ) ::memcpy( buf, in, n );
    buf[n] = 0x80;
    
    if( n <= 55 )
    {
        copy4( buf + 56, b );
        copy64( M, buf );
        mdfour64( M );
    }
    else
    {
        copy4( buf + 120, b );
        copy64( M, buf );
        mdfour64( M );
        copy64( M, buf + 64 );
        mdfour64( M );
    }
}

/*
===============
idMD4SystemLocal::mdfour_update
===============
*/
void idMD4SystemLocal::mdfour_update( struct mdfour* md, U8* in, S32 n )
{
    U32 M[16];
    
    m = md;
    
    if( n == 0 ) mdfour_tail( in, n );
    
    while( n >= 64 )
    {
        copy64( M, in );
        mdfour64( M );
        in += 64;
        n -= 64;
        m->totalN += 64;
    }
    
    mdfour_tail( in, n );
}

/*
===============
idMD4SystemLocal::mdfour_result
===============
*/
void idMD4SystemLocal::mdfour_result( struct mdfour* md, U8* out )
{
    m = md;
    
    copy4( out, m->A );
    copy4( out + 4, m->B );
    copy4( out + 8, m->C );
    copy4( out + 12, m->D );
}

/*
===============
idMD4SystemLocal::mdfour
===============
*/
void idMD4SystemLocal::mdfour( U8* out, U8* in, S32 n )
{
    struct mdfour md;
    mdfour_begin( &md );
    mdfour_update( &md, in, n );
    mdfour_result( &md, out );
}

/*
===============
idMD4SystemLocal::mdfour_hex
===============
*/
void idMD4SystemLocal::mdfour_hex( const U8 md4[16], S32 hex[32] )
{
    static const UTF8 digits[] = "0123456789abcdef";
    
    S32 i, j, t;
    for( i = 0, j = 0; i < 16; i += 1, j += 2 )
    {
        // high nibble
        t = ( md4[i] & 0xf0 ) >> 4;
        hex[j] = digits[t];
        // low nibble
        t = md4[i] & 0x0f;
        hex[j + 1] = digits[t];
    }
}

/*
===============
idMD4SystemLocal::BlockChecksum
===============
*/
U32 idMD4SystemLocal::BlockChecksum( const void* buffer, S32 length )
{
    S32	digest[4];
    U32	val;
    
#ifdef USE_OPENSSL
    MD4( ( U8* )buffer, length, ( U8* )digest );
    return digest[0] ^ digest[1] ^ digest[2] ^ digest[3];
#else
    mdfour( ( U8* )digest, ( U8* )buffer, length );
    val = digest[0] ^ digest[1] ^ digest[2] ^ digest[3];
    
    return val;
#endif	/* USE_OPENSSL */
}
