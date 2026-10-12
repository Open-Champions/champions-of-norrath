// Host check for netInit. Not an EE plan input.
// Build on the host with: c++ -std=c++17 -fno-builtin host_check.cpp
#include "net_init.cpp"
#include <cstdio>
#include <cstring>
#include <string>

extern "C" {
unsigned char netInitState;
int trueNetFrameNm, localNetFrameNm, localNetSentFrameNm;
unsigned char totallyDisconnected;
int netTrouble;
unsigned char netKickedOut;
unsigned char netCameraYaw[8];
unsigned char padBlank[48];
unsigned char netLocalPadState[32];
int localPredictedPlayer, logChecksum;
Compression compImmediateCompressor, compImmediateDecompressor, compLocalCompressor, compLocalDecompressor;
unsigned kickState[2];
int msgBufferSent;
unsigned char localPlayerPadState[8];
int derivedFrameNm, sentStatePacketFrameNm, noAdvanceTime, currentTime, lastPingTime;
unsigned char isModem;
unsigned char cd[64], g_net_buf[16], myaddr[16];
}
static unsigned char* comp_bytes(int o) {
    Compression* all[4] = {&compImmediateCompressor, &compImmediateDecompressor, &compLocalCompressor, &compLocalDecompressor};
    return (unsigned char*)all[o];
}

static std::string log_text;
static int libnet_result_ignored, list_result, list_store, list_calls, control_calls, search_result;
static void* ctl_args[2];
static int ctl_id, ctl_code, ctl_len;
static void* ctl_opt;
static int search_calls;
static bool bad;

static const char* tag_of(void* p) {
    if (p == netCameraYaw) return "Y";
    if (p == padBlank) return "P";
    if (p == netLocalPadState) return "N";
    if (p == kickState) return "K";
    if (p == localPlayerPadState) return "L";
    return "?";
}
static int stack_chunks;
extern "C" void* memset(void* d, int v, unsigned long n) {
    if (v != 0) bad = true;
    const char* t = tag_of(d);
    if (t[0] == '?') {  // a stack template chunk: sixteen bytes each, consecutive
        static unsigned char* last = nullptr;
        if (n != 16) bad = true;
        if (stack_chunks % 5 != 0 && (unsigned char*)d != last + 16) bad = true;
        last = (unsigned char*)d;
        ++stack_chunks;
        log_text += "t";
    } else {
        log_text += t; log_text += std::to_string(n);
    }
    unsigned char* b = (unsigned char*)d;
    for (unsigned long i = 0; i < n; ++i) b[i] = 0;
    return d;
}
extern "C" void sceSifMInitRpc(int mode) { log_text += "R"; if (mode != 0) bad = true; if (isModem != 1) {} currentTime = currentTime; }
extern "C" int sceLibnetInitialize(void* o, int a, int b, int c) {
    log_text += "I"; if (o != cd || a != 2048 || b != 8192 || c != 32) bad = true;
    if (isModem != 0) bad = true;  // isModem is cleared before this call
    return 0;
}
extern "C" int sceInetGetInterfaceList(void* o, void* buf, int* list, int count) {
    log_text += "G"; ++list_calls;
    if (o != cd || buf != g_net_buf || count != 1 || *list != -1000) bad = true;
    *list = list_store;
    return list_result;
}
extern "C" int sceInetInterfaceControl(void* o, void* buf, int id, int code, void* opt, int len) {
    log_text += "C"; ++control_calls;
    if (o != cd || buf != g_net_buf || code != 9 || opt != myaddr || len != 16) bad = true;
    ctl_id = id;
    return 0;
}
extern "C" int sceSifSearchModuleByName(char* name) {
    log_text += "S"; ++search_calls;
    if (std::strcmp(name, "INET_SPEED_UART_driver") != 0) bad = true;
    return search_result;
}

static void fill() {
    netInitState = 0;
    trueNetFrameNm = localNetFrameNm = localNetSentFrameNm = 77;
    totallyDisconnected = 5; netTrouble = 5; netKickedOut = 5;
    std::memset(netCameraYaw, 0xaa, 8); std::memset(padBlank, 0xaa, 48); std::memset(netLocalPadState, 0xaa, 32);
    localPredictedPlayer = logChecksum = 5;
    for (int o = 0; o < 4; ++o) std::memset(comp_bytes(o), 0xaa, 76);
    kickState[0] = kickState[1] = 0xaaaaaaaa; msgBufferSent = 5;
    std::memset(localPlayerPadState, 0xaa, 8);
    derivedFrameNm = sentStatePacketFrameNm = noAdvanceTime = currentTime = lastPingTime = 5;
    isModem = 9;
    log_text.clear(); list_calls = control_calls = search_calls = 0; stack_chunks = 0; bad = false;
}
static int run(int result, int store, int search) {
    fill(); list_result = result; list_store = store; search_result = search;
    netInit__Fv();
    if (bad) return 100;
    if (netInitState != 1 || trueNetFrameNm != 1 || localNetFrameNm != 1 || localNetSentFrameNm != 1) return 1;
    if (totallyDisconnected || netTrouble || netKickedOut) return 2;
    if (localPredictedPlayer || logChecksum || kickState[0] || kickState[1] || msgBufferSent) return 3;
    if (derivedFrameNm != -10 || sentStatePacketFrameNm || noAdvanceTime || currentTime || lastPingTime) return 4;
    for (unsigned char b : netCameraYaw) if (b) return 5;
    for (unsigned char b : padBlank) if (b) return 6;
    for (unsigned char b : netLocalPadState) if (b) return 7;
    for (unsigned char b : localPlayerPadState) if (b) return 8;
    static_assert(sizeof(Compression) == 76, "copy size");
    for (int o = 0; o < 4; ++o)
        for (int i = 0; i < 76; ++i) {
            bool float_byte = i < 64 && i % 16 >= 8 && i % 16 < 12;  // words at 8, 24, 40, 56
            unsigned char want = float_byte ? (i % 16 == 11 ? 0x43 : 0) : 0;  // 0x43000000 little endian
            if (comp_bytes(o)[i] != want) return 9;
        }
    return 0;
}
int main() {
    // guard: nothing happens once initialised
    fill(); netInitState = 1;
    netInit__Fv();
    if (!log_text.empty() || trueNetFrameNm != 77 || kickState[0] != 0xaaaaaaaa) return 50;

    // the interface list reports one interface: control call uses the word the callee stored
    int rc = run(1, 42, -1);
    if (rc) return rc;
    if (log_text != "Y8P48N32tttttttttttttttttttt" "K8L8" "R" "I" "G" "C" "S") return 60;
    if (ctl_id != 42 || isModem != 0) return 61;
    // module found: isModem set
    rc = run(1, 7, 0);
    if (rc) return rc;
    if (isModem != 1) return 62;
    // list failure: the word is replaced by -100 and the control call is skipped
    rc = run(0, 7, -1);
    if (rc) return rc;
    if (log_text != "Y8P48N32tttttttttttttttttttt" "K8L8" "R" "I" "G" "S" || control_calls != 0) return 63;
    // a negative module result leaves isModem clear; a positive one sets it
    rc = run(1, 1, 5);
    if (rc) return rc;
    return isModem == 1 ? 0 : 64;
}
