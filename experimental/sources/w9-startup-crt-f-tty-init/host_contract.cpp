// Host check only; not an EE build input.
// Build: c++ -std=c++17 -Wall -Wextra -DNORRATH_HOST_CONTRACT -o /tmp/c host_contract.cpp tty_init.cpp \
//            ../w9-startup-crt-f-tty-state/tty_state.cpp
#include <cstring>
#include "../w9-startup-crt-f-tty-state/tty_state.hpp"

static int flush_arg = -1, flush_calls, open_calls, open_result, open_protocol, queue_calls, queue_size;
static void* open_option; static void (*open_handler)();
static TtyQueue fake_queue;
extern "C" {
void FlushCache(int mode) { ++flush_calls; flush_arg = mode; }
int sceDeci2Open(int protocol, void* option, void (*handler)()) {
    ++open_calls; open_protocol = protocol; open_option = option; open_handler = handler;
    return open_result;
}
TtyQueue* QueueInit(int size) { ++queue_calls; queue_size = size; return &fake_queue; }
void sceTtyHandler() {}
int sceTtyInit();
}
int main() {
    // negative socket: the socket is stored, nothing else changes, result 0
    std::memset(wbuf, 0x5a, sizeof wbuf);
    tinfo.busy = 7; tinfo.length = 8; tinfo.received = 9; tinfo.queue = nullptr;
    open_result = -3;
    if (sceTtyInit() != 0 || flush_calls != 1 || flush_arg != 0 || open_calls != 1) return 1;
    if (open_protocol != 528 || open_option != &tinfo || open_handler != sceTtyHandler) return 2;
    if (tinfo.socket != -3 || tinfo.busy != 7 || tinfo.length != 8 || tinfo.received != 9 || queue_calls != 0) return 3;
    for (unsigned i = 0; i < 16; ++i) if (wbuf[i] != 0x5a) return 4;
    // success
    open_result = 0;   // zero is a valid socket
    if (sceTtyInit() != 1 || tinfo.socket != 0) return 5;
    if (tinfo.busy != 0 || tinfo.length != 0 || tinfo.received != 0) return 6;
    if (tinfo.sendCursor != wbuf || tinfo.receiveBuffer != rbuf) return 7;   // plain pointers on the host
    if (tinfo.queue != &fake_queue || queue_calls != 1 || queue_size != 256) return 8;
    // header fields: halfword 2 zero, halfword 4 = 528, bytes 6/7 = 'E'/'H', word 8 zero; data area untouched
    const unsigned short zero = wbuf[2] | (wbuf[3] << 8), protocol = wbuf[4] | (wbuf[5] << 8);
    if (zero != 0 || protocol != 528 || wbuf[6] != 'E' || wbuf[7] != 'H') return 9;
    for (int i = 8; i < 12; ++i) if (wbuf[i] != 0) return 10;
    if (wbuf[0] != 0x5a || wbuf[1] != 0x5a || wbuf[12] != 0x5a) return 11;   // length halfword and data left alone
    return 0;
}
