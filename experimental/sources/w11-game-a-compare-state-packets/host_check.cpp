// Host contract for compareStatePackets; not an EE input.
#include "compare_state_packets.cpp"
#include <cstdio>
#include <cstdarg>
#include <cstring>
#include <string>
#include <vector>
signed char MC_Debug;
int megatileSeed, megatileRot;
unsigned eRandState;
unsigned char partners[4 * 17632];
static std::vector<std::string> events;
int sprintf(char* buffer, const char* format, ...) {
    va_list args; va_start(args, format);
    int n = std::vsnprintf(buffer, 80, format, args);
    va_end(args);
    events.push_back(std::string("fmt:") + format);
    return n;
}
void ustrcpy__FPUsPc(unsigned short* destination, char* source) {
    size_t i = 0;
    for (; source[i]; ++i) destination[i] = static_cast<unsigned char>(source[i]);
    destination[i] = 0;
    events.push_back("widen");
}
static std::string shown;
void hudSetInfoMessage__FPUsiic(unsigned short* text, int ticks, int color, char flag) {
    shown.clear();
    for (; *text; ++text) shown += static_cast<char>(*text);
    events.push_back("hud:" + std::to_string(ticks) + ":" + std::to_string(color) + ":" + std::to_string(flag));
}
static int colors[4];
void engineSetScreenColor__Fiiii(int r, int g, int b, int a) { colors[0] = r; colors[1] = g; colors[2] = b; colors[3] = a; events.push_back("color"); }
int main() {
    int bad = 0;
    unsigned first[4] = {1, 0, 5, 0}, second[4] = {0, 0, 5, 0};
    // equal words: nothing
    MC_Debug = 1; events.clear();
    compareStatePackets__FP11statePacketT0i(first, second, 2);
    if (!events.empty() || partners[2 * 17632 + 17623]) { std::puts("fail: equal words"); bad = 1; }
    // first word zero: nothing even when the second words differ
    second[2] = 9; first[0] = 0;
    compareStatePackets__FP11statePacketT0i(first, second, 2);
    if (!events.empty() || partners[2 * 17632 + 17623]) { std::puts("fail: first word zero"); bad = 1; }
    // mismatch with debug off flags only the partner
    first[0] = 7; MC_Debug = 0;
    compareStatePackets__FP11statePacketT0i(first, second, 3);
    if (!events.empty() || !partners[3 * 17632 + 17623] || partners[2 * 17632 + 17623]) { std::puts("fail: flag only"); bad = 1; }
    // mismatch with debug on
    MC_Debug = -1; megatileSeed = 0x1234abcd; megatileRot = 3; eRandState = 1;
    compareStatePackets__FP11statePacketT0i(first, second, 1);
    unsigned s1 = 1u * 1664525u + 1013904223u, s2 = s1 * 1664525u + 1013904223u, s3 = s2 * 1664525u + 1013904223u;
    if (shown != "Out of sync (seed=1234abcd:3)\n") { std::puts("fail: text"); bad = 1; }
    if (events.size() != 4 || events[0] != "fmt:Out of sync (seed=%08x:%d)\n" || events[1] != "widen" || events[2] != "hud:300:-1:0" || events[3] != "color") { std::puts("fail: order"); bad = 1; }
    if (colors[0] != static_cast<int>((s1 >> 16) & 255) || colors[1] != static_cast<int>((s2 >> 16) & 255) || colors[2] != static_cast<int>((s3 >> 16) & 255) || colors[3] != 128) { std::puts("fail: color"); bad = 1; }
    if (eRandState != s3 || !partners[1 * 17632 + 17623]) { std::puts("fail: state"); bad = 1; }
    if (!bad) std::puts("compareStatePackets ok");
    return bad;
}
