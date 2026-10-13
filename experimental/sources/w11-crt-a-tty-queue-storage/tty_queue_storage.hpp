// Layout of the original's local queue object q.12 (0x008de500, zero-initialised).
// Observed: the next symbol, tinfo, starts 0x110 bytes later; QueueInit and
// QueuePeekReadDone treat the first 16 bytes as the TtyQueue header and the
// ring starts right behind it (header + 16). QueueInit is only called with 256
// by sceTtyInit, so the ring is taken as 256 bytes (16 + 256 = 0x110).
#ifndef W11_CRT_A_TTY_QUEUE_STORAGE_HPP
#define W11_CRT_A_TTY_QUEUE_STORAGE_HPP
#include "../w9-startup-crt-f-tty-state/tty_state.hpp"

struct TtyQueueObject {
    TtyQueue header;
    unsigned char ring[256];
};

// Descriptive name for the local symbol q.12; not an original name.
extern TtyQueueObject norrath_tty_queue;
#endif
