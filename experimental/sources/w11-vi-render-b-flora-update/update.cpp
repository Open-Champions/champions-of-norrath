// Observed: Update__19VIRadialFloraSystemRC7VIVect3 at 0x010d4110 is 1348 bytes. The system holds
// the scene pointer at word 0, a zone word at offset 12, a pair of words at 16 and 20 (A) and a
// pair at 24 and 28 (B), and a table of nine words at offsets 32 to 64 (three rows of three).
// A location record is five words (f0 to f4). The function:
//   1. flag = GetWorldGridNumRows(scene) == 1 || GetWorldGridNumCols(scene) == 1;
//   2. copies the position argument (three floats) to a local and calls
//      CalcRadialLoc__19VIRadialFloraSystemG7VIVect3R16VIRadialFloraLoc(system, copy, loc), where
//      loc starts as {-1, 0, 0, 0, 0};
//   3. the location is "changed" unless loc.f0 equals the zone word at 12 and (loc.f3, loc.f4) equals
//      B and (loc.f1, loc.f2) equals A;
//   4. if changed: with offset.x = (B0 - loc.f3) + 50 * (A0 - loc.f1) and
//      offset.y = (B1 - loc.f4) + 50 * (A1 - loc.f2), each nonzero table entry k = 3 * row + col
//      whose row + offset.x and col + offset.y both lie in 0 to 2 is moved to table index
//      3 * offset.x + offset.y + k of a fresh zeroed nine-word table; every other nonzero entry
//      is released with ReleaseSwatch__19VIRadialFloraSystemi(system, k). The fresh table then
//      replaces the nine words at 32;
//   5. fill pass: a second record next = {-1, loc.f1, loc.f2, loc.f3 - 1, loc.f4 - 1} walks the
//      nine table slots in order. Before each row and column step, if flag is zero and the b-field
//      is below -25 the a-field is decreased by one and the b-field raised by 50 (done for f3 at
//      the start, for f4 at the start of each row); after each column step f4 is increased by one
//      and, if flag is zero and f4 reached 25, f2 is increased by one and f4 set to -25; after each
//      row f3 is increased likewise with f1. For an empty slot whose next.f1 is within
//      [0, rows) and next.f2 within [0, cols) (rows and cols from the scene, queried only as
//      needed), it calls CalcZoneIndex__19VIRadialFloraSystemR16VIRadialFloraLoc(system, next)
//      and then CreateSwatch__19VIRadialFloraSystemiR16VIRadialFloraLoc(system, slot, next);
//      a negative result returns -1 at once with the state not yet committed;
//   6. stores loc.f0 at 12, loc.f1 and loc.f2 at A, loc.f3 and loc.f4 at B and returns zero.
// Inferred: the fields are grid coordinates of the camera's patch (f1, f2 the grid cell, f3, f4
// the offset inside it in a range of about -25 to 25, hence the 50 in the offset formula); the
// nine slots are the 3x3 neighbourhood of swatches kept around the camera, re-indexed when the
// camera moves, with far swatches released and missing ones created.
// Unknown: the true names of the record fields and of the offset ranges (25 and 50 are observed
// constants), and what a "swatch" holds.
extern "C" {
struct VIRadialFloraSystem;
struct VIScene;

struct VIRadialFloraLoc {
    int f0;
    int f1;
    int f2;
    int f3;
    int f4;
};

int GetWorldGridNumRows__7VIScene(VIScene* scene);
int GetWorldGridNumCols__7VIScene(VIScene* scene);
int CalcRadialLoc__19VIRadialFloraSystemG7VIVect3R16VIRadialFloraLoc(VIRadialFloraSystem* system,
                                                                     const float* position,
                                                                     VIRadialFloraLoc& loc);
int ReleaseSwatch__19VIRadialFloraSystemi(VIRadialFloraSystem* system, int slot);
int CalcZoneIndex__19VIRadialFloraSystemR16VIRadialFloraLoc(VIRadialFloraSystem* system, VIRadialFloraLoc& loc);
int CreateSwatch__19VIRadialFloraSystemiR16VIRadialFloraLoc(VIRadialFloraSystem* system, int slot,
                                                            VIRadialFloraLoc& loc);

int Update__19VIRadialFloraSystemRC7VIVect3(VIRadialFloraSystem* system, const float* position) {
    int* words = reinterpret_cast<int*>(system);
    VIRadialFloraLoc loc = {-1, 0, 0, 0, 0};
    VIRadialFloraLoc next = {-1, 0, 0, 0, 0};
    int newTable[9];
    int flag = 0;
    VIScene* scene = reinterpret_cast<VIScene*>(words[0]);
    if (GetWorldGridNumRows__7VIScene(scene) == 1) {
        flag = 1;
    } else {
        flag = 0;
        if (GetWorldGridNumCols__7VIScene(reinterpret_cast<VIScene*>(words[0])) == 1) flag = 1;
    }
    float copy[3];
    copy[0] = position[0];
    copy[1] = position[1];
    copy[2] = position[2];
    CalcRadialLoc__19VIRadialFloraSystemG7VIVect3R16VIRadialFloraLoc(system, copy, loc);

    int* pairA = words + 4;
    int* pairB = words + 6;
    bool changed = true;
    if (loc.f0 == words[3]) {
        if (loc.f3 == pairB[0] && loc.f4 == pairB[1]) changed = !(loc.f1 == pairA[0] && loc.f2 == pairA[1]);
    }
    if (changed) {
        for (int i = 0; i < 9; ++i) newTable[i] = 0;
        int offsetX = (pairB[0] - loc.f3) + 50 * (pairA[0] - loc.f1);
        int offsetY = (pairB[1] - loc.f4) + 50 * (pairA[1] - loc.f2);
        int slot = 0;
        for (int row = 0; row < 3; ++row) {
            for (int col = 0; col < 3; ++col, ++slot) {
                int entry = words[8 + slot];
                if (entry == 0) continue;
                int targetRow = row + offsetX;
                int targetCol = col + offsetY;
                if (targetRow >= 0 && targetRow < 3 && targetCol >= 0 && targetCol < 3) {
                    newTable[3 * offsetX + offsetY + slot] = entry;
                    words[8 + slot] = 0;
                } else {
                    ReleaseSwatch__19VIRadialFloraSystemi(system, slot);
                }
            }
        }
        for (int i = 0; i < 9; ++i) words[8 + i] = newTable[i];
    }

    int slot = 0;
    next.f1 = loc.f1;
    next.f3 = loc.f3 - 1;
    if (flag == 0 && next.f3 < -25) {
        next.f1 = loc.f1 - 1;
        next.f3 = next.f3 + 50;
    }
    for (int row = 0; row < 3; ++row) {
        next.f4 = loc.f4 - 1;
        next.f2 = loc.f2;
        if (flag == 0 && next.f4 < -25) {
            next.f2 = loc.f2 - 1;
            next.f4 = next.f4 + 50;
        }
        for (int col = 0; col < 3; ++col, ++slot) {
            if (words[8 + slot] == 0 && next.f1 >= 0 &&
                next.f1 < GetWorldGridNumRows__7VIScene(reinterpret_cast<VIScene*>(words[0])) && next.f2 >= 0 &&
                next.f2 < GetWorldGridNumCols__7VIScene(reinterpret_cast<VIScene*>(words[0]))) {
                CalcZoneIndex__19VIRadialFloraSystemR16VIRadialFloraLoc(system, next);
                if (CreateSwatch__19VIRadialFloraSystemiR16VIRadialFloraLoc(system, slot, next) < 0) return -1;
            }
            next.f4 += 1;
            if (flag == 0 && next.f4 >= 25) {
                next.f2 += 1;
                next.f4 = -25;
            }
        }
        next.f3 += 1;
        if (flag == 0 && next.f3 >= 25) {
            next.f1 += 1;
            next.f3 = -25;
        }
    }
    words[3] = loc.f0;
    pairA[0] = loc.f1;
    pairA[1] = loc.f2;
    pairB[0] = loc.f3;
    pairB[1] = loc.f4;
    return 0;
}
}
