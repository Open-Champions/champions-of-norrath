// Host check only; not an EE build input.
// Build: c++ -std=c++17 -Wall -Wextra -o /tmp/c host_check.cpp
#include <cstdio>
#include <cstdint>
#include <cstring>
#include "gsm_frame_encode.cpp"
static unsigned seen_state; static void* seen_frame; static void* seen_out; static int calls;
extern "C" int gsm_encode(unsigned state, void* frame, void* out) {
    ++calls; seen_state = state; seen_frame = frame; seen_out = out; return 1234;
}
int main() {
    unsigned char object[64];
    for (int i = 0; i < 64; ++i) object[i] = (unsigned char)(0xa0 + i);
    unsigned word20, word24;
    std::memcpy(&word20, object + 20, 4); std::memcpy(&word24, object + 24, 4);
    int a, b;
    int r = gsm_frame_encode(&a, &b, object);
    if (r != 1234 || calls != 1) { std::printf("gsm_frame_encode result/call count wrong\n"); return 1; }
    if (seen_state != word20 || seen_state == word24) { std::printf("gsm_frame_encode state word wrong\n"); return 2; }
    if (seen_frame != &a || seen_out != &b) { std::printf("gsm_frame_encode argument order wrong\n"); return 3; }
    std::printf("gsm_frame_encode cases passed\n");
    return 0;
}
