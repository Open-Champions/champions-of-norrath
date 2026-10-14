// Observed: MC_SetRamNmPlayers__Fi at 0x0025b4d0 is 88 bytes. With n the argument:
//  1. the halfword at ramSlot + 106 becomes the low 16 bits of n;
//  2. off = ramSlot + 112 + GetSaveFootPrint__6Camera(gameCam) + jumpGateSaveFootPrint__Fv(),
//     in that call order;
//  3. MC_ByteCopy__FPvT0i(off + 4100, &n, 4) copies the four bytes of n there.
// Returns nothing.
// Inferred: the layout follows loadRamSlot__Fv, which after the 112-byte header, the camera and
// jump-gate blocks reads 4096 bytes of level seeds, one int (gameAutoMapMode) and then
// nmPlayers; so byte 4100 is the saved nmPlayers and byte 106 of the header holds a copy.
// Unknown: what else the header halfword at 106 means.

extern "C" unsigned char ramSlot[];
extern "C" void* gameCam;
extern "C" int GetSaveFootPrint__6Camera(void* camera);
extern "C" int jumpGateSaveFootPrint__Fv();
extern "C" void MC_ByteCopy__FPvT0i(void* destination, void* source, int count);

extern "C" void MC_SetRamNmPlayers__Fi(int n) {
    *reinterpret_cast<unsigned short*>(ramSlot + 106) = static_cast<unsigned short>(n);
    unsigned char* off = ramSlot + 112 + GetSaveFootPrint__6Camera(gameCam);
    off += jumpGateSaveFootPrint__Fv();
    MC_ByteCopy__FPvT0i(off + 4100, &n, 4);
}
