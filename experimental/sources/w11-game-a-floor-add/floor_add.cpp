// Observed floorAdd__FG6Point3sfii at 0x00191ef8, 956 bytes. Arguments: a point (passed by
// reference in this ABI, copied to a local), a 16-bit angle, a float scale, a kind and an
// "expand" flag. It appends one floor sprite to the 30 by 30 grid sprGrid of 256-unit cells.
//   * Nothing happens when the kind is 0 and g_BloodLevel is 0, and nothing happens for kinds 1 and 2.
//   * The cell is x = ((int)px + 3840) >> 8 (must be below 30 unsigned) and y = ((int)py + 3840) >> 8
//     (0 to 29). A missing cell block is taken from the head of freeList (nothing happens when the
//     list is empty); the list link is the word at block+752, which is cleared on taking.
//   * The block holds 24-byte records at +0 and a record count at +760. A block with more than 30
//     records is left alone. The record is: colour bytes of lightFloorSprite(point) at +0, +2, +4;
//     128 at +6; (int)(4px), (int)(4py), (int)(4(pz+3)) as shorts at +8, +10, +12; +14 is 1 when
//     the kind is below 3 (unsigned) else 0; +16 and +18 are (int)(icos(angle) * scale * 4) and
//     (int)(isin(angle) * scale * 4); +20 is 979 + 2 * kind. The block's count then grows by one,
//     the word at +764 grows by the +14 value, the shorts at +756 and +758 keep the lowest and
//     highest pz seen (compared as floats, stored truncated), and spriteCount grows by one.
//   * When the expand flag is nonzero the first expandList record (16 records of 20 bytes) whose
//     word at +4 is zero is filled: byte +0 = the flag, word +4 = the address of the new record's
//     +16 field, short +8 = the angle, word +12 = 0, float +16 = scale. A full list does nothing.
// Inferred: a decal/blood splat list with a per-cell record bound and z range; the expand record
// is a growing-splat entry. Unknown: the meaning of the kinds beyond blood being kind 0 and the
// fields at +744..+751 of a block. Nothing here ran on the EE.
struct FloorSprite {
    short colour[3];
    short fixed;
    short x4, y4, z4;
    short small;
    short dx, dy, texture;
    short pad;
};
struct FloorBlock {
    FloorSprite sprites[31];
    char unknown[8];
    FloorBlock* next;
    short zMin, zMax;
    int count;
    int smallCount;
};
struct ExpandRecord {
    unsigned char flag;
    short pad0;
    short* sprite;
    short angle;
    int zero;
    float scale;
};

extern "C" {
extern int g_BloodLevel;
extern int spriteCount;
extern FloorBlock* freeList;
extern FloorBlock* sprGrid[];
extern ExpandRecord expandList[];
int lightFloorSprite__FG6Point3(const float* point);
float icos__Fi(int angle);
float isin__Fi(int angle);
}

extern "C" void floorAdd__FG6Point3sfii(const float* at, short angle, float scale, int kind, int expand) {
    float p[3] = {at[0], at[1], at[2]};
    if (g_BloodLevel == 0 && kind == 0) return;
    if (kind == 1 || kind == 2) return;
    int cx = ((int)p[0] + 3840) >> 8;
    int cy = ((int)p[1] + 3840) >> 8;
    if ((unsigned)cx >= 30) return;
    if (cy < 0 || cy >= 30) return;
    FloorBlock* block = sprGrid[cy * 30 + cx];
    if (!block) {
        block = freeList;
        if (!block) return;
        sprGrid[cy * 30 + cx] = block;
        freeList = block->next;
        block->next = 0;
    }
    int n = block->count;
    if (n >= 31) return;
    int colour = lightFloorSprite__FG6Point3(p);
    FloorSprite* s = &block->sprites[n];
    s->colour[0] = colour & 0xff;
    s->colour[1] = (colour >> 8) & 0xff;
    s->colour[2] = (colour >> 16) & 0xff;
    s->fixed = 128;
    s->x4 = (int)(p[0] * 4.0f);
    s->y4 = (int)(p[1] * 4.0f);
    s->z4 = (int)((p[2] + 3.0f) * 4.0f);
    s->small = (unsigned)kind < 3 ? 1 : 0;
    s->dx = (int)(icos__Fi(angle) * scale * 4.0f);
    s->dy = (int)(isin__Fi(angle) * scale * 4.0f);
    s->texture = 979 + kind * 2;
    block->count = block->count + 1;
    block->smallCount = block->smallCount + s->small;
    if (p[2] < (float)block->zMin) block->zMin = (int)p[2];
    if ((float)block->zMax < p[2]) block->zMax = (int)p[2];
    spriteCount = spriteCount + 1;
    if (!expand) return;
    int i = 0;
    if (expandList[0].sprite) {
        for (;;) {
            ++i;
            if (i >= 16) return;
            if (!expandList[i].sprite) break;
        }
    }
    ExpandRecord* e = &expandList[i];
    e->zero = 0;
    e->sprite = &s->dx;
    e->scale = scale;
    e->angle = angle;
    e->flag = expand;
}
