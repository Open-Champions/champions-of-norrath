// Observed: ConstructDMAPacket__10VIMaterialR8VIRasterP7VIQWord at 0x010cec78 is 380 bytes in the pinned SLUS-20565 image. Arguments:
// the material (this), the raster and an output quadword pointer. Let passes = 1, or the material's word at +8 when the raster's
// word at +17816 is nonzero. If the material's word at +12 has bit 2 set, it calls GetTime__8VIRaster(raster) and
// Process__10VIMateriall(material, that time).
// It then writes, starting at the output pointer q:
//   q[0..1] = 0; q[2] = 0x10000000; q[3] = ((passes * 17 + 1) << 16) | 0x6c0001b3
//   q[4] = passes; q[5..7] = 0
// and for each pass index i from 0 while i < passes (a do-while: the body is skipped entirely when passes <= 0), with pass record i at
// material + 48 + 144*i and output position q + 32 + 272*i bytes, it calls one builder with (material, i, raster, output position):
//   record word at +4 has bit 5 (0x20) set:  ConstructWirePass__10VIMaterialiR8VIRasterP7VIQWord
//   else record word at +0 != -1:            ConstructTextPass__10VIMaterialiR8VIRasterP7VIQWord
//   else:                                    ConstructSolidFillPass__10VIMaterialiR8VIRasterP7VIQWord
// Finally it stores zero in the material's word at +32 and returns 2 + 17 * passes (the quadword count written, assuming each
// builder fills 17 quadwords). The builders' return values are ignored.
// Inferred: builds a VIF/GIF packet for one material, the first two quadwords being a header (a FLUSHE-like code, an UNPACK sized
// to the pass count and a pass-count quadword) followed by 17 quadwords per pass; the word at +32 is a "needs rebuild" flag, and
// bit 2 of +12 means the material is animated. The header VIFcode 0x6c0001b3 with the count in bits 16-23 is an UNPACK.
// Unknown: the builders' bodies, whether the pass count is capped (the word at +8 can exceed what the output buffer holds), and the
// meaning of the pass flags other than 0x20. Nothing here ran on the EE.
extern "C" {
int GetTime__8VIRaster(void* raster);
void Process__10VIMateriall(void* material, int time);
void ConstructWirePass__10VIMaterialiR8VIRasterP7VIQWord(void* material, int pass, void* raster, void* out);
void ConstructTextPass__10VIMaterialiR8VIRasterP7VIQWord(void* material, int pass, void* raster, void* out);
void ConstructSolidFillPass__10VIMaterialiR8VIRasterP7VIQWord(void* material, int pass, void* raster, void* out);

int ConstructDMAPacket__10VIMaterialR8VIRasterP7VIQWord(unsigned char* material, unsigned char* raster, unsigned* out) {
    int passes = 1;
    if (*reinterpret_cast<int*>(raster + 17816) != 0) passes = *reinterpret_cast<int*>(material + 8);
    if ((*reinterpret_cast<unsigned*>(material + 12) & 4) != 0) {
        Process__10VIMateriall(material, GetTime__8VIRaster(raster));
    }
    out[2] = 0x10000000u;
    out[0] = 0;
    out[3] = (static_cast<unsigned>(passes * 17 + 1) << 16) | 0x6c0001b3u;
    out[1] = 0;
    unsigned* count = out + 4;
    count[0] = static_cast<unsigned>(passes);
    count[1] = 0;
    count[2] = 0;
    count[3] = 0;
    unsigned* cursor = out + 8;
    int quadwords = 2;
    int pass = 0;
    if (passes > 0) {
        unsigned char* record = material + 48;
        do {
            if ((*reinterpret_cast<unsigned*>(record + 4) & 0x20u) != 0) {
                ConstructWirePass__10VIMaterialiR8VIRasterP7VIQWord(material, pass, raster, cursor);
            } else if (*reinterpret_cast<int*>(record) != -1) {
                ConstructTextPass__10VIMaterialiR8VIRasterP7VIQWord(material, pass, raster, cursor);
            } else {
                ConstructSolidFillPass__10VIMaterialiR8VIRasterP7VIQWord(material, pass, raster, cursor);
            }
            ++pass;
            cursor += 68;
            quadwords += 17;
            record += 144;
        } while (pass < passes);
    }
    *reinterpret_cast<unsigned*>(material + 32) = 0;
    return quadwords;
}
}
