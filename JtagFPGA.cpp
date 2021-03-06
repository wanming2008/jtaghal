/***********************************************************************************************************************
*                                                                                                                      *
* ANTIKERNEL v0.1                                                                                                      *
*                                                                                                                      *
* Copyright (c) 2012-2016 Andrew D. Zonenberg                                                                          *
* All rights reserved.                                                                                                 *
*                                                                                                                      *
* Redistribution and use in source and binary forms, with or without modification, are permitted provided that the     *
* following conditions are met:                                                                                        *
*                                                                                                                      *
*    * Redistributions of source code must retain the above copyright notice, this list of conditions, and the         *
*      following disclaimer.                                                                                           *
*                                                                                                                      *
*    * Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the       *
*      following disclaimer in the documentation and/or other materials provided with the distribution.                *
*                                                                                                                      *
*    * Neither the name of the author nor the names of any contributors may be used to endorse or promote products     *
*      derived from this software without specific prior written permission.                                           *
*                                                                                                                      *
* THIS SOFTWARE IS PROVIDED BY THE AUTHORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED   *
* TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL *
* THE AUTHORS BE HELD LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES        *
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR       *
* BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT *
* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE       *
* POSSIBILITY OF SUCH DAMAGE.                                                                                          *
*                                                                                                                      *
***********************************************************************************************************************/

/**
	@file
	@author Andrew D. Zonenberg
	@brief Implementation of JtagFPGA
 */

#include "jtaghal.h"

using namespace std;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Construction / destruction

/**
	@brief Initializes this device

	@param idcode	The ID code of this device
	@param iface	The JTAG adapter this device was discovered on
	@param pos		Position in the chain that this device was discovered
 */
JtagFPGA::JtagFPGA(unsigned int idcode, JtagInterface* iface, size_t pos)
: JtagDevice(idcode, iface, pos)
{
}

/**
	@brief Default virtual destructor
 */
JtagFPGA::~JtagFPGA()
{

}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Device identification

bool JtagFPGA::GetUserVIDPID(unsigned int& idVendor, unsigned int& idProduct)
{
	//If we have no user instructions that's the end of it
	if(GetNumUserInstructions() < 1)
		return false;

	//We have user instructions... try the first one
	SelectUserInstruction(0);

	//Read 32 bits from it
	unsigned char zeros[4] = {0};
	unsigned char rdata[4] = {0};
	ScanDR(zeros, rdata, 32);

	//Format the output
	idVendor = (rdata[3] << 16) | (rdata[2] << 8) | (rdata[1]);
	idProduct = rdata[0];
	return true;
}
