/**
 * @file applemidi.h
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

/*
 * https://developer.apple.com/library/archive/documentation/Audio/Conceptual/MIDINetworkDriverProtocol/MIDI/MIDI.html
 */

#ifndef APPLEMIDI_H_
#define APPLEMIDI_H_

#include <stdint.h>
#include <stdbool.h>

#include "midi.h"

#include "mdns.h"

enum TAppleMidiUdpPort {
	APPLE_MIDI_UPD_PORT_CONTROL_DEFAULT	= 5004,
	APPLE_MIDI_UPD_PORT_MIDI_DEFAULT	= (APPLE_MIDI_UPD_PORT_CONTROL_DEFAULT + 1)
};

enum TAppleMidiSessionName {
	APPLE_MIDI_SESSION_NAME_LENGTH_MAX = 24
};

enum TAppleMidiVersion {
	APPLE_MIDI_VERSION = 2
};

struct TExchangePacket {
	uint16_t nSignature;
	uint16_t nCommand;
	uint32_t nProtocolVersion;
	uint32_t nInitiatorToken;
	uint32_t nSSRC;
	uint8_t aName[APPLE_MIDI_SESSION_NAME_LENGTH_MAX + 1];
}__attribute__((packed));

enum TSessionState {
	SESSION_STATE_WAITING_IN_CONTROL,
	SESSION_STATE_WAITING_IN_MIDI,
	SESSION_STATE_IN_SYNC,
	SESSION_STATE_ESTABLISHED
};

struct TSessionStatus {
	TSessionState tSessionState;
	uint32_t nRemoteIp;
	uint16_t nRemotePortMidi;
	uint32_t nSynchronizationTimestamp;
};

class AppleMidi: public MDNS {
public:
	AppleMidi(void);
	virtual ~AppleMidi(void);

	void Start(void);
	void Stop(void);

	void Run(void);

	void SetPort(uint16_t nPort = APPLE_MIDI_UPD_PORT_CONTROL_DEFAULT);
	void SetSessionName(const uint8_t *pSessionName);

	uint32_t GetSSRC(void) {
		return m_nSSRC;
	}

	void Print(void);

protected:
	uint32_t Now(void);
	bool Send(const uint8_t *pBuffer, uint32_t nLength);

private:
	void HandleControlMessage(void);
	void HandleMidiMessage(void);

	virtual void HandleRtpMidi(const uint8_t *pBuffer);

private:
	uint32_t m_nStartTime;
	uint32_t m_nSSRC;
	uint16_t m_nPort;
	int32_t m_nHandleControl;
	int32_t m_nHandleMidi;
	uint8_t *m_pBuffer;
	uint32_t m_nRemoteIp;
	uint16_t m_nRemotePort;
	uint16_t m_nBytesReceived;
	TExchangePacket m_ExchangePacketReply;
	uint16_t m_nExchangePacketReplySize;
	TSessionStatus m_tSessionStatus;
};

#endif /* APPLEMIDI_H_ */
