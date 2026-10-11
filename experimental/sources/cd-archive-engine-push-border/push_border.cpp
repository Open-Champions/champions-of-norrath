// Observed enginePushBorderColor at 0x00191ba8. Both instructions are a return
// and a nop. cdWait passes 0x00ffffff; that word is not read or stored.
extern "C" void norrath_archive_engine_push_border(unsigned) {}
