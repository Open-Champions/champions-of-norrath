#include "../save-and-quests-bytes/bytes.hpp"
// Observed scriptStart__FPc at 0x001d3e88, 132 bytes. A null name, or a null
// current script (curScriptPtr, the word at 0x00486b94), returns -1. Otherwise
// amx_FindPublic looks the name up. A nonzero status is a failed lookup: the
// function-local static complainFrame (word at 0x0048508c, initially -10000)
// is moved to drawFrameNm + 20 when it is behind drawFrameNm, and -1 is
// returned. A zero status runs amx_Exec on that public with no parameters and
// returns the value it stored. The status slot is also the result slot, so a
// run that stores nothing would return the zero status.
extern "C" {
int amx_FindPublic(void* amx, const char* name, int* index);
unsigned long long amx_Exec(void* amx, int* result, long long index, long long count, const void* params);
extern unsigned char norrath_script_current_amx_word_view[4];
extern unsigned char norrath_script_complain_frame_word_view[4];
extern int drawFrameNm;

int scriptStart__FPc(char* name) {
    using namespace sq;
    if (name == nullptr) return -1;
    void* amx = pointer(read_word(norrath_script_current_amx_word_view));
    if (amx == nullptr) return -1;
    int index;
    int result = amx_FindPublic(amx, name, &index);
    if (result != 0) {
        if (signed_word(read_word(norrath_script_complain_frame_word_view)) < drawFrameNm)
            write_word(norrath_script_complain_frame_word_view, static_cast<unsigned>(drawFrameNm + 20));
        return -1;
    }
    amx_Exec(amx, &result, index, 0, nullptr);
    return result;
}
}
