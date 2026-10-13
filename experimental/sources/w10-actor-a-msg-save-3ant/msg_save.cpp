// Observed msg_save__3AntPc at 0x00208870, 72 bytes.
// Calls the Creature save routine on the buffer, which returns a byte count n. The word at
// object+208 is then stored at buffer+n and n + 4 is returned.
// Nothing here ran on the EE.
extern "C" {
int msg_save__8CreaturePc(void* self, char* buffer);
}

extern "C" int msg_save__3AntPc(void* self, char* buffer) {
    int n = msg_save__8CreaturePc(self, buffer);
    *reinterpret_cast<unsigned*>(buffer + n) = *reinterpret_cast<unsigned*>(static_cast<unsigned char*>(self) + 208);
    return n + 4;
}
