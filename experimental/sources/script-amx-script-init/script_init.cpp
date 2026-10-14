// scriptInit__Fv at 0x001d4010 is 52 bytes. It registers scriptTask__Fv with
// engineAddTask and then stores zero at curScriptPtr. curScriptPtr is the
// four-byte word at 0x00486b94, the same object as
// norrath_script_current_amx_word_view. The task text at 0x00485090 has no
// ELF symbol. Its bytes are "Script" and the terminating zero. The next byte
// is scriptInPostAction and is not part of this text.
extern "C" {
void engineAddTask__FPFv_viPci(void (*callback)(), int second, char* third, int fourth);
void scriptTask__Fv();
extern unsigned char norrath_script_current_amx_word_view[4];

static char task_text[] = {'S', 'c', 'r', 'i', 'p', 't', '\0'};

void scriptInit__Fv() {
    engineAddTask__FPFv_viPci(scriptTask__Fv, 125, task_text, 0);
    norrath_script_current_amx_word_view[0] = 0;
    norrath_script_current_amx_word_view[1] = 0;
    norrath_script_current_amx_word_view[2] = 0;
    norrath_script_current_amx_word_view[3] = 0;
}
}
