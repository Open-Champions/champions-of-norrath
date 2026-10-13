// Observed msg_save__11LavaMonsterPc at 0x001ee1e8, 144 bytes.
// Calls msg_save__8CreaturePc with the buffer, which returns the byte count n. Then looks through four
// 8-byte records starting at object+1012 (a halfword type, then halfwords at +2, +4 and +6) for the
// first whose type is 21, and writes one word after the n bytes: 1 when that record's second halfword
// (at +2) is nonzero, otherwise 0 (also 0 when no record has type 21). Returns n + 4.
// Inferred: the records are timed effects, and type 21 is one particular effect, from the load
// routine that fills them. Unknown: what the effect is. Nothing here ran on the EE.
extern "C" {
int msg_save__8CreaturePc(void* self, char* buffer);
}

extern "C" int msg_save__11LavaMonsterPc(void* self, char* buffer) {
    unsigned char* object = static_cast<unsigned char*>(self);
    unsigned* out = reinterpret_cast<unsigned*>(buffer + msg_save__8CreaturePc(self, buffer));
    int value = 0;
    for (int i = 0; i < 4; ++i) {
        const short* record = reinterpret_cast<const short*>(object + 1012 + 8 * i);
        if (record[0] == 21) {
            value = record[1];
            break;
        }
    }
    *out++ = value != 0 ? 1u : 0u;
    return static_cast<int>(reinterpret_cast<char*>(out) - buffer);
}
