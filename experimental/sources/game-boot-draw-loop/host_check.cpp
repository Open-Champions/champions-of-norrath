// Host contract for simpleDrawLoop. Not an EE plan input.
#include "simple_draw.cpp"

#include <cstdio>
#include <cstring>

extern "C" {
unsigned frameNm = 7u;
unsigned char supersampleMode = 7u;
unsigned coreClearColor = 0u;
unsigned gameControlPaused = 0u;
unsigned char pad[128];
}

static int failures = 0;
static char log_text[64];
static int log_used = 0;
static int callback_calls = 0;
static int callback_limit = 1;
static unsigned frame_at_end[4];
static int ends = 0;
static unsigned color_seen = 0;
static int end_arg = -1;
static int clear_arg = -1;
static int lump_arg = -1;
static int reset_args[3];
static int resets = 0;

static void expect(bool ok, const char* message) {
    if (!ok) {
        std::fprintf(stderr, "fail: %s\n", message);
        ++failures;
    }
}
static void mark(char step) {
    if (log_used < 63) log_text[log_used++] = step;
    log_text[log_used] = 0;
}
static void write_u32(unsigned char* p, unsigned value) {
    p[0] = static_cast<unsigned char>(value);
    p[1] = static_cast<unsigned char>(value >> 8);
    p[2] = static_cast<unsigned char>(value >> 16);
    p[3] = static_cast<unsigned char>(value >> 24);
}

extern "C" void engineSetDisplayDisable__Fi(int value) { mark(value == 2 ? 'D' : 'd'); }
extern "C" void engineFrameStart__FUib(unsigned color, int second) {
    mark('S');
    color_seen = color;
    if (second != 0) mark('?');
}
extern "C" void padProcess__Fv() { mark('P'); }
extern "C" void cdProcess__Fv() { mark('C'); }
extern "C" void engineFrameEnd__Fb(int value) {
    mark('E');
    end_arg = value;
    if (ends < 4) frame_at_end[ends] = frameNm;
    ++ends;
}
extern "C" void soundFrame__Fv() { mark('F'); }
extern "C" void gameClear__Fi(int value) {
    mark('G');
    clear_arg = value;
}
extern "C" void soundResetStream__Fi(int stream) {
    mark('R');
    if (resets < 3) reset_args[resets] = stream;
    ++resets;
}
extern "C" void lumpClear__Fi(int value) {
    mark('L');
    lump_arg = value;
}

static int callback() {
    ++callback_calls;
    mark('B');
    return callback_calls >= callback_limit ? 1 : 0;
}

static void reset_log() {
    log_used = 0;
    log_text[0] = 0;
    callback_calls = 0;
    ends = 0;
    resets = 0;
    color_seen = 0;
    end_arg = -1;
    clear_arg = -1;
    lump_arg = -1;
}

int main() {
    std::memset(pad, 0, sizeof pad);
    coreClearColor = 0x01020304u;
    gameControlPaused = 4u;
    frameNm = 9u;
    supersampleMode = 7u;
    callback_limit = 1;
    reset_log();
    simpleDrawLoop__FPFv_i(callback);
    expect(std::strcmp(log_text, "DSPBCEFGdRRRL") == 0, "one pass then exit");
    expect(frameNm == 1u && frame_at_end[0] == 1u, "frame count visible to frame end");
    expect(supersampleMode == 1u && color_seen == 0x01020304u, "sample mode and clear color");
    expect(end_arg == 0 && clear_arg == 1 && lump_arg == 1, "end, clear, and lump arguments");
    expect(reset_args[0] == 0 && reset_args[1] == 1 && reset_args[2] == 2, "three stream resets");
    expect(gameControlPaused == 3u, "pause counter decrements when both pad words are clear");

    gameControlPaused = 4u;
    write_u32(pad + 8, 1u);
    callback_limit = 1;
    reset_log();
    simpleDrawLoop__FPFv_i(callback);
    expect(gameControlPaused == 4u, "low pad bit holds the pause counter");

    gameControlPaused = 2u;
    write_u32(pad + 8, 0x00100000u);
    write_u32(pad + 96, 0u);
    reset_log();
    simpleDrawLoop__FPFv_i(callback);
    expect(gameControlPaused == 1u, "masked-off pad bit still decrements");

    gameControlPaused = 2u;
    write_u32(pad + 8, 0u);
    write_u32(pad + 96, 1u);
    reset_log();
    simpleDrawLoop__FPFv_i(callback);
    expect(gameControlPaused == 2u, "high pad word holds the pause counter");

    gameControlPaused = 0u;
    write_u32(pad + 96, 0u);
    reset_log();
    simpleDrawLoop__FPFv_i(callback);
    expect(gameControlPaused == 0u, "zero counter is left alone");

    gameControlPaused = 0x80000000u;
    reset_log();
    simpleDrawLoop__FPFv_i(callback);
    expect(gameControlPaused == 0x80000000u, "negative counter is left alone");

    std::memset(pad, 0, sizeof pad);
    gameControlPaused = 5u;
    callback_limit = 3;
    reset_log();
    simpleDrawLoop__FPFv_i(callback);
    expect(callback_calls == 3 && ends == 3 && frameNm == 3u, "zero results repeat before the exit");
    expect(gameControlPaused == 2u, "counter steps once per repeated pass");
    expect(std::strcmp(log_text, "DSPBCEFSPBCEFSPBCEFGdRRRL") == 0, "exit runs once after the repeating passes");
    if (failures != 0) return 1;
    std::puts("simple draw contract passed");
    return 0;
}
