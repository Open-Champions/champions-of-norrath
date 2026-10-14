// Observed Username__C13VIGameOptions at 0x01019ac8, 8 bytes. It returns
// the object pointer plus 16 and calls nothing. SetUsername__13VIGameOptionsPCc
// (the neighbour) copies at most 63 bytes to that same offset and terminates
// at offset 79, so the field spans 64 bytes.
// Inferred: the receiver is a VIGameOptions object; the name comes from the
// "C" (const method) mangling. Unknown: the other fields of the object.
extern "C" {
char* Username__C13VIGameOptions(void* options) {
    return reinterpret_cast<char*>(options) + 16;
}
}
