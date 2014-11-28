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

extern const char* getTimeStr(void); // returns str (hh:mm:ss) of length 9 (with zero symbol)
extern const char* getTimeStrHoursMinutes(void); // returns str (hh:mm) of length 6 (with zero symbol)
extern uint08 getHours(void);
extern uint08 getMinutes(void);
extern uint08 getSeconds(void);

extern uint32 getRawTime(void);

// static functions
extern uint08 increase24(const uint08 val);
extern uint08 increase60(const uint08 val);
extern uint08 decrease24(const uint08 val);
extern uint08 decrease60(const uint08 val);

extern uint08 getHoursFrom(const uint32 time);
extern uint08 getMinutesFrom(const uint32 time);
extern uint08 getSecondsFrom(const uint32 time);
extern uint32 getTimeFrom(const uint08 hours, const uint08 minutes, const uint08 seconds);

extern uint32 updateHours(const uint32 time, const uint08 hours);
extern uint32 updateMinutes(const uint32 time, const uint08 minutes);
extern uint32 updateSeconds(const uint32 time, const uint08 seconds);

#endif // _CLOCK_H_
