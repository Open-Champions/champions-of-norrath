// Observed gamePositionVisible__FP6Point3f at 0x001cd898, 252 bytes. a0 = pointer to a
// point (x at 0, y at 4; z is never read), F12 = a margin.
//  1. If the signed byte objectVisUseCamCenter (0x00483ac9) is nonzero, the answer is 1
//     when worldViewCenter (0x0037ee90) x and y are both within the margin of the point:
//     p.x <= c.x + r, c.x - r <= p.x, and the same for y. Any failing (or NaN) compare
//     gives 0.
//  2. Otherwise each player slot i in 0 .. nmPlayers-1 (stride 23984 bytes from player,
//     0x00408600) is tested in order: the pointer at slot offset 21872 is skipped when
//     null, else its x (offset 16) and y (offset 20) get the same four compares against
//     the point. The first success returns 1.
//  3. After the loop the result is (i != nmPlayers). For nmPlayers > 0 that is 0. For
//     nmPlayers == 0 it is 0, and for a negative nmPlayers the loop does not run and it
//     is 1. The answer is returned in V0 as 0 or 1.
// Unknown: what the object at slot offset 21872 is (only its Point3 at +16 is read).
// Fresh compilation is not an original-byte match.
struct Point3 {
    float x, y, z;
};

struct PlayerSlotObject {
    unsigned char unknown_0[16];
    float x;
    float y;
};

extern "C" {
extern signed char objectVisUseCamCenter;
extern Point3 worldViewCenter;
extern int nmPlayers;
extern unsigned char player[];

int gamePositionVisible__FP6Point3f(const Point3* point, float margin) {
    if (objectVisUseCamCenter != 0) {
        const float px = point->x;
        const float cx = worldViewCenter.x;
        if (!(px <= cx + margin)) return 0;
        if (!(cx - margin <= px)) return 0;
        const float cy = worldViewCenter.y;
        const float py = point->y;
        if (!(py <= cy + margin)) return 0;
        return (cy - margin <= py) ? 1 : 0;
    }
    const int count = nmPlayers;
    int i = 0;
    const unsigned char* slot = player + 21872;
    for (; i < count; ++i, slot += 23984) {
        const PlayerSlotObject* object = *reinterpret_cast<PlayerSlotObject* const*>(slot);
        if (object == 0) continue;
        const float ox = object->x;
        const float px = point->x;
        if (!(px <= ox + margin)) continue;
        if (!(ox - margin <= px)) continue;
        const float oy = object->y;
        const float py = point->y;
        if (!(py <= oy + margin)) continue;
        if (oy - margin <= py) return 1;
    }
    return i != count;
}
}
