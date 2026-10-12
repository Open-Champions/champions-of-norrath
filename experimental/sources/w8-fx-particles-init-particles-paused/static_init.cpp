// Observed _GLOBAL_$I$particlesPaused at 0x00187778, 32 bytes, calling the file-local initializer at
// 0x00187440 (824 bytes) with (1, 0xffff). With those arguments the initializer does, in order:
//  1. Sets 16 consecutive 16-byte entries at globalSystem (0x0037f730) and 16 at globalSystemSave (0x0037f830,
//     256 bytes, the next object) to the words 0, 0, 1, 257. This is the same entry layout P_Clear__Fv resets
//     (the executable builds it as four groups of four entries at a 64-byte stride).
//  2. Constructs three 16-byte ResourceMap objects with __11ResourceMapPcT1(object, "engine.lmp", name):
//     particletex (0x006ec800) with "particletex.tex", trailTexture (0x006ec810) with "trail.tex" and blanktex
//     (0x006ec820) with "blank.tex".
//  3. Constructs invisible (0x006f5a80, defined by the particles package) with the ParticleDef constructor:
//     every integer and float argument zero except the first of the three trailing integers, which is 2.
//  4. Sets the first 256 bytes of each of the 16 PS records (0x006f5ad0, stride 312) to the same entries as in step 1.
//  The executable also runs three loops that store nothing (32, 4 and 20 by 64 iterations over arrays of
//  empty-constructor elements). They have no effect and are not reproduced. Their bounds are the only evidence of
//  those arrays' element counts.
// globalSystemSave, particletex, trailTexture and blanktex have no definition elsewhere in the draft, so they are
// defined here with external linkage; the executable's symbol table lists them as local or data objects. What the
// objects are used for is not established here. Fresh compilation is not an original-byte match.
extern "C" {
extern unsigned char globalSystem[];
extern unsigned char PS[];
extern unsigned char invisible[];
void __11ResourceMapPcT1(void* map, const char* group, const char* name);
void __11ParticleDefiiiiiiiifffiiiPFPv_v(void* self, int a, int b, int c, int d, int e, int f, int g, int h,
                                         float x, float y, float z, int i, int j, int k, void (*callback)(void*));

unsigned char globalSystemSave[256];
unsigned char particletex[16];
unsigned char trailTexture[16];
unsigned char blanktex[16];
}

static void clear_entries(unsigned char* base) {
    unsigned* words = reinterpret_cast<unsigned*>(base);
    for (unsigned entry = 0u; entry < 16u; ++entry) {
        words[entry * 4u] = 0u;
        words[entry * 4u + 1u] = 0u;
        words[entry * 4u + 2u] = 1u;
        words[entry * 4u + 3u] = 257u;
    }
}

static void __static_initialization_and_destruction_0(int initialize, int priority) {
    if (priority != 0xffff) return;
    if (initialize != 1) return;
    clear_entries(globalSystem);
    clear_entries(globalSystemSave);
    __11ResourceMapPcT1(particletex, "engine.lmp", "particletex.tex");
    __11ResourceMapPcT1(trailTexture, "engine.lmp", "trail.tex");
    __11ResourceMapPcT1(blanktex, "engine.lmp", "blank.tex");
    __11ParticleDefiiiiiiiifffiiiPFPv_v(invisible, 0, 0, 0, 0, 0, 0, 0, 0, 0.0f, 0.0f, 0.0f, 2, 0, 0, 0);
    for (unsigned record = 0u; record < 16u; ++record) clear_entries(PS + record * 312u);
}

extern "C" void _GLOBAL_$I$particlesPaused() {
    __static_initialization_and_destruction_0(1, 0xffff);
}
