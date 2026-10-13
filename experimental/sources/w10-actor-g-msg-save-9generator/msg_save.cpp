// Observed msg_save__9GeneratorPc at 0x001f2088, 36 bytes.
// Writes the signed halfword at object+572 as a full word at buffer+0 and the word at object+976
// at buffer+4, then returns 8. It does not call the Creature save routine.
// Unknown: what the two saved values mean. Nothing here ran on the EE.
extern "C" int msg_save__9GeneratorPc(void* self, char* buffer) {
    const unsigned char* object = static_cast<const unsigned char*>(self);
    unsigned* out = reinterpret_cast<unsigned*>(buffer);
    out[0] = static_cast<unsigned>(static_cast<int>(*reinterpret_cast<const short*>(object + 572)));
    out[1] = *reinterpret_cast<const unsigned*>(object + 976);
    return 8;
}
