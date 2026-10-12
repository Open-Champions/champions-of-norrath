// Observed msg_load__7ShooterPc at 0x001e5d98, 24 bytes.
// Reads a signed 16-bit value from the start of the stream and stores it at object+82.
// Returns 2, the number of stream bytes consumed.
// Unknown: what the field means.

namespace {
typedef short Half __attribute__((may_alias));
}

extern "C" int msg_load__7ShooterPc(void* self, const char* stream) {
    unsigned char* object = static_cast<unsigned char*>(self);
    *reinterpret_cast<Half*>(object + 82) = *reinterpret_cast<const Half*>(stream);
    return 2;
}
