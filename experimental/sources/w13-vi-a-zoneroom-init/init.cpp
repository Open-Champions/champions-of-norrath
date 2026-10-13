// Observed: Init__10VIZoneRoomiiUiUiR6VIBBox at 0x0111f3a8 is 204 bytes. Arguments: room (a0),
// ints a1 and a2, unsigned a3 and t0, and a box (t1: six floats, min at +0, max at +12).
// Stores: the three floats at box+0 to room+4, the three at box+12 to room+16; room[0] |= 2;
// -1 at +28; a3 at +40; t0 at +48; zero at +76 and +84; a1 at +80; a2 at +88. It then calls
// CalcDataSize__10VIZoneRoom(room); a negative result returns -1. Otherwise it calls
// Allocate__10VIZoneRoom(room); a negative result returns -1. On success it adds the word at
// +116 to the words at +92 and +96 (each updated separately) and returns 0.
// Inferred: room+4 and room+16 hold the room's bounding box, +0 is a flag word (bit 1 = set
// up), +116 is a base address produced by Allocate that turns two stored offsets (+92, +96)
// into addresses.
// Unknown: the meaning of a1, a2, a3, t0, +28, +76 and +84, and what the sizes are.
extern "C" {
int CalcDataSize__10VIZoneRoom(void* room);
int Allocate__10VIZoneRoom(void* room);

int Init__10VIZoneRoomiiUiUiR6VIBBox(unsigned char* room, int a1, int a2, unsigned a3, unsigned t0, const float* box) {
    float* low = reinterpret_cast<float*>(room + 4);
    low[0] = box[0];
    low[1] = box[1];
    low[2] = box[2];
    float* high = reinterpret_cast<float*>(room + 16);
    high[0] = box[3];
    high[1] = box[4];
    high[2] = box[5];
    *reinterpret_cast<unsigned*>(room) |= 2;
    *reinterpret_cast<int*>(room + 28) = -1;
    *reinterpret_cast<unsigned*>(room + 40) = a3;
    *reinterpret_cast<unsigned*>(room + 48) = t0;
    *reinterpret_cast<int*>(room + 80) = a1;
    *reinterpret_cast<int*>(room + 88) = a2;
    *reinterpret_cast<int*>(room + 76) = 0;
    *reinterpret_cast<int*>(room + 84) = 0;
    if (CalcDataSize__10VIZoneRoom(room) < 0 || Allocate__10VIZoneRoom(room) < 0) {
        return -1;
    }
    int base = *reinterpret_cast<int*>(room + 116);
    *reinterpret_cast<int*>(room + 92) += base;
    *reinterpret_cast<int*>(room + 96) += base;
    return 0;
}
}
