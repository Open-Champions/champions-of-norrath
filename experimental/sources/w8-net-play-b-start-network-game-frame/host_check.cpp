// Host check for netStartNetworkGameFrame. Not an EE plan input.
// Build on the host with: c++ -std=c++17 -fno-builtin host_check.cpp
#include "net_start_network_game_frame.cpp"
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>

extern "C" {
signed char netBroadCastStart;
unsigned drawFrameNm;
int broadcastConnection;
int broadcastConnectionFlags;
int nmPartners;
int nmPlayers;
unsigned char pad[0x160];
alignas(8) unsigned char partners[70528];
unsigned char cd[8], g_net_buf[8];
char levelName[0x142];
char* defaultFont = reinterpret_cast<char*>(0x1234);
}

struct Send { int connection; unsigned word; int length, flags, a, b; unsigned char block[16]; void* object; void* buffer; };
static std::vector<Send> sends;
static std::string events;
static int recv_calls, recv_result;
static unsigned recv_header, recv_word;
static unsigned char recv_from[16];
static bool bad;
struct Init { int index, second; std::string name; int b1, b2, b3, b4; char* tint; };
static std::vector<Init> inits;
struct Print { int x, y; std::string text; int limit; };
static std::vector<Print> prints;
static int font_starts, font_ends;
static void* font_arg; static int font_size, font_flags; static unsigned font_color;

extern "C" void* memset(void* d, int v, unsigned long n) {
    unsigned char* b = (unsigned char*)d;
    for (unsigned long i = 0; i < n; ++i) b[i] = (unsigned char)v;
    return d;
}
extern "C" int sceInetSendTo(void* o, void* buf, int c, void* data, int len, int flags, void* addr, int a, int b2) {
    Send s{c, *(unsigned*)data, len, flags, a, b2, {}, o, buf};
    std::memcpy(s.block, addr, 16);
    sends.push_back(s);
    events += "S";
    return 0;
}
extern "C" int sceInetRecvFrom(void* o, void* buf, int c, void* data, int len, int* flags, void* addr, void* word, int opt) {
    ++recv_calls; events += "R";
    if (o != cd || buf != g_net_buf || c != broadcastConnection || len != 512 || flags != &broadcastConnectionFlags || opt != 0)
        bad = true;
    if (broadcastConnectionFlags != 0) bad = true;
    if (recv_result >= 0) {
        *(unsigned*)data = recv_header;
        std::memcpy(addr, recv_from, 16);
        *(unsigned*)word = recv_word;
    }
    return recv_result;
}
extern "C" void playerInit__FiiPcScScScScT2(int i, int s, char* n, signed char a, signed char b, signed char c, signed char d, char* t) {
    inits.push_back({i, s, n, a, b, c, d, t});
}
extern "C" void fontStart__FP4FontiUii(void* f, int s, unsigned c, int fl) { ++font_starts; font_arg = f; font_size = s; font_color = c; font_flags = fl; events += "F"; }
extern "C" void fontPrintCentered__FiiPci(int x, int y, char* t, int l) { prints.push_back({x, y, t, l}); }
extern "C" void fontEnd__Fv() { ++font_ends; events += "E"; }

static void reset() {
    netBroadCastStart = 1; drawFrameNm = 0; broadcastConnection = 77; broadcastConnectionFlags = 9;
    nmPartners = 0; nmPlayers = 99; std::memset(pad, 0, sizeof pad); std::memset(partners, 0, sizeof partners);
    std::memset(levelName, 'x', sizeof levelName);
    sends.clear(); events.clear(); inits.clear(); prints.clear();
    recv_calls = font_starts = font_ends = 0; recv_result = -1; recv_header = 0; recv_word = 0;
    std::memset(recv_from, 0, 16); bad = false;
}
static void set_from(int a, int b, int c, int d) { recv_from[4] = a; recv_from[5] = b; recv_from[6] = c; recv_from[7] = d; }
static unsigned char* rec(int i) { return partners + i * 17632; }
static bool block_ok(const Send& s) {
    for (int i = 0; i < 16; ++i) {
        unsigned char want = i == 4 ? 0xff : i == 5 ? 0x01 : i == 6 ? 0xa8 : i == 7 ? 0xc0 : 0;
        if (s.block[i] != want) return false;
    }
    return s.object == cd && s.buffer == g_net_buf && s.connection == 77 && s.length == 4 && s.flags == 0 && s.a == 3000 && s.b == 1000;
}
#define CHECK(c, n) do { if (!(c)) { std::printf("fail %d\n", n); return n; } } while (0)

int main() {
    reset(); netBroadCastStart = 0;
    CHECK(netStartNetworkGameFrame__Fv() == 0 && events.empty() && broadcastConnectionFlags == 9, 1);

    // announce every 64th frame, no second broadcast with an empty table; failed receive draws
    reset(); drawFrameNm = 128;
    CHECK(netStartNetworkGameFrame__Fv() == 0, 2);
    CHECK(sends.size() == 1 && sends[0].word == 0xbd3d && block_ok(sends[0]), 3);
    CHECK(events == "SRFE" && !bad, 4);
    CHECK(font_arg == defaultFont && font_size == 6 && font_color == 0x80808080u && font_flags == -1, 5);
    reset(); drawFrameNm = 129;
    netStartNetworkGameFrame__Fv();
    CHECK(sends.empty() && events == "RFE", 6);

    // second broadcast: two partners, or the pad bit
    reset(); drawFrameNm = 5; nmPartners = 2;
    netStartNetworkGameFrame__Fv();
    CHECK(sends.size() == 8 && events == "SSSSSSSSRFE", 7);
    for (auto& s : sends) CHECK(s.word == 0xbd3e && block_ok(s), 8);
    reset(); drawFrameNm = 5; *(unsigned*)(pad + 12) = 0x40;
    netStartNetworkGameFrame__Fv();
    CHECK(sends.size() == 8, 9);
    reset(); drawFrameNm = 5; *(unsigned*)(pad + 12) = 0xffffffbf; *(unsigned*)(pad + 8) = 0x40;
    netStartNetworkGameFrame__Fv();
    CHECK(sends.empty(), 10);
    reset(); drawFrameNm = 64; nmPartners = 1;  // announce and nothing else
    netStartNetworkGameFrame__Fv();
    CHECK(sends.size() == 1 && sends[0].word == 0xbd3d, 11);
    reset(); drawFrameNm = 64; nmPartners = 3;  // announce, then eight of the other kind
    netStartNetworkGameFrame__Fv();
    CHECK(sends.size() == 9 && sends[0].word == 0xbd3d && sends[8].word == 0xbd3e, 12);

    // a valid announcement adds a partner
    reset(); drawFrameNm = 1; recv_result = 16; recv_header = 0xbd3d; recv_word = 777; set_from(10, 1, 2, 3);
    CHECK(netStartNetworkGameFrame__Fv() == 0, 13);
    CHECK(nmPartners == 1 && rec(0)[0] == 10 && rec(0)[1] == 1 && rec(0)[2] == 2 && rec(0)[3] == 3, 14);
    CHECK(*(int*)(rec(0) + 4) == 777, 15);
    CHECK(prints.size() == 1 && prints[0].x == 320 && prints[0].y == 50 && prints[0].text == "3.2.1.10" && prints[0].limit == 10000, 16);
    CHECK(inits.empty() && events == "RFE" && levelName[0] == 'x', 17);
    // the same address again changes nothing
    int before = nmPartners;
    netStartNetworkGameFrame__Fv();
    CHECK(nmPartners == before, 18);

    // sorting: first octets 5, 9, 7 end as 9, 7, 5 and whole records move
    reset(); drawFrameNm = 1; recv_result = 16; recv_header = 0xbd3d;
    for (int i = 0; i < 4; ++i) rec(i)[1000 + i] = 0;
    for (int i = 0; i < 4; ++i) rec(i)[20] = 0x70 + i;  // marker of the slot, not touched by adding
    int octets[3] = {5, 9, 7};
    for (int n = 0; n < 3; ++n) { set_from(octets[n], 0, 0, n + 1); netStartNetworkGameFrame__Fv(); }
    CHECK(nmPartners == 3, 19);
    CHECK(rec(0)[0] == 9 && rec(1)[0] == 7 && rec(2)[0] == 5, 20);
    // markers: slot0 got swapped with slot1 (second add), then slot1 with slot2 (third add)
    // add 5 -> [5(m70)]; add 9 into slot 1 (m71) -> swap -> [9(m71),5(m70)]; add 7 into slot 2 (m72):
    // j=2 swaps (5<7) -> [9(m71),7(m72),5(m70)]; j=1: 9<7 false.
    CHECK(rec(0)[20] == 0x71 && rec(1)[20] == 0x72 && rec(2)[20] == 0x70, 21);
    CHECK(rec(0)[3] == 2 && rec(1)[3] == 3 && rec(2)[3] == 1, 22);

    // start message: players initialised, level name set, nothing drawn
    reset(); drawFrameNm = 1; recv_result = 16; recv_header = 0xbd3e; set_from(1, 2, 3, 4);
    nmPartners = 0;
    CHECK(netStartNetworkGameFrame__Fv() == 1, 23);
    CHECK(nmPartners == 1 && nmPlayers == 1 && inits.size() == 1, 24);
    CHECK(inits[0].index == 0 && inits[0].second == 1 && inits[0].name == "Toonces" && inits[0].b1 == -1 && inits[0].b2 == 0 && inits[0].b3 == -1 && inits[0].b4 == 0 && inits[0].tint == nullptr, 25);
    CHECK(std::strcmp(levelName, "vtest2") == 0 && levelName[7] == 'x', 26);
    CHECK(font_starts == 0 && events == "R", 27);
    // an already known sender with four partners starts with all four names
    reset(); drawFrameNm = 1; recv_result = 16; recv_header = 0xbd3e; set_from(9, 9, 9, 9);
    nmPartners = 4;
    for (int i = 0; i < 4; ++i) rec(i)[0] = 100 + i, rec(i)[1] = 1;
    CHECK(netStartNetworkGameFrame__Fv() == 1 && nmPartners == 4 && nmPlayers == 4, 28);  // table full: not added
    const char* names[4] = {"Toonces", "Edna", "Otis", "Gaylord"};
    CHECK(inits.size() == 4, 29);
    for (int i = 0; i < 4; ++i) CHECK(inits[i].index == i && inits[i].name == names[i], 30);
    // a known sender at index 2 is found and is not added again
    reset(); drawFrameNm = 1; recv_result = 16; recv_header = 0xbd3e; set_from(7, 7, 7, 7);
    nmPartners = 3;
    rec(0)[0] = 1; rec(1)[0] = 2; rec(2)[0] = 7; rec(2)[1] = 7; rec(2)[2] = 7; rec(2)[3] = 7;
    CHECK(netStartNetworkGameFrame__Fv() == 1 && nmPartners == 3 && nmPlayers == 3, 31);
    // a near match (one byte off) is a different sender
    reset(); drawFrameNm = 1; recv_result = 16; recv_header = 0xbd3d; set_from(7, 7, 7, 8);
    nmPartners = 1; rec(0)[0] = 7; rec(0)[1] = 7; rec(0)[2] = 7; rec(0)[3] = 7;
    netStartNetworkGameFrame__Fv();
    CHECK(nmPartners == 2, 32);
    // a bad header changes nothing and draws
    reset(); drawFrameNm = 1; recv_result = 16; recv_header = 0xbd3f; set_from(7, 7, 7, 8);
    CHECK(netStartNetworkGameFrame__Fv() == 0 && nmPartners == 0 && font_starts == 1, 33);
    reset(); drawFrameNm = 1; recv_result = 0; recv_header = 0xbd3d; set_from(7, 7, 7, 8);  // zero-length receive still counts
    netStartNetworkGameFrame__Fv();
    CHECK(nmPartners == 1, 34);
    // four partners drawn with rising y
    reset(); drawFrameNm = 1; nmPartners = 4;
    for (int i = 0; i < 4; ++i) { rec(i)[0] = 10 + i; rec(i)[1] = 20; rec(i)[2] = 30; rec(i)[3] = 40 + i; }
    netStartNetworkGameFrame__Fv();
    CHECK(prints.size() == 4 && prints[3].y == 110 && prints[1].text == "41.30.20.11", 35);
    return 0;
}
