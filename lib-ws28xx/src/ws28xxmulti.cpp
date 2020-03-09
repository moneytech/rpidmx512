/**
 * @file ws28xxmulti.cpp
 *
 */
/* Copyright (C) 2020 by Arjan van Vught mailto:info@orangepi-dmx.nl
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:

 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.

 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#ifdef NDEBUG
#undef NDEBUG
#endif

#include <stdint.h>
#include <assert.h>

#include "ws28xxmulti.h"

#include "debug.h"

static TWS28XXType s_NotSupported[] = {WS2801, APA102, P9813};	// SPI Clock based

WS28xxMulti::WS28xxMulti(void):
	m_tBoard(WS28XXMULTI_BOARD_4X),
	m_tWS28xxType(WS2812B),
	m_nLedCount(170),
	m_tRGBMapping(RGB_MAPPING_UNDEFINED),
	m_nLowCode(0),
	m_nHighCode(0),
	m_nBufSize(0),
	m_pBuffer4x(0),
	m_pBlackoutBuffer4x(0),
	m_pBuffer8x(0),
	m_pBlackoutBuffer8x(0)
{
	DEBUG_ENTRY

	m_tBoard = (IsMCP23017() ? WS28XXMULTI_BOARD_4X : WS28XXMULTI_BOARD_8X);

	DEBUG_PRINTF("m_tBoard=%d [%s]", (int) m_tBoard, m_tBoard == WS28XXMULTI_BOARD_4X ? "4x" : "8x");
	DEBUG_EXIT
}

WS28xxMulti::~WS28xxMulti(void) {
	if (m_tBoard == WS28XXMULTI_BOARD_4X) {
		delete[] m_pBlackoutBuffer4x;
		m_pBlackoutBuffer4x = 0;

		delete[] m_pBuffer4x;
		m_pBuffer4x = 0;
	} else {
		m_pBlackoutBuffer8x = 0;
		m_pBuffer8x = 0;
	}
}

void WS28xxMulti::Initialize(TWS28XXType tWS28xxType, uint16_t nLedCount, TRGBMapping tRGBMapping, uint8_t nT0H, uint8_t nT1H, bool bUseSI5351A) {
	DEBUG_ENTRY
	DEBUG_PRINTF("m_tWS28xxType=%d (%s), m_nLedCount=%d, m_nBufSize=%d", m_tWS28xxType, WS28xx::GetLedTypeString(m_tWS28xxType), m_nLedCount, m_nBufSize);
	DEBUG_PRINTF("m_tRGBMapping=%d (%s), m_nLowCode=0x%X, m_nHighCode=0x%X", (int) m_tRGBMapping, RGBMapping::ToString(m_tRGBMapping), (int) m_nLowCode, (int) m_nHighCode);

	assert(nLedCount > 0);

	for (uint32_t i = 0; i < sizeof(s_NotSupported) / sizeof(s_NotSupported[0]) ; i++) {
		if (tWS28xxType == s_NotSupported[i]) {
			m_tWS28xxType = WS2812B;
			break;
		}
	}

	if (m_tRGBMapping == RGB_MAPPING_UNDEFINED) {
		m_tRGBMapping = WS28xx::GetRgbMapping(m_tWS28xxType);
	}

	uint8_t nLowCode;
	uint8_t nHighCode;

	WS28xx::GetTxH(m_tWS28xxType, nLowCode, nHighCode);

	if (m_nLowCode == 0) {
		m_nLowCode = nLowCode;
	}

	if (m_nHighCode == 0) {
		m_nHighCode = nHighCode;
	}

	if (m_tWS28xxType == SK6812W) {
		m_nLedCount = nLedCount <= LEDCOUNT_RGBW_MAX ? nLedCount : LEDCOUNT_RGBW_MAX;
		m_nBufSize = nLedCount * SINGLE_RGBW;
	} else {
		m_nLedCount = nLedCount <= LEDCOUNT_RGB_MAX ? nLedCount : LEDCOUNT_RGB_MAX;
		m_nBufSize = nLedCount * SINGLE_RGB;
	}

	DEBUG_PRINTF("m_tWS28xxType=%d (%s), m_nLedCount=%d, m_nBufSize=%d", m_tWS28xxType, WS28xx::GetLedTypeString(m_tWS28xxType), m_nLedCount, m_nBufSize);
	DEBUG_PRINTF("m_tRGBMapping=%d (%s), m_nLowCode=0x%X, m_nHighCode=0x%X", (int) m_tRGBMapping, RGBMapping::ToString(m_tRGBMapping), (int) m_nLowCode, (int) m_nHighCode);

	if (m_tBoard == WS28XXMULTI_BOARD_4X) {
		SetupMCP23017(ReverseBits(m_nLowCode), ReverseBits(m_nHighCode));
		if (bUseSI5351A) {
			SetupSI5351A();
		}
		SetupGPIO();
		SetupBuffers4x();
	} else {
		SetupHC595(m_nLowCode, m_nHighCode);
		SetupSPI();
		SetupBuffers8x();
	}

	DEBUG_EXIT
}
