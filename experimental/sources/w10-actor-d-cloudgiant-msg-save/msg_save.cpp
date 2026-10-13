// Observed msg_save__10CloudGiantPc at 0x001e9a00, 72 bytes.
// Calls the Creature save routine on the buffer, which returns a byte count n. The word at
// object+208 is then written at buffer+n and n + 4 is returned.
// Inferred: pairs with msg_load__10CloudGiantPc (same word at +208). Unknown: what the word
// means. Nothing here ran on the EE.
extern "C" {
int msg_save__8CreaturePc(void* self, char* buffer);
}

extern "C" int msg_save__10CloudGiantPc(void* self, char* buffer) {
    const unsigned char* object = static_cast<const unsigned char*>(self);
    int n = msg_save__8CreaturePc(self, buffer);
    *reinterpret_cast<int*>(buffer + n) = *reinterpret_cast<const int*>(object + 208);
    return n + 4;
}
