// Observed: InitSolidFill__10VIMaterial at 0x010ce708 is 172 bytes. On the object
// it stores: word 1 at offset 8; word 19 at 12; bytes 0,0,0,0 at 16 to 19; word 1
// at 52; word 1 at 32; word -1 at 48; bytes 0xff at 56 to 59; word 0 at 60. It then
// calls Identity__10VIMatrix33 on offset 64. It stores 0.0f at 104, 108, 112 and
// 116, word -1 at 120, and the float bits 0x3f4ccccc at 100. It returns 0.
// Inferred: this configures a plain solid-colour material; 19 and the colour
// bytes are an enum/colour whose meaning is not established here.
extern "C" {
void Identity__10VIMatrix33(void* matrix);

int InitSolidFill__10VIMaterial(void* object) {
    unsigned char* bytes = static_cast<unsigned char*>(object);
    *reinterpret_cast<int*>(bytes + 8) = 1;
    *reinterpret_cast<int*>(bytes + 12) = 19;
    bytes[19] = 0;
    bytes[16] = 0;
    bytes[17] = 0;
    bytes[18] = 0;
    *reinterpret_cast<int*>(bytes + 52) = 1;
    *reinterpret_cast<int*>(bytes + 32) = 1;
    *reinterpret_cast<int*>(bytes + 48) = -1;
    bytes[56] = 0xff;
    bytes[57] = 0xff;
    bytes[58] = 0xff;
    bytes[59] = 0xff;
    *reinterpret_cast<int*>(bytes + 60) = 0;
    Identity__10VIMatrix33(bytes + 64);
    float* tail = reinterpret_cast<float*>(bytes + 112);
    tail[1] = 0.0f;
    tail[0] = 0.0f;
    *reinterpret_cast<int*>(bytes + 120) = -1;
    float* mid = reinterpret_cast<float*>(bytes + 104);
    mid[1] = 0.0f;
    mid[0] = 0.0f;
    *reinterpret_cast<unsigned int*>(bytes + 100) = 0x3f4cccccu;
    return 0;
}
}
