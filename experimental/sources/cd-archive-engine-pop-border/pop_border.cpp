// Observed enginePopBorderColor at 0x00191bb0. Both instructions are a return
// and a nop. cdWait calls it with no arguments after the read-active loop.
extern "C" void norrath_archive_engine_pop_border() {}
