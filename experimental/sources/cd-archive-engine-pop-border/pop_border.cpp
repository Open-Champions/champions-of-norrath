// Observed enginePopBorderColor at 0x00191bb0. Both instructions are a return
// and a nop. cdWait calls it with no arguments after the read-active loop.
extern "C" void norrath_archive_engine_pop_border() {}
// Same eight-byte body as enginePopBorderColor__Fv at 0x00191bb0.
#if defined(__APPLE__)
asm(".globl _enginePopBorderColor__Fv\n.set _enginePopBorderColor__Fv, _norrath_archive_engine_pop_border\n");
#else
asm(".globl enginePopBorderColor__Fv\n.set enginePopBorderColor__Fv, norrath_archive_engine_pop_border\n");
#endif
