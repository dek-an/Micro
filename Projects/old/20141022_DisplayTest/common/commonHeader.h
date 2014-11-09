#ifndef _COMMONHEADER_H_
#define _COMMONHEADER_H_

#define SFT(x) _BV(x)

#define SBI(port, pin) (port) |= SFT(pin)
#define CBI(port, pin) (port) &= ~SFT(pin)
#define TBI(port, pin) (port) ^= SFT(pin)
#define GBI(port, pin) ((port) >> (pin)) & 1

#endif // _COMMONHEADER_H_
