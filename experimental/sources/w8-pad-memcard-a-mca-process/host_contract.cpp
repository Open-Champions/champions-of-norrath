#include "mca_process.cpp"

#include <cstdio>
#include <initializer_list>
#include <cstring>

queueCmd commandQueue[256];
unsigned char nullBuff[16384];
unsigned char MCA_DirContents[4096];
int MCA_Port = 1, MCA_Slot = 2, MCA_FileOpen = 7, MCA_Result, cardType, queueStart, queueEnd, checkResults, clearBytes = -1;

static int sync_value, sync_calls, sync_mode, serviced, serviced_ok;
static int* sync_result_ptr;
static int* serviced_error;
static queueCmd serviced_cmd;
static int last_call, calls;
static int a[6];
static const void* p[3];
static int open_ret = 55, at_call_queue_third, at_call_clear, at_call_fileopen;

static void note(int id, int x0 = 0, int x1 = 0, int x2 = 0, int x3 = 0, int x4 = 0) {
    last_call = id; ++calls; a[0] = x0; a[1] = x1; a[2] = x2; a[3] = x3; a[4] = x4;
    at_call_queue_third = commandQueue[queueStart].third;
    at_call_clear = clearBytes;
    at_call_fileopen = MCA_FileOpen;
}
extern "C" int sceMcSync(int mode, int* cmd, int* result) {
    ++sync_calls; sync_mode = mode; sync_result_ptr = result;
    if (cmd != 0) return -99;
    return sync_value;
}
extern "C" int sceMcOpen(int port, int slot, const void* name, int mode) { note(1, port, slot, mode); p[0] = name; return open_ret; }
extern "C" int sceMcClose(int fd) { note(2, fd); return 0; }
extern "C" int sceMcSeek(int fd, const void* off, int origin) { note(3, fd, origin); p[0] = off; return 0; }
extern "C" int sceMcRead(int fd, const void* buf, int size) { note(4, fd, size); p[0] = buf; return 0; }
extern "C" int sceMcWrite(int fd, const void* buf, int size) { note(5, fd, size); p[0] = buf; return 0; }
extern "C" int sceMcGetDir(int port, int slot, const void* name, int mode, int max, void* table) {
    note(6, port, slot, mode, max); p[0] = name; p[1] = table; return 0;
}
extern "C" int sceMcDelete(int port, int slot, const void* name) { note(7, port, slot); p[0] = name; return 0; }
extern "C" int sceMcGetInfo(const void* port, int slot, int* type, int free, int format) {
    note(8, slot, free, format); p[0] = port; p[1] = type; return 0;
}
extern "C" int sceMcFormat(const void* port, int slot) { note(9, slot); p[0] = port; return 0; }
extern "C" int sceMcChdir(int port, int slot, const void* dir, int cur) { note(10, port, slot, cur); p[0] = dir; return 0; }
extern "C" int sceMcMkdir(int port, int slot, const void* name) { note(11, port, slot); p[0] = name; return 0; }
extern "C" void serviceResult__FP8queueCmdPi(queueCmd* c, int* err) {
    ++serviced; serviced_cmd = *c; serviced_error = err; serviced_ok = checkResults;
}
extern "C" int commandQueueEmpty__Fv() { return queueStart == queueEnd; }

static int err;
static void put(int slot, int cmd, const void* text, int third, int fourth) {
    commandQueue[slot].command = cmd; commandQueue[slot].text = text;
    commandQueue[slot].third = third; commandQueue[slot].fourth = fourth;
}
static void reset() {
    sync_value = 1; sync_calls = 0; serviced = 0; calls = 0; last_call = 0; checkResults = 0;
    queueStart = 0; queueEnd = 0; clearBytes = -1; MCA_FileOpen = 7; std::memset(commandQueue, 0, sizeof commandQueue);
}

int main() {
    static char name[] = "n";
    // Empty queue, finished command: nothing is dispatched, result 0.
    reset(); sync_value = -1;
    if (MCA_Process__FPi(&err) != 0 || sync_calls != 1 || sync_mode != 1 || sync_result_ptr != &MCA_Result) return 1;
    if (calls != 0 || serviced != 0) return 2;
    // A completed command with checkResults set is serviced once, with the previous cq.
    reset(); checkResults = 1; queueEnd = 1; put(0, 0, name, 3, 0);
    if (MCA_Process__FPi(&err) != 0) return 3;  // the queue is empty again after the dispatch
    if (serviced != 1 || serviced_error != &err || serviced_ok != 1) return 4;  // cq is stale on the first call
    if (checkResults != 1 || last_call != 1 || a[0] != 1 || a[1] != 2 || a[2] != 3 || p[0] != name) return 5;
    if (at_call_fileopen != 1 || MCA_FileOpen != 1) return 6;
    if (queueStart != 1) return 7;
    // Second call: serviced entry is the open command just copied.
    sync_value = 1; queueEnd = 1;
    if (MCA_Process__FPi(&err) != 0 || serviced != 2 || serviced_cmd.command != 0 || serviced_cmd.third != 3) return 8;
    if (checkResults != 0) return 9;
    // A running command blocks dispatch and servicing, and the result stays 1.
    reset(); sync_value = 0; checkResults = 1; queueEnd = 1; put(0, 0, name, 3, 0);
    if (MCA_Process__FPi(&err) != 1 || serviced != 0 || calls != 0 || checkResults != 1 || queueStart != 0) return 10;
    reset(); sync_value = 0;
    if (MCA_Process__FPi(&err) != 1) return 11;  // empty queue but still running

    // Each command in turn.
    reset(); queueEnd = 12;
    open_ret = 77;
    put(0, 0, name, 2, 0);
    if (MCA_Process__FPi(&err) != 1 || last_call != 1 || a[0] != 1 || a[1] != 2 || a[2] != 2) return 12;
    put(1, 1, 0, 0, 0);
    MCA_FileOpen = 5;
    MCA_Process__FPi(&err);
    if (last_call != 2 || a[0] != 77 || MCA_FileOpen != 0 || at_call_fileopen != 0) return 13;
    put(2, 2, name, 9, 0);
    MCA_Process__FPi(&err);
    if (last_call != 3 || a[0] != 77 || a[1] != 9 || p[0] != name) return 14;
    put(3, 3, name, 16, 0);
    MCA_Process__FPi(&err);
    if (last_call != 4 || a[0] != 77 || a[1] != 16 || p[0] != name) return 15;
    put(4, 4, name, 20, 0);
    MCA_Process__FPi(&err);
    if (last_call != 4 || a[1] != 20) return 16;
    put(5, 5, name, 30, 0);
    MCA_Process__FPi(&err);
    if (last_call != 5 || a[0] != 77 || a[1] != 30 || p[0] != name || clearBytes != -1) return 17;
    put(6, 6, name, 0, 0);
    MCA_Process__FPi(&err);
    if (last_call != 6 || a[0] != 1 || a[1] != 2 || a[2] != 0 || a[3] != 64 || p[0] != name || p[1] != MCA_DirContents) return 18;
    put(7, 7, name, 0, 0);
    MCA_Process__FPi(&err);
    if (last_call != 7 || a[0] != 1 || a[1] != 2 || p[0] != name) return 19;
    put(8, 8, name, 4, 0x1234);
    MCA_Process__FPi(&err);
    if (last_call != 8 || a[0] != 4 || a[1] != 0x1234 || a[2] != 0 || p[0] != name || p[1] != &cardType) return 20;
    put(9, 9, name, 6, 0);
    MCA_Process__FPi(&err);
    if (last_call != 9 || a[0] != 6 || p[0] != name) return 21;
    put(10, 10, name, 0, 0);
    MCA_Process__FPi(&err);
    if (last_call != 10 || a[0] != 1 || a[1] != 2 || a[2] != 0 || p[0] != name) return 22;
    put(11, 11, name, 0, 0);
    MCA_Process__FPi(&err);
    if (last_call != 11 || a[0] != 1 || a[1] != 2 || p[0] != name) return 23;
    if (queueStart != 12) return 24;

    // Commands 12 and above issue nothing but still advance.
    for (int cmd : {12, 13, 255, -1}) {
        reset(); queueEnd = 1; put(0, cmd, name, 0, 0);
        int before = calls;
        MCA_Process__FPi(&err);
        if (calls != before || queueStart != 1 || checkResults != 1) return 25;
    }

    // Zero-fill write: three chunks for 40000 bytes.
    reset(); queueEnd = 1; put(0, 5, 0, 40000, 0);
    int sizes[3] = {16384, 16384, 7232};
    for (int i = 0; i < 3; ++i) {
        MCA_Process__FPi(&err);
        if (last_call != 5 || a[1] != sizes[i] || p[0] != nullBuff) return 26;
        if (at_call_queue_third != sizes[i]) return 27;
        if (i < 2 && (queueStart != 0 || clearBytes != 40000 - 16384 * (i + 1))) return 28;
    }
    if (clearBytes != -1 || queueStart != 1) return 29;
    // Small fill ends in one call; an exact multiple ends without a leftover.
    reset(); queueEnd = 1; put(0, 5, 0, 100, 0);
    MCA_Process__FPi(&err);
    if (a[1] != 100 || clearBytes != -1 || queueStart != 1) return 30;
    reset(); queueEnd = 1; put(0, 5, 0, 32768, 0);
    MCA_Process__FPi(&err);
    if (a[1] != 16384 || clearBytes != 16384 || queueStart != 0) return 31;
    MCA_Process__FPi(&err);
    if (a[1] != 16384 || clearBytes != -1 || queueStart != 1) return 32;

    // The queue index wraps at 256.
    reset(); queueStart = 255; queueEnd = 3; put(255, 12, 0, 0, 0);
    MCA_Process__FPi(&err);
    if (queueStart != 0) return 33;
    std::puts("mca process passed");
    return 0;
}
