/**
 * @file showfileosc.h
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

#ifndef SHOWFILEOSC_H_
#define SHOWFILEOSC_H_

#include <stdint.h>

enum TOscServerPort {
	OSCSERVER_PORT_DEFAULT_INCOMING = 9000,
	OSCSERVER_PORT_DEFAULT_OUTGOING = 8000
};

#define OSCSERVER_PATH_LENGTH_MAX	128
#define OSCSERVER_FILES_ENTRIES_MAX	20

class ShowFileOSC {
public:
	ShowFileOSC(void);
	~ShowFileOSC(void);

	void Start(void);
	void Stop(void);
	void Run(void);

	void Print(void);

	void SetPortIncoming(uint16_t nPortIncoming) {
		m_nPortIncoming = nPortIncoming;
	}
	uint16_t GetPortIncoming(void) {
		return m_nPortIncoming;
	}

	void SetPortOutgoing(uint16_t nPortOutgoing) {
		m_nPortOutgoing = nPortOutgoing;
	}
	uint16_t GetPortOutgoing(void) {
		return m_nPortOutgoing;
	}

private:
	void SendStatus(void);
	void Reload(void);

private:
	uint16_t m_nPortIncoming;
	uint16_t m_nPortOutgoing;
	int32_t m_nHandle;
	uint32_t m_nRemoteIp;
	uint16_t m_nRemotePort;
	uint8_t *m_pBuffer;
	int32_t m_aFileIndex[OSCSERVER_FILES_ENTRIES_MAX];

public:
	static ShowFileOSC *Get(void) {
		return s_pThis;
	}

private:
	static ShowFileOSC *s_pThis;
};

#endif /* SHOWFILEOSC_H_ */
