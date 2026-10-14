// Host comparison only. Not an EE build input.
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>
extern "C" {
unsigned char norrath_lgaud_client[64];
int norrath_lgaud_semaphore = -1;
unsigned char* norrath_lgaud_aligned_buffer;
}
static std::vector<std::string> events;
static int rpc_result;
static std::string text;
alignas(16) static unsigned char block[64];
extern "C" int w9_printf_stub(const char* f, ...) { text = f; events.push_back("printf"); return 0; }
extern "C" int WaitSema(int s) { events.push_back("wait" + std::to_string(s)); return 0; }
extern "C" int SignalSema(int s) {
    events.push_back("signal" + std::to_string(s));
    // The status word must already have been read: clobber it after the signal.
    std::memset(block, 0x77, 4);
    return 0;
}
extern "C" int sceSifCallRpc(void* c, int op, int mode, void* send, int ss, void* recv, int rs, void* done, void* da) {
    events.push_back("rpc");
    if (c != norrath_lgaud_client || op != 3 || mode != 0 || send != block || recv != block || ss != 32 || rs != 32 || done || da) events.push_back("BADARGS");
    int h; std::memcpy(&h, block + 12, 4);
    if (h != 0x1234) events.push_back("BADHANDLE");
    int reply = 0x55; std::memcpy(block, &reply, 4);
    return rpc_result;
}
#define printf w9_printf_stub
#include "lg_aud_close.cpp"
#undef printf

int main() {
    int failures = 0;
    norrath_lgaud_aligned_buffer = block;
    if (lgAudClose(0x1234) != static_cast<int>(0x8000000au) || !events.empty()) { std::printf("uninitialised wrong\n"); ++failures; }
    norrath_lgaud_semaphore = 6;
    rpc_result = 0;
    int r = lgAudClose(0x1234);
    if (r != 0x55) { std::printf("status must be read before the signal: %x\n", r); ++failures; }
    if (events != std::vector<std::string>{"wait6", "rpc", "signal6"}) { std::printf("event order wrong\n"); ++failures; }
    events.clear();
    rpc_result = -1;
    r = lgAudClose(0x1234);
    if (r != static_cast<int>(0x80000000u) || events != std::vector<std::string>{"wait6", "rpc", "printf", "signal6"} || text != "lgAudClose: rpc call failed\n") { std::printf("failure path wrong\n"); ++failures; }
    if (failures) return 2;
    std::printf("lgAudClose cases passed\n");
    return 0;
}
