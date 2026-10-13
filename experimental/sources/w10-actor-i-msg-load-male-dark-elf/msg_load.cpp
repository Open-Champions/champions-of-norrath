// Observed msg_load__11MaleDarkElfPc at 0x002896f8, 136 bytes.
// Calls msg_load__8CreaturePc with the buffer, which returns the byte count n. After those n bytes
// reads five words in the order msg_save__11MaleDarkElfPc wrote them: the low byte goes to
// object+960, the words to 964, 968, 1112 and 208. The word at 208 is stored before
// initType__11MaleDarkElfb(object, 1) runs. Returns n + 20.
// Inferred: initType applies the loaded state. Unknown: the meaning of the fields.
// Nothing here ran on the EE.
extern "C" {
int msg_load__8CreaturePc(void* self, char* buffer);
void initType__11MaleDarkElfb(void* self, bool flag);
}

extern "C" int msg_load__11MaleDarkElfPc(void* self, char* buffer) {
    unsigned char* object = static_cast<unsigned char*>(self);
    const unsigned* in = reinterpret_cast<const unsigned*>(buffer + msg_load__8CreaturePc(self, buffer));
    object[960] = static_cast<unsigned char>(*in++);
    *reinterpret_cast<unsigned*>(object + 964) = *in++;
    *reinterpret_cast<unsigned*>(object + 968) = *in++;
    *reinterpret_cast<unsigned*>(object + 1112) = *in++;
    *reinterpret_cast<unsigned*>(object + 208) = *in++;
    initType__11MaleDarkElfb(self, true);
    return static_cast<int>(reinterpret_cast<const char*>(in) - buffer);
}
