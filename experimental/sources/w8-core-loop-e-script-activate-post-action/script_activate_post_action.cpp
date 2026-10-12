// Observed scriptActivatePostAction__Fv at 0x001d3ca0, 156 bytes.
// postAction (0x007d55e8, 200 bytes by the existing view) is the pending
// action text. If its first byte is zero, nothing happens. Otherwise the text
// is copied to a 208-byte local buffer, the first byte of postAction is
// cleared, and with a nonzero curScriptPtr the byte scriptInPostAction is set
// to 1 before amx_FindPublic runs with the local copy. A zero status is
// followed by amx_Exec(amx, &result, index, 0, 0), where result holds the
// status. A nonzero status skips Exec and, only if complainFrame is below
// drawFrameNm, sets it to drawFrameNm + 20. scriptInPostAction is stored zero
// on every path that reaches the end, including a null curScriptPtr, but is
// not touched when the text was empty. Whether callers of the post action
// tolerate the clear before execution is unknown.
extern "C" {
extern int drawFrameNm;
extern signed char scriptInPostAction;
extern unsigned char norrath_script_post_action_view[];
extern unsigned char norrath_script_current_amx_word_view[4];
extern unsigned char norrath_script_complain_frame_word_view[4];
char* strcpy(char* destination, const char* source);
unsigned long long norrath_amx_find_public(void* amx, const char* name, int* public_index);
unsigned long long norrath_amx_exec(void* amx, int* result, long long public_index,
                                    long long parameter_count, const void* parameter_data);
}

namespace {
unsigned word_at(const unsigned char* bytes) {
    return bytes[0] | (unsigned(bytes[1]) << 8) | (unsigned(bytes[2]) << 16)
        | (unsigned(bytes[3]) << 24);
}
}

extern "C" void scriptActivatePostAction__Fv() {
    char* const pending = reinterpret_cast<char*>(norrath_script_post_action_view);
    if (pending[0] == 0) return;
    char action[208];
    strcpy(action, pending);
    pending[0] = 0;
    const unsigned amx_word = word_at(norrath_script_current_amx_word_view);
    if (amx_word != 0u) {
        void* const amx = reinterpret_cast<void*>(static_cast<__UINTPTR_TYPE__>(amx_word));
        scriptInPostAction = 1;
        int public_index;
        int result = static_cast<int>(norrath_amx_find_public(amx, action, &public_index));
        if (result != 0) {
            unsigned char* const threshold = norrath_script_complain_frame_word_view;
            if (static_cast<int>(word_at(threshold)) < drawFrameNm) {
                const unsigned deadline = static_cast<unsigned>(drawFrameNm) + 20u;
                threshold[0] = static_cast<unsigned char>(deadline);
                threshold[1] = static_cast<unsigned char>(deadline >> 8);
                threshold[2] = static_cast<unsigned char>(deadline >> 16);
                threshold[3] = static_cast<unsigned char>(deadline >> 24);
            }
        } else {
            (void)norrath_amx_exec(amx, &result, public_index, 0ll, nullptr);
        }
    }
    scriptInPostAction = 0;
}
