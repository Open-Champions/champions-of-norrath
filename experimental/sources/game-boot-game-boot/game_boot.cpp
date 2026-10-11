// Observed gameBoot at 0x001d0cc8, 84 bytes.
// The guard _$tmp_0.1090 at 0x00484eac is 4 zero bytes in the image. When that
// word is zero, __6Camera is called with c.1089 at 0x007d0988 and the guard
// is then set to 1. When the word is already nonzero, the constructor is
// skipped. Both paths store c.1089 in gameCam before calling
// gameInitSkillRamps, then store 0 in gameNetReplicateQuestItems.
// c.1089 has symbol size 0. The next BSS symbol, worldBoundingBox, is 384
// bytes later. That gap is not a measured object size. The constructor writes
// halfwords through offset 136 and tail-calls Reset; the full object size
// remains unknown, so c.1089 is left undefined here.
extern "C" void __6Camera(unsigned char* object);
extern "C" void gameInitSkillRamps__Fv();
extern "C" unsigned char norrath_game_boot_camera[] asm("c.1089");
extern "C" {
unsigned norrath_game_boot_camera_guard asm("_$tmp_0.1090");
}
extern "C" unsigned char* gameCam;
extern "C" unsigned gameNetReplicateQuestItems;

extern "C" void gameBoot__Fv() {
    if (norrath_game_boot_camera_guard == 0u) {
        __6Camera(norrath_game_boot_camera);
        norrath_game_boot_camera_guard = 1u;
    }
    gameCam = norrath_game_boot_camera;
    gameInitSkillRamps__Fv();
    gameNetReplicateQuestItems = 0u;
}
