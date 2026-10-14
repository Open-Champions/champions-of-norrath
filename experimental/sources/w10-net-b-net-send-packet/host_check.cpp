// Host check for netSendPacket__FiiPUci. Not an EE plan input.
// Build on the host with: c++ -std=c++17 -fno-builtin host_check.cpp
#include "net_send_packet.cpp"
#include <cstdio>
#include <string>
#include <vector>

extern "C" {
signed char netNetworkMode;
int nmPartners;
alignas(8) unsigned char partners[4 * 17632];
int broadcastConnection;
int broadcastConnectionFlags;
unsigned char cd[8], g_net_buf[8];
void (*pktHandler[4])(unsigned char*, int);
}

struct Send { int connection; std::vector<unsigned char> packet; int length, flags, port, second; unsigned char block[16]; };
static std::vector<Send> sends;
static std::vector<std::vector<unsigned char>> handled;
static std::vector<int> handled_len;

extern "C" void* memset(void* d, int v, unsigned long n) {
    unsigned char* b = (unsigned char*)d;
    for (unsigned long i = 0; i < n; ++i) b[i] = (unsigned char)v;
    return d;
}
extern "C" void* memcpy(void* d, const void* s, unsigned long n) {
    unsigned char* b = (unsigned char*)d; const unsigned char* c = (const unsigned char*)s;
    for (unsigned long i = 0; i < n; ++i) b[i] = c[i];
    return d;
}
extern "C" int sceInetSendTo(void*, void*, int connection, void* data, int length, int flags, void* address,
                             int first, int second) {
    Send s{connection, std::vector<unsigned char>((unsigned char*)data, (unsigned char*)data + length), length, flags, first, second, {}};
    for (int i = 0; i < 16; ++i) s.block[i] = ((unsigned char*)address)[i];
    if (broadcastConnectionFlags != 0) s.flags = -1;
    sends.push_back(s);
    return 0;
}
static void handler(unsigned char* data, int length) {
    handled.push_back(std::vector<unsigned char>(data, data + length));
    handled_len.push_back(length);
}
static void reset(int mode, int count) {
    netNetworkMode = (signed char)mode; nmPartners = count; broadcastConnection = 0x1234;
    for (auto& b : partners) b = 0;
    for (auto& h : pktHandler) h = 0;
    sends.clear(); handled.clear(); handled_len.clear();
    broadcastConnectionFlags = 0x55;
}
static void rec(int i, unsigned char a, unsigned char b, unsigned char c, unsigned char d, int port, int flag1, int flag2) {
    unsigned char* r = partners + 17632 * i;
    r[0] = a; r[1] = b; r[2] = c; r[3] = d; *(int*)(r + 4) = port; r[17620] = (unsigned char)flag1; r[17621] = (unsigned char)flag2;
}

int main() {
    unsigned char data[6] = {1, 2, 3, 4, 5, 6};
    // Network mode zero: nothing happens, flags untouched.
    reset(0, 2); rec(0, 10, 0, 0, 1, 77, 0, 0);
    netSendPacket__FiiPUci(1, 3, data, 6);
    if (!sends.empty() || !handled.empty() || broadcastConnectionFlags != 0x55) return 1;
    // Two partners, both selected, one sends and one has a handler.
    reset(1, 3);
    rec(0, 192, 168, 1, 5, 1111, 0, 0);
    rec(1, 192, 168, 1, 6, 2222, 1, 0);
    rec(2, 9, 9, 9, 9, 3333, 0, 1);
    pktHandler[2] = handler;
    netSendPacket__FiiPUci(2, 7, data, 6);
    if (sends.size() != 1 || handled.size() != 1) return 2;
    const Send& s = sends[0];
    unsigned char want_packet[10] = {0x46, 0xbd, 0, 0, 1, 2, 3, 4, 5, 6};
    if (s.length != 10 || s.connection != 0x1234 || s.flags != 0 || s.port != 1111 || s.second != 1000) return 3;
    for (int i = 0; i < 10; ++i) if (s.packet[i] != want_packet[i]) return 4;
    unsigned char want_block[16] = {0, 0, 0, 0, 192, 168, 1, 5, 0, 0, 0, 0, 0, 0, 0, 0};
    for (int i = 0; i < 16; ++i) if (s.block[i] != want_block[i]) return 5;
    if (handled_len[0] != 6) return 6;
    for (int i = 0; i < 6; ++i) if (handled[0][i] != data[i]) return 7;
    if (broadcastConnectionFlags != 0) return 8;
    // Mask excludes partner 0; a missing handler is skipped.
    reset(1, 2);
    rec(0, 1, 2, 3, 4, 5, 0, 0);
    rec(1, 5, 6, 7, 8, 9, 1, 0);
    netSendPacket__FiiPUci(0, 2, data, 0);
    if (!sends.empty() || !handled.empty()) return 9;
    netSendPacket__FiiPUci(0, 1, data, 0);
    if (sends.size() != 1 || sends[0].length != 4 || sends[0].packet[0] != 0x44) return 10;
    return 0;
}
