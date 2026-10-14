#include "comm_process.cpp"
signed char netNetworkMode;
int frontEndRunning;
float commRecord, commThresh, commPlayback;
int commEcho, commPV, commRVThresh, commRVInternal, commSendmask;
unsigned char enableMallocWrapper;
static int realCalls, wrapperInside, localNm = 2;
extern "C" int netGetLocalPlayerNmSafe__Fi(int index) { return index == 0 ? localNm : -1; }
extern "C" void commProcessReal__Fv() { ++realCalls; wrapperInside = enableMallocWrapper; }
int main() {
    commRecord = 0.5f; commThresh = 0.25f; commPlayback = 0.75f;
    commSendmask = 0x31;
    netNetworkMode = 0;
    commProcess__Fv();
    if (realCalls != 0 || commPV != 0 || commSendmask != 0x31) return 1;
    netNetworkMode = 1;
    frontEndRunning = 1;
    commProcess__Fv();
    if (realCalls != 0 || commPV != 0) return 2;
    frontEndRunning = 0;
    commEcho = 1;
    commProcess__Fv();
    // thresh 16384*32767/131072 = 4095 (trunc); record 0.5*4*65536=131072*25/65536=50; playback 0.75*4*65536=196608
    if (commRVThresh != 4095 || commRVInternal != 50 || commPV != 196608) return 3;
    if (commSendmask != (0x31 | 4)) return 4;
    if (realCalls != 1 || wrapperInside != 1 || enableMallocWrapper != 0) return 5;
    commEcho = 0;
    commSendmask = 0xff;
    commRecord = -0.5f; commThresh = -0.25f;
    commProcess__Fv();
    if (commSendmask != 0xfb) return 6;
    if (commRVThresh != -4095 || commRVInternal != -50) return 7;
    // truncation toward zero
    commThresh = 0.0001f;
    commProcess__Fv();
    if (commRVThresh != static_cast<int>(0.0001f * 65536.0f) * 32767 / 131072) return 8;
    return 0;
}
