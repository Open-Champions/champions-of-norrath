// Observed msg_load__3AntPc at 0x002088b8, 124 bytes.
// Calls the Creature load routine on the buffer, which returns a byte count n. The word at
// buffer+n is read as v; byte object+156 is cleared and word object+208 gets v. If v is 1 the
// byte at +156 becomes 2; if v is 2 it becomes 4; any other v leaves it zero. Returns n + 4.
// Inferred: the pair with msg_save__3AntPc (same word at +208). Unknown: what the word and the
// byte at +156 mean. Nothing here ran on the EE.
extern "C" {
int msg_load__8CreaturePc(void* self, char* buffer);
}

extern "C" int msg_load__3AntPc(void* self, char* buffer) {
    unsigned char* object = static_cast<unsigned char*>(self);
    int n = msg_load__8CreaturePc(self, buffer);
    int v = *reinterpret_cast<int*>(buffer + n);
    object[156] = 0;
    *reinterpret_cast<int*>(object + 208) = v;
    if (v == 1) object[156] = 2;
    else if (v == 2) object[156] = 4;
    return n + 4;
}
