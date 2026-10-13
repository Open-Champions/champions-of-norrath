// Host check only; not an EE build input.
// Build: c++ -std=c++17 -Wall -Wextra -DNORRATH_HOST_CONTRACT -o /tmp/c host_contract.cpp tty_write.cpp \
//            ../w9-startup-crt-f-tty-state/tty_state.cpp
#include <cstring>
#include "../w9-startup-crt-f-tty-state/tty_state.hpp"

static int dintr_result = 1, dintr_calls, eintr_calls, send_calls, send_socket, send_arg, send_result;
static int polls, polls_until_clear, poll_socket_bad, length_at_send, busy_at_send, interrupts_before_send;
static unsigned short header_length_at_send;
extern "C" {
int DIntr() { ++dintr_calls; return dintr_result; }
int EIntr() { ++eintr_calls; return 0; }
int sceDeci2ReqSend(int socket, int arg) {
    ++send_calls; send_socket = socket; send_arg = arg;
    length_at_send = tinfo.length; busy_at_send = tinfo.busy; interrupts_before_send = dintr_calls;
    header_length_at_send = static_cast<unsigned short>(wbuf[0] | (wbuf[1] << 8));
    return send_result;
}
int sceDeci2Poll(int socket) {
    ++polls; if (socket != 42) ++poll_socket_bad;
    if (polls >= polls_until_clear) tinfo.busy = 0;
    return 0;
}
int sceTtyWrite(const char* source, int count);
}
static void reset() {
    dintr_result = 1; dintr_calls = eintr_calls = send_calls = polls = poll_socket_bad = 0; send_result = 0; polls_until_clear = 3;
    tinfo.socket = 42; tinfo.busy = 0; tinfo.length = 0; tinfo.sendCursor = nullptr;
    std::memset(wbuf, 0, sizeof wbuf); wbuf[7] = 'H';
}
int main() {
    // busy: refused at once, no interrupt handling at all
    reset(); tinfo.busy = 1;
    if (sceTtyWrite("hi", 2) != -1 || dintr_calls != 0 || send_calls != 0) return 1;
    // plain text
    reset();
    if (sceTtyWrite("hello", 5) != 5) return 2;
    if (std::memcmp(wbuf + 12, "hello", 5) != 0 || wbuf[17] != 0) return 3;
    if (tinfo.length != 17 || length_at_send != 17 || header_length_at_send != 17 || busy_at_send != 1) return 4;
    if (send_calls != 1 || send_socket != 42 || send_arg != 'H' || interrupts_before_send != 1) return 5;
    if (polls != 3 || poll_socket_bad != 0 || tinfo.busy != 0 || eintr_calls != 1) return 6;
    if (tinfo.sendCursor != wbuf) return 7;
    // line feeds become CR LF; the result counts source bytes only
    reset();
    if (sceTtyWrite("a\nb\n", 4) != 4) return 8;
    if (std::memcmp(wbuf + 12, "a\r\nb\r\n", 6) != 0 || tinfo.length != 18) return 9;
    // the byte at offset 7 goes out sign-extended
    reset(); wbuf[7] = 0x90;
    // (offset 7 is rewritten by init in the real program; here it is only read)
    sceTtyWrite("x", 1);
    if (send_arg != static_cast<signed char>(0x90)) return 10;
    // interrupts: EIntr only when DIntr returned nonzero
    reset(); dintr_result = 0;
    sceTtyWrite("x", 1);
    if (eintr_calls != 0 || dintr_calls != 1) return 11;
    // zero length sends an empty packet
    reset();
    if (sceTtyWrite("zz", 0) != 0 || tinfo.length != 12 || wbuf[12] != 0 || send_calls != 1) return 12;
    // 256 stored bytes at most
    static char big[600]; std::memset(big, 'k', sizeof big);
    reset();
    if (sceTtyWrite(big, 600) != 256 || tinfo.length != 268 || wbuf[12 + 255] != 'k' || wbuf[12 + 256] != 0) return 14;
    // an LF whose CR lands on the 256th byte: neither the LF nor its source byte is counted
    static char text[300]; std::memset(text, 'k', sizeof text); text[255] = '\n';
    reset();
    if (sceTtyWrite(text, 300) != 255 || tinfo.length != 268 || wbuf[12 + 255] != '\r' || wbuf[12 + 254] != 'k') return 15;
    // an LF at the 255th byte fits with its CR on byte 255 and LF as the 256th
    text[255] = 'k'; text[254] = '\n';
    reset();
    if (sceTtyWrite(text, 300) != 255 || tinfo.length != 268 || wbuf[12 + 254] != '\r' || wbuf[12 + 255] != '\n') return 16;
    // send failure: busy cleared, interrupts restored, -1, no polling
    reset(); send_result = -2;
    if (sceTtyWrite("ab", 2) != -1 || tinfo.busy != 0 || eintr_calls != 1 || polls != 0) return 17;
    reset(); send_result = -2; dintr_result = 0;
    if (sceTtyWrite("ab", 2) != -1 || eintr_calls != 0) return 18;
    return 0;
}
