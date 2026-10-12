// Observed msg_load__7TriggerPc at 0x001d1a90, 36 bytes.
// Reads a 4-byte word from the start of the stream and stores it at object+100, then
// reads the next byte as a signed value and stores its low byte at object+96.
// Returns 5, the number of stream bytes consumed. The stream is read in place; the
// word load is a plain aligned load in the image.
// Unknown: what the two fields mean.

namespace {
typedef int Word __attribute__((may_alias));
}

extern "C" int msg_load__7TriggerPc(void* self, const char* stream) {
    unsigned char* object = static_cast<unsigned char*>(self);
    int word = *reinterpret_cast<const Word*>(stream);
    char byte = stream[4];
    *reinterpret_cast<Word*>(object + 100) = word;
    object[96] = static_cast<unsigned char>(byte);
    return 5;
}
