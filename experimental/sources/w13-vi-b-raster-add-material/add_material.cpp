// Observed: AddMaterial__8VIRasteriPP10_sceDmaTag at 0x0110dd08, 768 bytes. The int argument
// is used as the address of a material record M (as in GetActorTransform__7VISceneiR10VIMatrix44);
// the last argument is the address of the DMA write pointer. R is the raster. In order:
//  1. If R word +17804 is nonzero return 0.
//  2. For each of the M word +8 passes (144-byte records starting at M + 48; the count is
//     re-read after each call): if the pass's first word is not -1, call
//     Cache__13VIEESurfCacheR9VISurfaceR10VIMaterialR14VIMaterialPassR8VIRasterPP10_sceDmaTag
//     (&SurfCache, surface, M, pass, R, pointer slot, 0), where surface is the first word of
//     entry (pass word) of the 12-byte table at R word +19388; a negative result returns -1.
//  3. SetGSDitherMatrix__8VIRasteriPP10_sceDmaTag(R, (M word +12 & 0x10) != 0, slot).
//  4. Material cache slot in M word +36 (the pool is the object at R + 19452; its item array
//     is the pointer at pool + 8, items are 24 bytes: +0 packet word, +4 size, +8 owner, +12
//     frame stamp). If M word +32 is nonzero or bit 2 of M word +12 is set or M word +36 is -1:
//     release the current slot if any (item owner = -1, M word +36 = -1), take
//     Begin__Ct6VIPool1Z19VIRasterMaterialLRU(pool) as a victim, clear its previous owner's
//     M word +36 and its owner word, MoveToLast__t6VIPool1Z19VIRasterMaterialLRUi(pool, victim)
//     to get the new slot, store M there as owner, set item +4 to
//     ConstructDMAPacket__10VIMaterialR8VIRasterP7VIQWord(M, R, item word +0), set item +12 to R
//     word +19500 and add 1 to R word +19480. Otherwise move the current slot to the last
//     position with MoveToLast, store the result in M word +36 and, if its item stamp differs from
//     R word +19500, set the stamp and add 1 to R word +19480.
//  5. Write a reference DMA tag at the pointer: byte 3 = 0x30, word 1 = the item's word +0,
//     16-bit field 0 = the item's word +4; advance the pointer by 16. If the word at scratchpad
//     0x70000224 differs from M word +16, copy that word (four bytes) from M + 16 into the
//     scratchpad and call UploadPerMaterial__8VIRasterPP10_sceDmaTag(R, slot).
//  6. Store M in R word +17824 and return 0.
// Inferred: a material cache with LRU recycling of per-material DMA packets; the packet is
// referenced with a DMA "ref" tag so the GS-side material state is only re-uploaded when the
// per-material color word changes. M word +32 and bit 2 of M word +12 appear to force a
// rebuild every time.
// Unknown: the member names, the callee return types beyond what is tested, the type of the
// first item word (a packet address is assumed) and what the word at M + 16 is.
#if defined(NORRATH_HOST_CONTRACT)
// On the 64-bit host a pointer field is modelled as a 32-bit offset into an arena, and the
// int address argument is an arena offset as well.
extern "C" unsigned char* norrath_host_address(int value);
extern "C" unsigned char* norrath_host_load_pointer(const void* field);
extern "C" void norrath_host_store_pointer(void* field, unsigned char* value);
extern "C" volatile unsigned char* norrath_host_scratch_byte(unsigned address);
#define NORRATH_ADDRESS(value) norrath_host_address(value)
#define NORRATH_LOAD_POINTER(field) norrath_host_load_pointer(field)
#define NORRATH_STORE_POINTER(field, value) norrath_host_store_pointer(field, value)
#define NORRATH_SCRATCH_BYTE(address) norrath_host_scratch_byte(address)
#else
#define NORRATH_ADDRESS(value) reinterpret_cast<unsigned char*>(static_cast<long>(value))
#define NORRATH_LOAD_POINTER(field) (*reinterpret_cast<unsigned char* const*>(field))
#define NORRATH_STORE_POINTER(field, value) (*reinterpret_cast<unsigned char**>(field) = (value))
#define NORRATH_SCRATCH_BYTE(address) reinterpret_cast<volatile unsigned char*>(static_cast<unsigned long>(address))
#endif

extern "C" {
extern unsigned char SurfCache[];
int Cache__13VIEESurfCacheR9VISurfaceR10VIMaterialR14VIMaterialPassR8VIRasterPP10_sceDmaTagi(void* cache, void* surface, void* material, void* pass, void* raster, void* slot, int flag);
int SetGSDitherMatrix__8VIRasteriPP10_sceDmaTag(void* raster, int enable, void* slot);
int Begin__Ct6VIPool1Z19VIRasterMaterialLRU(void* pool);
int MoveToLast__t6VIPool1Z19VIRasterMaterialLRUi(void* pool, int index);
int ConstructDMAPacket__10VIMaterialR8VIRasterP7VIQWord(void* material, void* raster, unsigned int packet);
int UploadPerMaterial__8VIRasterPP10_sceDmaTag(void* raster, void* slot);

int AddMaterial__8VIRasteriPP10_sceDmaTag(void* raster, int materialAddress, void* slot) {
    unsigned char* r = static_cast<unsigned char*>(raster);
    unsigned char* m = NORRATH_ADDRESS(materialAddress);
    unsigned char* pool = r + 19452;
    if (*reinterpret_cast<int*>(r + 17804) != 0) return 0;
    int passes = *reinterpret_cast<int*>(m + 8);
    if (passes > 0) {
        int k = 0;
        unsigned char* pass = m + 48;
        do {
            int surfaceIndex = *reinterpret_cast<int*>(pass);
            if (surfaceIndex != -1) {
                unsigned char* surface = NORRATH_LOAD_POINTER(NORRATH_LOAD_POINTER(r + 19388) + surfaceIndex * 12);
                if (Cache__13VIEESurfCacheR9VISurfaceR10VIMaterialR14VIMaterialPassR8VIRasterPP10_sceDmaTagi(SurfCache, surface, m, pass, raster, slot, 0) < 0) return -1;
                passes = *reinterpret_cast<int*>(m + 8);
            }
            ++k;
            pass += 144;
        } while (k < passes);
    }
    if ((*reinterpret_cast<int*>(m + 12) & 0x10) != 0) SetGSDitherMatrix__8VIRasteriPP10_sceDmaTag(raster, 1, slot);
    else SetGSDitherMatrix__8VIRasteriPP10_sceDmaTag(raster, 0, slot);
    int current = *reinterpret_cast<int*>(m + 36);
    int index;
    if (*reinterpret_cast<int*>(m + 32) != 0 || (*reinterpret_cast<int*>(m + 12) & 4) != 0 || current == -1) {
        if (current != -1) {
            *reinterpret_cast<int*>(NORRATH_LOAD_POINTER(pool + 8) + current * 24 + 8) = -1;
            *reinterpret_cast<int*>(m + 36) = -1;
        }
        int victim = Begin__Ct6VIPool1Z19VIRasterMaterialLRU(pool);
        unsigned char* victimItem = NORRATH_LOAD_POINTER(pool + 8) + victim * 24;
        int previousOwner = *reinterpret_cast<int*>(victimItem + 8);
        if (previousOwner != -1) {
            *reinterpret_cast<int*>(NORRATH_ADDRESS(previousOwner) + 36) = -1;
            *reinterpret_cast<int*>(victimItem + 8) = -1;
        }
        int slotIndex = MoveToLast__t6VIPool1Z19VIRasterMaterialLRUi(pool, victim);
        *reinterpret_cast<int*>(m + 36) = slotIndex;
        *reinterpret_cast<int*>(NORRATH_LOAD_POINTER(pool + 8) + slotIndex * 24 + 8) = materialAddress;
        unsigned char* item = NORRATH_LOAD_POINTER(pool + 8) + *reinterpret_cast<int*>(m + 36) * 24;
        int size = ConstructDMAPacket__10VIMaterialR8VIRasterP7VIQWord(m, raster, *reinterpret_cast<unsigned int*>(item));
        unsigned char* items = NORRATH_LOAD_POINTER(pool + 8);
        *reinterpret_cast<int*>(items + *reinterpret_cast<int*>(m + 36) * 24 + 4) = size;
        *reinterpret_cast<int*>(items + *reinterpret_cast<int*>(m + 36) * 24 + 12) = *reinterpret_cast<int*>(r + 19500);
        *reinterpret_cast<int*>(r + 19480) += 1;
        index = *reinterpret_cast<int*>(m + 36);
    } else {
        int slotIndex = MoveToLast__t6VIPool1Z19VIRasterMaterialLRUi(pool, current);
        unsigned char* items = NORRATH_LOAD_POINTER(pool + 8);
        *reinterpret_cast<int*>(m + 36) = slotIndex;
        int stamp = *reinterpret_cast<int*>(r + 19500);
        unsigned char* item = items + slotIndex * 24;
        if (*reinterpret_cast<int*>(item + 12) != stamp) {
            *reinterpret_cast<int*>(item + 12) = stamp;
            *reinterpret_cast<int*>(r + 19480) += 1;
        }
        index = *reinterpret_cast<int*>(m + 36);
    }
    unsigned char* item = NORRATH_LOAD_POINTER(pool + 8) + index * 24;
    unsigned int packet = *reinterpret_cast<unsigned int*>(item);
    int size = *reinterpret_cast<int*>(item + 4);
    unsigned char** tagSlot = static_cast<unsigned char**>(slot);
    NORRATH_LOAD_POINTER(tagSlot)[3] = 0x30;
    unsigned int scratchWord = *reinterpret_cast<volatile unsigned int*>(NORRATH_SCRATCH_BYTE(0x70000224));
    reinterpret_cast<unsigned int*>(NORRATH_LOAD_POINTER(tagSlot))[1] = packet;
    unsigned int color = *reinterpret_cast<unsigned int*>(m + 16);
    unsigned char* tag = NORRATH_LOAD_POINTER(tagSlot);
    *reinterpret_cast<unsigned short*>(tag) = static_cast<unsigned short>(size);
    NORRATH_STORE_POINTER(tagSlot, tag + 16);
    if (scratchWord != color) {
        unsigned char* colorBytes = m + 16;
        volatile unsigned char* destination = NORRATH_SCRATCH_BYTE(0x70000224);
        destination[0] = colorBytes[0];
        destination[1] = colorBytes[1];
        destination[2] = colorBytes[2];
        destination[3] = colorBytes[3];
        UploadPerMaterial__8VIRasterPP10_sceDmaTag(raster, slot);
    }
    *reinterpret_cast<int*>(r + 17824) = materialAddress;
    return 0;
}
}
