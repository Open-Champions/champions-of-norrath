// Observed hasLightOrParticleNumber__FPPci at 0x21df18, extent 828.
// Looks for three tags named by number: ParticleEmitter<n>, DynamicLight<n>
// and Halo<n> (formats at 0x470c90, 0x470ca8, 0x485468). For each, the name
// is formatted into a 32-byte buffer even when the argument list is null,
// lowercased in place with ctype bit 0, and compared to every entry of the
// zero-terminated string list with strncmp over the name length. The first
// entry that matches and has '=' right after the name supplies atoi of the text
// after the '='; later entries are not read. A null list, no match, or a match
// without '=' gives zero. The result is one when the first or second tag value
// is nonzero, and otherwise whether the third value is nonzero.
extern "C" {
extern const unsigned char norrath_factory_ctype_minimum_view[];
unsigned norrath_factory_strlen(const char* text);
int norrath_factory_strncmp(const char* left, const char* right, unsigned count);
int norrath_factory_atoi(const char* text);
int norrath_factory_sprintf(char* destination, const char* format, ...);
}

namespace {
const char particle_format[] = "ParticleEmitter%d";
const char light_format[] = "DynamicLight%d";
const char halo_format[] = "Halo%d";

int signed_byte(unsigned char value) {
    return value < 128u ? static_cast<int>(value) : static_cast<int>(value) - 256;
}

int numbered_tag_value(char** arguments, const char* format, int number) {
    char name[32];
    norrath_factory_sprintf(name, format, number);
    if (arguments == nullptr) return 0;
    for (unsigned char* cursor = reinterpret_cast<unsigned char*>(name); *cursor != 0u; ++cursor) {
        const int character = signed_byte(*cursor);
        const unsigned flags = norrath_factory_ctype_minimum_view[static_cast<unsigned>(character + 128)];
        *cursor = static_cast<unsigned char>((flags & 1u) != 0u ? character + 32 : character);
    }
    const unsigned length = norrath_factory_strlen(name);
    for (char** entry = arguments; *entry != nullptr; ++entry) {
        if (norrath_factory_strncmp(*entry, name, length) != 0) continue;
        if (signed_byte(static_cast<unsigned char>((*entry)[length])) != 61) continue;
        return norrath_factory_atoi(*entry + length + 1);
    }
    return 0;
}
}

extern "C" int norrath_has_light_or_particle_number(char** arguments, int number) {
    if (numbered_tag_value(arguments, particle_format, number) != 0) return 1;
    if (numbered_tag_value(arguments, light_format, number) != 0) return 1;
    return numbered_tag_value(arguments, halo_format, number) != 0 ? 1 : 0;
}
