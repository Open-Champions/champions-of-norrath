#include "cd_wait.cpp"

#include <cstdio>

unsigned char cdBusy;
unsigned char streaming;
unsigned char streamPaused;

static char log_[64];
static int log_length;
static int process_calls;
static int busy_polls;
static unsigned pushed;

static void note(char c) { log_[log_length++] = c; }

extern "C" void enginePushBorderColor__FUi(unsigned colour) { pushed = colour; note('['); }
extern "C" void enginePopBorderColor__Fv() { note(']'); }
extern "C" void cdProcess__Fv() {
    note('p');
    ++process_calls;
    if (--busy_polls == 0) cdBusy = 0;
}
extern "C" void engineSyncVBlank__Fv() { note('v'); }
extern "C" int sceCdStPause() { note('S'); return 1; }

static void reset(unsigned char busy, int polls, unsigned char stream, unsigned char paused) {
    log_length = 0;
    process_calls = 0;
    cdBusy = busy;
    busy_polls = polls;
    streaming = stream;
    streamPaused = paused;
}

static bool log_is(const char* expected) {
    log_[log_length] = 0;
    for (int i = 0;; ++i) {
        if (log_[i] != expected[i]) return false;
        if (!expected[i]) return true;
    }
}

int main() {
    reset(0, 0, 0, 0);
    cdWait__Fv();
    if (!log_is("[]") || pushed != 0xffffffu || streamPaused != 0) return 1;

    reset(1, 3, 0, 0);
    cdWait__Fv();
    if (!log_is("[pvpvpv]") || process_calls != 3 || cdBusy != 0) return 2;

    reset(0, 0, 1, 0);
    cdWait__Fv();
    if (!log_is("[]S") || streaming != 0 || streamPaused != 1) return 3;

    reset(1, 1, 1, 0);
    cdWait__Fv();
    if (!log_is("[pv]S") || streaming != 0 || streamPaused != 1) return 4;

    // Any nonzero byte counts as set.
    reset(0xff, 2, 0x80, 0);
    cdWait__Fv();
    if (!log_is("[pvpv]S") || streamPaused != 1) return 5;

    // streamPaused is left alone when not streaming.
    reset(0, 0, 0, 1);
    cdWait__Fv();
    if (streamPaused != 1 || !log_is("[]")) return 6;
    std::puts("cdWait__Fv passed");
    return 0;
}
