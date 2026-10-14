#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <setjmp.h>

#define NORRATH_HOST_CONTRACT 1
#include "frame_end.cpp"

extern "C" {
volatile signed char coreLoadingScreenRunning;
volatile unsigned drawFrameNm;
int fullFrameMode;
unsigned switchToHalfCounter_797 asm("switchToHalfCounter.797");
int vblankSemaphore;
int switchTo30Counter_798 asm("switchTo30Counter.798");
int engineDisplayDisable;
unsigned lastFrameVBlankCount;
volatile signed char powerOffMode;
volatile int vblankCount;
volatile unsigned char vblankField;
int vblankSemaphore2;
unsigned draw[32];
}

namespace {
unsigned failures;
unsigned char stack_bytes[5120];
int stack_loads;
int stack_out;
int zero_stores;
int timer_loads;
int timer_stores;
unsigned timer_value;
unsigned timer_stored;
int prints;
int polls;
int equal_polls_remaining;
int waits;
int wait_ids[8];
int video_count;
int video_mode[4];
unsigned half_at_video;
int thirty_at_video;
unsigned frame_at_pack;
unsigned quot_at_super;
int super_selector;
int super_word;
int super_argument;
int sync_mode;
int sync_timeout;
char sequence[512];
jmp_buf escape_buf;
int escape_armed;

void expect(bool condition, const char* message) {
    if (!condition) {
        std::fprintf(stderr, "fail: %s\n", message);
        ++failures;
    }
}

void note(const char* name) {
    if (sequence[0] != 0) std::strcat(sequence, ",");
    std::strcat(sequence, name);
}

void reset() {
    std::memset(stack_bytes, 0xa5, sizeof stack_bytes);
    stack_loads = stack_out = zero_stores = 0;
    timer_loads = timer_stores = 0;
    timer_value = 100u;
    timer_stored = 0xffffffffu;
    prints = polls = waits = video_count = 0;
    equal_polls_remaining = 1;
    frame_at_pack = 0xffffffffu;
    quot_at_super = 0xffffffffu;
    super_selector = super_word = super_argument = -1;
    sync_mode = sync_timeout = -1;
    sequence[0] = 0;
    escape_armed = 0;
    coreLoadingScreenRunning = 0;
    drawFrameNm = 10u;
    fullFrameMode = 0;
    switchToHalfCounter_797 = 40u;
    vblankSemaphore = 5;
    switchTo30Counter_798 = 4;
    engineDisplayDisable = 3;
    lastFrameVBlankCount = 0xffffffffu;
    powerOffMode = 0;
    vblankCount = 8;
    vblankField = 0;
    vblankSemaphore2 = 9;
    for (unsigned i = 0; i < 32u; ++i) draw[i] = 0x100u + i;
    draw[4] = 0;
    draw[7] = 0;
    draw[15] = 0;
    draw[9] = 5;
    draw[13] = 5;
    draw[14] = 5;
}
}  // namespace

extern "C" unsigned char norrath_host_load8(unsigned address) {
    if (address >= 0x01fc0000u && address < 0x01fc0000u + 5120u) {
        ++stack_loads;
        return stack_bytes[address - 0x01fc0000u];
    }
    ++stack_out;
    return 0;
}

extern "C" unsigned norrath_host_load32(unsigned address) {
    if (address != 0x10000000u) {
        expect(false, "timer load address");
        return 0;
    }
    ++timer_loads;
    return timer_value;
}

extern "C" void norrath_host_store32(unsigned address, unsigned value) {
    if (address == 0u && value == 0u) {
        ++zero_stores;
        return;
    }
    if (address == 0x10000000u) {
        ++timer_stores;
        timer_stored = value;
        timer_value = value;
        return;
    }
    expect(false, "unexpected store");
}

extern "C" void scePrintf(const char* text, const char* site, int line) {
    ++prints;
    expect(std::strcmp(text, "stack error!  %s:%d\n") == 0, "stack text");
    expect(std::strcmp(site, "engine.cc") == 0, "stack site");
    expect(line == 1245, "stack line");
}

extern "C" void drawWaitFinished__Fv() { note("drawWait"); }
extern "C" void sceGsSyncPath(int mode, int timeout) {
    note("sync");
    sync_mode = mode;
    sync_timeout = timeout;
}
extern "C" void texturePackDown__Fv() {
    note("pack");
    frame_at_pack = drawFrameNm;
}
extern "C" void engineSetVideoMode__Fb(int mode) {
    note("video");
    if (video_count < 4) video_mode[video_count] = mode;
    ++video_count;
    half_at_video = switchToHalfCounter_797;
    thirty_at_video = switchTo30Counter_798;
}
extern "C" void WaitSema(int id) {
    note("wait");
    if (waits < 8) wait_ids[waits] = id;
    ++waits;
    if (escape_armed && id == vblankSemaphore2) longjmp(escape_buf, 1);
}
extern "C" int PollSema(int id) {
    note("poll");
    ++polls;
    if (polls > 6) std::abort();
    expect(id == vblankSemaphore, "poll id");
    if (equal_polls_remaining > 0) {
        --equal_polls_remaining;
        return id;
    }
    return id + 7;
}
extern "C" int DIntr() {
    note("dintr");
    return 1;
}
extern "C" void EIntr() { note("eintr"); }
extern "C" void doSuperSample__Fiib(int selector, int word, int argument) {
    note("super");
    super_selector = selector;
    super_word = word;
    super_argument = argument;
    quot_at_super = lastFrameVBlankCount;
}

namespace {
void expect_sequence(const char* expected) {
    expect(std::strcmp(sequence, expected) == 0, expected);
}

void test_clean_present() {
    reset();
    engineFrameEnd__Fb(0);
    expect(stack_loads == 5120 && stack_out == 0 && prints == 0 && zero_stores == 0, "clean canary");
    expect(frame_at_pack == 11u && drawFrameNm == 11u, "frame number before pack");
    expect(sync_mode == 0 && sync_timeout == 0, "gs sync arguments");
    expect(switchToHalfCounter_797 == 0u && timer_loads == 1, "half counter cleared without the early timer read");
    expect(polls == 2, "poll repeats while it returns the semaphore");
    expect(video_count == 0, "no video change");
    expect(draw[6] == 1u && draw[7] == 0u && draw[8] == 0x105u && draw[9] == 11u, "even pair copy");
    expect(draw[3] == 0u && draw[4] == 0u && draw[13] == 5u, "list head cleared and odd pair kept");
    expect(waits == 1 && wait_ids[0] == 5, "matching words wait on the vblank semaphore");
    expect(switchTo30Counter_798 == 0, "30 counter cleared after that wait");
    expect(engineDisplayDisable == 2, "display disable decreases by one");
    expect(timer_stores == 1 && timer_stored == 0u, "timer cleared");
    expect(quot_at_super == (100u + 4884u) / 9766u && lastFrameVBlankCount == quot_at_super, "quotient visible to supersample");
    expect(super_selector == 0 && super_word == 0 && super_argument == 0, "supersample arguments");
    expect_sequence("drawWait,sync,pack,poll,poll,dintr,eintr,wait,super");
}

void test_loading_screen_skips_canary() {
    reset();
    coreLoadingScreenRunning = static_cast<signed char>(0x80);
    stack_bytes[0] = 0;
    engineFrameEnd__Fb(2);
    expect(stack_loads == 0 && prints == 0 && zero_stores == 0, "nonzero loading byte skips the scan");
    expect(super_argument == 2, "argument is passed through");
}

void test_canary_mismatch_continues() {
    reset();
    stack_bytes[0] = 0;
    stack_bytes[5119] = 1;
    engineFrameEnd__Fb(0);
    expect(prints == 2 && zero_stores == 2 && stack_loads == 5120 && stack_out == 0, "both mismatches report and the scan finishes");
    expect(std::strncmp(sequence, "drawWait,", 9) == 0, "present continues after the null store");
}

void test_timer_paths() {
    reset();
    coreLoadingScreenRunning = 1;
    fullFrameMode = 1;
    timer_value = 10000u;
    switchToHalfCounter_797 = 123u;
    engineFrameEnd__Fb(0);
    expect(switchToHalfCounter_797 == 0u && video_count == 0 && timer_loads == 2, "large timer clears the half counter");

    reset();
    coreLoadingScreenRunning = 1;
    fullFrameMode = 1;
    timer_value = 100u;
    switchToHalfCounter_797 = 50u;
    engineFrameEnd__Fb(0);
    expect(switchToHalfCounter_797 == 50u - 100u + 9766u && video_count == 0, "small timer accumulates");

    reset();
    coreLoadingScreenRunning = 1;
    fullFrameMode = 1;
    timer_value = 100u;
    switchToHalfCounter_797 = 20000u;
    engineFrameEnd__Fb(0);
    const unsigned next = 20000u - 100u + 9766u;
    expect(video_count == 1 && video_mode[0] == 0 && half_at_video == next, "large accumulation calls video mode 0");
    expect(switchToHalfCounter_797 == next, "accumulation remains after that call");

    reset();
    coreLoadingScreenRunning = 1;
    fullFrameMode = 1;
    timer_value = 0x80000000u;
    switchToHalfCounter_797 = 0u;
    engineFrameEnd__Fb(0);
    const unsigned wrapped = 0u - 0x80000000u + 9766u;
    expect(video_count == 0 && switchToHalfCounter_797 == wrapped, "negative timer difference does not switch");

    reset();
    coreLoadingScreenRunning = 1;
    timer_value = 0xfffffff0u;
    engineFrameEnd__Fb(0);
    expect(lastFrameVBlankCount == (0xfffffff0u + 4884u) / 9766u, "timer add wraps before the division");

    reset();
    coreLoadingScreenRunning = 1;
    timer_value = 4882u;
    engineFrameEnd__Fb(0);
    expect(lastFrameVBlankCount == 1u, "4882 plus 4884 is one divisor");
}

void test_draw_pairs() {
    reset();
    coreLoadingScreenRunning = 1;
    drawFrameNm = 0xffffffffu;
    draw[4] = 7u;
    draw[5] = 9u;
    draw[13] = 0x55u;
    engineFrameEnd__Fb(0);
    expect(drawFrameNm == 0u, "frame number wraps");
    expect(draw[6] == 1u && draw[7] == 7u && draw[8] == 9u && draw[9] == 0u, "word 16 selects the even pair");
    expect(draw[13] == 0u, "nonzero word 16 clears offset 52");
    expect(super_selector == 0 && super_word == 7, "early path keeps selector 0 and the saved word");
    expect(draw[4] == 7u && draw[5] == 9u, "source words stay");

    reset();
    coreLoadingScreenRunning = 1;
    draw[15] = 1u;
    draw[11] = 0u;
    draw[4] = 0u;
    draw[7] = 3u;
    draw[9] = 0x11u;
    draw[13] = 0x22u;
    engineFrameEnd__Fb(0);
    expect(waits == 1, "word 28 does not add the gate wait when the gate is zero");
    expect(draw[6] == 1u && draw[9] == 11u && draw[13] == 0x22u, "word 28 still uses the even pair and does not clear");
    expect(super_selector == 0, "word 28 path does not compare the pairs");

    reset();
    coreLoadingScreenRunning = 1;
    draw[4] = 0u;
    draw[7] = 0u;
    draw[9] = 0u;
    draw[13] = 0xffffffffu;
    draw[14] = 0xffffffffu;
    draw[5] = 0x44u;
    engineFrameEnd__Fb(4);
    expect(super_selector == 1 && super_argument == 4, "negative low word selects the odd pair");
    expect(draw[9] == 0u && draw[10] == 1u && draw[13] == 11u, "odd pair receives the frame number");
    expect(draw[12] == 0x44u, "odd pair copies word 20");

    reset();
    coreLoadingScreenRunning = 1;
    draw[4] = 0u;
    draw[7] = 0u;
    draw[9] = 5u;
    draw[13] = 5u;
    draw[14] = 9u;
    switchTo30Counter_798 = 30;
    engineFrameEnd__Fb(0);
    expect(super_selector == 0 && switchTo30Counter_798 == 31 && video_count == 0, "30 stays below the threshold");

    reset();
    coreLoadingScreenRunning = 1;
    draw[4] = 0u;
    draw[7] = 0u;
    draw[9] = 5u;
    draw[13] = 5u;
    draw[14] = 9u;
    switchTo30Counter_798 = 31;
    engineFrameEnd__Fb(0);
    expect(video_count == 1 && video_mode[0] == 1 && thirty_at_video == 32, "31 calls video mode 1 after the increment");
    expect(switchTo30Counter_798 == 0, "that call is followed by clearing the counter");

    reset();
    coreLoadingScreenRunning = 1;
    draw[4] = 0u;
    draw[7] = 0u;
    draw[9] = 5u;
    draw[13] = 5u;
    draw[14] = 9u;
    switchTo30Counter_798 = -1;
    engineFrameEnd__Fb(0);
    expect(video_count == 0 && switchTo30Counter_798 == 0, "a negative 30 counter does not switch");

    reset();
    coreLoadingScreenRunning = 1;
    draw[15] = 1u;
    draw[11] = 4u;
    engineFrameEnd__Fb(0);
    expect(waits == 2 && wait_ids[0] == 5 && wait_ids[1] == 5, "nonzero gate waits before the later wait");
}

void test_display_disable_sign() {
    reset();
    coreLoadingScreenRunning = 1;
    engineDisplayDisable = -3;
    engineFrameEnd__Fb(0);
    expect(engineDisplayDisable == -3, "negative display disable stays");
    reset();
    coreLoadingScreenRunning = 1;
    engineDisplayDisable = 1;
    engineFrameEnd__Fb(0);
    expect(engineDisplayDisable == 0, "one decreases to zero");
}

void test_poll_once_when_different() {
    reset();
    coreLoadingScreenRunning = 1;
    equal_polls_remaining = 0;
    engineFrameEnd__Fb(0);
    expect(polls == 1, "a different poll result is not repeated");
}

void test_power_off_does_not_return() {
    reset();
    coreLoadingScreenRunning = 1;
    powerOffMode = 1;
    vblankField = 0xab;
    escape_armed = 1;
    const int jumped = setjmp(escape_buf);
    if (jumped == 0) {
        engineFrameEnd__Fb(0);
        expect(false, "power-off mode returned");
        return;
    }
    expect(engineDisplayDisable == 1000, "power-off stores 1000");
    expect(super_selector == 0, "supersample runs before the power-off wait");
    expect(waits >= 1 && wait_ids[waits - 1] == 9, "the wait that does not return uses the second semaphore");
}
}  // namespace

int main() {
    test_clean_present();
    test_loading_screen_skips_canary();
    test_canary_mismatch_continues();
    test_timer_paths();
    test_draw_pairs();
    test_display_disable_sign();
    test_poll_once_when_different();
    test_power_off_does_not_return();
    if (failures != 0) {
        std::fprintf(stderr, "%u frame-end checks failed\n", failures);
        return 1;
    }
    std::printf("frame-end checks passed\n");
    return 0;
}
