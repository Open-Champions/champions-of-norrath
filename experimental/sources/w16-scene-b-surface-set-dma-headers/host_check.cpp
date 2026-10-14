// Host check only; not a build input. Words hold arena offsets (see NORRATH_HOST_ARENA).
#define NORRATH_HOST_ARENA 1
#include "set_dma_headers.cpp"
#include <cstdio>
#include <cstring>
alignas(16) static unsigned char arena[0x4000];
unsigned char* norrath_host_pointer(unsigned address) { return arena + address; }

static void expect_header(const unsigned char* b, int size, bool& ok) {
    unsigned w[4];
    unsigned long long q[6];
    std::memcpy(w, b, 16);
    std::memcpy(q, b + 16, 48);
    unsigned long long last = (static_cast<unsigned long long>(static_cast<long long>(static_cast<int>((size - 4) | 0x8000)))) | (1ull << 59);
    if (w[0] || w[1] || w[2] || w[3] != (static_cast<unsigned>(size - 1) | 0x50000000u)) ok = false;
    if (q[0] != 0x1000000000000001ull || q[1] != 14 || q[2] != 0 || q[3] != 0x53 || q[4] != last || q[5] != 0) ok = false;
}

int main() {
    bool ok = true;
    for (unsigned count = 0; count < 4; ++count) {
        for (unsigned flag = 0; flag < 2; ++flag) {
            std::memset(arena, 0x5a, sizeof arena);
            unsigned* self = reinterpret_cast<unsigned*>(arena);
            self[10] = flag;
            self[78] = 0x1000;
            self[14] = 0x100;
            self[15] = 40;
            self[16] = count;
            for (unsigned i = 0; i < 4; ++i) {
                self[21 + 6 * i] = 0x200 + 0x100 * i;
                self[22 + 6 * i] = 8 + i;
            }
            if (SetDMAHeaders__9VISurface(arena) != 0) ok = false;
            if (flag) {
                if (self[14] != 0x1100) ok = false;
                expect_header(arena + 0x1100, 40, ok);
            } else if (self[14] != 0x100) {
                ok = false;
            }
            for (unsigned i = 0; i < 4; ++i) {
                if (i < count) {
                    if (self[21 + 6 * i] != 0x1200 + 0x100 * i) ok = false;
                    expect_header(arena + 0x1200 + 0x100 * i, 8 + i, ok);
                } else if (self[21 + 6 * i] != 0x200 + 0x100 * i) {
                    ok = false;
                }
            }
        }
    }
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
