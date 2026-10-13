// Host check only; not an EE build input.
// Build: c++ -std=c++17 -Wall -Wextra -DNORRATH_HOST_CONTRACT -o /tmp/c host_contract.cpp tty_handler.cpp \
//            ../w9-startup-crt-f-tty-state/tty_state.cpp
#include <cstdarg>
#include <cstring>
#include <initializer_list>
#include "../w9-startup-crt-f-tty-state/tty_state.hpp"

static int prints, recv_calls, send_calls, recv_socket, send_socket, queue_done, recv_result, send_result;
static const void* recv_buffer;
static const void* send_buffer;
static unsigned recv_length, send_length;
static char printed[8][64];
static int printed_arg[8];
static unsigned char sink[400];
static int writes_seen_at_done[400];
extern "C" {
void kprintf(const char* format, ...) {
    std::strncpy(printed[prints], format, 63);
    va_list args;
    va_start(args, format);
    printed_arg[prints] = std::strchr(format, '%') ? va_arg(args, int) : 0;
    va_end(args);
    ++prints;
}
int sceDeci2ExRecv(int socket, void* buffer, unsigned length) {
    ++recv_calls; recv_socket = socket; recv_buffer = buffer; recv_length = length; return recv_result;
}
int sceDeci2ExSend(int socket, void* buffer, unsigned length) {
    ++send_calls; send_socket = socket; send_buffer = buffer; send_length = length; return send_result;
}
void QueuePeekWriteDone(TtyQueue* queue) {
    writes_seen_at_done[queue_done] = *(queue->writePtr);  // the byte is stored before the call
    ++queue_done;
    ++queue->writePtr;
}
void sceTtyHandler(int event, int count, TtyInfo* info);
}
static bool said(int i, const char* text) { return std::strcmp(printed[i], text) == 0; }
static unsigned char packet[400];
static TtyQueue queue;
static void reset() {
    prints = recv_calls = send_calls = queue_done = 0; recv_result = send_result = 0;
    std::memset(packet, 0, sizeof packet); std::memset(sink, 0xee, sizeof sink);
    queue.size = 64; queue.count = 0; queue.readPtr = sink; queue.writePtr = sink;
    tinfo.socket = 42; tinfo.length = 0; tinfo.received = 0; tinfo.busy = 1;
    tinfo.sendCursor = wbuf; tinfo.receiveBuffer = packet; tinfo.queue = &queue;
}
int main() {
    // unknown events: nothing at all
    reset();
    for (int ev : {-1, 0, 5, 6, 100}) { sceTtyHandler(ev, 7, &tinfo); sceTtyHandler(ev, 0, &tinfo); }
    if (prints || recv_calls || send_calls || queue_done || tinfo.busy != 1 || tinfo.received != 0) return 1;

    // receive, event 1 and 2 alike
    for (int ev : {1, 2}) {
        reset(); tinfo.received = 10; recv_result = 6;
        sceTtyHandler(ev, 0x10006, &tinfo);                                 // count masked to 16 bits
        if (recv_calls != 1 || recv_socket != 42 || recv_buffer != packet + 10 || recv_length != 6) return 2;
        if (tinfo.received != 16 || prints != 1 || !said(0, "TTY: packet size larger than expect\n")) return 3;  // the size test uses the unmasked count
        reset(); tinfo.received = 315; recv_result = 5;
        sceTtyHandler(ev, 5, &tinfo);                                       // exactly 320: no message
        if (prints != 0 || tinfo.received != 320 || recv_buffer != packet + 315) return 4;
        reset(); tinfo.received = 316; recv_result = 5;
        sceTtyHandler(ev, 5, &tinfo);                                       // 321: message, call still made
        if (prints != 1 || !said(0, "TTY: packet size larger than expect\n") || recv_calls != 1 || tinfo.received != 321) return 5;
        reset(); tinfo.received = 20; recv_result = -3;
        sceTtyHandler(ev, 4, &tinfo);                                       // error: message, result still added
        if (prints != 1 || !said(0, "TTY: receive error") || tinfo.received != 17) return 6;
    }

    // count zero: forward bytes 12.. of the packet to the queue
    reset();
    packet[0] = 16; packet[1] = 0;
    packet[12] = 'a'; packet[13] = 'b'; packet[14] = 'c'; packet[15] = 'd'; packet[16] = 'X';
    tinfo.received = 55;
    sceTtyHandler(1, 0, &tinfo);
    if (queue_done != 4 || std::memcmp(sink, "abcd", 4) != 0 || sink[4] != 0xee) return 7;
    if (tinfo.received != 0 || recv_calls != 0 || prints != 0) return 8;
    for (int i = 0; i < 4; ++i) if (writes_seen_at_done[i] != "abcd"[i]) return 9;
    reset(); packet[0] = 12;                                                // length 12: nothing forwarded
    tinfo.received = 9; packet[12] = 'z';
    sceTtyHandler(2, 0, &tinfo);
    if (queue_done != 0 || tinfo.received != 0 || sink[0] != 0xee) return 10;
    reset(); packet[0] = 0x2c; packet[1] = 0x01;                            // 16-bit length 300 (little endian)
    sceTtyHandler(2, 0, &tinfo);
    if (queue_done != 288) return 11;

    // send
    reset(); tinfo.sendCursor = wbuf + 4; tinfo.length = 0x10010; tinfo.busy = 1; send_result = 8;
    sceTtyHandler(3, 0, &tinfo);
    if (send_calls != 1 || send_socket != 42 || send_buffer != wbuf + 4 || send_length != 0x10) return 12;   // length masked
    if (tinfo.sendCursor != wbuf + 12 || tinfo.length != 0x10010 - 8 || tinfo.busy != 1 || prints != 0) return 13;
    reset(); tinfo.sendCursor = wbuf; tinfo.length = 20; send_result = 0;
    sceTtyHandler(3, 0, &tinfo);
    if (tinfo.sendCursor != wbuf || tinfo.length != 20 || tinfo.busy != 1 || prints != 0) return 14;       // zero is not an error
    reset(); tinfo.sendCursor = wbuf; tinfo.length = 20; send_result = -9;
    sceTtyHandler(3, 0, &tinfo);
    if (prints != 1 || !said(0, "TTY: send err %d\n") || printed_arg[0] != -9 || tinfo.busy != 0 || tinfo.length != 20 || tinfo.sendCursor != wbuf) return 15;

    // event 4: busy cleared, message only when length is not zero
    reset();
    sceTtyHandler(4, 0, &tinfo);
    if (prints != 0 || tinfo.busy != 0) return 16;
    reset(); tinfo.length = 5;
    sceTtyHandler(4, 0, &tinfo);
    if (prints != 1 || !said(0, "TTY: err ti->wlen=%08x\n") || printed_arg[0] != 5 || tinfo.busy != 0 || tinfo.length != 5) return 17;
    reset(); tinfo.length = -1;                                              // any nonzero value, negative included
    sceTtyHandler(4, 0, &tinfo);
    if (prints != 1 || printed_arg[0] != -1) return 18;
    return 0;
}
