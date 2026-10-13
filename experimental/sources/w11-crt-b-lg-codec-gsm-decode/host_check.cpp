// Host comparison only. Not an EE build input. Records the forwarded arguments.
#include <cstdio>
#include "lg_codec_gsm_decode.cpp"
static int seen[8];
static void* ptrs[3];
static void (*fnseen)();
extern "C" void gsm_frame_decode() {}
extern "C" int lgFrameDecode(int a, int b, int c, int d, void* e, int f, void* g, int h, int i, void (*fn)(), void* object) {
    seen[0] = a; seen[1] = b; seen[2] = c; seen[3] = d; seen[4] = f; seen[5] = h; seen[6] = i;
    ptrs[0] = e; ptrs[1] = g; ptrs[2] = object; fnseen = fn;
    return 77;
}
int main() {
    static unsigned char object[1024];
    int r = lgCodecGSMDecode(object, 11, 22, 33, 44);
    bool ok = r == 77 && seen[0] == 11 && seen[1] == 22 && seen[2] == 33 && seen[3] == 44 && seen[4] == 33 && seen[5] == 160 && seen[6] == 33
        && ptrs[0] == object + 352 && ptrs[1] == object + 388 && ptrs[2] == object && fnseen == gsm_frame_decode;
    if (!ok) { std::printf("lgCodecGSMDecode wrong\n"); return 2; }
    std::printf("lgCodecGSMDecode cases passed\n");
    return 0;
}
