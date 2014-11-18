#ifndef _CLOCK_H_
#define _CLOCK_H_

#include "clockDef.h"
#include <common/commonHeader.h>

// //////////////////////////////////////////////////////////
// Interface
//

extern void initClock(void) /*__attribute__((always_inline))*/;
extern void setClock(
	const uint08 hours,
	const uint08 minutes,
	const uint08 seconds);

extern void setHours(const uint08 hours);
extern void setMinutes(const uint08 minutes);
extern void setSeconds(const uint08 seconds);

extern const char* getTimeStrProgmem(void);
extern const char* getTimeStrHoursMinutesProgMem(void);
extern uint08 getHours(void);
extern uint08 getMinutes(void);
extern uint08 getSeconds(void);

#endif // _CLOCK_H_
