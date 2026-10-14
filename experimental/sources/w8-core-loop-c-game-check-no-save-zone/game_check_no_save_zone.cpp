// gameCheckNoSaveZone at 0x001d0f08, 160 bytes, in SLUS_205.65.
// Observed: returns 1 at once when the word gameRecallInhibit (0x00486694) is nonzero, or when
// DLG_Active() is nonzero. Otherwise it stores 1 in the local word saveOk (0x00486b18) and runs
// objectIterateRunLists with the function checkSave (0x001cdb38). Then, for each of the first
// nmPlayers records of player (stride 23984; the count is read once), it reads the pointer at
// byte 21872. If that pointer is nonzero and the word at byte 1328 behind it is nonzero the
// function returns 1. After the loop it returns 1 when saveOk is zero, else 0.
// Inferred: saveOk is cleared by checkSave for objects that forbid saving. Unknown: what the
// word at 1328 behind the player pointer holds.
extern "C" int gameRecallInhibit;
extern "C" int saveOk;
extern "C" int nmPlayers;
extern "C" unsigned char player[];
extern "C" int DLG_Active__Fv();
extern "C" void checkSave__FP10GameObject(void* object);
extern "C" void objectIterateRunLists__FPFP10GameObject_v(void (*callback)(void*));

extern "C" int gameCheckNoSaveZone__Fv() {
    if (gameRecallInhibit != 0) return 1;
    if (DLG_Active__Fv() != 0) return 1;
    saveOk = 1;
    objectIterateRunLists__FPFP10GameObject_v(checkSave__FP10GameObject);
    int count = nmPlayers;
    for (int index = 0; index < count; ++index) {
        unsigned char* record = player + index * 23984;
        unsigned char* pointer = *reinterpret_cast<unsigned char**>(record + 21872);
        if (pointer != 0 && *reinterpret_cast<int*>(pointer + 1328) != 0) return 1;
    }
    return saveOk == 0;
}
