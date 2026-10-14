// Host check only; not a build input. Also compared with the original in a private interpreter (9 paired
// scenarios, all 66 words covered). The widening routine and the 16-bit overload are stubs that log; the check
// pins the 512-byte buffer size, the widened text, that the colour and position reach the overload as equal but
// separate copies, every other argument, and the returned value.
#include "blitbar_narrow.cpp"
#include <cstdio>
#include <cstring>
#include <string>

static std::string trace;
static const VIColor32F* seen_color;
static const VIVect2I* seen_pos;
static int bar_result;
static std::string N(long v) { return std::to_string(v); }
static std::string fbits(float f) { unsigned v; std::memcpy(&v, &f, 4); return N(v); }
extern "C" void str8to16__8VIStringPUsUiPCc(unsigned short* d, unsigned size, const char* s) {
    trace += "W" + N(size) + ",'" + s + "';";
    unsigned i = 0;
    for (; s[i]; ++i) d[i] = static_cast<unsigned char>(s[i]);
    d[i] = 0;
}
extern "C" int BlitBar__4VIUIPCUsG10VIColor32FG8VIVect2I9VIUITexIdT4i16VIAlphaBlendModefUii(
    void* self, const unsigned short* text, const VIColor32F& color, const VIVect2I& pos, int left, int middle, int width, int blend,
    float alpha, unsigned flags, int extra) {
    std::string t;
    for (; *text; ++text) t += static_cast<char>(*text);
    seen_color = &color;
    seen_pos = &pos;
    trace += "B" + N(reinterpret_cast<long>(self) & 0xffff) + ",'" + t + "'," + fbits(color.v[0]) + "." + fbits(color.v[1]) + "." + fbits(color.v[2]) + "." + fbits(color.v[3]) + "," + N(pos.x) + "/" + N(pos.y) + "," +
             N(left) + "," + N(middle) + "," + N(width) + "," + N(blend) + "," + fbits(alpha) + "," + N(flags) + "," + N(extra) + ";";
    return bar_result;
}

int main() {
    bool ok = true;
    int object;
    const VIColor32F color = {{0.25f, -1.5f, 3.0f, 0.0f}};
    VIVect2I position;
    position.x = -40;
    position.y = 99;
    for (const char* text : {"", "Z", "a longer caption text"}) for (int result : {0, -1, 7}) {
        bar_result = result;
        trace.clear();
        seen_color = nullptr;
        seen_pos = nullptr;
        const int got = BlitBar__4VIUIPCcG10VIColor32FG8VIVect2I9VIUITexIdT4i16VIAlphaBlendModefUii(&object, text, color, position, 11, 22, 33, 44, 0.75f, 0x8001u, -6);
        const std::string want = std::string("W512,'") + text + "';B" + N(reinterpret_cast<long>(&object) & 0xffff) + ",'" + text + "'," + fbits(0.25f) + "." + fbits(-1.5f) + "." + fbits(3.0f) + "." + fbits(0.0f) +
                                 ",-40/99,11,22,33,44," + fbits(0.75f) + ",32769,-6;";
        ok = ok && got == result && trace == want && seen_color != &color && seen_pos != &position;
    }
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
