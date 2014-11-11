#ifndef _MESSAGE_MANAGER_H_
#define _MESSAGE_MANAGER_H_

#include "messageManagerDef.h"

#include <common/commonHeader.h>

typedef uint16 MessageParameter;
typedef void (*Handler)(MessageParameter);

// should be defined by user
extern void idleHandler(MessageParameter param);

// //////////////////////////////////////////////////////////
// Interface
//
extern void initMessageManager(void) __attribute__((always_inline));

extern void registerHandler(MessageType msg, Handler hnd);
extern void unregisterHandler(MessageType msg, Handler hnd);

extern void sendMessage(MessageType msg, MessageParameter param);
extern void dispatchMessages(void) __attribute__((always_inline));

extern void setTimer(MessageType msg, MessageParameter param, uint16 timeMs);
extern void killTimer(MessageType msg);

#endif // _MESSAGE_MANAGER_H_