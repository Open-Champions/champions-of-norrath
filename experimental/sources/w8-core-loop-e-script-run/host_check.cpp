// Host check for scriptRun__Fv. Not an EE plan input.
#include <cstdint>
#include <string>
extern "C" {
int nmDeferedCalls = 0;
int nextNmDeferedCalls = 0;
int scriptFrame = 0;
int drawFrameNm = 0;
unsigned char deferedCall[2 * 5120];
unsigned char norrath_script_current_amx_word_view[4];
unsigned char norrath_script_complain_frame_word_view[4];
}
static std::string trace;
static int find_status[8];
static int calls = 0;
static int find_index_out = 40;
extern "C" unsigned long long norrath_amx_find_public(void* amx, const char* name, int* public_index) {
    trace += "F" + std::to_string((uintptr_t)amx) + ":" + std::to_string((const unsigned char*)name - deferedCall) + ";";
    int st = find_status[calls++ & 7];
    if (st == 0) *public_index = find_index_out++;
    return (unsigned long long)(long long)st;
}
extern "C" unsigned long long norrath_amx_exec(void* amx, int* result, long long public_index,
                                               long long parameter_count, const void* parameter_data) {
    trace += "E" + std::to_string((uintptr_t)amx) + ",r" + std::to_string(*result) + ",i"
        + std::to_string(public_index) + "," + std::to_string(parameter_count)
        + (parameter_data ? "p" : "n") + ";";
    return 5;
}
#include "script_run.cpp"
static void put(unsigned char* w, unsigned v) { for (int i = 0; i < 4; ++i) w[i] = (unsigned char)(v >> (8 * i)); }
static int get(unsigned char* w) { return (int)(w[0] | (w[1] << 8) | (w[2] << 16) | ((unsigned)w[3] << 24)); }
int main() {
    put(norrath_script_current_amx_word_view, 0x1000);
    put(norrath_script_complain_frame_word_view, (unsigned)-10000);
    // Odd frame, three records: ok, fail, ok.
    nmDeferedCalls = 3; nextNmDeferedCalls = 2; scriptFrame = 1; drawFrameNm = 500;
    find_status[0] = 0; find_status[1] = 1; find_status[2] = 0;
    scriptRun__Fv();
    if (trace != "F4096:5120;E4096,r0,i40,0n;F4096:5200;F4096:5280;E4096,r0,i41,0n;") return 1;
    if (get(norrath_script_complain_frame_word_view) != 520) return 2;
    if (nmDeferedCalls != 2 || nextNmDeferedCalls != 0 || scriptFrame != 2) return 3;
    // A later failure with the threshold already at or above the frame keeps it.
    trace.clear(); calls = 0; find_status[0] = 7; drawFrameNm = 510;
    nmDeferedCalls = 1; nextNmDeferedCalls = 0; scriptFrame = 2;
    scriptRun__Fv();
    if (trace != "F4096:0;") return 4;
    if (get(norrath_script_complain_frame_word_view) != 520) return 5;
    // Frame past the threshold moves it again.
    trace.clear(); calls = 0; drawFrameNm = 521; nmDeferedCalls = 1; scriptFrame = 0;
    scriptRun__Fv();
    if (get(norrath_script_complain_frame_word_view) != 541) return 6;
    // Without a current script no callee runs, but the bookkeeping does.
    trace.clear(); put(norrath_script_current_amx_word_view, 0);
    nmDeferedCalls = 4; nextNmDeferedCalls = 9; scriptFrame = 3;
    scriptRun__Fv();
    if (!trace.empty() || nmDeferedCalls != 9 || nextNmDeferedCalls != 0 || scriptFrame != 4) return 7;
    return 0;
}
