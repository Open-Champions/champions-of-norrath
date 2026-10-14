// Host check for netGameStep. Not an EE plan input.
// Build on the host with: c++ -std=c++17 -fno-builtin host_check.cpp
// Pointers are 64-bit here; the checks only use the code's own structures.
#include "net_game_step.cpp"
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>

extern "C" {
int nmPartners;
unsigned char netNmPartners;
alignas(8) unsigned char partners[70528];
int trueNetFrameNm, localNetFrameNm, localNetSentFrameNm, noAdvanceTime, msgBufferSent;
alignas(8) unsigned char kickState[8];
int broadcastConnection, broadcastConnectionFlags, drawFrameNm;
unsigned char cd[8], g_net_buf[8];
int packetSendCycleLength, packetSendCycleTime;
signed char isModem, totallyDisconnected;
int netTrouble;
alignas(8) unsigned char pad[0x160], netLocalPad[0x160], netLocalPadState[64], padBlank[64];
alignas(8) static unsigned char camera_storage[64];
Camera* gameCam = reinterpret_cast<Camera*>(camera_storage);
alignas(8) unsigned char player[95936];
}

static std::string log_text;
static int local_player, ai_steps, pad_result;
struct Sent { unsigned header; int limit; unsigned char block[16]; int length; };
static std::vector<Sent> sent;
struct Control { int partner, first, second, header, local; bool flag; };
static std::vector<Control> controls;
static BitWriter* last_writer;
static int writer_position_at_control = -1;
static unsigned short writer_units_at_control[8];
static int analyses;

extern "C" void* memset(void* d, int v, unsigned long n) {
    unsigned char* b = (unsigned char*)d;
    for (unsigned long i = 0; i < n; ++i) b[i] = (unsigned char)v;
    log_text += "m";
    return d;
}
extern "C" int netGetLocalPlayerNm__Fi(int) { return local_player; }
extern "C" void receivePackets__Fv() { log_text += "R"; }
extern "C" int sceInetSendTo(void*, void*, int, void* data, int length, int, void* address, int limit, int) {
    Sent s{};
    std::memcpy(&s.header, data, 4);
    s.limit = limit; s.length = length;
    std::memcpy(s.block, address, 16);
    sent.push_back(s);
    log_text += "S";
    return 0;
}
extern "C" void sendControlPacket__FiiiiiR9CCompressb(int partner, int first, int second, int header, int local, CCompress&, bool flag) {
    controls.push_back({partner, first, second, header, local, flag});
    if (header == 0xbd3f && last_writer) {
        writer_position_at_control = last_writer->position;
        std::memcpy(writer_units_at_control, last_writer->data, sizeof writer_units_at_control);
    }
    log_text += "C";
}
extern "C" int padGetUnprocessedData__FiP18padUnprocessedData(int, void* data) { std::memset(data, 0, 12); return pad_result; }
extern "C" int GetYaw__6Camera(Camera*) { return 0x100; }
extern "C" void padProcessData__FP3padP18padUnprocessedDataP12padStateData(void*, void*, void*) { log_text += "P"; }
extern "C" int engineGetAIStepCount__Fv() { return ai_steps; }
extern "C" void analCompress__9CCompressP15BitBufferWriterPQ29CCompress9analStatePUci(void*, void* writer, void*, unsigned char*, int) {
    last_writer = static_cast<BitWriter*>(writer);
    ++analyses;
}
extern "C" void ustrcpy__FPUsPc(unsigned short*, char*) {}
extern "C" char* textGetString__FPc(char*) { return nullptr; }
extern "C" void ustrcat__FPUsT0(unsigned short*, char*) {}
extern "C" void hudSetInfoMessage__FPUsiic(unsigned short*, int, int, char) {}
extern "C" void advanceRealState__Fi(int) { log_text += "A"; }
extern "C" void runPrediction__Fi(int) { log_text += "X"; }

static unsigned char* rec(int i) { return partners + i * 17632; }
static int& W(unsigned char* b, int o) { return *reinterpret_cast<int*>(b + o); }
static void reset() {
    std::memset(partners, 0, sizeof partners);
    std::memset(kickState, 0, 8); std::memset(pad, 0, sizeof pad); std::memset(netLocalPad, 0, sizeof netLocalPad);
    std::memset(padBlank, 0, sizeof padBlank);
    std::memset(camera_storage, 0, sizeof camera_storage);
    *reinterpret_cast<int*>(camera_storage + 52) = 1;
    nmPartners = 0; trueNetFrameNm = 100; localNetFrameNm = 100; localNetSentFrameNm = 99; noAdvanceTime = 0;
    msgBufferSent = 0; drawFrameNm = 1; packetSendCycleLength = 5; packetSendCycleTime = 1; isModem = 0; totallyDisconnected = 0;
    netTrouble = 0; broadcastConnection = 7; local_player = 0; ai_steps = 0; pad_result = 0;
    log_text.clear(); sent.clear(); controls.clear(); last_writer = nullptr; writer_position_at_control = -1; analyses = 0;
    count_state = 0; min_steps_to_take = 100; min_steps_timer = 0;
}
#define CHECK(c, n) do { if (!(c)) { std::printf("fail %d\n", n); return n; } } while (0)

int main() {
    // pings: a nonzero counter advances, wraps after 121, and the old value divisible by 10 sends
    reset(); nmPartners = 2;
    W(rec(0), 17628) = 10; W(rec(1), 17628) = 5; W(rec(0), 4) = 1234;
    rec(0)[0] = 10; rec(0)[1] = 1; rec(0)[2] = 2; rec(0)[3] = 3;
    netGameStep__FbT0(false, false);
    CHECK(W(rec(0), 17628) == 11 && W(rec(1), 17628) == 6, 1);
    CHECK(sent.size() == 1 && sent[0].header == 0xbd43 && sent[0].limit == 1234 && sent[0].length == 4, 2);
    CHECK(sent[0].block[4] == 10 && sent[0].block[5] == 1 && sent[0].block[6] == 2 && sent[0].block[7] == 3 && sent[0].block[0] == 0, 3);
    reset(); nmPartners = 1; W(rec(0), 17628) = 121;
    netGameStep__FbT0(false, false);
    CHECK(W(rec(0), 17628) == 0 && sent.empty(), 4);  // 121 % 10 != 0 and the counter wraps
    reset(); nmPartners = 1; W(rec(0), 17628) = 120;
    netGameStep__FbT0(false, false);
    CHECK(W(rec(0), 17628) == 121 && sent.size() == 1, 5);
    reset(); nmPartners = 1; W(rec(0), 17628) = 10;
    netGameStep__FbT0(true, false);
    CHECK(W(rec(0), 17628) == 10 && sent.empty(), 6);  // flagA suppresses pings
    reset(); nmPartners = 1; W(rec(0), 17628) = 0;
    netGameStep__FbT0(false, false);
    CHECK(W(rec(0), 17628) == 0 && sent.empty(), 7);

    // timeout marking, only when the local player is zero
    reset(); nmPartners = 1; W(rec(0), 17612) = 900;
    netGameStep__FbT0(false, false);
    CHECK(rec(0)[17622] == 0 && W(rec(0), 17612) == 901, 8);
    reset(); nmPartners = 1; W(rec(0), 17612) = 901;
    netGameStep__FbT0(false, false);
    CHECK(rec(0)[17622] == 1 && W(rec(0), 17624) == -1 && W(kickState, 0) == 1, 9);
    reset(); nmPartners = 1; W(rec(0), 17612) = 901; local_player = 1;
    netGameStep__FbT0(false, false);
    CHECK(rec(0)[17622] == 0 && W(rec(0), 17612) == 901, 10);

    // an encoded packet for two identical frames
    reset(); nmPartners = 1; local_player = 0; packetSendCycleTime = 0; packetSendCycleLength = 5;
    W(rec(0), 92) = 10; W(rec(0), 17600) = 12;
    for (int f = 10; f < 12; ++f) {
        unsigned char* e = rec(0) + 172 + (f & 0xff) * 24;
        *reinterpret_cast<unsigned short*>(e + 4) = 0x1234;
        W(e, 12) = 0x100;
    }
    *reinterpret_cast<short*>(rec(0) + 96 + 64) = 0x100;
    *reinterpret_cast<short*>(rec(0) + 96 + 66) = 0x100;
    *reinterpret_cast<short*>(rec(0) + 96 + 68) = 0;
    ai_steps = 0;
    netGameStep__FbT0(false, false);
    CHECK(controls.size() == 1 && controls[0].header == 0xbd3f && controls[0].partner == 0 && controls[0].first == 12, 11);
    CHECK(controls[0].second == 99 && controls[0].local == 0 && !controls[0].flag, 12);
    CHECK(analyses == 4 && W(rec(0), 92) == 12, 13);
    // 16 bits 0x1234, run 2 (5 bits), yaw run 2 (5 bits), flag 0 (1 bit), flag run 2 (5 bits), 0xa5a5 (16 bits)
    CHECK(writer_position_at_control == 48, 14);
    // expected bit string for the first 48 bits
    std::string want = "0001001000110100" "00010" "00010" "0" "00010" "1010010110100101";
    std::string got;
    for (int b = 0; b < 48; ++b) got += (writer_units_at_control[b >> 4] >> (15 - (b & 15))) & 1 ? '1' : '0';
    if (got != want) { std::printf("bits %s want %s\n", got.c_str(), want.c_str()); return 15; }
    CHECK(packetSendCycleTime == 1, 16);
    return 0;
}
