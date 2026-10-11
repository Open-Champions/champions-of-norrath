#define NORRATH_HOST_CONTRACT 1
#include "set_def_disp_env.cpp"

#include <cstdarg>
#include <cstdio>
#include <cstring>

static unsigned char param[16];
static int model_calls;
static int model_result;
static int offset_calls;
static int offset_mode;
static int break_calls;
static int print_calls;
static int printed_mode;

extern "C" unsigned char* sceGsGetGParam() { return param; }
extern "C" int checkModelVersion() {
    model_calls += 1;
    return model_result;
}
extern "C" void _GetGsDxDyOffset(int mode, int* dx0, int* dy0, int* dx1, int* dy1) {
    offset_calls += 1;
    offset_mode = mode;
    *dx0 = 10;
    *dy0 = 20;
    *dx1 = 30;
    *dy1 = 40;
}
extern "C" void norrath_gs_width_break() { break_calls += 1; }
extern "C" int scePrintf(const char* text, ...) {
    print_calls += 1;
    if (std::strcmp(text, "sceGsDefDispEnv:Not support displaymode for 0x%x!!\n") != 0) return 1;
    va_list args;
    va_start(args, text);
    printed_mode = va_arg(args, int);
    va_end(args);
    return 0;
}

static void set_half(int offset, int value) {
    param[offset] = static_cast<unsigned char>(value);
    param[offset + 1] = static_cast<unsigned char>(value >> 8);
}
static unsigned long long qword(const unsigned char* env, int offset) {
    unsigned long long value = 0;
    for (int byte = 0; byte < 8; ++byte)
        value |= static_cast<unsigned long long>(env[offset + byte]) << (8 * byte);
    return value;
}
static void prepare(int interlace, int frame, int field4) {
    std::memset(param, 0, sizeof param);
    set_half(0, interlace);
    set_half(2, frame);
    set_half(4, field4);
    model_calls = 0;
    model_result = 0;
    offset_calls = 0;
    break_calls = 0;
    print_calls = 0;
    printed_mode = 0;
}

int main() {
    alignas(8) unsigned char env[40];
    std::memset(env, 0x11, sizeof env);
    prepare(1, 2, 1);
    sceGsSetDefDispEnv(env, 0, 640, 224, 0, 0);
    if (model_calls != 0 || break_calls != 0 || print_calls != 0) return 1;
    if (qword(env, 0) != 102ull || qword(env, 8) != 3ull || qword(env, 16) != 0x1400ull) return 2;
    if (qword(env, 24) != 0x1bf9ff0183227cull || qword(env, 32) != 0ull) return 3;

    std::memset(env, 0x11, sizeof env);
    prepare(1, 2, 0);
    sceGsSetDefDispEnv(env, 0, 640, 224, 0, 0);
    if (qword(env, 8) != 1ull || qword(env, 24) != 0x0df9ff0183227cull) return 4;

    std::memset(env, 0x11, sizeof env);
    prepare(0, 2, 1);
    sceGsSetDefDispEnv(env, 0, 640, 224, 0, 0);
    if (qword(env, 8) != 2ull || qword(env, 24) != 0x0df9ff0181927cull) return 5;

    std::memset(env, 0x11, sizeof env);
    prepare(1, 3, 1);
    sceGsSetDefDispEnv(env, 0, 640, 256, 0, 0);
    if (qword(env, 24) != 0x1ff9ff01848290ull) return 6;

    std::memset(env, 0x11, sizeof env);
    prepare(0, 3, 0);
    sceGsSetDefDispEnv(env, 0, 640, 256, 0, 0);
    if (qword(env, 24) != 0x0ff9ff01824290ull) return 7;

    std::memset(env, 0x11, sizeof env);
    prepare(1, 80, 1);
    sceGsSetDefDispEnv(env, 0, 640, 480, 0, 0);
    if (model_calls != 1 || offset_calls != 0) return 8;
    if (qword(env, 24) != 0x1df4ff00823138ull) return 9;

    std::memset(env, 0x11, sizeof env);
    prepare(1, 80, 1);
    model_result = 1;
    sceGsSetDefDispEnv(env, 0, 640, 480, 0, 0);
    if (offset_calls != 1 || offset_mode != 80) return 10;
    if (qword(env, 24) != 0x1df4ff00837142ull) return 11;

    std::memset(env, 0x22, sizeof env);
    prepare(1, 2, 1);
    sceGsSetDefDispEnv(env, 1, 0, 224, 0, 0);
    if (break_calls != 1 || qword(env, 16) != 0x8000ull) return 12;
    if (qword(env, 24) != 0x2222222222222222ull || qword(env, 32) != 0x2222222222222222ull) return 13;

    std::memset(env, 0x22, sizeof env);
    prepare(1, 7, 1);
    sceGsSetDefDispEnv(env, 0, 640, 224, 0, 0);
    if (model_calls != 1 || print_calls != 1 || printed_mode != 7) return 14;
    if (qword(env, 24) != 0x2222222222222222ull || qword(env, 32) != 0ull) return 15;

    std::memset(env, 0x11, sizeof env);
    prepare(1, 2, 1);
    sceGsSetDefDispEnv(env, 0, 320, 224, 0, 0);
    if (qword(env, 16) != 0xa00ull || qword(env, 24) != 0x1bf9ff0383227cull) return 16;
    std::puts("set default display contract passed");
    return 0;
}
