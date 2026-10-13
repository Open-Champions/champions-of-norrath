/* Host contract. Not a plan input. __udivdi3 is a recording stub. */
#define NORRATH_HOST_CONTRACT 1
#include "get_time.cpp"
#include <cstdio>
extern "C" { unsigned long long rollover_counter = 0; volatile unsigned norrath_host_timer0_count = 0; }
static unsigned long long seen_n, seen_d;
extern "C" unsigned long long __udivdi3(unsigned long long n, unsigned long long d) { seen_n = n; seen_d = d; return 0x0123456789abcdefull; }
int main() {
    struct Case { unsigned long long roll; unsigned count; };
    const Case cases[] = {{0, 0}, {1, 0xffff}, {0x12345, 0xabcd1234u}, {0xffffffffffffull, 0xffffffffu}};
    for (const Case& c : cases) {
        rollover_counter = c.roll; norrath_host_timer0_count = c.count;
        const unsigned long long r = get_time();
        const unsigned long long want = (c.roll << 16) | (c.count & 0xffffu);
        if (r != 0x0123456789abcdefull || seen_n != want || seen_d != 576) { std::printf("get_time wrong for %llx %x\n", c.roll, c.count); return 1; }
    }
    std::puts("get_time contract passed");
    return 0;
}
