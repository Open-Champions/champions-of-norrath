// Observed modelDraw__FP12_modelHeaderP8_textureiG6Point3P8Matrix34P14AnimationStateUlP11_drawRecordi
// at 0x0016ae78, 1940 bytes.
// Arguments in order: model header, texture, an int forwarded to
// drawAddToList, a Point3 passed by address (copied at entry), a Matrix34
// pointer, an AnimationState pointer (may be null), a 64-bit flag word, a
// drawRecord pointer, and an int level index (ninth argument, on the stack).
//
// The display-list cursor is glistp (the word at 0x004844c8). A local two-word packet {cursor, last tag} starts at that
// value. A Matrix44 is built by matMul(local, matrix, worldCharViewMatrix) and
// modelSetupCharacter(packet, local, point, model, modelDrawFade) is called.
//
// If modelOption (0x0048408c) is nonzero a state-setup DMA packet is built:
//  * the cursor is advanced 8 and zero-filled to 12 modulo 16; a VIF DIRECT
//    word 0x50000000 sits there and a GIF A+D tag 0x1000000000008000 / 14 follows;
//  * option bit 1: ZBUF_1 and ZBUF_2 (registers 0x4e, 0x4f) from zwritesOff;
//  * option bits 0x28: TEST_1 and TEST_2 (0x47, 0x48), built from ATST/AFAIL
//    (bit 8) and ZTST (bit 0x20);
//  * option bit 2: DIMX (0x44) 0x1212303012123030;
//  * option bit 4: SCISSOR_1/2 (0x40, 0x41) from scx1/scy1/scx2/scy2;
//  * option bit 0x10: PRMODECONT (0x1a) = 0 and PRMODE (0x1b) = 128;
//  then the GIF loop count, VIF immediate and DMA quad count (tag 0x10000000
//  = CNT) are patched in.
// Then a CNT tag with a VIF MSCAL word is appended: address 0x1e when no
// animation state is given, 0xc otherwise (after animLoadBoneMatricies when
// the drawRecord pointer is null).
// If model flags (half at +16) have bit 0x20 and nmHalos > 0 then
// modelDrawHalo(packet, model, 1, halos, nmHalos) is called; if bit 0x40,
// modelProcessGlows(model, anim, matrix) is called.
// The level index is clamped to [0, byte(model+38)-1]. The level's table at
// model+40 + index*(4*byte(model+18)+4) holds byte(model+18)+1 addresses. With
// flag bits for segments below byte(model+18) (bit byte(model+18) is forced
// clear), each maximal run of segments whose bit is SET is emitted as a REF
// DMA tag (0x30000000, address in the upper word, quad count in the low) at
// the next clear bit; the last bit is always clear, so every run is flushed.
// Then, if modelOption is nonzero, a second state packet restores: FLUSH VIF
// word 0x11000000 instead of padding; option bit 1: ZBUF from zwritesOn;
// option bits 0x28: TEST_2 = 0x50013; bit 2: DIMX 0x3131757531317575; bit 4:
// SCISSOR_1/2 from engineScissory1/2 with x1 = 0x04ff; bit 0x10: PRMODECONT 1.
// The final DMA tag is changed: if its id is CNT it becomes id 6 in place,
// otherwise a tag 0x60000000 is appended. (The packet's second word, the last
// tag address, is written for each REF tag but nothing here reads it.) glistp is advanced and
// drawAddToList(start, int, texture, drawRecord, 0) is called.
// The meaning of the model and GS register values is not established here.
struct Packet {
    unsigned char* cursor;
    unsigned char* last;
};

extern "C" {
extern unsigned char* glistp;
extern signed char modelDrawFade;
extern int modelOption;
extern int nmHalos;
extern const void* halos;
extern unsigned long long zwritesOff;
extern unsigned long long zwritesOn;
extern int scx1;
extern int scy1;
extern int scx2;
extern int scy2;
extern int engineScissory1;
extern int engineScissory2;
extern unsigned char worldCharViewMatrix[64];

void matMul__FP8Matrix44P8Matrix34T0(void* out, const void* a, const void* b);
void modelSetupCharacter__FP6PacketP8Matrix44P6Point3P12_modelHeaderc(Packet* packet, void* matrix, const float* point, const void* model, signed char fade);
void animLoadBoneMatricies__FP6PacketP14AnimationState(Packet* packet, void* anim);
void modelDrawHalo__FP6PacketP12_modelHeadercP7HaloDefi(Packet* packet, const void* model, signed char flag, const void* halos, int count);
void modelProcessGlows__FP12_modelHeaderP14AnimationStateP8Matrix34(const void* model, void* anim, void* matrix);
void drawAddToList__FPviP8_textureP11_drawRecordi(void* list, int value, void* texture, void* record, int extra);
}

namespace {
inline void store64(unsigned char* where, unsigned long long value) {
    *reinterpret_cast<unsigned long long*>(where) = value;
}
inline void store32(unsigned char* where, unsigned value) {
    *reinterpret_cast<unsigned*>(where) = value;
}
inline unsigned long long load64(const unsigned char* where) {
    return *reinterpret_cast<const unsigned long long*>(where);
}
inline unsigned load32(const unsigned char* where) {
    return *reinterpret_cast<const unsigned*>(where);
}

struct State {
    unsigned char* tag;
    unsigned char* vif;
    unsigned char* dma;
};

// Starts a DMA packet: remembers its tag address, optionally stores a FLUSH
// VIF word after the DMA tag, pads with zero bytes to offset 12 modulo 16,
// then writes the DIRECT VIF word and an A+D GIF tag.
inline State begin_packet(Packet& packet, bool flush) {
    State state;
    state.dma = packet.cursor;
    unsigned char* at = packet.cursor + 8;
    if (flush) {
        store32(at, 0x11000000u);
        at += 4;
    }
    while ((reinterpret_cast<__UINTPTR_TYPE__>(at) & 0xfu) != 12u) *at++ = 0;
    state.vif = at;
    state.tag = at + 4;
    store32(state.vif, 0x50000000u);
    store64(state.tag, 0x1000000000008000ull);
    store64(state.tag + 8, 14ull);
    packet.cursor = state.tag + 16;
    return state;
}

inline void entry(Packet& packet, unsigned long long value, unsigned long long reg) {
    store64(packet.cursor, value);
    store64(packet.cursor + 8, reg);
    packet.cursor += 16;
}

inline void finish_packet(const Packet& packet, const State& state) {
    unsigned char* cursor = packet.cursor;
    const int gif_loop = (static_cast<int>(cursor - state.tag) / 8 - 2) >> 1;
    const int vif_count = (static_cast<int>(cursor - state.vif) - 4) >> 4;
    const int dma_count = (static_cast<int>(cursor - state.dma) / 8 - 2) >> 1;
    store64(state.tag, load64(state.tag) | static_cast<unsigned long long>(static_cast<long long>(gif_loop)));
    store32(state.vif, load32(state.vif) | static_cast<unsigned>(vif_count));
    store64(state.dma, static_cast<unsigned long long>(static_cast<unsigned>(dma_count) | 0x10000000u));
}
}

extern "C" void modelDraw__FP12_modelHeaderP8_textureiG6Point3P8Matrix34P14AnimationStateUlP11_drawRecordi(
    unsigned char* model, void* texture, int value, const float* position, void* matrix, void* anim,
    unsigned long long flags, void* record, int level) {
    unsigned char matrix44[64];
    Packet packet;
    float point[3] = {position[0], position[1], position[2]};

    packet.cursor = glistp;
    matMul__FP8Matrix44P8Matrix34T0(matrix44, matrix, worldCharViewMatrix);
    modelSetupCharacter__FP6PacketP8Matrix44P6Point3P12_modelHeaderc(&packet, matrix44, point, model, modelDrawFade);

    const int option = modelOption;
    if (option != 0) {
        State state = begin_packet(packet, false);
        if ((option & 1) != 0) {
            entry(packet, zwritesOff, 78);
            entry(packet, zwritesOff, 79);
        }
        if ((option & 0x28) != 0) {
            int atst = 1;
            int afail = 0;
            if ((option & 8) != 0) {
                atst = 0;
                afail = 2;
            }
            const unsigned ztst = (option & 0x20) != 0 ? 1u : 2u;
            const unsigned long long test = static_cast<unsigned long long>(
                (static_cast<unsigned>(atst) << 1 | 0x10u | 0x10000u) |
                ((ztst << 17) | 1u | (static_cast<unsigned>(afail) << 12)));
            entry(packet, test, 71);
            entry(packet, test, 72);
        }
        if ((option & 2) != 0) {
            entry(packet, 0x1212303012123030ull, 68);
        }
        if ((option & 4) != 0) {
            const unsigned long long scissor =
                static_cast<unsigned long long>(static_cast<long long>(scx1)) |
                (static_cast<unsigned long long>(static_cast<long long>(scy2)) << 48) |
                (static_cast<unsigned long long>(static_cast<long long>(scy1)) << 32) |
                (static_cast<unsigned long long>(static_cast<long long>(scx2)) << 16);
            entry(packet, scissor, 64);
            entry(packet, scissor, 65);
        }
        if ((option & 0x10) != 0) {
            entry(packet, 0ull, 26);
            entry(packet, 128ull, 27);
        }
        finish_packet(packet, state);
    }

    unsigned char* last_tag = packet.cursor;
    if (anim == nullptr) {
        store64(packet.cursor, 0x10000000ull);
        store64(packet.cursor + 8, 0x1400001eull);
        packet.cursor += 16;
    } else {
        if (record == nullptr) {
            animLoadBoneMatricies__FP6PacketP14AnimationState(&packet, anim);
        }
        last_tag = packet.cursor;
        store64(packet.cursor, 0x10000000ull);
        store64(packet.cursor + 8, 0x1400000cull);
        packet.cursor += 16;
    }

    const unsigned model_flags = *reinterpret_cast<const unsigned short*>(model + 16);
    if ((model_flags & 0x20u) != 0 && nmHalos > 0) {
        modelDrawHalo__FP6PacketP12_modelHeadercP7HaloDefi(&packet, model, 1, halos, nmHalos);
    }
    if (((*reinterpret_cast<const unsigned short*>(model + 16)) & 0x40u) != 0) {
        modelProcessGlows__FP12_modelHeaderP14AnimationStateP8Matrix34(model, anim, matrix);
    }

    const int segments = static_cast<signed char>(model[18]);
    int lod = level < 0 ? 0 : level;
    const int lod_limit = static_cast<signed char>(model[38]) - 1;
    if (lod > lod_limit) lod = lod_limit;
    const unsigned stride = static_cast<unsigned>(segments * 4 + 4);
    const unsigned char* table = model + 40 + static_cast<unsigned>(lod) * stride;
    const long long mask = static_cast<long long>(flags & ~(1ull << (segments & 63)));
    if (segments >= 0) {
        long long bits = mask;
        const unsigned char* at = table;
        unsigned start = load32(at);
        for (int i = 0; i <= segments; ++i, at += 4) {
            const bool clear = (static_cast<int>(bits) & 1) == 0;
            bits >>= 1;
            if (clear) {
                const unsigned end = load32(at);
                if (start != end) {
                    const unsigned long long tag = (static_cast<unsigned long long>(start) << 32) |
                                                   static_cast<unsigned long long>(static_cast<unsigned>(
                                                       static_cast<int>(end - start) >> 4)) |
                                                   0x30000000ull;
                    unsigned char* where = packet.cursor;
                    packet.cursor = where + 16;
                    store64(where, tag);
                    store64(where + 8, 0ull);
                    packet.last = where;
                }
                start = load32(at + 4);
            }
        }
    }

    if (modelOption != 0) {
        State state = begin_packet(packet, true);
        last_tag = state.dma;
        const int option2 = modelOption;
        if ((option2 & 1) != 0) {
            entry(packet, zwritesOn, 78);
            entry(packet, zwritesOn, 79);
        }
        if ((option2 & 0x28) != 0) {
            entry(packet, 0x50013ull, 72);
        }
        if ((option2 & 2) != 0) {
            entry(packet, 0x3131757531317575ull, 68);
        }
        if ((option2 & 4) != 0) {
            const unsigned long long scissor =
                (static_cast<unsigned long long>(static_cast<long long>(engineScissory2)) << 48) |
                (static_cast<unsigned long long>(static_cast<long long>(engineScissory1)) << 32) | 0x04ff0000ull;
            entry(packet, scissor, 64);
            entry(packet, scissor, 65);
        }
        if ((option2 & 0x10) != 0) {
            entry(packet, 1ull, 26);
        }
        finish_packet(packet, state);
    }

    {
        unsigned long long tag = load64(last_tag);
        if (((tag >> 28) & 7u) == 1u) {
            store64(last_tag, (tag & 0xffffffff8fffffffull) | 0x60000000ull);
        } else {
            unsigned char* where = packet.cursor;
            store64(where, 0x60000000ull);
            store64(where + 8, 0ull);
            packet.cursor = where + 16;
        }
    }
    unsigned char* list = glistp;
    glistp = packet.cursor;
    drawAddToList__FPviP8_textureP11_drawRecordi(list, value, texture, record, 0);
}
