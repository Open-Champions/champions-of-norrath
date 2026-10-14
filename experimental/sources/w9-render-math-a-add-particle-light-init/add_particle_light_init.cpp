// Observed lightInit__11AddParticlePPc at 0x002233c0, 928 bytes. a0 = this; the char**
// parameter in a1 is never read. All names are looked up with findTagInt__10GameObjectPci
// (object, name, 0); each lookup name is a literal prefix followed by a suffix text.
//  1. The suffix is sprintf of "%d" with the signed word at this+76 when that word is
//     non-negative, otherwise the empty string.
//  2. The name "DynamicLight" + suffix is looked up. A zero answer skips to step 4.
//  3. Otherwise this+84 = 1, and these lookups store at: "lsrad" -> +324, "lerad" -> +328,
//     "lrc" -> +332, "lrt" -> +336, "lsr" -> +340, "lsg" -> +344, "lsb" -> +348,
//     "lsa" -> +352. The words at +324 and +328 are then each clamped to 0..300 (above 300
//     becomes 300, below zero becomes 0).
//  4. The name "Halo" + suffix is looked up. A non-zero answer sets this+88 = 1 and stores
//     "hlr" -> +356, "hlg" -> +360, "hlb" -> +364, "hla" -> +368, "hls" -> +372,
//     "hlf" -> +376.
//  5. this+380 = 100 in every case.
// Every lookup default is zero. Each prefix is copied into a 16-byte stack buffer with its
// terminator and the suffix is appended by strcat. The tag strings are the exact bytes at
// 0x470cc8, 0x4856f0..0x485758 and 0x485470 in the executable; their meaning as light
// radius, colour, flare and halo settings is inferred only from the names. Fresh
// compilation is not an original-byte match.
struct AddParticle {
    unsigned char unknown_0[76];
    int light_number;
    unsigned char unknown_80[4];
    int has_light;
    int has_halo;
    unsigned char unknown_92[232];
    int values[15];
};
static_assert(__builtin_offsetof(AddParticle, light_number) == 76, "number offset");
static_assert(__builtin_offsetof(AddParticle, has_light) == 84, "light flag offset");
static_assert(__builtin_offsetof(AddParticle, has_halo) == 88, "halo flag offset");
static_assert(__builtin_offsetof(AddParticle, values) == 324, "first value offset");
static_assert(sizeof(AddParticle) == 384, "size through +380");

extern "C" {
int sprintf(char* buffer, const char* format, ...);
char* strcat(char* destination, const char* source);
int findTagInt__10GameObjectPci(void* object, const char* name, int default_value);
}

namespace {
int look_up(AddParticle* self, char* name, const char* prefix, const char* suffix) {
    char* out = name;
    do {
        *out++ = *prefix;
    } while (*prefix++ != 0);
    strcat(name, suffix);
    return findTagInt__10GameObjectPci(self, name, 0);
}
}

extern "C" void lightInit__11AddParticlePPc(AddParticle* self, char**) {
    char suffix[16];
    char name[32];
    if (self->light_number >= 0) {
        sprintf(suffix, "%d", self->light_number);
    } else {
        suffix[0] = 0;
    }
    if (look_up(self, name, "DynamicLight", suffix) != 0) {
        self->has_light = 1;
        self->values[0] = look_up(self, name, "lsrad", suffix);
        self->values[1] = look_up(self, name, "lerad", suffix);
        self->values[2] = look_up(self, name, "lrc", suffix);
        self->values[3] = look_up(self, name, "lrt", suffix);
        self->values[4] = look_up(self, name, "lsr", suffix);
        self->values[5] = look_up(self, name, "lsg", suffix);
        self->values[6] = look_up(self, name, "lsb", suffix);
        self->values[7] = look_up(self, name, "lsa", suffix);
        if (self->values[0] > 300) self->values[0] = 300;
        if (self->values[0] < 0) self->values[0] = 0;
        if (self->values[1] > 300) self->values[1] = 300;
        if (self->values[1] < 0) self->values[1] = 0;
    }
    if (look_up(self, name, "Halo", suffix) != 0) {
        self->has_halo = 1;
        self->values[8] = look_up(self, name, "hlr", suffix);
        self->values[9] = look_up(self, name, "hlg", suffix);
        self->values[10] = look_up(self, name, "hlb", suffix);
        self->values[11] = look_up(self, name, "hla", suffix);
        self->values[12] = look_up(self, name, "hls", suffix);
        self->values[13] = look_up(self, name, "hlf", suffix);
    }
    self->values[14] = 100;
}
