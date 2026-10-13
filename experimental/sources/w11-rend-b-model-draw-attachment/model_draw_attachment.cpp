// Observed modelDrawAttachment__FP12_modelHeaderP8_textureiT0P11_drawRecordUlii at
// 0x0016c060, extent 468. Arguments in order: model header, texture, an int forwarded to
// drawAddToList, a second model header, a drawRecord pointer, a 64-bit flag word, an int
// forwarded to modelSetupAttachment, and an int level index. Returns nothing. Nothing is
// done when the first header is null.
//
// The display-list cursor is glistp (0x004844c8, the word at gp-13608). A local two-word
// packet {cursor, last tag} starts at that value, modelSetupAttachment(packet, header,
// second header, int) runs, and then, when the half at header+16 has bit 0x20 and nmHalos
// is above zero, modelDrawHalo(packet, header, 1, halos, nmHalos) runs.
// The level index is clamped to [0, byte(header+38) - 1]. The level's table at
// header+40 + level * (4 * byte(header+18) + 4) holds byte(header+18) + 1 addresses.
// Flag bit byte(header+18) is forced clear; each maximal run of segments whose flag bit
// is set is emitted as a REF DMA tag (0x30000000, start address in the upper word,
// quad count in the low word) at the next clear bit, exactly as modelDraw does. Each REF
// tag address is stored in the packet's second word. The count is the signed difference
// of the two addresses shifted right by 4, so it sign-extends into the upper word.
// The tag the second word points at is then closed: if its id (bits 28 to 30) is CNT it
// becomes id 6 in place, otherwise a tag 0x60000000 with a zero second word is appended.
// glistp is advanced to the cursor and drawAddToList(old glistp, int, texture,
// drawRecord, 0) is called.
// Observed: the packet's second word is read at the close even when no REF tag was made;
// it then holds what modelSetupAttachment stored there (it stores to it early).
// Unknown: the rest of what modelSetupAttachment writes and what the model values mean.
struct ModelDrawAttachmentPacket {
    unsigned char* cursor;
    unsigned char* last;
};

extern "C" {
extern unsigned char* glistp;
extern int nmHalos;
extern const void* halos;

void modelSetupAttachment__FP6PacketP12_modelHeaderT1i(ModelDrawAttachmentPacket*, const unsigned char*,
                                                       const unsigned char*, int);
void modelDrawHalo__FP6PacketP12_modelHeadercP7HaloDefi(ModelDrawAttachmentPacket*, const unsigned char*, signed char,
                                                        const void*, int);
void drawAddToList__FPviP8_textureP11_drawRecordi(void*, int, void*, void*, int);

void modelDrawAttachment__FP12_modelHeaderP8_textureiT0P11_drawRecordUlii(
    const unsigned char* header, void* texture, int value, const unsigned char* header2, void* record,
    unsigned long long flags, int setupArg, int level) {
    ModelDrawAttachmentPacket packet;
    packet.cursor = glistp;
    if (header == nullptr) {
        return;
    }
    modelSetupAttachment__FP6PacketP12_modelHeaderT1i(&packet, header, header2, setupArg);
    if ((*reinterpret_cast<const unsigned short*>(header + 16) & 0x20) != 0 && nmHalos > 0) {
        modelDrawHalo__FP6PacketP12_modelHeadercP7HaloDefi(&packet, header, 1, halos, nmHalos);
    }

    const int segments = static_cast<signed char>(header[18]);
    int lod = level < 0 ? 0 : level;
    const int lodLimit = static_cast<signed char>(header[38]) - 1;
    if (lod > lodLimit) lod = lodLimit;
    const unsigned char* at = header + 40 + lod * (segments * 4 + 4);
    long long bits = static_cast<long long>(flags & ~(1ull << (segments & 63)));
    if (segments >= 0) {
        unsigned start = *reinterpret_cast<const unsigned*>(at);
        for (int i = 0; i <= segments; ++i, at += 4) {
            const bool clear = (static_cast<int>(bits) & 1) == 0;
            bits >>= 1;
            if (clear) {
                const unsigned end = *reinterpret_cast<const unsigned*>(at);
                if (start != end) {
                    const unsigned long long tag =
                        (static_cast<unsigned long long>(start) << 32) |
                        static_cast<unsigned long long>(static_cast<long long>(static_cast<int>(end - start) >> 4)) |
                        0x30000000ull;
                    unsigned char* where = packet.cursor;
                    packet.cursor = where + 16;
                    *reinterpret_cast<unsigned long long*>(where) = tag;
                    *reinterpret_cast<unsigned long long*>(where + 8) = 0;
                    packet.last = where;
                }
                start = *reinterpret_cast<const unsigned*>(at + 4);
            }
        }
    }

    unsigned long long* lastTag = reinterpret_cast<unsigned long long*>(packet.last);
    const unsigned long long tag = *lastTag;
    if (((tag >> 28) & 7u) == 1u) {
        *lastTag = (tag & 0xffffffff8fffffffull) | 0x60000000ull;
    } else {
        packet.last = nullptr;
        unsigned char* where = packet.cursor;
        packet.cursor = where + 16;
        *reinterpret_cast<unsigned long long*>(where) = 0x60000000ull;
        *reinterpret_cast<unsigned long long*>(where + 8) = 0;
    }
    unsigned char* list = glistp;
    glistp = packet.cursor;
    drawAddToList__FPviP8_textureP11_drawRecordi(list, value, texture, record, 0);
}
}
