// Definitions for the tty objects; see tty_state.hpp. rbuf's size is not
// established; 256 bytes is the length passed to the queue and is used here
// as a lower bound.
#include "tty_state.hpp"

TtyInfo tinfo;
unsigned char wbuf[320];
unsigned char rbuf[256];
