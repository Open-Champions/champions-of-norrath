// Host check only; not an EE build input. Build the source with -Dread=w8_read.
#include <cstdio>
#include <initializer_list>
int ttyinit = 0;
static int init_calls, read_calls, init_result = 1, read_result = 3;
static void* init_arg; static void* read_buf; static unsigned read_count;
extern "C" int sceTtyInit(void* b) { ++init_calls; init_arg = b; return init_result; }
extern "C" int sceTtyRead(void* b, unsigned n) { ++read_calls; read_buf = b; read_count = n; return read_result; }
extern "C" int w8_read(int, void*, unsigned);

int main() {
    char data[8] = {};
    for (int fd : {-1, 1, 2, 3, 100, (int)0x80000000}) {
        if (w8_read(fd, data, 4) != -1) return 1;
    }
    if (init_calls || read_calls || ttyinit) return 2;
    init_result = 0;
    if (w8_read(0, data, 4) != -1) return 3;
    if (init_calls != 1 || init_arg != data || read_calls || ttyinit) return 4;
    init_result = 1;
    if (w8_read(0, data, 8) != 3) return 5;
    if (init_calls != 2 || ttyinit != 1 || read_calls != 1 || read_buf != data || read_count != 8) return 6;
    read_result = -2;
    if (w8_read(0, data + 2, 1) != -2) return 7;
    if (init_calls != 2 || read_calls != 2 || read_buf != data + 2 || read_count != 1) return 8;
    ttyinit = 0;
    w8_read(0, data, 1);
    if (init_calls != 3) return 9;
    std::puts("read ok");
    return 0;
}
