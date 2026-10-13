// Host contract for msg_save__3NPCPc; not an EE input.
#include <cstdio>
#include <cstring>
#include <string>
static std::string calls;
extern "C" unsigned objectSaveAddToStringPool__FPCc(const char* t) { calls += std::string(t) + ";"; return 100 + static_cast<unsigned>(calls.size()); }
#include "msg_save.cpp"
int main() {
    alignas(16) static unsigned char o[1280];
    alignas(16) static int buf[16];
    int bad = 0;
    std::memset(o, 0, sizeof o);
    std::strcpy(reinterpret_cast<char*>(o + 960), "lump"); std::strcpy(reinterpret_cast<char*>(o + 992), "model"); std::strcpy(reinterpret_cast<char*>(o + 1024), "tex");
    short h = -5; std::memcpy(o + 1066, &h, 2);
    int w = 0x1234567; std::memcpy(o + 244, &w, 4);
    float f = 60.5f; std::memcpy(o + 176, &f, 4);
    std::memset(buf, 0x7f, sizeof buf);
    int n = msg_save__3NPCPc(o, reinterpret_cast<char*>(buf));
    float g; std::memcpy(&g, buf + 5, 4);
    if (n != 24 || calls != "lump;model;tex;" || buf[0] != 105 || buf[1] != 111 || buf[2] != 115 || buf[3] != -5 || buf[4] != w || g != f) { std::printf("fail %d %s\n", n, calls.c_str()); bad = 1; }
    for (int i = 6; i < 16; ++i) if (buf[i] != 0x7f7f7f7f) bad = 1;
    return bad;
}
