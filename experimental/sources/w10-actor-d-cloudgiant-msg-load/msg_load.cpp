// Observed msg_load__10CloudGiantPc at 0x001e9a48, 88 bytes.
// Calls the Creature load routine on the buffer, which returns a byte count n. The word at
// buffer+n is read into object+208. If the signed halfword at object+572 equals 10, the word at
// +984 is cleared. Returns n + 4.
// Inferred: pairs with msg_save__10CloudGiantPc. Unknown: what the words at +208 and +984 mean and
// what the value 10 in the halfword at +572 stands for. Nothing here ran on the EE.
extern "C" {
int msg_load__8CreaturePc(void* self, char* buffer);
}

extern "C" int msg_load__10CloudGiantPc(void* self, char* buffer) {
    unsigned char* object = static_cast<unsigned char*>(self);
    int n = msg_load__8CreaturePc(self, buffer);
    int v = *reinterpret_cast<int*>(buffer + n);
    *reinterpret_cast<int*>(object + 208) = v;
    if (*reinterpret_cast<short*>(object + 572) == 10) *reinterpret_cast<int*>(object + 984) = 0;
    return n + 4;
}
