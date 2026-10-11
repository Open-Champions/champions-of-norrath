#include "set_script.hpp"

extern "C" void norrath_script_set_script(unsigned char* script) {
    const unsigned script_word =
        static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(script));
    // Original program data immediately follows the 96-byte AMX state.
    const unsigned program_word = script_word + 96u;
    void* const captured_amx = script;

    // Publish even when null, and before either real provider can observe it.
    volatile unsigned char* const current = norrath_script_current_amx_word_view;
    current[0] = static_cast<unsigned char>(script_word);
    current[1] = static_cast<unsigned char>(script_word >> 8);
    current[2] = static_cast<unsigned char>(script_word >> 16);
    current[3] = static_cast<unsigned char>(script_word >> 24);
    if (script == nullptr) return;

    void* const program =
        reinterpret_cast<void*>(static_cast<__UINTPTR_TYPE__>(program_word));
    (void)norrath_amx_initialize(captured_amx, program);
    // This call occurs even after an initialization error. Reuse the captured
    // argument, not the current-script word which either provider may change.
    (void)norrath_amx_register_game_natives(captured_amx);
}
