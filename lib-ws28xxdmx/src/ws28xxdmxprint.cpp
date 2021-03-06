/**
 * @file ws28xxdmxprint.cpp
 *
 */
/* Copyright (C) 2018-2019 by Arjan van Vught mailto:info@raspberrypi-dmx.nl
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

#include <stdio.h>

#include "ws28xxdmx.h"
#include "ws28xxdmxparams.h"

#include "ws28xx.h"

void WS28xxDmx::Print(void) {
	printf("Led parameters\n");
	printf(" Type  : %s [%d]\n", WS28xx::GetLedTypeString(m_tLedType), m_tLedType);
	printf(" Count : %d\n", (int) m_nLedCount);
	if ((m_tLedType == WS2801) || (m_tLedType == APA102)) {
		printf(" Clock : %d Hz %s {Default: %d Hz, Maximum %d Hz}\n", (int) m_nClockSpeedHz, (m_nClockSpeedHz == 0 ? "Default" : ""), WS2801_SPI_SPEED_DEFAULT_HZ, WS2801_SPI_SPEED_MAX_HZ);
	}
	if (m_tLedType == APA102) {
		printf(" GlbBr : %d\n", (int) m_nGlobalBrightness);
	}
}
