// Observed: SetDefaults__13VIGameOptions at 0x01019a60 is 100 bytes. It clears the 1024 bytes at this with
// memset, then stores: float 1.0 at +8 and +12; byte 0 at +4, 1 at +5, 0 at +6, 0 at +7; word 0 at +0; byte 1
// at +151; word 0x01234567 at +1020. Nothing is returned.
// Inferred: the object is a 1024-byte options record whose last word holds a marker value; the two floats are
// default scale-like values of 1.0, the bytes at +4..+7 and +151 are default flags. No field is named here.
// Unknown: what each default means in gameplay, and whether 0x01234567 is a version or validity marker.
extern "C" {
void* memset(void* dest, int value, unsigned long size);

void SetDefaults__13VIGameOptions(void* self) {
    unsigned char* bytes = static_cast<unsigned char*>(self);
    memset(self, 0, 1024);
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
