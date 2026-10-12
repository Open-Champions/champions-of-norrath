// Observed scriptRun__Fv at 0x001d3ba8, 244 bytes.
// For each index below nmDeferedCalls (read again after each processed
// record) it takes the record at deferedCall + (scriptFrame & 1) * 5120 +
// index * 80. When curScriptPtr is nonzero it calls amx_FindPublic with that
// AMX, the record as the public name and a pointer to an index slot. A zero
// status is followed by amx_Exec(amx, &result, index, 0, 0), with result
// holding that status; Exec's own status is ignored and the script result
// is dropped. A nonzero status stores result and, only when complainFrame is
// below drawFrameNm, sets complainFrame to drawFrameNm + 20 and skips
// Exec. This repeats the body of scriptStart__FPc inline. Finally
// scriptFrame is incremented, nmDeferedCalls takes nextNmDeferedCalls and
// nextNmDeferedCalls becomes zero. The existing authored providers
// norrath_amx_find_public and norrath_amx_exec stand for the two callees.
// What the complaint threshold throttles is unknown.
extern "C" {
extern int nmDeferedCalls;
extern int nextNmDeferedCalls;
extern int scriptFrame;
extern int drawFrameNm;
extern unsigned char deferedCall[];
extern unsigned char norrath_script_current_amx_word_view[4];
extern unsigned char norrath_script_complain_frame_word_view[4];
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

extern "C" void scriptRun__Fv() {
    for (int index = 0; index < nmDeferedCalls; ++index) {
        char* const record = reinterpret_cast<char*>(
            deferedCall + (scriptFrame & 1) * 5120 + index * 80);
        const unsigned amx_word = word_at(norrath_script_current_amx_word_view);
        if (amx_word == 0u) continue;
        void* const amx = reinterpret_cast<void*>(static_cast<__UINTPTR_TYPE__>(amx_word));
        int public_index;
        int result = static_cast<int>(norrath_amx_find_public(amx, record, &public_index));
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
    const int next_frame = scriptFrame + 1;
    const int pending = nextNmDeferedCalls;
    nmDeferedCalls = pending;
    nextNmDeferedCalls = 0;
    scriptFrame = next_frame;
}
