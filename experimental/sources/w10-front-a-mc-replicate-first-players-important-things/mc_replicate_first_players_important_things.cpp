// Observed: MC_ReplicateFirstPlayersImportantThings__Fv at 0x002510b8 is 308 bytes. With
// p = ramSlot + 112 + GetSaveFootPrint__6Camera(gameCam) + jumpGateSaveFootPrint__Fv() + 4104
// (the order of the two calls is camera first), the loop runs for n = 0 to 3 and each pass moves
// p on through one saved-player entry: two signed bytes, then twelve bytes, then a 23984-byte
// record (the layout read by loadRamSlot__Fv):
//  - n = 0 reads and keeps the two bytes, the twelve bytes and the position of record 0;
//  - n = 1 to 3 write those two bytes and twelve bytes into their entries, then copy the text
//    at record byte 36 with strcpy, 12 bytes at record byte 68 and 4100 bytes at record byte
//    17768 from record 0 into record n (MC_ByteCopy__FPvT0i for the last two, in that order).
// After the loop gameNetReplicateQuestItems becomes 1. Nothing is returned.
// Inferred: the entries hold the start yaw, start position and player record that
// loadRamSlot__Fv stores, so the effect is to give players 1 to 3 the first player's start
// position, name, position and 4100-byte block in the saved slot image; byte 36 is the name
// and byte 68 the position, as in MC_CheckPlayerImportOrChange__Fv.
// Unknown: the meaning of the 4100-byte block.

extern "C" {
extern unsigned char ramSlot[];
extern void* gameCam;
extern int gameNetReplicateQuestItems;
int GetSaveFootPrint__6Camera(void* camera);
int jumpGateSaveFootPrint__Fv();
char* strcpy(char* to, const char* from);
void MC_ByteCopy__FPvT0i(void* destination, void* source, int count);
}

namespace {
struct Twelve {
    unsigned char bytes[12];
};
}

extern "C" void MC_ReplicateFirstPlayersImportantThings__Fv() {
    signed char yaw[2];
    Twelve start;
    unsigned char* first = nullptr;
    unsigned char* p = ramSlot + 112 + GetSaveFootPrint__6Camera(gameCam);
    p += jumpGateSaveFootPrint__Fv();
    p += 4104;
    for (int n = 0; n < 4; ++n) {
        if (n == 0) {
            yaw[0] = static_cast<signed char>(p[0]);
            yaw[1] = static_cast<signed char>(p[1]);
        } else {
            p[0] = static_cast<unsigned char>(yaw[0]);
            p[1] = static_cast<unsigned char>(yaw[1]);
        }
        p += 2;
        if (n == 0) start = *reinterpret_cast<Twelve*>(p);
        else *reinterpret_cast<Twelve*>(p) = start;
        p += 12;
        if (n == 0) {
            first = p;
        } else {
            strcpy(reinterpret_cast<char*>(p + 36), reinterpret_cast<char*>(first + 36));
            MC_ByteCopy__FPvT0i(p + 68, first + 68, 12);
            MC_ByteCopy__FPvT0i(p + 17768, first + 17768, 4100);
        }
        p += 23984;
    }
    gameNetReplicateQuestItems = 1;
}
