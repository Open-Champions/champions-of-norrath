// Observed scriptSuperClear__Fv at 0x001d4048, 24 bytes.
// It does what scriptClear__Fv does and additionally stores a zero byte at
// postAction (0x007d55e8), the first byte of the pending post-action text,
// so that text becomes empty. The byte store happens between the first and
// second word stores. Only that first byte is touched.
extern "C" int nmDeferedCalls;
extern "C" int nextNmDeferedCalls;
extern "C" int scriptFrame;
extern "C" unsigned char norrath_script_post_action_view[];

extern "C" void scriptSuperClear__Fv() {
    nmDeferedCalls = 0;
    norrath_script_post_action_view[0] = 0;
    nextNmDeferedCalls = 0;
    scriptFrame = 0;
}
