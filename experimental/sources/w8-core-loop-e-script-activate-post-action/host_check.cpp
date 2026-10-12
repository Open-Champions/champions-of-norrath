// Host check for scriptActivatePostAction__Fv. Not an EE plan input.
#include <cstdint>
#include <cstring>
#include <string>
extern "C" {
int drawFrameNm = 0;
signed char scriptInPostAction = 5;
unsigned char norrath_script_post_action_view[200];
unsigned char norrath_script_current_amx_word_view[4];
unsigned char norrath_script_complain_frame_word_view[4];
}
static std::string trace;
static int find_status = 0;
extern "C" char* norrath_host_strcpy(char* d, const char* s) { trace += "C"; return std::strcpy(d, s); }
#define strcpy norrath_host_strcpy
extern "C" unsigned long long norrath_amx_find_public(void* amx, const char* name, int* public_index) {
    trace += "F" + std::to_string((uintptr_t)amx) + ":" + name + (scriptInPostAction == 1 ? "!" : "?");
    trace += norrath_script_post_action_view[0] == 0 ? "z;" : "n;";
    if (find_status == 0) *public_index = 12;
    return (unsigned long long)(long long)find_status;
}
extern "C" unsigned long long norrath_amx_exec(void* amx, int* result, long long public_index,
                                               long long parameter_count, const void* parameter_data) {
    trace += "E" + std::to_string(*result) + "," + std::to_string(public_index) + ","
        + std::to_string(parameter_count) + (parameter_data ? "p" : "n") + ";";
    return 0;
}
#include "script_activate_post_action.cpp"
#undef strcpy
static void put(unsigned char* w, unsigned v) { for (int i = 0; i < 4; ++i) w[i] = (unsigned char)(v >> (8 * i)); }
static int get(unsigned char* w) { return (int)(w[0] | (w[1] << 8) | (w[2] << 16) | ((unsigned)w[3] << 24)); }
int main() {
    put(norrath_script_current_amx_word_view, 0x2000);
    put(norrath_script_complain_frame_word_view, (unsigned)-10000);
    // Empty text: no flag change, no calls.
    norrath_script_post_action_view[0] = 0;
    scriptActivatePostAction__Fv();
    if (!trace.empty() || scriptInPostAction != 5) return 1;
    // Text present: copy, clear, flag set during lookup, exec on success, flag cleared.
    std::strcpy((char*)norrath_script_post_action_view, "hello");
    scriptActivatePostAction__Fv();
    if (trace != "CF8192:hello!z;E0,12,0n;") return 2;
    if (scriptInPostAction != 0 || norrath_script_post_action_view[0] != 0) return 3;
    if (std::strcmp((char*)norrath_script_post_action_view + 1, "ello") != 0) return 4;
    // Lookup failure moves complainFrame once and skips Exec.
    trace.clear(); find_status = 3; drawFrameNm = 100; scriptInPostAction = 5;
    std::strcpy((char*)norrath_script_post_action_view, "bye");
    scriptActivatePostAction__Fv();
    if (trace != "CF8192:bye!z;") return 5;
    if (get(norrath_script_complain_frame_word_view) != 120 || scriptInPostAction != 0) return 6;
    trace.clear(); drawFrameNm = 110;
    std::strcpy((char*)norrath_script_post_action_view, "x");
    scriptActivatePostAction__Fv();
    if (get(norrath_script_complain_frame_word_view) != 120) return 7;
    // No current script: text consumed, flag still ends at zero, no callee.
    trace.clear(); put(norrath_script_current_amx_word_view, 0); scriptInPostAction = 5;
    std::strcpy((char*)norrath_script_post_action_view, "y");
    scriptActivatePostAction__Fv();
    if (trace != "C" || scriptInPostAction != 0 || norrath_script_post_action_view[0] != 0) return 8;
    return 0;
}
