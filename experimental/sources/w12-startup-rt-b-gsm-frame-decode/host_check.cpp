// Host check only; not an EE build input.
// Build: c++ -std=c++17 -Wall -Wextra -o /tmp/c host_check.cpp
#include <cstdio>
#include <cstdint>
#include <cstring>
#include "gsm_frame_decode.cpp"
static unsigned seen_state; static void* seen_frame; static void* seen_out; static int calls;
extern "C" int gsm_decode(unsigned state, void* frame, void* out) {
    ++calls; seen_state = state; seen_frame = frame; seen_out = out; return 1234;
}
int main() {
    unsigned char object[64];
    for (int i = 0; i < 64; ++i) object[i] = (unsigned char)(0xa0 + i);
    unsigned word24, word20;
    std::memcpy(&word20, object + 20, 4); std::memcpy(&word24, object + 24, 4);
    int a, b;
    int r = gsm_frame_decode(&a, &b, object);
    if (r != 1234 || calls != 1) { std::printf("gsm_frame_decode result/call count wrong\n"); return 1; }
    if (seen_state != word24 || seen_state == word20) { std::printf("gsm_frame_decode state word wrong\n"); return 2; }
    if (seen_frame != &a || seen_out != &b) { std::printf("gsm_frame_decode argument order wrong\n"); return 3; }
    std::printf("gsm_frame_decode cases passed\n");
    return 0;
}
