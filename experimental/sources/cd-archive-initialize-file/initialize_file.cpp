// Observed initFile at 0x001641a0. strrchr finds '.' (46). The bytes after it
// are compared with the eight adjacent literals at 0x00483f98: anm, vif, tex,
// fnt, world, vag, uni, etex. worldInitWorld's nonzero result is subtracted
// from the size with unsigned wrap. vag passes noNewVags at 0x00483f90 and
// returns size-64. The etex compare's result is discarded. No dot returns 0.
extern "C" {
extern const unsigned char norrath_archive_animation_extension_view[];
extern unsigned norrath_archive_no_new_vags_view;
char* norrath_archive_last_character(const char* text, int character);
int norrath_string_compare(const char* left, const char* right);
void animInitAnimation__FP15AnimationHeader(void* payload);
void modelInitModel__FPv(void* payload);
void textureInitTexture__FP8_texture(void* payload);
void fontInitFont__FP4Font(void* payload);
int worldInitWorld__FP12_worldHeader(void* payload);
void soundAddSound__FPcP10_vagHeaderb(char* name, void* payload, int allow);
void textAddText__FPv(void* payload);
unsigned norrath_archive_initialize_file(char* name, void* payload, int size) {
    char* extension = norrath_archive_last_character(name, 46);
    if (extension == nullptr) return 0u;
    extension += 1;
    const unsigned char* table = norrath_archive_animation_extension_view;
    if (norrath_string_compare(extension, reinterpret_cast<const char*>(table + 0)) == 0) {
        animInitAnimation__FP15AnimationHeader(payload);
        return 0u;
    }
    if (norrath_string_compare(extension, reinterpret_cast<const char*>(table + 8)) == 0) {
        modelInitModel__FPv(payload);
        return 0u;
    }
    if (norrath_string_compare(extension, reinterpret_cast<const char*>(table + 16)) == 0) {
        textureInitTexture__FP8_texture(payload);
        return 0u;
    }
    if (norrath_string_compare(extension, reinterpret_cast<const char*>(table + 24)) == 0) {
        fontInitFont__FP4Font(payload);
        return 0u;
    }
    if (norrath_string_compare(extension, reinterpret_cast<const char*>(table + 32)) == 0) {
        const int produced = worldInitWorld__FP12_worldHeader(payload);
        if (produced != 0) return static_cast<unsigned>(size) - static_cast<unsigned>(produced);
        return 0u;
    }
    if (norrath_string_compare(extension, reinterpret_cast<const char*>(table + 40)) == 0) {
        soundAddSound__FPcP10_vagHeaderb(name, payload, static_cast<int>(norrath_archive_no_new_vags_view));
        return static_cast<unsigned>(size) - 64u;
    }
    if (norrath_string_compare(extension, reinterpret_cast<const char*>(table + 48)) == 0) {
        textAddText__FPv(payload);
        return 0u;
    }
    (void)norrath_string_compare(extension, reinterpret_cast<const char*>(table + 56));
    return 0u;
}
}
