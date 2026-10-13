// Host contract for msg_getPokeName__6PhPropPUsi; not an EE input. Includes the source.
#include "msg_get_poke_name.cpp"
#include <cstdarg>
#include <cstdio>
#include <cstring>
#include <initializer_list>
#include <string>
int frameNm;
static std::string calls;
static unsigned short keyText[2], nameText[2];
static unsigned char desc;
extern "C" unsigned short* textGetString__FPc(char* key) { calls += std::string("text:") + key + ";"; return keyText; }
extern "C" void ustrcat__FPUsT0(unsigned short* d, unsigned short* s) {
    calls += std::string("cat16:") + (s == keyText ? "key" : s == nameText ? "name" : "?") + (d ? "" : "!") + ";";
}
extern "C" void ustrcat__FPUsPc(unsigned short*, char* s) { calls += std::string("cat8:[") + s + "];"; }
extern "C" unsigned short* getName__4itemPQ24item11descriptioni(void* d, int a) {
    calls += "name(" + std::to_string(static_cast<unsigned char*>(d) - &desc) + "," + std::to_string(a) + ");";
    return nameText;
}
extern "C" int sprintf(char* dst, const char* fmt, ...) {
    va_list ap; va_start(ap, fmt);
    int n = std::vsnprintf(dst, 208, fmt, ap);
    va_end(ap);
    calls += "sprintf;";
    return n;
}
int main() {
    alignas(8) unsigned char object[512];
    unsigned short out[4];
    for (short count : {-5, 0, 1, 2, 3, 99, 32767, -32768})
    for (int fn : {0, 7, -1}) {
        std::memset(object, 0x44, sizeof object);
        std::memcpy(object + 332, &count, 2);
        frameNm = fn; calls.clear();
        unsigned char before[512]; std::memcpy(before, object, 512);
        int r = msg_getPokeName__6PhPropPUsi(object, out, 12345);
        if (r != 1) return 1;
        std::string want = "text:PickUp;cat16:key;cat8:[ ];name(0,1);cat16:name;";
        // description pointer is object+328; the stub measures from &desc, so rebuild with the real offset
        want = "text:PickUp;cat16:key;cat8:[ ];name(" + std::to_string(reinterpret_cast<unsigned char*>(object + 328) - &desc) + ",1);cat16:name;";
        if (count >= 2) want += "sprintf;cat8:[ x " + std::to_string(count) + "];";
        if (calls != want) return 2;
        int w; std::memcpy(&w, object + 260, 4);
        if (w != fn) return 3;
        for (int i = 0; i < 512; ++i) if ((i < 260 || i >= 264) && object[i] != before[i]) return 4;
    }
    return 0;
}
