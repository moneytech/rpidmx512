/**
 * @file artnetreader.h
 *
 */
/* Copyright (C) 2019 by Arjan van Vught mailto:info@raspberrypi-dmx.nl
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

#ifndef H3_ARTNETREADER_H_
#define H3_ARTNETREADER_H_

#include <stdint.h>

#include "artnettimecode.h"

#include "ltc.h"
#include "midi.h"

class ArtNetReader: public ArtNetTimeCode {
public:
	ArtNetReader(struct TLtcDisabledOutputs *pLtcDisabledOutputs);
	~ArtNetReader(void);

	void Start(void);
	void Stop(void);

	void Run(void);

	void Handler(const struct TArtNetTimeCode *);

private:
	alignas(uint32_t) struct TLtcDisabledOutputs *m_ptLtcDisabledOutputs;
	alignas(uint32_t) struct _midi_send_tc m_tMidiTimeCode;
};

#endif /* H3_ARTNETREADER_H_ */