#include "AllHeader.h"
#include "typedef.h"
#include "bootloader.h"

//#define	__DEBUG
/*

	S1.0A:		Basic LIOP operation

*/

uint32_t const identyty[ID_NUMS] =
{
	0x000000B5,
	0x00000000,
	0x00000000,
	0x00000000,
};

uint8_t const version_uea_VGS505[SW_SIZE] = "S1.0A";

uint8_t const hardware_uea_VGS505[HW_SIZE] = "H1.0";
uint8_t const device_name_VGS505[DV_SIZE] = {
	'V', 'G', 'S', '5', '0', '5'
};


uint16_t const version_fver = FVER_G_741;

#ifdef __DEBUG
uint32_t const version_chksum = DEBUG_CHKSUM;
#else
uint32_t const version_chksum = 0xffffffff;
#endif

