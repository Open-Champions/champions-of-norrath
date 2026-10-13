// Host contract for KbmReaderThread. Not an EE plan input. GLibData and LocalData are byte arrays;
// the RPC call, the semaphore wait and the six local callees are stubs that record what they were
// given in a trace string and, for the RPC, fill the reply buffer from a script. The thread never
// returns, so the RPC stub throws once its script runs out.
#include <cstdio>
#include <cstring>
#include <functional>
#include <string>
#include <vector>
#include "kbm_reader_thread.cpp"

alignas(16) unsigned char GLibData[1640];
alignas(16) unsigned char LocalData[2048];
struct Stop {};
static std::string trace;
static std::vector<std::function<int(unsigned char*)>> script;   // one entry per RPC call: fills the reply, returns the result
static size_t next_call;
static int sema_seen;

static std::string at(const void* p) {
    const unsigned char* c = static_cast<const unsigned char*>(p);
    if (c == GLibData) return "G";
    if (c >= LocalData && c < LocalData + sizeof LocalData) return "L" + std::to_string(c - LocalData);
    return c ? "?" : "0";
}
extern "C" int sceSifCallRpc(void* client, unsigned function, unsigned mode, void* send, int ssize, void* recv, int rsize,
                             void (*end)(void*), void* arg) {
    trace += "rpc(" + at(client) + "," + std::to_string(function) + "," + std::to_string(mode) + "," + at(send) + "," +
             std::to_string(ssize) + "," + at(recv) + "," + std::to_string(rsize) + "," + (end == kdm_rpccall_end ? "end" : "?") + "," + at(arg) + ");";
    if (next_call >= script.size()) throw Stop();
    return script[next_call++](static_cast<unsigned char*>(recv));
}
extern "C" int WaitSema(int s) { sema_seen = s; trace += "wait" + std::to_string(s) + ";"; return 0; }
extern "C" void InitialiseIopDriver() { trace += "init;"; }
extern "C" void kdm_rpccall_end(void*) {}
extern "C" void mark_device_unplugged(int d) { trace += "unplug" + std::to_string(d) + ";"; }
extern "C" void setup_kbd_device(void* r) { trace += "setkbd@" + at(r) + ";"; }
extern "C" void setup_mouse_device(void* r) { trace += "setmouse@" + at(r) + ";"; }
static bool shrink_count;
extern "C" void process_kbd_data(void* r) { trace += "pkbd@" + at(r) + ";"; if (shrink_count) LocalData[2] = 1; }
extern "C" void process_mouse_data(void* r) { trace += "pmouse@" + at(r) + ";"; }

static void put_word(int offset, int v) { std::memcpy(GLibData + offset, &v, 4); }
static int get_word(int offset) { int v; std::memcpy(&v, GLibData + offset, 4); return v; }
static void put_half(unsigned char* base, int offset, short v) { std::memcpy(base + offset, &v, 2); }
static short get_half(int offset) { short v; std::memcpy(&v, GLibData + offset, 2); return v; }
static void reset(int kbd, int mouse, int sema) {
    std::memset(GLibData, 0x77, sizeof GLibData); std::memset(LocalData, 0x55, sizeof LocalData);
    put_word(64, kbd); put_word(68, mouse); put_word(40, sema); put_word(60, 0); put_word(80, 0); put_word(84, 0);
    GLibData[64] = static_cast<unsigned char>(kbd); GLibData[68] = static_cast<unsigned char>(mouse);
    trace.clear(); script.clear(); next_call = 0; shrink_count = false;
}
static void run() { try { KbmReaderThread(); } catch (Stop&) {} }

static int agree(unsigned char* b) { b[0] = 1; b[1] = 2; return 0; }
int main() {
    const std::string poll = "rpc(G,1,1,L512,144,L512,144,end,L512);wait4;";
    // 1. set-up, then the wait loop: two polls with an empty reply, a third reporting one device
    reset(2, 1, 9);
    script.push_back([](unsigned char* b) { b[0] = 0; b[1] = 0; return 0; });
    script.push_back([](unsigned char* b) { b[0] = 0; b[1] = 0; return 0; });
    script.push_back([](unsigned char* b) {
        b[0] = 3; b[1] = 0;
        put_half(b, 4 + 2, 0); put_half(b, 4 + 4, 11);         // record 0 at LocalData+516: unplugged, id 11
        put_half(b, 4 + 16 + 2, 1);                             // record 1: keyboard
        put_half(b, 4 + 32 + 2, 2);                             // record 2: mouse
        return 0; });
    run();
    if (LocalData[3] != 16 || LocalData[514] != 3 || LocalData[512] != 3 || LocalData[513] != 0) return 1;   // [512] was set by the third poll reply
    for (int i = 0; i < 2; ++i) if (get_half(140 + 272 * i) != -1) return 2;
    if (get_half(140 + 272 * 2) != 0x7777) return 3;
    if (get_half(1228) != -1 || get_half(1228 + 104) != 0x7777) return 4;
    const std::string poll9 = "rpc(G,1,1,L512,144,L512,144,end,L512);wait9;";
    const std::string expected_head = "init;" + poll9 + poll9 + poll9 + "unplug11;setkbd@L532;setmouse@L548;";
    if (trace.compare(0, expected_head.size(), expected_head) != 0) { std::printf("trace: %s\n", trace.c_str()); return 5; }
    if (get_word(80) != 3 || get_word(84) != 0) return 6;
    // the loop then reads device data (function 2 on LocalData) and stops at the empty script
    if (trace.find("rpc(G,2,1,L0,464,L0,464,end,L0);") == std::string::npos) return 7;

    // set-up of the second array for two mouse slots
    reset(1, 2, 9);
    script.push_back([](unsigned char* b) { b[0] = 1; b[1] = 1; return 0; });
    run();
    if (get_half(1228) != -1 || get_half(1228 + 104) != -1 || get_half(1228 + 208) != 0x7777 || get_half(1228 + 105) == -1) return 14;

    // 2. main loop: no poll while the replies agree with the words; data with two records and the flag word
    reset(1, 1, 4);
    put_word(80, 1); put_word(84, 2);
    script.push_back(agree);                                  // set-up zeroes LocalData[512..513], so the first pass polls
    script.push_back([](unsigned char* b) { b[0] = 1; b[1] = 2; b[2] = 3;
        put_half(b, 4 + 4, 1); put_half(b, 4 + 28 + 4, 2); put_half(b, 4 + 56 + 4, 9); return 0; });
    run();
    if (trace != "init;" + poll + "rpc(G,2,1,L0,464,L0,464,end,L0);wait4;pkbd@L4;pmouse@L32;rpc(G,2,1,L0,464,L0,464,end,L0);") { std::printf("trace2: %s\n", trace.c_str()); return 8; }
    // the processors run in order and the third record (kind 9) is skipped; the count is re-read after a processor
    reset(1, 1, 4);
    put_word(80, 1); put_word(84, 2); shrink_count = true;
    script.push_back(agree);
    script.push_back([](unsigned char* b) { b[0] = 1; b[1] = 2; b[2] = 4;
        put_half(b, 4 + 4, 1); put_half(b, 4 + 28 + 4, 1); put_half(b, 4 + 56 + 4, 1); put_half(b, 4 + 84 + 4, 1); return 0; });
    run();
    if (trace.find("pkbd@L4;") == std::string::npos || trace.find("pkbd@L32;") != std::string::npos) { std::printf("trace3: %s\n", trace.c_str()); return 9; }

    // 3. a nonzero flag word: second call (function 4), wait only for a nonnegative result, flag cleared
    for (int result : {0, -1}) {
        reset(1, 1, 4);
        put_word(80, 1); put_word(84, 2); put_word(60, 5);
        script.push_back(agree);
        script.push_back([](unsigned char* b) { b[0] = 9; b[1] = 9; b[2] = 0; return 0; });
        script.push_back([result](unsigned char*) { return result; });
        run();
        const std::string first = "init;" + poll + "rpc(G,2,1,L0,464,L0,464,end,L0);wait4;rpc(G,4,1,0,0,0,0,end,0);";
        const std::string want = first + (result >= 0 ? "wait4;" : "") + "rpc(G,2,1,L0,464,L0,464,end,L0);";
        if (trace.compare(0, want.size(), want) != 0 || get_word(60) != 0) { std::printf("trace4: %s\n", trace.c_str()); return 10; }
        if (get_word(80) != 1) return 11;                                  // the reply bytes were not copied on this pass
    }

    // 4. the reply bytes are copied into the words when they differ, and a difference sends the loop back to a poll
    reset(1, 1, 4);
    put_word(80, 1); put_word(84, 2);
    script.push_back(agree);
    script.push_back([](unsigned char* b) { b[0] = 7; b[1] = 8; b[2] = 0; return 0; });
    script.push_back([](unsigned char* b) { b[0] = 7; b[1] = 8; return 0; });         // the poll
    run();
    if (get_word(80) != 7 || get_word(84) != 8) return 12;
    if (trace.find("rpc(G,2,1,L0,464,L0,464,end,L0);wait4;rpc(G,1,1,L512,144,L512,144,end,L512);") == std::string::npos) { std::printf("trace5: %s\n", trace.c_str()); return 13; }
    std::puts("KbmReaderThread cases passed");
    return 0;
}
