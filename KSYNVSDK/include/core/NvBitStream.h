//================================================================================
//
// (c) Copyright China Digital Video (Beijing) Limited, 2015. All rights reserved.
//
// This code and information is provided "as is" without warranty of any kind,
// either expressed or implied, including but not limited to the implied
// warranties of merchantability and/or fitness for a particular purpose.
//
//--------------------------------------------------------------------------------
//   Birth Date:    Jun 6. 2015
//   Author:        NewAuto video team
//================================================================================
#pragma once

#include <NvDef.h>
#include <qendian.h>


typedef struct {
    uint32_t buf0;          /* Current uint32_t value */
    uint32_t buf1;          /* The uint32_t value next to current */
    unsigned int pos;       /* Bit position in current uint32_t value */
    uint32_t *tail;         /* Current uint32_t value pointer */
    uint32_t *start;        /* Start pointer of the stream */
    unsigned int length;    /* Effective length of the stream */
    unsigned int initPos;   /* Bit position of the effective stream start pointer */
    uint32_t *lastSafeTail; /* The last uint32_t value pointer we can safely access */
} SNvReadBitStream;


/* perform byte swapping */
#ifndef NV_BSWAP_UINT32
#ifdef _MSC_VER
#define NV_BSWAP_UINT32(a) { a = _byteswap_ulong((unsigned long)a); }
#else
#define NV_BSWAP_UINT32(a) { a = qbswap((uint32_t)a); }
#endif
#endif


// Initialize SNvReadBitStream structure
// Make sure the stream has at least 8 bytes readable data space!
static void inline NvReadBitStreamInit(SNvReadBitStream *readBitStream,
                                       void *stream,
                                       int length)
{
    uint32_t *alignedStream, tmp;
	size_t bytePos;

	/*
	* start the stream on a uint32_t boundary, by rounding down to the
	* previous uint32_t and skipping the intervening bytes.
	*/
    bytePos = (size_t)stream & (sizeof(uint32_t) - 1);
    alignedStream = (uint32_t *)((uint8_t *)stream - bytePos);
    readBitStream->start = readBitStream->tail = alignedStream;

	tmp = *readBitStream->start;
#if Q_BYTE_ORDER != Q_BIG_ENDIAN
    NV_BSWAP_UINT32(tmp);
#endif
	readBitStream->buf0 = tmp;

	tmp = *(readBitStream->start + 1);
#if Q_BYTE_ORDER != Q_BIG_ENDIAN
    NV_BSWAP_UINT32(tmp);
#endif
	readBitStream->buf1 = tmp;

	readBitStream->pos = readBitStream->initPos = (unsigned int)bytePos * 8;
	readBitStream->length = length;

    readBitStream->lastSafeTail = (uint32_t *)((uint8_t *)alignedStream
		+ ((length + bytePos - 1) & ~(sizeof(uint32_t) - 1)));
}


/* Reset SNvReadBitStream state */
static void inline NvReadBitStreamReset(SNvReadBitStream *readBitStream)
{
	uint32_t tmp;

	readBitStream->tail = readBitStream->start;

	tmp = *readBitStream->start;
#if Q_BYTE_ORDER != Q_BIG_ENDIAN
    NV_BSWAP_UINT32(tmp);
#endif
	readBitStream->buf0 = tmp;

	tmp = *(readBitStream->start + 1);
#if Q_BYTE_ORDER != Q_BIG_ENDIAN
    NV_BSWAP_UINT32(tmp);
#endif
	readBitStream->buf1 = tmp;

	readBitStream->pos = readBitStream->initPos;
}

/* Get the current pointer in the stream from pos, pos must be byte aligned */
static inline void * NvReadBitStreamGetPointer(SNvReadBitStream *readBitStream)
{
    Q_ASSERT((readBitStream->pos % 8) == 0);
	return (uint8_t *)readBitStream->tail + readBitStream->pos / 8;
}

/* Reads n bits(n <= 32) from SNvReadBitStream without changing the stream position */
static uint32_t inline NvReadBitStreamShowBits(SNvReadBitStream *readBitStream, int n)
{
	int bit = (readBitStream->pos + n) - 32;

	if (bit > 0) {
		return ((readBitStream->buf0 & (0xffffffff >> readBitStream->pos)) << bit) |
			(readBitStream->buf1 >> (32 - bit));
	} else {
		return (readBitStream->buf0 & (0xffffffff >> readBitStream->pos)) >> (-bit);
	}
}

/* Skip n bits(n <= 32) forward in SNvReadBitStream */
static inline void NvReadBitStreamSkip(SNvReadBitStream *readBitStream, int n)
{
	readBitStream->pos += n;

	if (readBitStream->pos >= 32) {
		uint32_t tmp;

		readBitStream->buf0 = readBitStream->buf1;
		tmp = *(readBitStream->tail + 2);
#if Q_BYTE_ORDER != Q_BIG_ENDIAN
        NV_BSWAP_UINT32(tmp);
#endif
		readBitStream->buf1 = tmp;
		readBitStream->tail++;
		readBitStream->pos -= 32;
	}
}

/* Skip n bits(n <= 32) forward in SNvReadBitStream */
static inline void NvReadBitStreamSafeSkip(SNvReadBitStream *readBitStream, int n)
{
	readBitStream->pos += n;

	if (readBitStream->pos >= 32) {
		readBitStream->buf0 = readBitStream->buf1;

		if (readBitStream->tail < readBitStream->lastSafeTail - 1) {
			uint32_t tmp;

			tmp = *(readBitStream->tail + 2);
#if Q_BYTE_ORDER != Q_BIG_ENDIAN
            NV_BSWAP_UINT32(tmp);
#endif
			readBitStream->buf1 = tmp;
			readBitStream->tail++;
		} else if (readBitStream->tail == readBitStream->lastSafeTail - 1) {
			readBitStream->tail++;
			readBitStream->buf1 = 0;
		} else {
			readBitStream->tail = readBitStream->lastSafeTail;
		}

		readBitStream->pos -= 32;
	}
}

/* Skip n bits(n could be any size) forward in SNvReadBitStream */
static inline void NvReadBitStreamArbitrarySkip(SNvReadBitStream *readBitStream, int n)
{
	if (readBitStream->pos + n < 64)
		return NvReadBitStreamSkip(readBitStream, n);

	uint32_t tmp;

	readBitStream->pos += n;
	readBitStream->tail += readBitStream->pos / 32;
	readBitStream->pos %= 32;

	tmp = *readBitStream->tail;
#if Q_BYTE_ORDER != Q_BIG_ENDIAN
    NV_BSWAP_UINT32(tmp);
#endif
	readBitStream->buf0 = tmp;

	tmp = *(readBitStream->tail + 1);
#if Q_BYTE_ORDER != Q_BIG_ENDIAN
    NV_BSWAP_UINT32(tmp);
#endif
	readBitStream->buf1 = tmp;
}

/* Skip n bits(n could be any size) forward in SNvReadBitStream */
static inline void NvReadBitStreamArbitrarySafeSkip(SNvReadBitStream *readBitStream, int n)
{
	if (readBitStream->pos + n < 64)
		return NvReadBitStreamSafeSkip(readBitStream, n);

	uint32_t tmp;

	readBitStream->pos += n;
	readBitStream->tail += readBitStream->pos / 32;
	readBitStream->pos %= 32;

	if (readBitStream->tail < readBitStream->lastSafeTail) {
		tmp = *readBitStream->tail;
#if Q_BYTE_ORDER != Q_BIG_ENDIAN
        NV_BSWAP_UINT32(tmp);
#endif
		readBitStream->buf0 = tmp;

		tmp = *(readBitStream->tail + 1);
#if Q_BYTE_ORDER != Q_BIG_ENDIAN
        NV_BSWAP_UINT32(tmp);
#endif
		readBitStream->buf1 = tmp;
	} else if (readBitStream->tail == readBitStream->lastSafeTail) {
		tmp = *readBitStream->tail;
#if Q_BYTE_ORDER != Q_BIG_ENDIAN
        NV_BSWAP_UINT32(tmp);
#endif
		readBitStream->buf0 = tmp;
		readBitStream->buf1 = 0;
	} else {
		readBitStream->buf0 = 0;
		readBitStream->buf1 = 0;
	}
}

/* Number of bits to next byte alignment */
static inline unsigned int NvReadBitStreamNumBitsToByteAlign(SNvReadBitStream *readBitStream)
{
	unsigned int n = (32 - readBitStream->pos) % 8;
	return n == 0 ? 8 : n;
}

/* Show n bits(n <= 32) from next byte alignment */
static inline uint32_t NvReadBitStreamShowBitsFromByteAlign(SNvReadBitStream *readBitStream, int n)
{
	int newPos = readBitStream->pos + NvReadBitStreamNumBitsToByteAlign(readBitStream);
	int bit = (newPos + n) - 32;

	if (newPos >= 32) {
		return readBitStream->buf1 >> (32 - bit);
	} else if (bit > 0) {
		return ((readBitStream->buf0 & (0xffffffff >> newPos)) << bit) |
			(readBitStream->buf1 >> (32 - bit));
	} else {
		return (readBitStream->buf0 & (0xffffffff >> newPos)) >> (-bit);
	}
}

/* Check whether the current position is byte aligned */
static inline int NvReadBitStreamCheckByteAlign(SNvReadBitStream *readBitStream)
{
	return (readBitStream->pos % 8) ? 0 : 1;
}

/* Move forward to the next byte boundary */
static inline void NvReadBitStreamByteAlign(SNvReadBitStream *readBitStream)
{
	int remainder = readBitStream->pos % 8;
	if (remainder)
		NvReadBitStreamSkip(readBitStream, 8 - remainder);
}

/* Get the length(unit bits) of SNvReadBitStream */
static unsigned int inline NvReadBitStreamPos(SNvReadBitStream *readBitStream)
{
	unsigned int bitCount;

	bitCount = (unsigned int)((uint8_t *)readBitStream->tail - (uint8_t *)readBitStream->start) * 8;
	bitCount += readBitStream->pos;
	bitCount -= readBitStream->initPos;

	return bitCount;
}

/* Read n bits(n <= 32) from SNvReadBitStream */
static uint32_t inline NvReadBitStreamGetBits(SNvReadBitStream *readBitStream, int n)
{
	uint32_t ret = NvReadBitStreamShowBits(readBitStream, n);

	NvReadBitStreamSkip(readBitStream, n);
	return ret;
}

/* Read single bit from SNvReadBitStream */
static uint32_t inline NvReadBitStreamGetBit(SNvReadBitStream *readBitStream)
{
	return NvReadBitStreamGetBits(readBitStream, 1);
}


typedef struct {
    uint32_t buf;           /* Bits already written to the output bitstream */
    unsigned int pos;       /* Bit position in current uint32_t value */
    uint32_t *tail;         /* Current uint32_t value pointer */
    uint32_t *start;        /* Start pointer of the stream */
    unsigned int length;    /* Max length of the output stream */
    unsigned int initPos;   /* Bit position of the output stream start pointer */
} SNvWriteBitStream;


// Initialize SNvWriteBitStream structure
// Make sure the stream has at least 4 bytes writable data space!
static void inline NvWriteBitStreamInit(SNvWriteBitStream *writeBitStream,
                                        void *stream,
                                        int length)
{
    uint32_t *alignedStream;
	size_t bytePos;

	/*
	* start the stream on a uint32_t boundary, by rounding down to the
	* previous uint32_t and skipping the intervening bytes.
	*/
    bytePos = (size_t)stream & (sizeof(uint32_t) - 1);
    alignedStream = (uint32_t *)((uint8_t *)stream - bytePos);
    writeBitStream->start = writeBitStream->tail = alignedStream;

	writeBitStream->pos = writeBitStream->initPos = (unsigned int)bytePos * 8;
	/* preserve the intervening bytes */
	if (writeBitStream->initPos > 0) {
        uint32_t tmp = *alignedStream;

#if Q_BYTE_ORDER != Q_BIG_ENDIAN
        NV_BSWAP_UINT32(tmp);
#endif

		writeBitStream->buf = tmp & (0xffffffff << (32 - writeBitStream->initPos));
	} else {
		writeBitStream->buf = 0;
	}

	writeBitStream->length = length;
}

static void inline NvWriteBitStreamReset(SNvWriteBitStream *writeBitStream)
{
	writeBitStream->tail = writeBitStream->start;

	/* preserve the intervening bytes */
	if (writeBitStream->initPos > 0) {
		uint32_t tmp = *writeBitStream->start;

#if Q_BYTE_ORDER != Q_BIG_ENDIAN
        NV_BSWAP_UINT32(tmp);
#endif

		writeBitStream->buf = tmp & (0xffffffff << (32 - writeBitStream->initPos));
	} else {
		writeBitStream->buf = 0;
	}

	writeBitStream->pos = writeBitStream->initPos;
}

/* Get the length(unit bits) of SNvWriteBitStream */
static unsigned int inline NvWriteBitStreamPos(SNvWriteBitStream *writeBitStream)
{
	unsigned int bitCount;

	bitCount = (unsigned int)((uint8_t *)writeBitStream->tail - (uint8_t *)writeBitStream->start) * 8;
	bitCount += writeBitStream->pos;
	bitCount -= writeBitStream->initPos;

	return bitCount;
}

/*
* Flush the bitstream & return length (unit bytes)
* NOTE: assumes no futher bitstream functions will be called.
*/
static uint32_t inline NvWriteBitStreamFlush(SNvWriteBitStream *writeBitStream)
{
	uint32_t len = (uint32_t)((uint8_t *)writeBitStream->tail - (uint8_t *)writeBitStream->start);

	if (writeBitStream->pos) {
		uint32_t b = writeBitStream->buf;

#if Q_BYTE_ORDER != Q_BIG_ENDIAN
		NV_BSWAP_UINT32(b);
#endif
		*writeBitStream->tail = b;

		len += (writeBitStream->pos + 7) / 8;
	}

	/* initPos is always on a byte boundary */
	if (writeBitStream->initPos)
		len -= writeBitStream->initPos / 8;

	return len;
}

/* Move bitstream position forward by n bits(n <= 32) and write out buffer */
static void inline NvWriteBitStreamForward(SNvWriteBitStream *writeBitStream, uint32_t nBits)
{
	writeBitStream->pos += nBits;

	if (writeBitStream->pos >= 32) {
		uint32_t b = writeBitStream->buf;

#if Q_BYTE_ORDER != Q_BIG_ENDIAN
		NV_BSWAP_UINT32(b);
#endif
		*writeBitStream->tail++ = b;
		writeBitStream->buf = 0;
		writeBitStream->pos -= 32;
	}
}

/* Write single bit to SNvWriteBitStream */
static void inline NvWriteBitStreamPutBit(SNvWriteBitStream *writeBitStream, uint32_t bit)
{
	if (bit)
		writeBitStream->buf |= (0x80000000 >> writeBitStream->pos);

	NvWriteBitStreamForward(writeBitStream, 1);
}

/* Write n bits(n <= 32) to SNvWriteBitStream */
static void inline NvWriteBitStreamPutBits(SNvWriteBitStream *writeBitStream,
                                           uint32_t value,
                                           uint32_t size)
{
	uint32_t shift = 32 - writeBitStream->pos - size;

	if (shift <= 32) {
		writeBitStream->buf |= value << shift;
		NvWriteBitStreamForward(writeBitStream, size);
	} else {
		uint32_t remainder;

		shift = size - (32 - writeBitStream->pos);
		writeBitStream->buf |= value >> shift;
		NvWriteBitStreamForward(writeBitStream, size - shift);
		remainder = shift;

		shift = 32 - shift;

		writeBitStream->buf |= value << shift;
		NvWriteBitStreamForward(writeBitStream, remainder);
	}
}

