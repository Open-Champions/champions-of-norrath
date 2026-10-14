#define _ctype_ w9_ctype
#include "text_send.cpp"

#include <cstdio>
#include <cstring>

unsigned char outMsg[300];
// Only the tab (0x09) and space (0x20) entries have bit 8 set.
extern "C" const unsigned char w9_ctype[257] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 8, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0,
};
int dataHandle, waiting;
static int sends;
static int send_handle, send_len;
static unsigned char send_copy[300];
extern "C" void netSendMessage__FiPUci(int h, unsigned char* m, int n) {
    ++sends; send_handle = h; send_len = n; std::memcpy(send_copy, m, sizeof send_copy);
    if (m != outMsg) send_len = -1;
}
#define CHECK(c) do { if (!(c)) { std::printf("FAIL line %d\n", __LINE__); return 1; } } while (0)

static void fill(unsigned char len, unsigned char pos, const char* text) {
    std::memset(outMsg, 0x55, sizeof outMsg);
    outMsg[0] = len; outMsg[1] = pos; outMsg[2] = 0x66;
    std::memcpy(outMsg + 3, text, len);
    sends = 0; waiting = 0; dataHandle = 42;
}

int main() {
    fill(5, 2, "abXcd");                 // remove index 2 -> "abcd"
    textSend__Fv();
    CHECK(outMsg[0] == 4 && outMsg[1] == 0xff && outMsg[2] == 0x66);
    CHECK(std::memcmp(outMsg + 3, "abcd", 4) == 0);
    CHECK(sends == 1 && send_handle == 42 && send_len == 9 && waiting == 1);

    fill(3, 0, "X  ");                    // only spaces remain
    textSend__Fv();
    CHECK(sends == 0 && waiting == 0);
    CHECK(outMsg[0] == 1 && outMsg[1] == 0 && outMsg[2] == 0x66 && outMsg[3] == 0x20 && outMsg[4] == 0);

    fill(3, 0, "X \t");                   // tab counts as space in this table
    textSend__Fv();
    CHECK(sends == 0 && outMsg[0] == 1 && outMsg[3] == 0x20);

    fill(3, 2, " a.");                    // removes last character
    textSend__Fv();
    CHECK(sends == 1 && send_len == 7 && outMsg[0] == 2 && outMsg[3] == ' ' && outMsg[4] == 'a');

    fill(1, 0, "Z");                      // nothing left
    textSend__Fv();
    CHECK(sends == 0 && waiting == 0 && outMsg[0] == 1 && outMsg[3] == 0x20 && outMsg[1] == 0);

    fill(4, 1, "a\xe9z ");                // byte above 0x7f indexes the table directly
    textSend__Fv();
    CHECK(sends == 1 && send_len == 8 && outMsg[0] == 3);
    std::puts("ok");
    return 0;
}
