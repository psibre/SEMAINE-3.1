/*******************************************************************************
 * feaTUM, fast, efficient audio feature extractor by TUM
 * Copyright (C) 2008  Florian Eyben, Martin Woellmer
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *******************************************************************************/


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

This class handles the reading of PCM WAVE files (RIFF format).
Other file types, such as compressed audio (MP3 or OGG) are not yet supported!

+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

#ifndef __LIVE_INPUT_H
#define __LIVE_INPUT_H

#ifdef __WINDOWS
//#error  live-input.c module currently has only LINUX support (/dev/dsp interface) 
#ifdef LIVE_REC
#ifndef USE_PORTAUDIO
#define USE_PORTAUDIO
#warning you must use USE_PORTAUDIO when compiling for windows
#endif
#endif
#endif

#include "pcm-data.h"

#ifdef USE_PORTAUDIO
#include <portaudio.h>
#define PORTAUDIO_REC_FRAMES_PER_BUFFER   1024
#endif

#define LI_STREAM_STARTED  1
#define LI_STREAM_STOPPED  0


#include "wave-common.h"

/*
#define BYTEORDER_LE 0
#define BYTEORDER_BE 1


typedef struct {
  long sampleRate;
  long sampleType;
  long nChan;
  long blockSize;
  long nBPS;      // bytes per sample 
  long byteOrder;  // BYTEORDER_LE or BYTEORDER_BE
  LONG_IDX nBlocks;
} sLiveParameters;
typedef sLiveParameters* pLiveParameters;
*/

/* The class data structure */
typedef struct {
  char devicename[PATH_STRING_SIZE+1];
  
  sWaveParameters  parameters;
  int              streamStatus;
  int              abort;
  
  #ifdef USE_PORTAUDIO
  PaStream*        stream;
  int              bufUpdate;  // set to 1 after each buffer update via the callback function
  pPcmBuffer       bufWrapper;
  int              rbLock;
  int              paFrames;
  #else   // /dev/dsp interface:
  int fd;
  int blkSize;
  #endif

  long  absTimeStart;  // absolute time (system time, etc.) of recording start (creation of class object)
  long  absTimeStop;  // absolute time (system time, etc.) of recording start (creation of class object)
  
  // max. and actual size of live buffer in bytes, set to -1 to load full live file
  LONG_IDX bufferSizeBytes;
  LONG_IDX bufferSizeBlocks;
  
  // write pointer and read pointer
  LONG_IDX idxWr, idxRd;  
  // corresponding ring buffer indicies
  LONG_IDX idxWarpedWr, idxWarpedRd;

  // corresponding ring buffer index (in blocks)
  LONG_IDX nReadTotal;  // total blocks actually read:  //??skips = idxRd - nReadTotal  
  LONG_IDX nSkipped;
    
  sPcmBuffer buffer;
  
} sLiveInput;
typedef sLiveInput* pLiveInput;

// list available audio devices  (object does not have to be created)
int liveInput_listDevices(void);

/* Constructor: creates a class data object 
   maxBuf is the max. size of the internal live buffer
   if the file is smaller than bufsize, only the amount of mem required to store
   the file will be allocated
   Set to -1 to always store the complete live file in memory (DANGEROUS!)
    ->Rather use a high number, e.g. 158 760 000 (for 1h of 44.1khz)
*/
// usage: pClassName testObj = NULL;
//        testObj = testClass_create( testObj, ... );
// status: 0: stopped, 1:started
pLiveInput liveInput_create( pLiveInput obj, const char *devicename, int deviceId, LONG_IDX maxBuf, int samplerate, int sampleType, int channels, int status);

// status: 0: stopped, 1:started
int liveInput_setStatus( pLiveInput obj, int status );

long liveInput_getSampleRate( pLiveInput obj );
LONG_IDX liveInput_secondsToSamples( pLiveInput obj, FLOAT_TYPE se );
FLOAT_TYPE liveInput_samplesToSeconds( pLiveInput obj, LONG_IDX sa );

/* gets data from current position, data must be allocated by calling function
   data->nSamp must be initialized with the number of samples to read
   after reading in the data, nSamp will be set to the actual number of samples
   read
*/
int liveInput_getDataSequential( pLiveInput obj, pPcmBuffer data );
int liveInput_getDataSequential_gen( void *obj, pPcmBuffer data );
//  { liveInput_getDataSequential( (pLiveInput)obj, data ); }

/* Destructor: frees the class data object and returns NULL */
pLiveInput liveInput_destroy( pLiveInput obj );
pLiveInput liveInput_destroyData( pLiveInput obj );

#endif // #ifndef __LIVE_INPUT_H