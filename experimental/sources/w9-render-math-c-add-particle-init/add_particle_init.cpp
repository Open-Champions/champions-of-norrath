// Observed particleInit__11AddParticlePPc at 0x00222d20, 1692 bytes (an AddParticle method:
// the second parameter, a char**, is passed in a1 but never read).
// The object is a game object (findTagInt__10GameObjectPci is called on it) with, in bytes:
//   76: int slot number (-1 when absent)   80: set to 1 when the emitter tag is present
//   100..107, 114..117: byte fields   108, 110, 112, 118..142: halfword fields
//   144, 148: word fields   152: flag byte   172: the 80-byte ParticleDef given to P_UpdateParticleType
// Steps: a text prefix is built (empty when word 76 is -1, otherwise "%d" of word 76 via sprintf). The
// tag key "ParticleEmitter" followed by the prefix is looked up with findTagInt(object, key, 0); a zero
// result ends the function (nothing else is touched). Otherwise word 80 is set to 1 and each field below is
// read by writing its three-letter name into the start of the key, appending the prefix again
// (strcat) and calling findTagInt(object, key, 0); the result is stored at the offset with the width shown:
//   psr b100 psg b101 pse b102 psa b103 per b104 peg b105 peb b106 pea b107 pss h108 pes h110 pgr h112
//   plf b114 prm b115 ptx b116 bcn b117 bxo h118 byo h120 bzo h122 bvx h124 bvy h126 bvz h128 byw h130
//   bpt h132 box h134 boy h136 boz h138 bvr h140 bar h142 bot w144 bft w148
// bwa sets bit 0 of byte 152 when its value is nonzero; bfa sets bit 1 of byte 152 when nonzero.
// Finally P_UpdateParticleType__FP11ParticleDefiiiiiiiifffiiiPFPv_v is called with (object + 172,
// bytes 100..107, (float) u16[108], (float) u16[110], (float) s16[112], bytes 114, 115, 116, null callback).
// Inferred: reads emitter parameters from level object tags into a particle definition. Unknown: the
// parameter meanings (the three letter tag names are as found), and the real type of the second argument.
extern "C" {
int sprintf(char* destination, const char* format, ...);
char* strcat(char* destination, const char* source);
}
extern "C" int findTagInt__10GameObjectPci(void* object, char* tag, int occurrence);
extern "C" void P_UpdateParticleType__FP11ParticleDefiiiiiiiifffiiiPFPv_v(void* definition, int a, int b, int c, int d,
                                                                         int e, int f, int g, int h, float x, float y,
                                                                         float z, int i, int j, int k, void (*callback)(void*));

namespace {
inline int lookup(unsigned char* object, char* key, const char* prefix, char a, char b, char c) {
    key[0] = a; key[1] = b; key[2] = c; key[3] = '\0';
    strcat(key, prefix);
    return findTagInt__10GameObjectPci(object, key, 0);
}
}

extern "C" void particleInit__11AddParticlePPc(unsigned char* self, char**) {
    char prefix[16];
    char key[24];
    const char emitter[16] = {'P', 'a', 'r', 't', 'i', 'c', 'l', 'e', 'E', 'm', 'i', 't', 't', 'e', 'r', '\0'};
    for (int i = 0; i < 16; ++i) key[i] = emitter[i];
    const int slot = *reinterpret_cast<int*>(self + 76);
    if (slot == -1) prefix[0] = '\0';
    else sprintf(prefix, "%d", slot);
    strcat(key, prefix);
    if (findTagInt__10GameObjectPci(self, key, 0) == 0) return;
    *reinterpret_cast<int*>(self + 80) = 1;
    self[100] = static_cast<unsigned char>(lookup(self, key, prefix, 'p', 's', 'r'));
    self[101] = static_cast<unsigned char>(lookup(self, key, prefix, 'p', 's', 'g'));
    self[102] = static_cast<unsigned char>(lookup(self, key, prefix, 'p', 's', 'e'));
    self[103] = static_cast<unsigned char>(lookup(self, key, prefix, 'p', 's', 'a'));
    self[104] = static_cast<unsigned char>(lookup(self, key, prefix, 'p', 'e', 'r'));
    self[105] = static_cast<unsigned char>(lookup(self, key, prefix, 'p', 'e', 'g'));
    self[106] = static_cast<unsigned char>(lookup(self, key, prefix, 'p', 'e', 'b'));
    self[107] = static_cast<unsigned char>(lookup(self, key, prefix, 'p', 'e', 'a'));
    *reinterpret_cast<short*>(self + 108) = static_cast<short>(lookup(self, key, prefix, 'p', 's', 's'));
    *reinterpret_cast<short*>(self + 110) = static_cast<short>(lookup(self, key, prefix, 'p', 'e', 's'));
    *reinterpret_cast<short*>(self + 112) = static_cast<short>(lookup(self, key, prefix, 'p', 'g', 'r'));
    self[114] = static_cast<unsigned char>(lookup(self, key, prefix, 'p', 'l', 'f'));
    self[115] = static_cast<unsigned char>(lookup(self, key, prefix, 'p', 'r', 'm'));
    self[116] = static_cast<unsigned char>(lookup(self, key, prefix, 'p', 't', 'x'));
    self[117] = static_cast<unsigned char>(lookup(self, key, prefix, 'b', 'c', 'n'));
    *reinterpret_cast<short*>(self + 118) = static_cast<short>(lookup(self, key, prefix, 'b', 'x', 'o'));
    *reinterpret_cast<short*>(self + 120) = static_cast<short>(lookup(self, key, prefix, 'b', 'y', 'o'));
    *reinterpret_cast<short*>(self + 122) = static_cast<short>(lookup(self, key, prefix, 'b', 'z', 'o'));
    *reinterpret_cast<short*>(self + 124) = static_cast<short>(lookup(self, key, prefix, 'b', 'v', 'x'));
    *reinterpret_cast<short*>(self + 126) = static_cast<short>(lookup(self, key, prefix, 'b', 'v', 'y'));
    *reinterpret_cast<short*>(self + 128) = static_cast<short>(lookup(self, key, prefix, 'b', 'v', 'z'));
    *reinterpret_cast<short*>(self + 130) = static_cast<short>(lookup(self, key, prefix, 'b', 'y', 'w'));
    *reinterpret_cast<short*>(self + 132) = static_cast<short>(lookup(self, key, prefix, 'b', 'p', 't'));
    *reinterpret_cast<short*>(self + 134) = static_cast<short>(lookup(self, key, prefix, 'b', 'o', 'x'));
    *reinterpret_cast<short*>(self + 136) = static_cast<short>(lookup(self, key, prefix, 'b', 'o', 'y'));
    *reinterpret_cast<short*>(self + 138) = static_cast<short>(lookup(self, key, prefix, 'b', 'o', 'z'));
    *reinterpret_cast<short*>(self + 140) = static_cast<short>(lookup(self, key, prefix, 'b', 'v', 'r'));
    *reinterpret_cast<short*>(self + 142) = static_cast<short>(lookup(self, key, prefix, 'b', 'a', 'r'));
    *reinterpret_cast<int*>(self + 144) = lookup(self, key, prefix, 'b', 'o', 't');
    *reinterpret_cast<int*>(self + 148) = lookup(self, key, prefix, 'b', 'f', 't');
    const unsigned char before = self[152];
    const int wa = lookup(self, key, prefix, 'b', 'w', 'a');
    self[152] = static_cast<unsigned char>(before | (wa != 0 ? 1u : 0u));
    unsigned char flags = self[152];
    const int fa = lookup(self, key, prefix, 'b', 'f', 'a');
    if (fa != 0) flags = static_cast<unsigned char>(flags | 2u);
    self[152] = flags;
    P_UpdateParticleType__FP11ParticleDefiiiiiiiifffiiiPFPv_v(
        self + 172, self[100], self[101], self[102], self[103], self[104], self[105], self[106], self[107],
        static_cast<float>(*reinterpret_cast<unsigned short*>(self + 108)),
        static_cast<float>(*reinterpret_cast<unsigned short*>(self + 110)),
        static_cast<float>(*reinterpret_cast<short*>(self + 112)), self[114], self[115], self[116], nullptr);
}
