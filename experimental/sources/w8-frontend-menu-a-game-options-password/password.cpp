// Observed Password__C13VIGameOptions at 0x01019b00, 8 bytes. It returns
// the object pointer plus 80 and calls nothing. SetPassword__13VIGameOptionsPCc
// copies at most 63 bytes to that offset and terminates at offset 143.
// Inferred: the receiver is a VIGameOptions object. Unknown: the other fields.
extern "C" {
char* Password__C13VIGameOptions(void* options) {
    return reinterpret_cast<char*>(options) + 80;
}
}
