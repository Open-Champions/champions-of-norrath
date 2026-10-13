// Observed storage used by CreateReaderThread (0x002e8e98) in the keyboard library the image
// links in.
//   Thread record at 0x008e8c40: a local symbol (`param.0`) whose extent runs to the next symbol at
//   0x008e8c80, so 64 bytes. CreateReaderThread writes words +4 (entry), +8 (stack), +12 (size),
//   +16 (gp) and +20 (priority) and passes the record to CreateThread.
//   Stack area at 0x008e8f50: no symbol; the thread record's size word is 6144 and the area is
//   defined with that size. Its end is 0x008ea750, 48 bytes before the next object seen (0x008ea780).
// Unknown: the record's remaining words, and whether the area is larger than the size passed to
// CreateThread. Placement differs from the original: the link decides where these land.
extern "C" {
alignas(4) int norrath_lgkbm_reader_thread_record_view[16];
alignas(16) unsigned char norrath_lgkbm_reader_thread_stack_view[6144];
}
