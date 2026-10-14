// Observed: MCA_Init__Fv at 0x0025c638 is 80 bytes. It stores -1 in
// clearBytes and 0 in queueEnd, queueStart, checkResults, lastDirCount,
// MCA_Port, MCA_Slot, and MCA_FileOpen, calls MCA_ClearInfo, then clears
// 16384 bytes at nullBuff.
// Inferred: nullBuff is the 16384-byte gap from that symbol to zeroTile.
// The five queue words were local. They are global here so MCA_Cmd can
// use the same words.
// Unknown: which consumer reads clearBytes, checkResults, and lastDirCount.

extern "C" void MCA_ClearInfo__Fv();
extern "C" void* memset(void* destination, int value, unsigned size);
extern "C" int MCA_Port;
extern "C" int MCA_Slot;
extern "C" int MCA_FileOpen;

extern "C" {
int clearBytes;
int queueEnd;
int queueStart;
int checkResults;
int lastDirCount;
}

namespace {
unsigned char nullBuff[16384];
}

extern "C" void MCA_Init__Fv() {
    clearBytes = -1;
    queueEnd = 0;
    queueStart = 0;
    checkResults = 0;
    lastDirCount = 0;
    MCA_Port = 0;
    MCA_Slot = 0;
    MCA_FileOpen = 0;
    MCA_ClearInfo__Fv();
    memset(nullBuff, 0, 16384u);
}
