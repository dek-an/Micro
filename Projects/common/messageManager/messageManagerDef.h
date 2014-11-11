#ifndef _MESSAGEMANAGERDEF_H_
#define _MESSAGEMANAGERDEF_H_

// NOTE: Values should be power of two!
#define MSG_MNGR_HANDLERS_SET_SIZE		16
#define MSG_MNGR_MESSAGES_QUEUE_SIZE	64
#define MSG_MNGR_TIMERS_SET_SIZE		32

typedef enum
{
	MSG_EMPTY = 0
} MessageType;

#endif // _MESSAGEMANAGERDEF_H_