/***********************************************************************************************************************
*                                                                                                                      *
* ANTIKERNEL v0.1                                                                                                      *
*                                                                                                                      *
* Copyright (c) 2012-2017 Andrew D. Zonenberg                                                                          *
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
	@brief Implementation of XilinxDevice
 */

#include "jtaghal.h"
#include "JEDECVendorID_enum.h"
#include "XilinxDeviceID_enum.h"

using namespace std;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Construction / destruction

/**
	@brief Default virtual destructor
 */
XilinxDevice::~XilinxDevice()
{

}

/**
	@brief Creates a XilinxDevice given an ID code

	@throw JtagException if the ID code supplied is not a valid Xilinx device, or not a known family number

	@param idcode	The ID code of this device
	@param iface	The JTAG adapter this device was discovered on
	@param pos		Position in the chain that this device was discovered

	@return A valid JtagDevice object, or NULL if the vendor ID was not recognized.
 */
JtagDevice* XilinxDevice::CreateDevice(unsigned int idcode, JtagInterface* iface, size_t pos)
{
	//Save the original ID code to give to the derived class
	unsigned int idcode_raw = idcode;

	//Rightmost bit is always a zero, ignore it
	idcode >>= 1;

	//Sanity check manufacturer ID
	if( (idcode & 0x7FF) != VENDOR_ID_XILINX)
	{
		throw JtagExceptionWrapper(
			"Invalid IDCODE supplied (wrong JEDEC manufacturer ID, not a Xilinx device)",
			"");
	}
	idcode >>= 11;

	//Next 9 bits are array size
	unsigned int arraysize =  idcode & 0x1FF;
	idcode >>= 9;

	//Next 7 bits are family
	unsigned int family = idcode & 0x7F;
	idcode >>= 7;

	//Revision
	unsigned int rev = idcode & 0xF;

	//Create the device
	switch(family)
	{

	case XILINX_FAMILY_CR2_A:
	case XILINX_FAMILY_CR2_B:
		return XilinxCoolRunnerIIDevice::CreateDevice(idcode_raw, iface, pos);

	case XILINX_FAMILY_SPARTAN3A:
		return XilinxSpartan3ADevice::CreateDevice(arraysize, rev, idcode_raw, iface, pos);

	case XILINX_FAMILY_SPARTAN6:
		return XilinxSpartan6Device::CreateDevice(arraysize, rev, idcode_raw, iface, pos);

	case XILINX_FAMILY_7SERIES:
		return Xilinx7SeriesDevice::CreateDevice(arraysize, rev, idcode_raw, iface, pos);

	default:
		throw JtagExceptionWrapper(
			"Unknown family ID - probably not yet supported",
			"");
	}
	return NULL;
}
