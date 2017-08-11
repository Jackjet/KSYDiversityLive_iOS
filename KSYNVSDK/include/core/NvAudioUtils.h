//================================================================================
//
// (c) Copyright China Digital Video (Beijing) Limited, 2014. All rights reserved.
//
// This code and information is provided "as is" without warranty of any kind,
// either expressed or implied, including but not limited to the implied
// warranties of merchantability and/or fitness for a particular purpose.
//
//--------------------------------------------------------------------------------
//   Birth Date:    Mar 7 2014
//   Author:        NewAuto video team
//================================================================================
#pragma once

#include "NvDef.h"


// Check if an audio sample format is planar
bool NvIsPlanarAudSmpFmt(ENvAudioSampleFormat audSmpFmt);

// Return sample size in bytes according to audio sample format
unsigned int NvSampleSizeInBytes(ENvAudioSampleFormat audSmpFmt);

// Calculate size in bytes that an audio samples buffer would occupy
// if stored in the given audio channel count, sample count and sample format
unsigned int NvAudioSamplesBufferGetSizeInBytes(unsigned int channelCount,
                                                unsigned int sampleCount,
                                                ENvAudioSampleFormat audSmpFmt,
                                                unsigned int *planeSizeInBytes = 0);

// Fill in the SNvAudioSamplesBuffer fields
void NvAudioSamplesBufferFillInfo(SNvAudioSamplesBuffer *samplesBuffer,
                                  const void *samplesBufferPtr,
                                  unsigned int channelCount,
                                  unsigned int sampleCount,
                                  ENvAudioSampleFormat audSmpFmt);

// Set audio samples to silence
void NvAudioSamplesSetSilence(const SNvAudioSamplesBuffer *samplesBuffer,
                              unsigned int offset,
                              unsigned int sampleCount,
                              unsigned int channelCount,
                              ENvAudioSampleFormat audSmpFmt);

// Copy audio samples
void NvAudioSamplesCopy(const SNvAudioSamplesBuffer *srcSamplesBuffer,
                        const SNvAudioSamplesBuffer *dstSamplesBuffer,
                        unsigned int srcOffset,
                        unsigned int dstOffset,
                        unsigned int sampleCount,
                        unsigned int channelCount,
                        ENvAudioSampleFormat audSmpFmt);

// Generate sine wave audio samples
// @phase: the start phase in 1/NV_TIME_BASE seconde time unit,
//         it will be updated to the end time of the last sample generated
void NvAudioSamplesGenerateSineWave(const SNvAudioSamplesBuffer *samplesBuffer,
                                    unsigned int offset,
                                    unsigned int sampleCount,
                                    unsigned int sampleRate,
                                    unsigned int channelCount,
                                    ENvAudioSampleFormat audSmpFmt,
                                    double amplitude,
                                    unsigned int frequency,
                                    int64_t &phase);

