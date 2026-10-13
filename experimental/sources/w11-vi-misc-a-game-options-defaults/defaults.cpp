// Observed: SetDefaults__13VIGameOptions at 0x01019a60 is 100 bytes. It clears 1024
// bytes of the object with memset and then stores: word 0 = 0; bytes 4, 6 and 7 =
// 0; byte 5 = 1; floats at 8 and 12 = 1.0f; byte 151 = 1; word 1020 = 0x01234567.
// It returns nothing. Unknown: the meaning of every option field; the value
// 0x01234567 may be a validity marker (inferred from its pattern only).
extern "C" {
void* memset(void* destination, int value, unsigned count);

void SetDefaults__13VIGameOptions(void* options) {
    unsigned char* bytes = static_cast<unsigned char*>(options);
    memset(options, 0, 1024);
    *reinterpret_cast<float*>(bytes + 12) = 1.0f;
    *reinterpret_cast<unsigned*>(bytes + 1020) = 0x01234567u;
    bytes[151] = 1;
    *reinterpret_cast<unsigned*>(bytes) = 0;
    bytes[4] = 0;
    bytes[5] = 1;
    *reinterpret_cast<float*>(bytes + 8) = 1.0f;
    bytes[6] = 0;
    bytes[7] = 0;
}
}
