
#ifndef _DICT_H_
#define	_DICT_H_
#include "Allheader.h"


/************************************************************************************************/
/* structure for object dictionary																*/
/************************************************************************************************/
struct _dict
{
	uint16_t object;
	uint8_t sub;
	uint8_t access;
	uint8_t size;
	void * pointer;
};
extern const struct _dict dict[];

uint32_t read_dict (uint8_t pos, uint8_t subindex);
uint8_t write_dict (uint8_t pos, uint8_t subindex, uint32_t value);
uint32_t search_dict (uint16_t index, uint8_t subindex, uint8_t type, uint8_t * pos);


#endif

