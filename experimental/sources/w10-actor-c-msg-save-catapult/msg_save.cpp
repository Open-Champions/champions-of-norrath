// Observed msg_save__8CatapultPc at 0x0028c4b0, 28 bytes.
// Writes one byte at the buffer: 1 when the signed byte at object+96 is greater than zero,
// otherwise 0. Returns 1 (the buffer advance). Nothing else is read or written.
// Unknown: what the byte at +96 means; msg_load__8Catapult does not write it back.
extern "C" int msg_save__8CatapultPc(void* object_storage, char* buffer) {
    const signed char* object = static_cast<const signed char*>(object_storage);
    *buffer = object[96] > 0 ? 1 : 0;
    return 1;
}
