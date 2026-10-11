// P_Init__Fv at 0x00180038, 492 bytes, read from the pinned SLUS-20565 image.
// Fresh compilation is not an original-byte match. No particle has been drawn
// on the EE.
//
// Sixty-four pairs are stored at wigglyTab. Entry i uses angle i << 10.
// The stored words are the cvt.w.s results of isin and icos multiplied by the
// bits 0x40a00000. On the reviewed EE compiler a float-to-int cast emits
// cvt.w.s. The host cast truncates; the host fixtures are exact integers.
// wigglyTab is 544 bytes at 0x006ffe60. Only the first 512 bytes are written.
//
// nmSingle, nmLines, and the P_FrameNum halfword are stored as zero.
// descListMark, 128 bytes at 0x00780280, is filled with 0xffffffff.
// allocDescriptor__FP11ParticleDef receives invisible.
//
// block at 0x00700080 is 128 pages of 4096 bytes, 256 records of 16. When the
// record index modulo 32 is 0, the record is two zero words, then
// value | 0x04000000 and value | 0x6d3c0000. value alternates 576, 128,
// starting at 576. When the index modulo 32 is 31, the first word is
// 0x14000018 and the other three words are zero. Every other record stores
// a zero halfword at offset 14 and leaves the other bytes alone.
//
// Then P_Clear__Fv, initTrails__Fv, initParticleSystems__Fv,
// engineAddTask(P_Update__Fv, 15, "updateParticles", 2), and
// engineAddTask(drawParticlesTask__Fv, 65, "drawParticles", 1). The names are
// the bytes at 0x00463e60 and 0x00463e70.
extern "C" {
float isin__Fi(int angle);
float icos__Fi(int angle);
void allocDescriptor__FP11ParticleDef(unsigned char* descriptor);
void P_Clear__Fv();
void initTrails__Fv();
void initParticleSystems__Fv();
void P_Update__Fv();
void drawParticlesTask__Fv();
void engineAddTask__FPFv_viPci(void (*task)(), int order, char* name, int flag);

extern unsigned char wigglyTab[];
extern unsigned char descListMark[];
extern unsigned char block[];
extern unsigned char invisible[];
extern unsigned nmSingle;
extern unsigned nmLines;
extern unsigned short P_FrameNum;

static char norrath_update_particles_name[] = "updateParticles";
static char norrath_draw_particles_name[] = "drawParticles";

static void norrath_store_u32(void* slot, unsigned value) {
    auto* dest = static_cast<unsigned char*>(slot);
    dest[0] = static_cast<unsigned char>(value);
    dest[1] = static_cast<unsigned char>(value >> 8);
    dest[2] = static_cast<unsigned char>(value >> 16);
    dest[3] = static_cast<unsigned char>(value >> 24);
}

void P_Init__Fv() {
    for (int index = 0; index < 64; ++index) {
        const int angle = index << 10;
        const int sine = static_cast<int>(isin__Fi(angle) * 5.0f);
        const int cosine = static_cast<int>(icos__Fi(angle) * 5.0f);
        norrath_store_u32(wigglyTab + index * 8, static_cast<unsigned>(sine));
        norrath_store_u32(wigglyTab + index * 8 + 4, static_cast<unsigned>(cosine));
    }
    nmSingle = 0;
    nmLines = 0;
    P_FrameNum = 0;
    for (int word = 0; word < 32; ++word) norrath_store_u32(descListMark + word * 4, 0xffffffffu);
    allocDescriptor__FP11ParticleDef(invisible);

    for (int page_index = 0; page_index < 128; ++page_index) {
        unsigned char* page = block + page_index * 4096;
        unsigned toggle = 0;
        for (int record = 0; record < 256; ++record) {
            unsigned char* bytes = page + record * 16;
            if ((record & 31) == 0) {
                const unsigned value = toggle == 0 ? 576u : 128u;
                toggle ^= 1u;
                norrath_store_u32(bytes + 0, 0);
                norrath_store_u32(bytes + 4, 0);
                norrath_store_u32(bytes + 8, value | 0x04000000u);
                norrath_store_u32(bytes + 12, value | 0x6d3c0000u);
            } else if ((record & 31) == 31) {
                norrath_store_u32(bytes + 0, 0x14000018u);
                norrath_store_u32(bytes + 4, 0);
                norrath_store_u32(bytes + 8, 0);
                norrath_store_u32(bytes + 12, 0);
            } else {
                bytes[14] = 0;
                bytes[15] = 0;
            }
        }
    }

    P_Clear__Fv();
    initTrails__Fv();
    initParticleSystems__Fv();
    engineAddTask__FPFv_viPci(P_Update__Fv, 15, norrath_update_particles_name, 2);
    engineAddTask__FPFv_viPci(drawParticlesTask__Fv, 65, norrath_draw_particles_name, 1);
}
}
