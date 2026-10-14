// Observed preCacheArmor__6Player at 0x00195cb8, 1076 bytes (Player pointer,
// returns an int). Pointer fields are 32-bit words on the target. `idx` is
// self[960] and rec = player + idx * 23984. Two output words (model, texture)
// start at zero for the whole call and are only overwritten where stated, so
// they carry over from one slot to the next. For slot = 0..15, item = word
// rec[21896 + 4*slot]; null items are skipped.
//  - Skip slot 3 when the pointer at rec[21960 + 4*sel] (sel = signed byte
//    rec[23572]) is nonzero and the signed byte at (*pointer)[30] is 4.
//  - entry = first word of item; kind = signed byte entry[28]. Skip kinds 8, 5,
//    6, 7; also skip kinds 2, 0, 3 when interfaceOn is zero and levelFlags has
//    bit 0x10000000.
//  - Slots 4, 10, 9, 11, 12, 13 use cache element n = 0, 1, 2, 3, 4, 5 of the
//    table at 0x007cb770 (local image symbol `cache`, no size; imported here as
//    norrath_player_armor_cache_view_7cb770) (element = base + 352*n + 88*idx), slot 3 uses
//    n = 9. With gamePlayersPresent nonzero and the word entry[48] nonzero: the
//    name is built by getPicName__4itemPciPQ24item11description(buffer,
//    rec[84], item), its last four characters are cut off (buffer[strlen-4] =
//    0) and cacheEquipment__FPcP10ArmorCachePP12_modelHeaderPP8_texture(name,
//    element, &model, &texture) is called. Slot 3 with entry[48] zero takes
//    model/texture from words 2 and 3 of the 16-byte record
//    _4item$globalItem[(short)entry[52]]; the other slots do nothing in that
//    case. With gamePlayersPresent zero, slot 3 sets both outputs to zero and the
//    others leave them.
//  - Every other slot uses getProxyMeshId__4itemPQ24item11description(item) as
//    an index into _4item$globalItem: model = word 2 of the record, then (a
//    second call) texture = word 3.
//  - After each handled slot the result becomes 1 when model or texture is zero.
// The result is 1 if any handled slot ended with a zero model or texture, else 0.
// What the slots stand for is not established.
extern "C" {
extern unsigned char player[];
extern signed char interfaceOn;
extern unsigned levelFlags;
extern int gamePlayersPresent;
extern unsigned char norrath_player_armor_cache_view_7cb770[];
extern unsigned char _4item$globalItem[];

void getPicName__4itemPciPQ24item11description(char* buffer, int cls, void* item);
unsigned strlen(const char* s);
void cacheEquipment__FPcP10ArmorCachePP12_modelHeaderPP8_texture(char* name, void* element, unsigned* model,
                                                                  unsigned* texture);
int getProxyMeshId__4itemPQ24item11description(void* item);
}

static inline unsigned char* Ptr(unsigned char* base, int off) {
    return (unsigned char*)(unsigned long)*(unsigned*)(base + off);
}

extern "C" int preCacheArmor__6Player(unsigned char* self) {
    unsigned model = 0, texture = 0;
    int result = 0;
    char name[128];
    unsigned char* rec = player + *(int*)(self + 960) * 23984;
    for (int slot = 0; slot < 16; ++slot) {
        unsigned char* item = Ptr(rec, 21896 + 4 * slot);
        if (item == 0) continue;
        unsigned char* guard = Ptr(rec, 21960 + 4 * (signed char)rec[23572]);
        if (guard != 0 && slot == 3 && (signed char)Ptr(guard, 0)[30] == 4) continue;
        unsigned char* entry = Ptr(item, 0);
        int kind = (signed char)entry[28];
        if (kind == 8 || kind == 5 || kind == 6 || kind == 7) continue;
        if (!interfaceOn && (levelFlags & 0x10000000u) && (kind == 2 || kind == 0 || kind == 3)) continue;

        int n = -1;
        switch (slot) {
        case 3: n = 9; break;
        case 4: n = 0; break;
        case 9: n = 2; break;
        case 10: n = 1; break;
        case 11: n = 3; break;
        case 12: n = 4; break;
        case 13: n = 5; break;
        }
        if (n >= 0) {
            int index = *(int*)(self + 960);
            unsigned char* element = norrath_player_armor_cache_view_7cb770 + n * 352 + index * 88;
            if (gamePlayersPresent == 0) {
                if (slot == 3) { texture = 0; model = 0; }
            } else if (*(unsigned*)(entry + 48) != 0) {
                getPicName__4itemPciPQ24item11description(name, *(int*)(rec + 84), item);
                name[strlen(name) - 4] = 0;
                cacheEquipment__FPcP10ArmorCachePP12_modelHeaderPP8_texture(name, element, &model, &texture);
            } else if (slot == 3) {
                unsigned char* record = _4item$globalItem + *(short*)(entry + 52) * 16;
                texture = *(unsigned*)(record + 12);
                model = *(unsigned*)(record + 8);
            }
        } else {
            model = *(unsigned*)(_4item$globalItem + getProxyMeshId__4itemPQ24item11description(item) * 16 + 8);
            texture = *(unsigned*)(_4item$globalItem + getProxyMeshId__4itemPQ24item11description(item) * 16 + 12);
        }
        if (model == 0 || texture == 0) result = 1;
    }
    return result;
}
