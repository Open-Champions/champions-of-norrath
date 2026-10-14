// Observed: Blit__8VIRasteriUiRC8VIVect2IN2316VIAlphaBlendModef at 0x0110b4b0 is 2648 bytes in the pinned SLUS-20565 image. Arguments (EABI):
// raster, id (int), flags (unsigned), pos, src, size (three pointers to int pairs), blend (the alpha-blend mode value), alpha (float in F12).
// All offsets are bytes. Steps, in order:
//   surface = first word of the 12-byte slot [id] of the table pointed to by the word at raster + 19388; material = the word at raster + 17848.
//   If the word at raster + 19532 is not 1: UploadRasterMicro__8VIRasterPP10_sceDmaTag(raster, raster + 19492). The tag slot is raster + 19492.
//   material word +48 = id; material word +60 = blend; SetDither__10VIMateriali(material, 0) if flags & 4, else (material, 1).
//   If id differs from the word at raster + 17852 or blend differs from the word at raster + 17856: raster +17856 = blend, material word +32 = 1,
//   raster +17852 = id.
//   AddMaterial__8VIRasteriPP10_sceDmaTag(raster, the word at raster + 17848, tag slot) < 0 -> return -1;
//   BlitBegin__8VIRasterPP10_sceDmaTag(raster, tag slot) < 0 -> return -1.
//   The packet is at the pointer in the word at raster + 52: four 64-byte vertex records. With (w, h) = size and (sx, sy) = src, the vertices are at
//   pos, pos + (w, 0), pos + (w, h) and pos + (0, h); each gets float words: +0 = (float(x) - F[17732] + 0.5) * F[17740],
//   +4 = (float(y) - F[17736] + 0.5) * (-F[17744]), +8 = 0, +12 = 1.0 (F = the raster's float words at those byte offsets);
//   texture words +16 and +20 (u, v, with +24 = 1.0 and +28 = 0), colour words +32..+44 = 0 and +48.. = 1.0, 1.0, 1.0, alpha.
//   u = float(texture x) / float(surface word +8) and v = float(texture y) / float(surface word +12), where the divisions are the reciprocal
//   1.0f / float(surface word) computed once and multiplied. The texture corners per flags & 3:
//     0: (sx, sy) (sx+w, sy) (sx+w, sy+h) (sx, sy+h);   1: (sx+w, sy) (sx, sy) (sx, sy+h) (sx+w, sy+h);
//     2: (sx, sy+h) (sx+w, sy+h) (sx+w, sy) (sx, sy);   3: (sx+w, sy+h) (sx, sy+h) (sx, sy) (sx+w, sy).
//   raster word +48 is incremented four times (once per vertex), then raster +52 grows by 256; BlitEnd__8VIRasterPP10_sceDmaTag(raster, tag slot)
//   < 0 -> return -1 (material word +48 is not reset). Otherwise material word +48 = -1 is stored and
//   FlushDMABufferIfFull__8VIRaster(raster) is called; it returns 0.
// Inferred: this builds a textured quad from a surface rectangle through the raster's DMA packet buffer: flags & 3 mirror the texture
// horizontally (1), vertically (2) or both (3); flags & 4 turns dithering off; the raster floats at 17732..17744 are the viewport origin and
// scale. The material at raster + 17848 is a scratch material whose layer 0 texture is set to the surface for the blit; its +32 word flags a
// state change.
// Unknown: the roles of the 16-byte groups beyond position and texture coordinates (the pattern looks like GIF tag data), the meaning of
// the cached id and blend words, whether the material words are in a layer record, and every callee's behaviour (imports). Flag bits other
// than 0..2 are not looked at. Nothing here ran on the EE.
#ifdef NORRATH_HOST_ARENA
extern "C" unsigned char* norrath_host_pointer(unsigned address);
#define NORRATH_WORD_TO_POINTER(word) norrath_host_pointer(word)
#else
#define NORRATH_WORD_TO_POINTER(word) reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(word))
#endif
extern "C" {
void UploadRasterMicro__8VIRasterPP10_sceDmaTag(unsigned char* raster, unsigned* tagSlot);
void SetDither__10VIMateriali(unsigned char* material, int dither);
int AddMaterial__8VIRasteriPP10_sceDmaTag(unsigned char* raster, int material, unsigned* tagSlot);
int BlitBegin__8VIRasterPP10_sceDmaTag(unsigned char* raster, unsigned* tagSlot);
int BlitEnd__8VIRasterPP10_sceDmaTag(unsigned char* raster, unsigned* tagSlot);
void FlushDMABufferIfFull__8VIRaster(unsigned char* raster);

int Blit__8VIRasteriUiRC8VIVect2IN2316VIAlphaBlendModef(unsigned char* raster, int id, unsigned flags, const int* pos, const int* src, const int* size,
                                                        int blend, float alpha) {
    unsigned char* table = NORRATH_WORD_TO_POINTER(*reinterpret_cast<unsigned*>(raster + 19388));
    unsigned char* surface = NORRATH_WORD_TO_POINTER(*reinterpret_cast<unsigned*>(table + id * 12));
    unsigned materialWord = *reinterpret_cast<unsigned*>(raster + 17848);
    unsigned* tagSlot = reinterpret_cast<unsigned*>(raster + 19492);
    if (*reinterpret_cast<int*>(raster + 19532) != 1) UploadRasterMicro__8VIRasterPP10_sceDmaTag(raster, tagSlot);
    unsigned char* material = NORRATH_WORD_TO_POINTER(materialWord);
    *reinterpret_cast<int*>(material + 48) = id;
    *reinterpret_cast<int*>(material + 60) = blend;
    SetDither__10VIMateriali(material, (flags & 4) != 0 ? 0 : 1);
    if (id != *reinterpret_cast<int*>(raster + 17852) || blend != *reinterpret_cast<int*>(raster + 17856)) {
        *reinterpret_cast<int*>(raster + 17856) = blend;
        *reinterpret_cast<int*>(material + 32) = 1;
        *reinterpret_cast<int*>(raster + 17852) = id;
    }
    if (AddMaterial__8VIRasteriPP10_sceDmaTag(raster, static_cast<int>(*reinterpret_cast<unsigned*>(raster + 17848)), tagSlot) < 0) return -1;
    if (BlitBegin__8VIRasterPP10_sceDmaTag(raster, tagSlot) < 0) return -1;

    const float one = 1.0f;
    const float half = 0.5f;
    float uScale = one / static_cast<float>(*reinterpret_cast<int*>(surface + 8));
    float vScale = one / static_cast<float>(*reinterpret_cast<int*>(surface + 12));
    float originX = *reinterpret_cast<float*>(raster + 17732);
    float originY = *reinterpret_cast<float*>(raster + 17736);
    float scaleX = *reinterpret_cast<float*>(raster + 17740);
    float scaleY = *reinterpret_cast<float*>(raster + 17744);
    int width = size[0];
    int height = size[1];
    int sx = src[0];
    int sy = src[1];
    int x0 = pos[0];
    int y0 = pos[1];
    int xs[4] = {x0, x0 + width, x0 + width, x0};
    int ys[4] = {y0, y0, y0 + height, y0 + height};
    int mode = static_cast<int>(flags & 3);
    int us[4];
    int vs[4];
    switch (mode) {
    case 0:
        us[0] = sx; us[1] = sx + width; us[2] = sx + width; us[3] = sx;
        vs[0] = sy; vs[1] = sy; vs[2] = sy + height; vs[3] = sy + height;
        break;
    case 1:
        us[0] = sx + width; us[1] = sx; us[2] = sx; us[3] = sx + width;
        vs[0] = sy; vs[1] = sy; vs[2] = sy + height; vs[3] = sy + height;
        break;
    case 2:
        us[0] = sx; us[1] = sx + width; us[2] = sx + width; us[3] = sx;
        vs[0] = sy + height; vs[1] = sy + height; vs[2] = sy; vs[3] = sy;
        break;
    default:
        us[0] = sx + width; us[1] = sx; us[2] = sx; us[3] = sx + width;
        vs[0] = sy + height; vs[1] = sy + height; vs[2] = sy; vs[3] = sy;
        break;
    }
    float* packet = reinterpret_cast<float*>(NORRATH_WORD_TO_POINTER(*reinterpret_cast<unsigned*>(raster + 52)));
    for (int k = 0; k < 4; ++k) {
        float* v = packet + 16 * k;
        float px = static_cast<float>(xs[k]);
        px = px - originX;
        px = px + half;
        px = px * scaleX;
        float py = static_cast<float>(ys[k]);
        py = py - originY;
        py = py + half;
        py = py * (-scaleY);
        v[0] = px;
        v[1] = py;
        v[2] = 0.0f;
        v[3] = one;
        v[4] = static_cast<float>(us[k]) * uScale;
        v[5] = static_cast<float>(vs[k]) * vScale;
        v[6] = one;
        v[7] = 0.0f;
        v[8] = 0.0f;
        v[9] = 0.0f;
        v[10] = 0.0f;
        v[11] = 0.0f;
        v[12] = one;
        v[13] = one;
        v[14] = one;
        v[15] = alpha;
        *reinterpret_cast<int*>(raster + 48) += 1;
    }
    *reinterpret_cast<unsigned*>(raster + 52) += 256;
    if (BlitEnd__8VIRasterPP10_sceDmaTag(raster, tagSlot) < 0) return -1;
    *reinterpret_cast<int*>(material + 48) = -1;
    FlushDMABufferIfFull__8VIRaster(raster);
    return 0;
}
}
