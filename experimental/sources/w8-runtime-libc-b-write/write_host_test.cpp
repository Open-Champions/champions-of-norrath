// Host check only; not an EE build input. Build the source with -Dwrite=w8_write.
#include <cstdio>
#include <initializer_list>
int ttyinit = 0;
static int init_calls, write_calls, init_result = 1, write_result = 7;
static const void* init_arg; static const void* write_buf; static unsigned write_count;
extern "C" int sceTtyInit(const void* b) { ++init_calls; init_arg = b; return init_result; }
extern "C" int sceTtyWrite(const void* b, unsigned n) { ++write_calls; write_buf = b; write_count = n; return write_result; }
extern "C" int w8_write(int, const void*, unsigned);

int main() {
    char data[4] = {};
    for (int fd : {-1, 0, 3, 100, (int)0x80000001}) {
        if (w8_write(fd, data, 4) != -1) return 1;
    }
    if (init_calls || write_calls || ttyinit) return 2;  // rejected before touching the tty
    init_result = 0;
    if (w8_write(1, data, 4) != -1) return 3;             // init failure
    if (init_calls != 1 || init_arg != data || write_calls || ttyinit) return 4;
    init_result = 1;
    if (w8_write(2, data, 4) != 7) return 5;              // first success initialises
    if (init_calls != 2 || ttyinit != 1 || write_calls != 1 || write_buf != data || write_count != 4) return 6;
    write_result = -5;
    if (w8_write(1, data + 1, 0) != -5) return 7;         // initialised: no second init
    if (init_calls != 2 || write_calls != 2 || write_buf != data + 1 || write_count != 0) return 8;
    ttyinit = 0;                                          // sceResetttyinit
    w8_write(1, data, 1);
    if (init_calls != 3) return 9;
    std::puts("write ok");
    return 0;
}
