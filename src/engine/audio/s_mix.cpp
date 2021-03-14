////////////////////////////////////////////////////////////////////////////////////////
// Copyright(C) 1999 - 2010 id Software LLC, a ZeniMax Media company.
// Copyright(C) 2011 - 2021 Dusan Jocic <dusanjocic@msn.com>
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
// File name:   s_mix.cpp
// Created:
// Compilers:   Microsoft (R) C/C++ Optimizing Compiler Version 19.26.28806 for x64,
//              gcc (Ubuntu 9.3.0-10ubuntu2) 9.3.0
// Description: portable code to mix sounds for s_dma.cpp
// -------------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////

#include <framework/precompiled.hpp>

static portable_samplepair_t paintbuffer[PAINTBUFFER_SIZE];
static sint snd_vol;

// bk001119 - these not static, required by unix/snd_mixa.s
sint*     snd_p;
sint      snd_linear_count;
schar16*   snd_out;

static void S_WriteLinearBlastStereo16( void )
{
    sint v1, v2;
    
    for( sint i = 0; i < snd_linear_count; i += 2 )
    {
        v1 = snd_p[i] >> 8;
        if( v1 > 32767 )
        {
            v1 = 32767;
        }
        else if( v1 < -32768 )
        {
            v1 = -32768;
        }
        
        v2 = snd_p[i + 1] >> 8;
        if( v2 > 32767 )
        {
            v2 = 32767;
        }
        else if( v2 < -32768 )
        {
            v2 = -32768;
        }
        
        *reinterpret_cast<uint*>( &snd_out[i] ) = ( v2 << 16 ) | ( v1 & 0xFFFF );
    }
}

void S_TransferStereo16( uint32* pbuf, sint endtime )
{
    sint lpos;
    sint ls_paintedtime;
    
    ls_paintedtime = s_paintedtime;
    
    while( ls_paintedtime < endtime )
    {
        // handle recirculating buffer issues
        lpos = ls_paintedtime % dma.fullsamples;
        
        snd_out = reinterpret_cast<schar16*>( pbuf ) + ( lpos << 1 );
        
        snd_linear_count = dma.fullsamples - lpos;
        if( ls_paintedtime + snd_linear_count > endtime )
        {
            snd_linear_count = endtime - ls_paintedtime;
        }
        
        snd_linear_count <<= 1;
        
        // write a linear blast of samples
        S_WriteLinearBlastStereo16();
        
        snd_p += snd_linear_count;
        ls_paintedtime += ( snd_linear_count >> 1 );
        
        if( clientAVISystem->VideoRecording() )
        {
            clientAVISystem->WriteAVIAudioFrame( reinterpret_cast<uchar8*>( snd_out ), snd_linear_count << 1 );
        }
    }
}

/*
===================
S_TransferPaintBuffer
===================
*/
void S_TransferPaintBuffer( sint endtime, portable_samplepair_t paintbuffer[] )
{
    sint out_idx;
    sint count;
    sint* p;
    sint step;
    sint val;
    sint i;
    uint32* pbuf;
    
    pbuf = reinterpret_cast<uint32*>( dma.buffer );
    
    if( s_testsound->integer )
    {
        sint count;
        
        // write a fixed sine wave
        count = ( endtime - s_paintedtime );
        for( i = 0; i < count; i++ )
        {
            paintbuffer[i].left = paintbuffer[i].right = sin( ( s_paintedtime + i ) * 0.1 ) * 20000 * 256;
        }
    }
    
    if( dma.samplebits == 16 && dma.channels == 2 )
    {
        snd_p = reinterpret_cast<sint*>( paintbuffer );
        
        // optimized case
        S_TransferStereo16( pbuf, endtime );
    }
    else
    {
        // general case
        p = reinterpret_cast< sint* >( paintbuffer );
        count = ( endtime - s_paintedtime ) * dma.channels;
        out_idx = ( s_paintedtime * dma.channels ) % dma.samples;
        step = 3 - MIN( dma.channels, 2 );
        
        if( dma.samplebits == 16 )
        {
            schar16* out = reinterpret_cast<schar16*>( pbuf );
            for( i = 0; i < count; i++ )
            {
                if( ( i % dma.channels ) >= 2 )
                {
                    val = 0;
                }
                else
                {
                    val = *p >> 8;
                    p += step;
                }
                
                if( val > 0x7fff )
                {
                    val = 0x7fff;
                }
                else if( val < -32768 )
                {
                    val = -32768;
                }
                
                out[out_idx] = val;
                out_idx = ( out_idx + 1 ) % dma.samples;
            }
        }
        else if( dma.samplebits == 8 )
        {
            uchar8* out = reinterpret_cast<uchar8*>( pbuf );
            for( i = 0; i < count; i++ )
            {
                if( ( i % dma.channels ) >= 2 )
                {
                    val = 0;
                }
                else
                {
                    val = *p >> 8;
                    p += step;
                }
                
                if( val > 0x7fff )
                {
                    val = 0x7fff;
                }
                else if( val < -32768 )
                {
                    val = -32768;
                }
                
                out[out_idx] = ( val >> 8 ) + 128;
                out_idx = ( out_idx + 1 ) % dma.samples;
            }
        }
    }
}


/*
===============================================================================

CHANNEL MIXING

===============================================================================
*/

static void S_PaintChannelFrom16_scalar( channel_t* ch, const sfx_t* sc, sint count, sint sampleOffset, sint bufferOffset )
{
    sint data, aoff, boff;
    sint leftvol, rightvol;
    sint i, j;
    portable_samplepair_t* samp;
    sndBuffer* chunk;
    schar16* samples;
    float32 ooff, fdata[2], fdiv, fleftvol, frightvol;
    
    samp = &paintbuffer[ bufferOffset ];
    
    if( ch->doppler )
    {
        sampleOffset = sampleOffset * ch->oldDopplerScale;
    }
    
    if( sc->soundChannels == 2 )
    {
        sampleOffset *= sc->soundChannels;
        
        if( sampleOffset & 1 )
        {
            sampleOffset &= ~1;
        }
    }
    
    chunk = sc->soundData;
    while( sampleOffset >= SND_CHUNK_SIZE )
    {
        chunk = chunk->next;
        sampleOffset -= SND_CHUNK_SIZE;
        if( !chunk )
        {
            chunk = sc->soundData;
        }
    }
    
    if( !ch->doppler || ch->dopplerScale == 1.0f )
    {
        leftvol = ch->leftvol * snd_vol;
        rightvol = ch->rightvol * snd_vol;
        samples = chunk->sndChunk;
        for( i = 0 ; i < count ; i++ )
        {
            data  = samples[sampleOffset++];
            samp[i].left += ( data * leftvol ) >> 8;
            
            if( sc->soundChannels == 2 )
            {
                data = samples[sampleOffset++];
            }
            samp[i].right += ( data * rightvol ) >> 8;
            
            if( sampleOffset == SND_CHUNK_SIZE )
            {
                chunk = chunk->next;
                samples = chunk->sndChunk;
                sampleOffset = 0;
            }
        }
    }
    else
    {
        fleftvol = ch->leftvol * snd_vol;
        frightvol = ch->rightvol * snd_vol;
        
        ooff = sampleOffset;
        samples = chunk->sndChunk;
        
        for( i = 0 ; i < count ; i++ )
        {
        
            aoff = ooff;
            ooff = ooff + ch->dopplerScale * sc->soundChannels;
            boff = ooff;
            fdata[0] = fdata[1] = 0;
            
            for( j = aoff; j < boff; j += sc->soundChannels )
            {
                if( j == SND_CHUNK_SIZE )
                {
                    chunk = chunk->next;
                    if( !chunk )
                    {
                        chunk = sc->soundData;
                    }
                    samples = chunk->sndChunk;
                    ooff -= SND_CHUNK_SIZE;
                }
                if( sc->soundChannels == 2 )
                {
                    fdata[0] += samples[j & ( SND_CHUNK_SIZE - 1 )];
                    fdata[1] += samples[( j + 1 ) & ( SND_CHUNK_SIZE - 1 )];
                }
                else
                {
                    fdata[0] += samples[j & ( SND_CHUNK_SIZE - 1 )];
                    fdata[1] += samples[j & ( SND_CHUNK_SIZE - 1 )];
                }
            }
            fdiv = 256 * ( boff - aoff ) / sc->soundChannels;
            samp[i].left += ( fdata[0] * fleftvol ) / fdiv;
            samp[i].right += ( fdata[1] * frightvol ) / fdiv;
        }
    }
}

static void S_PaintChannelFrom16( channel_t* ch, const sfx_t* sc, sint count, sint sampleOffset, sint bufferOffset )
{
    S_PaintChannelFrom16_scalar( ch, sc, count, sampleOffset, bufferOffset );
}

void S_PaintChannelFromWavelet( channel_t* ch, sfx_t* sc, sint count, sint sampleOffset, sint bufferOffset )
{
    sint data;
    sint leftvol, rightvol;
    sint i;
    portable_samplepair_t* samp;
    sndBuffer* chunk;
    schar16* samples;
    
    leftvol = ch->leftvol * snd_vol;
    rightvol = ch->rightvol * snd_vol;
    
    i = 0;
    samp = &paintbuffer[ bufferOffset ];
    chunk = sc->soundData;
    while( sampleOffset >= ( SND_CHUNK_SIZE_FLOAT * 4 ) )
    {
        chunk = chunk->next;
        sampleOffset -= ( SND_CHUNK_SIZE_FLOAT * 4 );
        i++;
    }
    
    if( i != sfxScratchIndex || sfxScratchPointer != sc )
    {
        S_AdpcmGetSamples( chunk, sfxScratchBuffer );
        sfxScratchIndex = i;
        sfxScratchPointer = sc;
    }
    
    samples = sfxScratchBuffer;
    
    for( i = 0 ; i < count ; i++ )
    {
        data  = samples[sampleOffset++];
        samp[i].left += ( data * leftvol ) >> 8;
        samp[i].right += ( data * rightvol ) >> 8;
        
        if( sampleOffset == SND_CHUNK_SIZE * 2 )
        {
            chunk = chunk->next;
            decodeWavelet( chunk, sfxScratchBuffer );
            sfxScratchIndex++;
            sampleOffset = 0;
        }
    }
}

void S_PaintChannelFromADPCM( channel_t* ch, sfx_t* sc, sint count, sint sampleOffset, sint bufferOffset )
{
    sint data;
    sint leftvol, rightvol;
    sint i;
    portable_samplepair_t* samp;
    sndBuffer* chunk;
    schar16* samples;
    
    leftvol = ch->leftvol * snd_vol;
    rightvol = ch->rightvol * snd_vol;
    
    i = 0;
    samp = &paintbuffer[ bufferOffset ];
    chunk = sc->soundData;
    
    if( ch->doppler )
    {
        sampleOffset = sampleOffset * ch->oldDopplerScale;
    }
    
    while( sampleOffset >= ( SND_CHUNK_SIZE * 4 ) )
    {
        chunk = chunk->next;
        sampleOffset -= ( SND_CHUNK_SIZE * 4 );
        i++;
    }
    
    if( i != sfxScratchIndex || sfxScratchPointer != sc )
    {
        S_AdpcmGetSamples( chunk, sfxScratchBuffer );
        sfxScratchIndex = i;
        sfxScratchPointer = sc;
    }
    
    samples = sfxScratchBuffer;
    
    for( i = 0 ; i < count ; i++ )
    {
        data  = samples[sampleOffset++];
        samp[i].left += ( data * leftvol ) >> 8;
        samp[i].right += ( data * rightvol ) >> 8;
        
        if( sampleOffset == SND_CHUNK_SIZE * 4 )
        {
            chunk = chunk->next;
            S_AdpcmGetSamples( chunk, sfxScratchBuffer );
            sampleOffset = 0;
            sfxScratchIndex++;
        }
    }
}

void S_PaintChannelFromMuLaw( channel_t* ch, sfx_t* sc, sint count, sint sampleOffset, sint bufferOffset )
{
    sint	data;
    sint	leftvol, rightvol;
    sint i;
    portable_samplepair_t* samp;
    sndBuffer* chunk;
    uchar8* samples;
    float32 ooff;
    
    leftvol = ch->leftvol * snd_vol;
    rightvol = ch->rightvol * snd_vol;
    
    samp = &paintbuffer[ bufferOffset ];
    chunk = sc->soundData;
    while( sampleOffset >= ( SND_CHUNK_SIZE * 2 ) )
    {
        chunk = chunk->next;
        sampleOffset -= ( SND_CHUNK_SIZE * 2 );
        if( !chunk )
        {
            chunk = sc->soundData;
        }
    }
    
    if( !ch->doppler )
    {
        samples = reinterpret_cast<uchar8*>( chunk->sndChunk ) + sampleOffset;
        for( i = 0 ; i < count ; i++ )
        {
            data  = mulawToShort[*samples];
            samp[i].left += ( data * leftvol ) >> 8;
            samp[i].right += ( data * rightvol ) >> 8;
            samples++;
            if( samples == reinterpret_cast<uchar8*>( chunk->sndChunk ) + ( SND_CHUNK_SIZE * 2 ) )
            {
                chunk = chunk->next;
                samples = reinterpret_cast<uchar8*>( chunk->sndChunk );
            }
        }
    }
    else
    {
        ooff = sampleOffset;
        samples = reinterpret_cast<uchar8*>( chunk->sndChunk );
        for( i = 0 ; i < count ; i++ )
        {
            data  = mulawToShort[samples[static_cast<sint>( ( ooff ) )]];
            ooff = ooff + ch->dopplerScale;
            samp[i].left += ( data * leftvol ) >> 8;
            samp[i].right += ( data * rightvol ) >> 8;
            if( ooff >= SND_CHUNK_SIZE * 2 )
            {
                chunk = chunk->next;
                if( !chunk )
                {
                    chunk = sc->soundData;
                }
                samples = reinterpret_cast<uchar8*>( chunk->sndChunk );
                ooff = 0.0;
            }
        }
    }
}

/*
===================
S_PaintChannels
===================
*/
void S_PaintChannels( sint endtime )
{
    sint i;
    sint end;
    channel_t* ch;
    sfx_t* sc;
    sint ltime, count;
    sint sampleOffset;
    
    snd_vol = s_volume->value * 255;
    
//Com_Printf ("%i to %i\n", s_paintedtime, endtime);
    while( s_paintedtime < endtime )
    {
        // if paintbuffer is smaller than DMA buffer
        // we may need to fill it multiple times
        end = endtime;
        
        if( endtime - s_paintedtime > PAINTBUFFER_SIZE )
        {
            end = s_paintedtime + PAINTBUFFER_SIZE;
        }
        
        // clear the paint buffer to either music or zeros
        if( s_rawend < s_paintedtime )
        {
            if( s_rawend )
            {
                //Com_DPrintf ("background sound underrun\n");
            }
            ::memset( paintbuffer, 0, ( end - s_paintedtime ) * sizeof( portable_samplepair_t ) );
        }
        else
        {
            // copy from the streaming sound source
            sint	s, stop;
            
            stop = ( end < s_rawend ) ? end : s_rawend;
            
            for( i = s_paintedtime ; i < stop ; i++ )
            {
                s = i & ( MAX_RAW_SAMPLES - 1 );
                paintbuffer[i - s_paintedtime] = s_rawsamples[s];
            }
//		if (i != end)
//			Com_Printf ("partial stream\n");
//		else
//			Com_Printf ("full stream\n");
            for( ; i < end ; i++ )
            {
                paintbuffer[i - s_paintedtime].left =
                    paintbuffer[i - s_paintedtime].right = 0;
            }
        }
        
        // paint in the channels.
        ch = s_channels;
        for( i = 0; i < MAX_CHANNELS ; i++, ch++ )
        {
            if( !ch->thesfx || ( ch->leftvol < 0.25 && ch->rightvol < 0.25 ) )
            {
                continue;
            }
            
            ltime = s_paintedtime;
            sc = ch->thesfx;
            
            sampleOffset = ltime - ch->startSample;
            count = end - ltime;
            if( sampleOffset + count > sc->soundLength )
            {
                count = sc->soundLength - sampleOffset;
            }
            
            if( count > 0 )
            {
                if( sc->soundCompressionMethod == 1 )
                {
                    S_PaintChannelFromADPCM( ch, sc, count, sampleOffset, ltime - s_paintedtime );
                }
                else if( sc->soundCompressionMethod == 2 )
                {
                    S_PaintChannelFromWavelet( ch, sc, count, sampleOffset, ltime - s_paintedtime );
                }
                else if( sc->soundCompressionMethod == 3 )
                {
                    S_PaintChannelFromMuLaw( ch, sc, count, sampleOffset, ltime - s_paintedtime );
                }
                else
                {
                    S_PaintChannelFrom16( ch, sc, count, sampleOffset, ltime - s_paintedtime );
                }
            }
        }
        
        // paint in the looped channels.
        ch = loop_channels;
        for( i = 0; i < numLoopChannels ; i++, ch++ )
        {
            if( !ch->thesfx || ( !ch->leftvol && !ch->rightvol ) )
            {
                continue;
            }
            
            ltime = s_paintedtime;
            sc = ch->thesfx;
            
            if( sc->soundData == nullptr || sc->soundLength == 0 )
            {
                continue;
            }
            // we might have to make two passes if it
            // is a looping sound effect and the end of
            // the sample is hit
            do
            {
                sampleOffset = ( ltime % sc->soundLength );
                
                count = end - ltime;
                if( sampleOffset + count > sc->soundLength )
                {
                    count = sc->soundLength - sampleOffset;
                }
                
                if( count > 0 )
                {
                    if( sc->soundCompressionMethod == 1 )
                    {
                        S_PaintChannelFromADPCM( ch, sc, count, sampleOffset, ltime - s_paintedtime );
                    }
                    else if( sc->soundCompressionMethod == 2 )
                    {
                        S_PaintChannelFromWavelet( ch, sc, count, sampleOffset, ltime - s_paintedtime );
                    }
                    else if( sc->soundCompressionMethod == 3 )
                    {
                        S_PaintChannelFromMuLaw( ch, sc, count, sampleOffset, ltime - s_paintedtime );
                    }
                    else
                    {
                        S_PaintChannelFrom16( ch, sc, count, sampleOffset, ltime - s_paintedtime );
                    }
                    ltime += count;
                }
            }
            while( ltime < end );
        }
        
        // transfer out according to DMA format
        S_TransferPaintBuffer( end, paintbuffer );
        s_paintedtime = end;
    }
}
