#define NORRATH_SQ_HOST 1
#include <cstdio>
#include <cstring>
extern "C" {
unsigned char norrath_script_current_amx_word_view[4];
unsigned char norrath_script_complain_frame_word_view[4] = {0xf0, 0xd8, 0xff, 0xff};
int drawFrameNm;
static int find_status;
static int find_index;
static int exec_value;
static int exec_calls;
static void* last_amx;
static long long last_index, last_count;
static const void* last_params;
static const char* last_name;
int amx_FindPublic(void* amx, const char* name, int* index) {
    last_amx = amx; last_name = name;
    *index = find_index;
    return find_status;
}
unsigned long long amx_Exec(void* amx, int* result, long long index, long long count, const void* params) {
    exec_calls += 1;
    last_amx = amx; last_index = index; last_count = count; last_params = params;
    *result = exec_value;
    return 0;
}
}
#include "script_start.cpp"
static int complain() { return sq::signed_word(sq::read_word(norrath_script_complain_frame_word_view)); }
int main() {
    char name[] = "preStart";
    int fake = 0;
    sq::write_word(norrath_script_current_amx_word_view, sq::pointer_bits(&fake));
    if (scriptStart__FPc(nullptr) != -1 || exec_calls != 0) return 1;
    // success runs the public with no parameters and returns its value
    find_status = 0; find_index = -3; exec_value = 77;
    drawFrameNm = 500;
    if (scriptStart__FPc(name) != 77 || exec_calls != 1) return 2;
    if (last_amx != &fake || last_index != -3 || last_count != 0 || last_params != nullptr) return 3;
    if (last_name != name || complain() != -10000) return 4;
    // a failed lookup returns -1 and moves complainFrame once it is behind
    find_status = 1;
    if (scriptStart__FPc(name) != -1 || exec_calls != 1) return 5;
    if (complain() != 520) return 6;
    drawFrameNm = 510;
    scriptStart__FPc(name);
    if (complain() != 520) return 7;
    drawFrameNm = 521;
    scriptStart__FPc(name);
    if (complain() != 541) return 8;
    // no script loaded
    sq::write_word(norrath_script_current_amx_word_view, 0);
    find_status = 0;
    if (scriptStart__FPc(name) != -1 || exec_calls != 1) return 9;
    std::puts("ok");
    return 0;
}
