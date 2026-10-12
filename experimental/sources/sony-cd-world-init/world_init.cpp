// Observed worldInitWorld__FP12_worldHeader at 0x001745d8, 1748 bytes.
// File offsets are relocated by the header. "%s.tex" at 0x00484168 is loaded
// through cdLoad into a 64-byte-aligned stack buffer. Emitters with bit
// 0x4000 are matched or appended in ParticleEmitterArray. A zero float at
// +104 becomes 960. The return is the word at +120 when the word at +112 is
// nonzero, and 0 otherwise.
extern "C" {
extern int megatile_ramkmax;
extern int nmParticleEmitterRecs;
extern short texmapstart;
extern short texmapend;
extern unsigned char texmap[];
extern char engineCurrentLevelName[];
extern unsigned char DynamicLightArray[];
extern unsigned char ParticleEmitterArray[];
int sprintf(char* dest, const char* format, const char* name);
int cdLoad__FPcPvii(char* name, void* destination, int bytes, int argument);
void lightDeactivate__FP5Light(void* light);
int __eq__19ParticleEmitterDataRC19ParticleEmitterData(void* left, void* right);
void P_UpdateParticleType__FP11ParticleDefiiiiiiiifffiiiPFPv_v(
    void* definition, int a, int b, int c, int d, int e, int f, int g, float x, float y, float z, int h, int i,
    int j, int k, void (*function)(void*));
void textureInitTexture__FP8_texture(void* texture);
void megatileInit__FP12MegatileRootPv(void* root, void* header);
void _engineCheckStack__FPCci(const char* file, int line);
int scePrintf(const char* text);
void megatileGenerate__FP12_worldHeader(void* header);
void earlyInit__3mapP12_worldHeader(void* header);
static unsigned load_word(const unsigned char* slot) {
    return static_cast<unsigned>(slot[0]) | (static_cast<unsigned>(slot[1]) << 8) |
           (static_cast<unsigned>(slot[2]) << 16) | (static_cast<unsigned>(slot[3]) << 24);
}
static int load_signed(const unsigned char* slot) { return static_cast<int>(load_word(slot)); }
static int load_short(const unsigned char* slot) {
    const unsigned value = static_cast<unsigned>(slot[0]) | (static_cast<unsigned>(slot[1]) << 8);
    return value < 0x8000u ? static_cast<int>(value) : static_cast<int>(value) - 65536;
}
static unsigned load_unsigned_short(const unsigned char* slot) {
    return static_cast<unsigned>(slot[0]) | (static_cast<unsigned>(slot[1]) << 8);
}
static void store_word(unsigned char* slot, unsigned value) {
    slot[0] = static_cast<unsigned char>(value);
    slot[1] = static_cast<unsigned char>(value >> 8);
    slot[2] = static_cast<unsigned char>(value >> 16);
    slot[3] = static_cast<unsigned char>(value >> 24);
}
static void store_half(unsigned char* slot, unsigned value) {
    slot[0] = static_cast<unsigned char>(value);
    slot[1] = static_cast<unsigned char>(value >> 8);
}
static unsigned pointer_bits(const void* pointer) {
    return static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(pointer));
}
static unsigned char* add_base(unsigned char* header, unsigned offset) { return header + load_word(header + offset); }
static unsigned char* stored_pointer(unsigned char* header, unsigned bits) {
#if defined(NORRATH_HOST_CONTRACT)
    return header + (bits - pointer_bits(header));
#else
    return reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(bits));
#endif
}
static void relocate_if_set(unsigned char* header, unsigned offset) {
    if (load_word(header + offset) != 0u) store_word(header + offset, pointer_bits(add_base(header, offset)));
}
static void copy_bytes(unsigned char* dest, const unsigned char* source, unsigned count) {
    for (unsigned index = 0; index < count; ++index) dest[index] = source[index];
}
static int align_shift(int value) {
    if (value < 0) value += 3;
    return value >> 2;
}
int worldInitWorld__FP12_worldHeader(void* payload) {
    auto* header = static_cast<unsigned char*>(payload);
    relocate_if_set(header, 4u);
    store_word(header + 36u, pointer_bits(add_base(header, 36u)));
    if (load_word(header + 24u) != 0u) {
        auto* cursor = add_base(header, 24u);
        store_word(header + 24u, pointer_bits(cursor));
        int count = load_signed(header + 20) * load_signed(header + 16);
        const unsigned extra = load_word(header + 120);
        while (count > 0) {
            store_word(cursor, pointer_bits(header + load_word(cursor)) + extra);
            cursor += 4;
            count -= 1;
        }
    }
    if (load_word(header + 32u) != 0u) {
        auto* cursor = add_base(header, 32u);
        store_word(header + 32u, pointer_bits(cursor));
        int count = load_signed(header + 28);
        while (count > 0) {
            store_word(cursor + 12, pointer_bits(header + load_word(cursor + 12)));
            cursor += 36;
            count -= 1;
        }
    }
    relocate_if_set(header, 56u);
    relocate_if_set(header, 76u);
    if (load_word(header + 84u) != 0u) {
        auto* group = add_base(header, 84u);
        store_word(header + 84u, pointer_bits(group));
        int groups = load_signed(header + 80);
        auto* records = stored_pointer(header, load_word(header + 32u));
        while (groups > 0) {
            const int which = load_short(group + 24);
            auto* record = records + (which * 9 * 4);
            auto* block = stored_pointer(header, load_word(record + 12));
            int cells = load_signed(block + 8) * load_signed(block + 12);
            auto* half = block + 20;
            while (cells > 0) {
                const unsigned bits = load_unsigned_short(half);
                if ((bits & 1u) != 0u) store_half(half, (bits & 0xfu) + 32000u);
                half += 2;
                cells -= 1;
            }
            store_half(block + 18, 2000u);
            group += 28;
            groups -= 1;
        }
    }
    store_word(header + 100u, pointer_bits(add_base(header, 100u)));
    unsigned char stack[20784];
    auto* loaded = stack + 48;
    if ((pointer_bits(loaded) & 63u) != 0u) {
        do {
            loaded += 4;
        } while ((pointer_bits(loaded) & 63u) != 0u);
    }
    char* text = reinterpret_cast<char*>(stack + 20592);
    sprintf(text, "%s.tex", engineCurrentLevelName);
    cdLoad__FPcPvii(text, loaded, 20480, 0);
    auto* records_in = loaded + 4;
    unsigned first = load_word(records_in);
    if (first != 0u) {
        int span = load_signed(header + 92) - load_signed(header + 88) + 1;
        if (span > 0) {
            auto* pairs = stored_pointer(header, load_word(header + 100u));
            for (int index = 0; index < span; ++index) {
                store_word(pairs, 0u);
                store_word(pairs + 4, 64u);
                pairs += 8;
            }
        }
        while (static_cast<unsigned>(static_cast<int>(first) + 2) >= 2u) {
            const unsigned key = first - load_word(header + 88);
            auto* pairs = stored_pointer(header, load_word(header + 100u));
            store_word(pairs + key * 8u, load_word(records_in + 4));
            store_word(pairs + (load_word(records_in) - load_word(header + 88)) * 8u + 4u, load_word(records_in + 8));
            records_in += 12;
            first = load_word(records_in);
        }
    }
    if (static_cast<int>(first) == -2) megatile_ramkmax = 15360;
    nmParticleEmitterRecs = 0;
    for (int light = 0; light < 16; ++light) {
        lightDeactivate__FP5Light(DynamicLightArray + 8 + light * 68);
        store_word(DynamicLightArray + light * 68, 0u);
        store_word(DynamicLightArray + light * 68 + 4u, 0u);
    }
    relocate_if_set(header, 128u);
    const int emitter_count = load_signed(header);
    for (int index = 0; index < emitter_count; ++index) {
        auto* emitter = stored_pointer(header, load_word(header + 36u)) + index * 60;
        const unsigned flags = load_word(emitter + 36);
        auto* body = header + load_word(emitter);
        store_word(emitter, pointer_bits(body));
        if ((flags & 0x80u) != 0u) continue;
        if ((flags & 0x4000u) != 0u) {
            if (nmParticleEmitterRecs >= 16) continue;
            int match = 0;
            while (match < nmParticleEmitterRecs) {
                if (__eq__19ParticleEmitterDataRC19ParticleEmitterData(body, ParticleEmitterArray + match * 152) != 0) break;
                match += 1;
            }
            store_word(emitter + 4, static_cast<unsigned>(match));
            if (match != nmParticleEmitterRecs) continue;
            const auto byte_at = [&](unsigned offset) { return static_cast<int>(body[offset]); };
            const float first = static_cast<float>(load_unsigned_short(body + 8));
            const float second = static_cast<float>(load_unsigned_short(body + 10));
            const float third = static_cast<float>(load_short(body + 12));
            auto* slot = ParticleEmitterArray + nmParticleEmitterRecs * 152;
            P_UpdateParticleType__FP11ParticleDefiiiiiiiifffiiiPFPv_v(
                slot + 72, byte_at(0), byte_at(1), byte_at(2), byte_at(3), byte_at(4), byte_at(5), byte_at(6), first,
                second, third, byte_at(7), byte_at(14), byte_at(15), byte_at(16), nullptr);
            copy_bytes(slot, body, 72u);
            nmParticleEmitterRecs += 1;
            continue;
        }
        if ((flags & 0x8000u) != 0u) store_word(emitter + 4, 0u);
    }
    unsigned float_bits = load_word(header + 104);
    if (float_bits == 0u) store_word(header + 104u, 0x44700000u);
    if (load_word(header + 108u) != 0u) {
        auto* texture = add_base(header, 108u);
        store_word(header + 108u, pointer_bits(texture));
        textureInitTexture__FP8_texture(texture);
    }
    if (load_word(header + 112u) != 0u) {
        auto* root = add_base(header, 112u);
        store_word(header + 80u, 0u);
        store_word(header + 112u, pointer_bits(root));
        store_word(header + 84u, 0u);
        megatileInit__FP12MegatileRootPv(root, header);
        _engineCheckStack__FPCci("world.cc", 327);
        scePrintf("qmt1\n");
        megatileGenerate__FP12_worldHeader(header);
        scePrintf("qmt2\n");
        _engineCheckStack__FPCci("world.cc", 331);
    } else {
        texmapstart = static_cast<short>(load_unsigned_short(header + 88));
        texmapend = static_cast<short>(load_unsigned_short(header + 92));
        if ((load_word(header + 116u) & 1u) != 0u) {
            texmapstart = static_cast<short>(align_shift(texmapstart));
            texmapend = static_cast<short>(align_shift(texmapend));
        }
        const int start = texmapstart;
        const int end = texmapend;
        if (end - start + 1 > 0) {
            for (int index = 0; index < end - start + 1; ++index)
                store_half(texmap + index * 2, static_cast<unsigned>(static_cast<unsigned short>(start) + index));
        }
    }
    if (load_word(header + 4u) != 0u) {
        auto* text_pointer = stored_pointer(header, load_word(header + 4u));
        if (load_short(text_pointer) == 17767) store_word(header + 4u, pointer_bits(text_pointer + 2));
    }
    earlyInit__3mapP12_worldHeader(header);
    if (load_word(header + 112u) == 0u) return 0;
    return load_signed(header + 120);
}
}
