/**
 * @file storenetwork.cpp
 *
 */
/* Copyright (C) 2018-2020 by Arjan van Vught mailto:info@raspberrypi-dmx.nl
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

#include <stdint.h>
#include <assert.h>

#include "spiflashstore.h"

#include "networkparams.h"

#include "debug.h"

#ifndef MIN
 #define MIN(a, b) ((a) < (b) ? (a) : (b))
#endif

StoreNetwork *StoreNetwork::s_pThis = 0;

StoreNetwork::StoreNetwork(void) {
	DEBUG_ENTRY

	s_pThis = this;

	DEBUG_PRINTF("%p", s_pThis);

	DEBUG_EXIT
}

StoreNetwork::~StoreNetwork(void) {
	DEBUG_ENTRY

	DEBUG_EXIT
}

void StoreNetwork::Update(const struct TNetworkParams *pNetworkParams) {
	DEBUG_ENTRY

	SpiFlashStore::Get()->Update(STORE_NETWORK, (void *)pNetworkParams, sizeof(struct TNetworkParams));

	DEBUG_EXIT
}

void StoreNetwork::Copy(struct TNetworkParams* pNetworkParams) {
	DEBUG_ENTRY

	SpiFlashStore::Get()->Copy(STORE_NETWORK, (void *)pNetworkParams, sizeof(struct TNetworkParams));

	DEBUG_EXIT
}

void StoreNetwork::SaveIp(uint32_t nIp) {
	DEBUG_ENTRY

	DEBUG_PRINTF("offsetof=%d", (int) __builtin_offsetof(struct TNetworkParams, nLocalIp));

	SpiFlashStore::Get()->Update(STORE_NETWORK, __builtin_offsetof(struct TNetworkParams, nLocalIp), (void *)&nIp, sizeof(uint32_t), NETWORK_PARAMS_MASK_IP_ADDRESS);

	DEBUG_EXIT
}

void StoreNetwork::SaveNetMask(uint32_t nNetMask) {
	DEBUG_ENTRY

	DEBUG_PRINTF("offsetof=%d", (int) __builtin_offsetof(struct TNetworkParams, nNetmask));

	SpiFlashStore::Get()->Update(STORE_NETWORK, __builtin_offsetof(struct TNetworkParams, nNetmask), (void *)&nNetMask, sizeof(uint32_t), NETWORK_PARAMS_MASK_NET_MASK);

	DEBUG_EXIT
}

void StoreNetwork::SaveHostName(const uint8_t *pHostName, uint32_t nLength) {
	DEBUG_ENTRY

	DEBUG_PRINTF("offsetof=%d", (int) __builtin_offsetof(struct TNetworkParams, aHostName));

	nLength = MIN(nLength, NETWORK_HOSTNAME_SIZE);

	SpiFlashStore::Get()->Update(STORE_NETWORK, __builtin_offsetof(struct TNetworkParams, aHostName), (void *)pHostName, nLength, NETWORK_PARAMS_MASK_HOSTNAME);

	DEBUG_EXIT
}

void StoreNetwork::SaveDhcp(bool bIsDhcpUsed) {
	DEBUG_ENTRY

	DEBUG_PRINTF("offsetof=%d", (int) __builtin_offsetof(struct TNetworkParams, bIsDhcpUsed));

	SpiFlashStore::Get()->Update(STORE_NETWORK, __builtin_offsetof(struct TNetworkParams, bIsDhcpUsed), (void *)&bIsDhcpUsed, sizeof(bool), NETWORK_PARAMS_MASK_DHCP);

	DEBUG_EXIT
}
