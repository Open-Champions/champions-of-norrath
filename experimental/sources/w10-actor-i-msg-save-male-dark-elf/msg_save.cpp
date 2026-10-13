// Observed msg_save__11MaleDarkElfPc at 0x00289680, 120 bytes.
// Calls msg_save__8CreaturePc with the buffer, which returns the byte count n. After those n bytes
// writes five words: the signed byte at object+960 (sign extended), the words at 964, 968, 1112 and
// 208, in that order. Returns n + 20.
// Inferred: the saved state is the variant byte plus four words. Unknown: their meaning.
// Nothing here ran on the EE.
extern "C" {
int msg_save__8CreaturePc(void* self, char* buffer);
}

extern "C" int msg_save__11MaleDarkElfPc(void* self, char* buffer) {
    unsigned char* object = static_cast<unsigned char*>(self);
    unsigned* out = reinterpret_cast<unsigned*>(buffer + msg_save__8CreaturePc(self, buffer));
    *out++ = static_cast<unsigned>(static_cast<int>(*reinterpret_cast<signed char*>(object + 960)));
    *out++ = *reinterpret_cast<unsigned*>(object + 964);
    *out++ = *reinterpret_cast<unsigned*>(object + 968);
    *out++ = *reinterpret_cast<unsigned*>(object + 1112);
    *out++ = *reinterpret_cast<unsigned*>(object + 208);
    return static_cast<int>(reinterpret_cast<char*>(out) - buffer);
}
