#include "memcard_title.cpp"

#include <cstdio>
#include <cstring>

static int order[8], n;
static char key[32];
static unsigned short fmt_obj[4];
static int slot_ret = 3, ga, gb;
static char* gc;
static unsigned short* u_out;
static unsigned short* u_fmt;
static float u_args[8];
extern "C" const char* textGetString__FPc(const char* k) {
    order[n++] = 1; std::strcpy(key, k); return reinterpret_cast<const char*>(fmt_obj);
}
extern "C" int getSlotName__FiiPc(int a, int b, char* c) { order[n++] = 2; ga = a; gb = b; gc = c; return slot_ret; }
extern "C" void usprintf__FPUsT0ffffffff(unsigned short* o, unsigned short* f, float a, float b,
                                         float c, float d, float e, float g, float h, float i) {
    order[n++] = 3; u_out = o; u_fmt = f;
    float v[8] = {a, b, c, d, e, g, h, i};
    for (int k = 0; k < 8; ++k) u_args[k] = v[k];
}

int main() {
    unsigned short out[8];
    unsigned short* r = memcardTitle__FPUs(out);
    if (r != out || u_out != out) return 1;
    if (n != 3 || order[0] != 1 || order[1] != 2 || order[2] != 3) return 2;
    if (std::strcmp(key, "MemcardPS2") != 0) return 3;
    if (ga != -1 || gb != -1 || gc != nullptr) return 4;
    if (u_fmt != fmt_obj) return 5;
    if (u_args[0] != 137.0f || u_args[1] != 3.0f) return 6;
    for (int k = 2; k < 8; ++k) if (u_args[k] != 0.0f) return 7;
    slot_ret = -2;
    memcardTitle__FPUs(out);
    if (u_args[1] != -2.0f) return 8;
    std::puts("memcard title passed");
    return 0;
}
