#include "cdvd_exit.cpp"

#include <cstdio>

int cb_thid, cb_semid, sceCdCbfunc_num, _sceCd_ncmd_semid, _sceCd_scmd_semid, _sceCd_rcmd_semid;

static char events[64];
static int n;
static int dintr_result;
static void ev(char c) { events[n++] = c; }

extern "C" int SignalSema(int s) { ev('S'); ev(static_cast<char>('0' + s)); return 0; }
extern "C" int DeleteSema(int s) { ev('D'); ev(static_cast<char>('0' + s)); return 0; }
extern "C" int DIntr() { ev('d'); return dintr_result; }
extern "C" int EIntr() { ev('e'); return 1; }
extern "C" void sceSifRemoveCmdHandler(unsigned c) { ev(c == 0x80000012u ? 'R' : '?'); }

static bool is(const char* expected) {
    events[n] = 0;
    for (int i = 0;; ++i) {
        if (events[i] != expected[i]) return false;
        if (!expected[i]) return true;
    }
}

int main() {
    _sceCd_ncmd_semid = 1; _sceCd_scmd_semid = 2; _sceCd_rcmd_semid = 3; cb_semid = 4;

    cb_thid = 0; sceCdCbfunc_num = 7; dintr_result = 1; n = 0;
    cdvd_exit();
    if (!is("D1D2D3D4dRe") || sceCdCbfunc_num != 7) return 1;

    cb_thid = 9; dintr_result = 0; n = 0;
    cdvd_exit();
    if (!is("S4D1D2D3D4dR") || sceCdCbfunc_num != -1) return 2;
    std::puts("cdvd_exit passed");
    return 0;
}
