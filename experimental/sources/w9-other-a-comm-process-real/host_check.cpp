#include "comm_process_real.cpp"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>

extern "C" {
signed char netNetworkMode;
int frontEndRunning, commEnable, lastCE, lastlastCE, comm_handle, commRVInternal, commRVInternalLast, commRVThresh, commPV;
int commSendmask, commHandle, commStatusHandle, commNameMask, codec, timeout, packetshift, buffrmode, buffrfull, buffrout;
int readcount, writecount, framestart, drawFrameNm;
float commCVol;
unsigned char buffr[1280];
unsigned char packetbuffer[8512];
}
static std::vector<int> trace;
static int writeSample, sentPackets;
static int modem, syncBusy, syncStatus, readResult, writeResult, openResult, readGot, writeGot;
static unsigned enumEnd = 1;
static unsigned char entries[2][212];
static int hintWord;
extern "C" {
int netIsModem__Fv() { trace.push_back(1); return modem; }
int netGetLocalPlayerNmSafe__Fi(int) { return 2; }
void netSendMessage__FiPUci(int h, unsigned char* d, int n) { trace.push_back(100 + h); trace.push_back(d[0]); trace.push_back(n); }
void netSendPacket__FiiPUci(int h, int m, unsigned char* d, int n) { ++sentPackets; trace.push_back(200 + h); trace.push_back(m); trace.push_back(d[0]); trace.push_back(d[132]); trace.push_back(n); }
void lgAudEnumHint(unsigned char* h) { std::memset(h, 0, 16); std::memcpy(h, &hintWord, 4); trace.push_back(2); }
unsigned lgAudEnumerate(int i, unsigned char* info) {
    trace.push_back(300 + i);
    if (static_cast<unsigned>(i) >= enumEnd) return 0x80000002u;
    std::memcpy(info, entries[i], 212);
    return 0;
}
int lgAudOpen(int i, unsigned char* p, int* handle, int v) {
    trace.push_back(400 + i); trace.push_back(v);
    for (int k = 0; k < 14; ++k) trace.push_back(p[k]);
    *handle = 77;
    return openResult;
}
int lgAudSetRecordingVolume(int h, int c, int v) { trace.push_back(500 + h); trace.push_back(c); trace.push_back(v); return 0; }
int lgAudStartRecording(int) { trace.push_back(3); return 0; }
int lgAudStartPlayback(int) { trace.push_back(4); return 0; }
int syncCalls, busyAfter = 1000;
int syncCalls_reset() { syncCalls = 0; busyAfter = 1000; writeSample = 0; sentPackets = 0; return 0; }
int lgAudASync(int, int* s) { trace.push_back(5); *s = syncStatus; return ++syncCalls > busyAfter ? 1 : syncBusy; }
int lgAudARead(int, int, unsigned char* b, int* n) {
    trace.push_back(6); trace.push_back(*n); trace.push_back(static_cast<int>(b - buffr));
    if (readGot == 1280) reinterpret_cast<short*>(buffr)[3] = -20000;
    *n = readGot; return readResult;
}
int lgAudAWrite(int, int, unsigned char* b, int* n) { writeSample = reinterpret_cast<short*>(b)[0]; trace.push_back(7); trace.push_back(*n); trace.push_back(static_cast<int>(b - buffr)); *n = writeGot; return writeResult; }
int lgAudClose(int h) { trace.push_back(8); trace.push_back(h); return 0; }
int lgCodecEncode(int, unsigned char* in, int n, unsigned char* out, int* on) { trace.push_back(9); trace.push_back(n); std::memset(out, 0xab, 132); *on = 132; return 0; }
int lgCodecDecode(int, unsigned char* in, int n, unsigned char* out, int* on) {
    trace.push_back(10); trace.push_back(n);
    short* o = reinterpret_cast<short*>(out);
    for (int i = 0; i < 640; ++i) o[i] = static_cast<short>(in[0] * 1000);
    return 0;
}
}

static int fails;
#define CHECK(c) do { if (!(c)) { std::printf("FAIL line %d: %s\n", __LINE__, #c); ++fails; } } while (0)

static void reset() {
    extern int syncCalls_reset();
    syncCalls_reset();
    std::memset(buffr, 0, sizeof buffr); std::memset(packetbuffer, 0, sizeof packetbuffer);
    trace.clear(); netNetworkMode = 1; frontEndRunning = 0; commEnable = 1; lastCE = 1; lastlastCE = 1; comm_handle = 77;
    commRVInternal = commRVInternalLast = 5; commRVThresh = 100; commPV = 32768; commSendmask = 6; commHandle = 3;
    commStatusHandle = 4; commNameMask = 0; codec = 1; timeout = 0; packetshift = 0; buffrmode = 0; buffrfull = 0;
    buffrout = 0; readcount = writecount = 0; modem = 0; syncBusy = 0; syncStatus = 0; readResult = 0; writeResult = 0;
    openResult = 0; readGot = 1280; writeGot = 1280; enumEnd = 1; hintWord = 1; drawFrameNm = 9; framestart = 0;
    std::memset(entries, 0, sizeof entries);
}
static void eligible(unsigned char* e, int idx) { unsigned char* p = e + 2 + 6 * idx; p[0] = 1; p[1] = 16; p[2] = 0x40; p[3] = 0x1f; p[4] = 0x40; p[5] = 0x1f; }

int main() {
    reset(); netNetworkMode = 0; commProcessReal__Fv(); CHECK(trace.empty());
    reset(); frontEndRunning = 1; commProcessReal__Fv(); CHECK(trace.empty());
    reset(); modem = 1; comm_handle = -1; commProcessReal__Fv(); CHECK(trace.size() == 1 && lastCE == 0 && lastlastCE == 1);
    reset(); commEnable = 0; comm_handle = -1; commProcessReal__Fv(); CHECK(trace.empty() && lastCE == 0);
    // no hint and unchanged enable: nothing enumerated
    reset(); comm_handle = -1; hintWord = 0; commProcessReal__Fv(); CHECK(trace.size() == 2 && comm_handle == -1);
    // open path: entry 0 has an eligible input and output
    reset(); comm_handle = -1; eligible(entries[0], 0); eligible(entries[0] + 98, 0); eligible(entries[0] + 98, 0);
    entries[0][0] = 1; entries[0][98] = 1; readGot = 0; readResult = 0x80000000;
    commProcessReal__Fv();
    CHECK(trace[0] == 1 && trace[1] == 2 && trace[2] == 300 && trace[3] == 400 && trace[4] == 200);
    const int expect[14] = {3, trace[6], 1, 16, 0x40, 0x1f, 200, 0, 1, 16, 0x40, 0x1f, 100, 0};
    for (int k = 0; k < 14; ++k) CHECK(trace[5 + k] == expect[k]);
    CHECK(framestart == 9 && commNameMask == 0);
    // open failure resets handle and walks on, then ends
    reset(); comm_handle = -1; eligible(entries[0], 0); eligible(entries[0] + 98, 0); entries[0][0] = 1; entries[0][98] = 1; openResult = 1;
    commProcessReal__Fv(); CHECK(comm_handle == -1);
    // volume change re-applied
    reset(); commRVInternal = 9; buffrmode = 7; commProcessReal__Fv(); CHECK(commRVInternalLast == 9 && trace.size() == 4);
    // full pipeline pass: a loud block is sent, two remote senders are mixed, the write completes, the next read goes busy
    reset(); busyAfter = 2; commPV = 16384;
    packetbuffer[0] = 3; packetbuffer[132] = 1;
    packetbuffer[2128] = 5; packetbuffer[2128 + 132] = 1;
    commProcessReal__Fv();
    CHECK(timeout == 4 && readcount == 1280 && writecount == 1280 && buffrmode == 1 && buffrfull == 0);
    CHECK(commCVol > 0.60f && commCVol < 0.62f);
    CHECK(commNameMask == 3 && packetshift == 1 && writeSample == 4000);
    CHECK(packetbuffer[132] == 0 && packetbuffer[2128 + 132] == 0);
    CHECK(sentPackets == 1);
    // read short: fill stored, mode stays 0
    reset(); readGot = 100; commProcessReal__Fv(); CHECK(buffrfull == 100 && buffrmode == 0 && readcount == 0);
    // busy sync returns without change
    reset(); syncBusy = 1; commProcessReal__Fv(); CHECK(buffrmode == 1 && buffrfull == 0);
    // sync error closes
    reset(); syncStatus = 5; commProcessReal__Fv(); CHECK(comm_handle == -1 && commNameMask == 0);
    // read request failure closes; with commEnable cleared during this call the device closes too
    reset(); readResult = 1; commProcessReal__Fv(); CHECK(comm_handle == -1);
    reset(); modem = 1; commProcessReal__Fv(); CHECK(comm_handle == -1 && commEnable == 0);
    // write failure closes, short write stores fill and goes to mode 3
    reset(); writeResult = 1; commProcessReal__Fv(); CHECK(comm_handle == -1);
    reset(); writeGot = 300; buffrmode = 4; buffrfull = 0; buffrout = 300; commProcessReal__Fv(); CHECK(buffrfull == 300 && buffrmode == 3);
    std::printf(fails ? "FAILED %d\n" : "ok\n", fails);
    return fails != 0;
}
