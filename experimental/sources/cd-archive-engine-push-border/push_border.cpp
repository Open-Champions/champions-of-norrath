// Observed enginePushBorderColor at 0x00191ba8. Both instructions are a return
// and a nop. cdWait passes 0x00ffffff; that word is not read or stored.
extern "C" void norrath_archive_engine_push_border(unsigned) {}
// Same eight-byte body as enginePushBorderColor__FUi at 0x00191ba8.
#if defined(__APPLE__)
asm(".globl _enginePushBorderColor__FUi\n.set _enginePushBorderColor__FUi, _norrath_archive_engine_push_border\n");
#else
asm(".globl enginePushBorderColor__FUi\n.set enginePushBorderColor__FUi, norrath_archive_engine_push_border\n");
#endif
