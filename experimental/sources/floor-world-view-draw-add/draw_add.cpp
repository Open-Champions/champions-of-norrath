// Observed drawAddToList__FPviP8_textureP11_drawRecordi at 0x0015eb80, 408 bytes.
// nmDrawUsed is compared with 2048 using a signed less-than. At the limit
// the function returns a null pointer and does not advance the count.
// The record is drawRecords[(writeBuffer << 11) + old count], 16 bytes:
// the packet address masked with 0x0fffffff, the fifth argument, the texture
// pointer, and a next pointer. A texture whose unsigned halfword at offset 4
// is not strictly less than textureMaxBlockSize is treated as absent.
// A texture whose word at offset 28 differs from drawFrameNm has the eight
// words at offsets 32..60 cleared, then stores the frame. The record pointer
// is written at texture+32+list*4.
// A draw record is linked at its word 12. Bit 1 of its word 4 is set on that
// record when clear, and on the new record when already set. With no draw
// record, a saved texture head whose fifth-argument bit 0 is clear is walked
// while bit 1 of each word 4 is set. Otherwise the link is the node stored
// at drawHead[(writeBuffer << 3) + list].
extern "C" {
extern unsigned char drawRecords[];
extern unsigned char drawHead[];
extern int writeBuffer;
extern int nmDrawUsed;
extern int textureMaxBlockSize;
extern unsigned drawFrameNm;
}

#if defined(NORRATH_HOST_DRAW_ADD)
extern unsigned char* norrath_draw_add_arena;
#endif

namespace {
__attribute__((always_inline)) inline unsigned load_word(const void* address) {
    const auto* bytes = static_cast<const unsigned char*>(address);
    return bytes[0] | (unsigned(bytes[1]) << 8) | (unsigned(bytes[2]) << 16) |
           (unsigned(bytes[3]) << 24);
}
__attribute__((always_inline)) inline void store_word(void* address, unsigned value) {
    auto* bytes = static_cast<unsigned char*>(address);
    bytes[0] = static_cast<unsigned char>(value);
    bytes[1] = static_cast<unsigned char>(value >> 8);
    bytes[2] = static_cast<unsigned char>(value >> 16);
    bytes[3] = static_cast<unsigned char>(value >> 24);
}
__attribute__((always_inline)) inline unsigned pointer_bits(const void* pointer) {
    if (pointer == nullptr) return 0u;
#if defined(NORRATH_HOST_DRAW_ADD)
    return static_cast<unsigned>(static_cast<const unsigned char*>(pointer) - norrath_draw_add_arena);
#else
    return static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(pointer));
#endif
}
__attribute__((always_inline)) inline void* pointer_from(unsigned bits) {
    if (bits == 0u) return nullptr;
#if defined(NORRATH_HOST_DRAW_ADD)
    return norrath_draw_add_arena + bits;
#else
    return reinterpret_cast<void*>(static_cast<__UINTPTR_TYPE__>(bits));
#endif
}
__attribute__((always_inline)) inline void link_after(void* node, void* record) {
    const unsigned next = load_word(static_cast<unsigned char*>(node) + 12);
    store_word(static_cast<unsigned char*>(record) + 12, next);
    store_word(static_cast<unsigned char*>(node) + 12, pointer_bits(record));
}
}

extern "C" void* drawAddToList__FPviP8_textureP11_drawRecordi(void* packet, int list,
                                                             void* texture, void* draw_record,
                                                             int flags) {
    if (nmDrawUsed >= 2048) return nullptr;
    const unsigned slot = ((static_cast<unsigned>(writeBuffer) << 11) +
                           static_cast<unsigned>(nmDrawUsed))
                          << 4;
    void* record = drawRecords + slot;
    nmDrawUsed += 1;
    if (texture != nullptr) {
        const unsigned half = static_cast<unsigned char*>(texture)[4] |
                              (unsigned(static_cast<unsigned char*>(texture)[5]) << 8);
        if (static_cast<int>(half) >= textureMaxBlockSize) texture = nullptr;
    }
    store_word(static_cast<unsigned char*>(record) + 4, static_cast<unsigned>(flags));
    store_word(static_cast<unsigned char*>(record) + 8, pointer_bits(texture));
    store_word(record, pointer_bits(packet) & 0x0fffffffu);

    void* previous = nullptr;
    if (texture != nullptr) {
        if (load_word(static_cast<unsigned char*>(texture) + 28) != drawFrameNm) {
            for (int word = 7; word >= 0; --word) {
                store_word(static_cast<unsigned char*>(texture) + 32 + word * 4, 0u);
            }
            store_word(static_cast<unsigned char*>(texture) + 28, drawFrameNm);
        } else {
            previous = pointer_from(load_word(static_cast<unsigned char*>(texture) + 32 + list * 4));
        }
        store_word(static_cast<unsigned char*>(texture) + 32 + list * 4, pointer_bits(record));
    }

    if (draw_record != nullptr) {
        unsigned marker = load_word(static_cast<unsigned char*>(draw_record) + 4);
        if ((marker & 2u) == 0u) {
            store_word(static_cast<unsigned char*>(draw_record) + 4, marker | 2u);
        } else {
            const unsigned record_flags = load_word(static_cast<unsigned char*>(record) + 4);
            store_word(static_cast<unsigned char*>(record) + 4, record_flags | 2u);
        }
        link_after(draw_record, record);
        return record;
    }
    if (previous != nullptr && (static_cast<unsigned>(flags) & 1u) == 0u) {
        void* node = previous;
        while ((load_word(static_cast<unsigned char*>(node) + 4) & 2u) != 0u) {
            node = pointer_from(load_word(static_cast<unsigned char*>(node) + 12));
        }
        link_after(node, record);
        return record;
    }
    const unsigned head_index = ((static_cast<unsigned>(writeBuffer) << 3) +
                                 static_cast<unsigned>(list))
                                << 2;
    void* head = pointer_from(load_word(drawHead + head_index));
    link_after(head, record);
    return record;
}
