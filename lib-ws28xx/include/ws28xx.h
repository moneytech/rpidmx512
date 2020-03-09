/**
 * @file ws28xx.h
 *
 */
/* Copyright (C) 2017-2020 by Arjan van Vught mailto:info@orangepi-dmx.nl
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

#ifndef WS28XX_H_
#define WS28XX_H_

#include <stdint.h>
#include <stdbool.h>

#include "rgbmapping.h"

enum TWS28XXType {
	WS2801 = 0,
	WS2811,
	WS2812,
	WS2812B,
	WS2813,
	WS2815,
	SK6812,
	SK6812W,
	APA102,
	UCS1903,
	UCS2903,
	P9813,
	WS28XX_UNDEFINED
};

enum {
	LEDCOUNT_RGB_MAX = (4 * 170), LEDCOUNT_RGBW_MAX = (4 * 128)
};

enum {
	SINGLE_RGB = 24, SINGLE_RGBW = 32
};

#define WS2801_SPI_SPEED_MAX_HZ		25000000	///< 25 MHz
#define WS2801_SPI_SPEED_DEFAULT_HZ	4000000		///< 4 MHz

#define P9813_SPI_SPEED_MAX_HZ		15000000	///< 15 MHz
#define P9813_SPI_SPEED_DEFAULT_HZ	4000000		///< 4 MHz

class WS28xx {
public:
	WS28xx(TWS28XXType Type, uint16_t nLedCount, TRGBMapping tRGBMapping = RGB_MAPPING_UNDEFINED, uint8_t nT0H = 0, uint8_t nT1H = 0, uint32_t nClockSpeed = WS2801_SPI_SPEED_DEFAULT_HZ);
	~WS28xx(void);

	bool Initialize (void);

	TWS28XXType GetLEDType(void) {
		return m_tLEDType;
	}

	TRGBMapping GetRgbMapping(void) {
		return m_tRGBMapping;
	}

	uint8_t GetLowCode(void) {
		return m_nLowCode;
	}

	uint8_t GetHighCode(void) {
		return m_nHighCode;
	}

	uint16_t GetLEDCount(void) {
		return m_nLedCount;
	}

	uint32_t GetClockSpeedHz(void) {
		return m_nClockSpeedHz;
	}

	void SetGlobalBrightness(uint8_t nGlobalBrightness);

	uint8_t GetGlobalBrightness(void) {
		return m_nGlobalBrightness;
	}

	void SetLED(uint32_t nLEDIndex, uint8_t nRed, uint8_t nGreen, uint8_t nBlue);
	void SetLED(uint32_t nLEDIndex, uint8_t nRed, uint8_t nGreen, uint8_t nBlue, uint8_t nWhite);

	void Update(void);
	void Blackout(void);

	bool IsUpdating(void) {
		return false;
	}

public:
	static const char *GetLedTypeString(TWS28XXType tType);
	static TWS28XXType GetLedTypeString(const char *pValue);
	static void GetTxH(TWS28XXType tType, uint8_t &nLowCode, uint8_t &nHighCode);
	static TRGBMapping GetRgbMapping(TWS28XXType tType);
	static float ConvertTxH(uint8_t nCode);
	static uint8_t ConvertTxH(float fTxH);

private:
	void SetColorWS28xx(uint32_t nOffset, uint8_t nValue);

protected:
	TWS28XXType m_tLEDType;
	uint16_t m_nLedCount;
	TRGBMapping m_tRGBMapping;
	bool m_bIsRTZProtocol;
	uint32_t m_nClockSpeedHz;
	uint32_t m_nBufSize;
	uint8_t m_nGlobalBrightness;
	uint8_t m_nLowCode;
	uint8_t m_nHighCode;
	alignas(uint32_t) uint8_t *m_pBuffer;
	alignas(uint32_t) uint8_t *m_pBlackoutBuffer;
};

#endif /* WS28XX_H_ */
