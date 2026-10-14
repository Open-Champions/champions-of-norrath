// Observed msg_load__10GameObjectPc at 0x00312e78, 8 bytes, a weak symbol. It does nothing with its
// arguments and returns 0, so the base object stores or reads no bytes.
extern "C" int msg_load__10GameObjectPc(void* self, char* buffer) {
    (void)self;
    (void)buffer;
    return 0;
}
