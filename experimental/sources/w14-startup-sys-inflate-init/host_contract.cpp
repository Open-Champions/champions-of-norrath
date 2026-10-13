// Host contract for inflateInit_. Not a build-plan input.
//   c++ -std=c++17 -o check host_contract.cpp && ./check
// inflateInit2_ is scripted: the contract checks the argument mapping (stream, 15, version, size) and that
// the result is returned unchanged for several values.
#include <cstdio>
static void* s_stream; static int s_bits, s_size, s_ret; static const char* s_version;
extern "C" int inflateInit2_(void* stream, int bits, const char* version, int size) {
    s_stream = stream; s_bits = bits; s_version = version; s_size = size; return s_ret;
}
#include "inflate_init.cpp"
int main() {
    int bad = 0, dummy;
    const char* v = "1.1.4";
    const int rets[] = {0, -2, -4, 7};
    for (int r : rets) {
        s_ret = r;
        if (inflateInit_(&dummy, v, 56) != r || s_stream != &dummy || s_bits != 15 || s_version != v || s_size != 56) { std::printf("ret %d\n", r); ++bad; }
    }
    return bad;
}
