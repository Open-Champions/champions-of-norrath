// Host check for receivePackets__Fv. Not an EE plan input.
// Build on the host with: c++ -std=c++17 -fno-builtin host_check.cpp
#include "receive_packets.cpp"
#include <cstring>
#include <string>
#include <vector>

extern "C" {
signed char netNetworkMode;
signed char netKickedOut;
int nmPartners;
alignas(8) unsigned char partners[4 * 17632];
int broadcastConnection;
int broadcastConnectionFlags;
unsigned char cd[8], g_net_buf[8];
void (*pktHandler[4])(unsigned char*, int);
}

static std::vector<std::vector<unsigned char>> queue;
static std::string events;
static int handled_value, handled_length;
static std::vector<float> object_floats;

extern "C" void* memset(void* d, int v, unsigned long n) {
    unsigned char* b = (unsigned char*)d;
    for (unsigned long i = 0; i < n; ++i) b[i] = (unsigned char)v;
    return d;
}
extern "C" int sceInetRecvFrom(void*, void*, int, void* data, int length, int* flags, void*, void*, int) {
    if (length != 1984 || *flags != 0 || ((unsigned long)data & 15) != 0) events += "!";
    if (queue.empty()) return -1;
    std::vector<unsigned char> p = queue.front();
    queue.erase(queue.begin());
    memcpy(data, p.data(), p.size());
    broadcastConnectionFlags = 7;
    return (int)p.size();
}
extern "C" void receiveControlPacket__FPiib(int* buffer, int length, bool flag) {
    events += flag ? "R1:" : "R0:";
    events += std::to_string(buffer[0]) + ":" + std::to_string(length) + ";";
}
extern "C" void sendControlPacket__FiiiiiR9CCompressb(int a, int b, int c, int d, int e, void* object, bool flag) {
    events += "S" + std::to_string(a) + "," + std::to_string(b) + "," + std::to_string(c) + "," + std::to_string(d) + "," +
              std::to_string(e) + (flag ? ",t;" : ",f;");
    const float* f = (const float*)object;
    object_floats.assign(f, f + 20);
    for (int i = 0; i < 20; ++i) if (i % 4 != 2 && i < 16 && ((const int*)object)[i] != 0) events += "!";
}
static void handler(unsigned char* data, int length) {
    handled_value = *(int*)data; handled_length = length;
    events += "H;";
}
static std::vector<unsigned char> packet(int kind, int second, int extra) {
    std::vector<unsigned char> p(8 + extra, 0);
    memcpy(p.data(), &kind, 4); memcpy(p.data() + 4, &second, 4);
    return p;
}
int main() {
    // Empty queue: returns after one receive.
    if (events != "") return 1;
    receivePackets__Fv();
    if (events != "") return 2;
    // Handler kinds 0xbd44..0xbd47, null handler skipped, others ignored.
    pktHandler[1] = handler; pktHandler[3] = handler;
    queue = {packet(0xbd45, 99, 12), packet(0xbd46, 5, 0), packet(0xbd48, 5, 0), packet(0xbd40, 1, 0), packet(0xbd47, 77, 0)};
    receivePackets__Fv();
    if (events != "H;H;" || handled_value != 77 || handled_length != 4) return 3;
    // Control packets and the kicked-out flag.
    events.clear();
    queue = {packet(0xbd3f, 7, 8), packet(0xbd42, 8, 0), packet(0xbd43, 0, 0)};
    receivePackets__Fv();
    if (events != "R0:48447:16;R1:48450:8;" || netKickedOut != 1) return 4;
    // Kick message 0xbd41 with mask 0b0101 and three partners.
    events.clear();
    nmPartners = 3;
    for (int i = 0; i < 3; ++i) {
        unsigned char* r = partners + 17632 * i;
        r[17620] = 0; r[17621] = 0; r[17622] = 0;
        *(int*)(r + 17596) = 100 + 10 * i;
    }
    partners[17632 * 1 + 17621] = 1;
    queue = {packet(0xbd41, 5, 0)};
    receivePackets__Fv();
    if (partners[17622] != 1 || partners[17632 * 2 + 17622] != 1 || partners[17632 + 17622] != 0) return 5;
    if (*(int*)(partners + 17624) != -1 || *(int*)(partners + 17632 * 2 + 17624) != -1) return 6;
    if (events != "S0,60,100,48450,0,t;S0,80,120,48450,2,t;") return 7;
    if (object_floats[2] != 128.0f || object_floats[6] != 128.0f || object_floats[10] != 128.0f || object_floats[14] != 128.0f) return 8;
    // A frame below 40 gives a first argument of zero.
    events.clear();
    *(int*)(partners + 17596) = 10; partners[17632 * 2 + 17622] = 0;
    queue = {packet(0xbd41, 1, 0)};
    receivePackets__Fv();
    if (events != "S0,0,10,48450,0,t;") return 9;
    return 0;
}
