// Host check only; not an EE build input. Compile the source as its own translation unit with
// -Dqsort=w12_qsort (the rename must not reach the system headers) and link it with this file.
// The expected counts and FNV-1a digests of the compare-call index sequence and of the
// final array come from running the original on the same generated arrays; the digests
// summarise behaviour only.
#include <cstdio>
#include <cstring>
#include <cstdint>
#include <vector>
extern "C" void w12_qsort(void*, unsigned, unsigned, int (*)(const void*, const void*));
static std::vector<std::pair<long, long>> trace;
static const unsigned* origin;
static int compare_ints(const void* a, const void* b) {
    const unsigned x = *static_cast<const unsigned*>(a), y = *static_cast<const unsigned*>(b);
    trace.push_back({static_cast<const unsigned*>(a) - origin, static_cast<const unsigned*>(b) - origin});
    return x == y ? 0 : (x > y ? 1 : -1);
}
static uint64_t fnv(uint64_t h, uint64_t v) {
    for (int i = 0; i < 8; ++i) h = (h ^ ((v >> (8 * i)) & 0xff)) * 0x100000001b3ull;
    return h;
}
struct Case { unsigned n; int kind; size_t calls; uint64_t trace_digest, array_digest; };
static const Case cases[] = {
    {5, 0, 8, 0x3027f4e235361661ull, 0x09466a0f7a020fb8ull},
    {9, 0, 25, 0x1464283bfc49a66bull, 0x548b7874225610f8ull},
    {30, 0, 144, 0x69e4e402f6b14b2eull, 0xf4a39fa301664ed1ull},
    {41, 0, 202, 0x0417457c5ea462d0ull, 0xcab25a706e9c25a8ull},
    {100, 0, 621, 0x5594f0160b3f8d9aull, 0xb5983107ae66067full},
    {300, 1, 633, 0x33c55a0e9d36676cull, 0xed8eea138f3618c6ull},
    {300, 0, 2547, 0xaa808b30efc16d25ull, 0x607ed948f7719e4aull},
    {150, 2, 952, 0xf21210169c438ce4ull, 0x8deef7e08db4da72ull},
};
int main() {
    int index = 0;
    for (const Case& c : cases) {
        ++index;
        std::vector<unsigned> v(c.n);
        unsigned x = 12345 + c.n;
        for (unsigned i = 0; i < c.n; ++i) {
            x = x * 1664525u + 1013904223u;
            unsigned value = x >> 16;
            if (c.kind == 1) value %= 4;
            if (c.kind == 2) value = (i % 3) ? i : 100 - i;
            v[i] = value;
        }
        origin = v.data();
        trace.clear();
        w12_qsort(v.data(), c.n, 4, compare_ints);
        uint64_t h = 0xcbf29ce484222325ull, g = h;
        for (auto& p : trace) { h = fnv(h, p.first); h = fnv(h, p.second); }
        for (unsigned i = 0; i < c.n; ++i) g = fnv(g, v[i]);
        for (unsigned i = 1; i < c.n; ++i) if (v[i - 1] > v[i]) return 10 * index + 1;
        if (trace.size() != c.calls) { std::printf("case %d calls %zu\n", index, trace.size()); return 10 * index + 2; }
        if (h != c.trace_digest) return 10 * index + 3;
        if (g != c.array_digest) return 10 * index + 4;
    }
    // Wide elements and odd sizes sort correctly (the swap paths): records of 3, 12 and 16 bytes keyed by the first int.
    for (unsigned size : {3u, 8u, 12u, 16u, 24u}) {
        for (unsigned n : {1u, 6u, 7u, 8u, 45u, 90u}) {
            std::vector<unsigned char> buf(size * n + 16, 0);
            unsigned x = size * 1000 + n;
            for (unsigned i = 0; i < n; ++i) {
                x = x * 1664525u + 1013904223u;
                buf[i * size] = (x >> 24) & 0xff;
                for (unsigned j = 1; j < size; ++j) buf[i * size + j] = (unsigned char)(i + j);
            }
            std::vector<unsigned char> keys(n);
            for (unsigned i = 0; i < n; ++i) keys[i] = buf[i * size];
            w12_qsort(buf.data(), n, size, [](const void* a, const void* b) {
                return (int)*static_cast<const unsigned char*>(a) - (int)*static_cast<const unsigned char*>(b);
            });
            for (unsigned i = 1; i < n; ++i) if (buf[(i - 1) * size] > buf[i * size]) return 100;
            // payload bytes travelled with their keys: byte j of a record is (original index + j) mod 256
            for (unsigned i = 0; i < n; ++i)
                for (unsigned j = 2; j < size; ++j)
                    if ((unsigned char)(buf[i * size + j] - buf[i * size + 1]) != (unsigned char)(j - 1)) return 101;
        }
    }
    std::puts("qsort ok");
    return 0;
}
