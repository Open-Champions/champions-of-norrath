// Observed Reset__6Cameraii at 0x0026be00, 480 bytes. Arguments are the camera,
// then two ints, called `defaults` and `fresh` here only to tell them apart.
// Nothing here was executed on the EE.
//
// defaults nonzero: halfwords at +108, +110, +112 become 8000, 10000, 13000.
// Floats become +176 170.0, +180 420.0, +184 500.0, +188 90.0, +192 60.0,
// +196 60.0, +220 150.0, +224 40.0, +228 20.0, +232 -20.0, +236 -25.0,
// +240 -25.0, +164 -1.0. The word at +80 becomes 0.
// fresh nonzero: words +88 = 0 and +84 = 1, and byte +353 = 0xff.
// Always, with i = word at +84 read after those: halfword +138 = -8192, word
// +160 = 0, +92 = 1, bytes +364 and +365 = 0, halfword +116 = the halfword at
// +108+2*i, word +68 = -1, float +144 = 1.0, words +56 and +76 = 0, floats
// +152, +156, +200, +204, +208 and +212 = 0.0. Byte +366 is the low byte of
// nmPlayersAlive (0x00484f44) when that signed word is above zero, otherwise
// the low byte of nmPlayers (0x00484538). The 96 bytes at +256 are cleared with
// memset. Then SetYaw__6Camerai(-8192), SetPitch__6Camerai with the signed
// halfword at +108+2*i, SetZoomDistance__6Cameraf with the float at +176+4*i,
// and BuildMatrix__6CamerafffP6Point3 with three 0.0 floats and a null point.
// Finally the word at +72 becomes 1. Nothing is returned.
// The camera field meanings are not established here.
extern "C" {
void* memset(void* dest, int value, unsigned long size);
void SetYaw__6Camerai(void* camera, int yaw);
void SetPitch__6Camerai(void* camera, int pitch);
void SetZoomDistance__6Cameraf(void* camera, float distance);
void BuildMatrix__6CamerafffP6Point3(void* camera, float a, float b, float c, void* point);
extern int nmPlayersAlive;
extern int nmPlayers;
}

typedef int __attribute__((may_alias)) AliasedInt;
typedef float __attribute__((may_alias)) AliasedFloat;
typedef short __attribute__((may_alias)) AliasedShort;

#define WORD(off) (*reinterpret_cast<AliasedInt*>(camera + (off)))
#define FLOAT(off) (*reinterpret_cast<AliasedFloat*>(camera + (off)))
#define HALF(off) (*reinterpret_cast<AliasedShort*>(camera + (off)))

extern "C" void Reset__6Cameraii(unsigned char* camera, int defaults, int fresh) {
    if (defaults != 0) {
        HALF(108) = 8000;
        HALF(110) = 10000;
        HALF(112) = 13000;
        FLOAT(176) = 170.0f;
        FLOAT(180) = 420.0f;
        FLOAT(184) = 500.0f;
        FLOAT(188) = 90.0f;
        FLOAT(196) = 60.0f;
        FLOAT(220) = 150.0f;
        FLOAT(224) = 40.0f;
        FLOAT(228) = 20.0f;
        FLOAT(232) = -20.0f;
        FLOAT(240) = -25.0f;
        FLOAT(164) = -1.0f;
        FLOAT(192) = 60.0f;
        FLOAT(236) = -25.0f;
        WORD(80) = 0;
    }
    if (fresh != 0) {
        WORD(88) = 0;
        WORD(84) = 1;
        camera[353] = 0xff;
    }
    HALF(138) = -8192;
    WORD(160) = 0;
    WORD(92) = 1;
    camera[364] = 0;
    camera[365] = 0;
    const int index = WORD(84);
    HALF(116) = HALF(108 + 2 * index);
    WORD(68) = -1;
    FLOAT(144) = 1.0f;
    WORD(56) = 0;
    WORD(76) = 0;
    FLOAT(152) = FLOAT(160);
    FLOAT(156) = FLOAT(160);
    FLOAT(200) = FLOAT(160);
    FLOAT(208) = FLOAT(160);
    FLOAT(212) = FLOAT(160);
    FLOAT(204) = FLOAT(160);
    camera[366] = nmPlayersAlive > 0 ? static_cast<unsigned char>(nmPlayersAlive)
                                     : static_cast<unsigned char>(nmPlayers);
    memset(camera + 256, 0, 96);
    SetYaw__6Camerai(camera, -8192);
    const int index2 = WORD(84);
    SetPitch__6Camerai(camera, HALF(108 + 2 * index2));
    const int index3 = WORD(84);
    SetZoomDistance__6Cameraf(camera, FLOAT(176 + 4 * index3));
    BuildMatrix__6CamerafffP6Point3(camera, 0.0f, 0.0f, 0.0f, nullptr);
    WORD(72) = 1;
}
