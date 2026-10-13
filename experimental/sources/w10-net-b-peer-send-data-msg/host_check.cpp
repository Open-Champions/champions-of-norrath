// Host check for peerSendDataMsg__FPciiP14sceInetAddressi. Not an EE plan input.
// Build on the host with: c++ -std=c++17 -fno-builtin host_check.cpp
#include "peer_send_data_msg.cpp"
#include <vector>

extern "C" {
unsigned char cd[8];
void* lnet_buf = reinterpret_cast<void*>(0x1000);
int udpLaunchConnection = 0x2222;
unsigned char netGameLaunchInfo[16];
alignas(8) unsigned char launchPeers[1008];
}
struct Send { std::vector<unsigned char> bytes; int length, flags_value, first, second, connection; void* address; };
static std::vector<Send> sends;
extern "C" void* memcpy(void* d, const void* s, unsigned long n) {
    unsigned char* b = (unsigned char*)d; const unsigned char* c = (const unsigned char*)s;
    for (unsigned long i = 0; i < n; ++i) b[i] = c[i];
    return d;
}
extern "C" int sceInetSendTo(void*, void*, int connection, void* data, int length, int* flags, void* address,
                             int first, int second) {
    sends.push_back({std::vector<unsigned char>((unsigned char*)data, (unsigned char*)data + length), length, *flags,
                     first, second, connection, address});
    *flags = 99;
    return 0;
}
int main() {
    char data[6] = {1, 2, 3, -4, -128, 5};
    netGameLaunchInfo[6] = 2;
    *(int*)(launchPeers + 252 * 2 + 64) = 0x01020304;
    int result = peerSendDataMsg__FPciiP14sceInetAddressi(data, 6, 9, (void*)0x55, 77);
    if (result != (int)0xa76b0009) return 1;
    if (sends.size() != 1 || sends[0].length != 18 || sends[0].flags_value != 0) return 2;
    if (sends[0].first != 77 || sends[0].second != 100 || sends[0].connection != 0x2222) return 3;
    const unsigned char want[18] = {4, 3, 2, 1, 4, 0, 0, 0, 9, 0, 0x6b, 0xa7, 1, 2, 3, 0xfc, 0x80, 5};
    for (int i = 0; i < 18; ++i) if (sends[0].bytes[i] != want[i]) return 4;
    // size zero: checksum is 535 << 16 | sequence, no payload.
    sends.clear();
    result = peerSendDataMsg__FPciiP14sceInetAddressi(data, 0, 3, 0, 1);
    if (result != ((535 << 16) | 3) || sends[0].length != 12) return 5;
    return 0;
}
