// Observed scriptSetScript__FPv at 0x001d3e48, 64 bytes.
// The argument is stored to curScriptPtr (gp-3676) first, null included.
// A null argument then returns. Otherwise amx_Init is called with the
// argument and the argument plus 96, and amxRegisterNatives__FP5__amx with
// the argument. Both results are ignored. The existing authored providers
// norrath_amx_initialize and norrath_amx_register_game_natives stand for
// those two callees. Offset 96 follows the file's observation; the 96-byte
// AMX header size is inferred from that and not otherwise established.
extern "C" {
extern unsigned char norrath_script_current_amx_word_view[4];
unsigned long long norrath_amx_initialize(void* amx, void* program);
unsigned long long norrath_amx_register_game_natives(void* amx);

void scriptSetScript__FPv(void* script) {
    const unsigned word =
        static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(script));
    norrath_script_current_amx_word_view[0] = static_cast<unsigned char>(word);
    norrath_script_current_amx_word_view[1] = static_cast<unsigned char>(word >> 8);
    norrath_script_current_amx_word_view[2] = static_cast<unsigned char>(word >> 16);
    norrath_script_current_amx_word_view[3] = static_cast<unsigned char>(word >> 24);
    if (script == nullptr) return;
    (void)norrath_amx_initialize(script, static_cast<char*>(script) + 96);
    (void)norrath_amx_register_game_natives(script);
}
}
