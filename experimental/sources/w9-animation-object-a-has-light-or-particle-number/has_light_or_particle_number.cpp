// Observed hasLightOrParticleNumber__FPPci at 0x0021df18, 828 bytes. Takes a
// zero-terminated array of "name=value" text pointers (a null array pointer is allowed)
// and a number. Three tag names are built in a 32-byte stack buffer with sprintf, in this
// order: "ParticleEmitter%d", "DynamicLight%d" and "Halo%d", each given the number. For
// each name the same inlined lookup runs: with a null array the result is 0; otherwise the
// name is lowercased in place (ctype bit 0, add 32), its length taken, and the first entry
// whose first length bytes match (strncmp) and whose next byte is '=' gives atoi of the text
// after the '='; no such entry gives 0. The function returns 1 as soon as one lookup
// is nonzero, and 0 when all three are zero (the third lookup's result is tested for
// nonzero, the first two are tested before the next name is built).
// Inferred: the lookup is the same scan as the integer-valued tag finder on a game
// object, applied to a raw tag array; the three names ask whether the tags carry an
// emitter, a dynamic light or a halo with that number. Note the built names are
// lowercased in place, so "ParticleEmitter3" is matched as "particleemitter3".
// Unknown: the semantics of the tags beyond their names, and the buffer size beyond 32. Two
// instructions of the original's scan (the step after a non-selected hit) cannot run in
// its three inlined copies, because the hit counter starts at zero and the first hit always
// returns; the authored scan has no such step and keeps the same results.
extern "C" {
int sprintf(char* destination, const char* format, ...);
unsigned norrath_factory_strlen(const char* text);
int norrath_factory_strncmp(const char* left, const char* right, unsigned count);
int norrath_factory_atoi(const char* text);
extern const unsigned char norrath_factory_ctype_minimum_view[];
}

namespace {
__attribute__((always_inline)) inline int tag_int(char** list, char* name) {
    int hits = 0;
    if (list == nullptr) return 0;
    for (unsigned char* cursor = reinterpret_cast<unsigned char*>(name); *cursor != 0u; ++cursor) {
        const int character = *cursor < 0x80u ? int(*cursor) : int(*cursor) - 0x100;
        const int folded = (norrath_factory_ctype_minimum_view[static_cast<unsigned>(character + 128)] & 1u) != 0u
                               ? character + 32
                               : character;
        *cursor = static_cast<unsigned char>(folded);
    }
    const unsigned length = norrath_factory_strlen(name);
    if (*list == nullptr) return 0;
    for (char** cursor = list; *cursor != nullptr; ++cursor) {
        if (norrath_factory_strncmp(*cursor, name, length) == 0 && (*cursor)[length] == '=') {
            hits -= 1;
            if (hits == -1) return norrath_factory_atoi(*cursor + length + 1);
        }
    }
    return 0;
}
}

extern "C" int hasLightOrParticleNumber__FPPci(char** tags, int number) {
    char name[32];
    sprintf(name, "ParticleEmitter%d", number);
    if (tag_int(tags, name) != 0) return 1;
    sprintf(name, "DynamicLight%d", number);
    if (tag_int(tags, name) != 0) return 1;
    sprintf(name, "Halo%d", number);
    return tag_int(tags, name) != 0;
}
