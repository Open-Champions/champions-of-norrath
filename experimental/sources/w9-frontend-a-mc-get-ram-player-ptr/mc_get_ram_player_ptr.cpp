// Observed: MC_GetRamPlayerPtr__Fi at 0x0025b450 is 120 bytes. It starts from ramSlot + 112,
// adds Camera::GetSaveFootPrint(gameCam) and jumpGateSaveFootPrint__Fv(), then 4104 and 14. For
// index 0 to 3 it returns that address plus index * 23998. Any other index returns 0.
// Inferred: the layout is the one updateRamSlot__Fii writes: 112 header bytes, the camera and
// jump-gate saves, 4096 level-seed bytes and one word, then one word, then a 14-byte header
// and a 23984-byte record per player. The 4104 is those 4096 bytes plus two words; the 14 is
// the first record's header.
// Unknown: nothing beyond that layout reading.

struct Camera;

extern "C" {
extern unsigned char ramSlot[];
extern Camera* gameCam;
int GetSaveFootPrint__6Camera(Camera* self);
int jumpGateSaveFootPrint__Fv();
}

extern "C" unsigned char* MC_GetRamPlayerPtr__Fi(int index) {
    unsigned char* record = ramSlot + 112 + GetSaveFootPrint__6Camera(gameCam);
    record += jumpGateSaveFootPrint__Fv();
    record += 4104 + 14;
    for (int i = 0; i < 4; ++i) {
        if (i == index) {
            return record;
        }
        record += 23984 + 14;
    }
    return 0;
}
