// Host check for scriptSetScript__FPv. Not an EE plan input.
#include <cstdint>
#include <string>
extern "C" {
unsigned char norrath_script_current_amx_word_view[4];
}
static std::string trace;
static void* init_amx; static void* init_prog;
extern "C" unsigned long long norrath_amx_initialize(void* amx, void* program) {
    trace += "I"; init_amx = amx; init_prog = program; return 7;
}
extern "C" unsigned long long norrath_amx_register_game_natives(void* amx) {
    trace += "R"; if (amx != init_amx) trace += "?"; return 9;
}
#include "script_set_script.cpp"
static uint32_t cur() {
    return norrath_script_current_amx_word_view[0] | (norrath_script_current_amx_word_view[1] << 8)
        | (norrath_script_current_amx_word_view[2] << 16)
        | ((uint32_t)norrath_script_current_amx_word_view[3] << 24);
}
int main() {
    alignas(16) static unsigned char blob[256];
    // Host pointers are wider than the target word; the stored word is the low 32 bits.
    scriptSetScript__FPv(blob);
    if (trace != "IR") return 1;
    if (init_amx != blob || init_prog != blob + 96) return 2;
    if (cur() != (uint32_t)(uintptr_t)blob) return 3;
    trace.clear();
    scriptSetScript__FPv(nullptr);
    if (!trace.empty() || cur() != 0) return 4;
    return 0;
}
