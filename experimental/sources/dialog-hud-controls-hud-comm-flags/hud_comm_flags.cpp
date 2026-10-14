// Observed HUD and comm words. chatOn at 0x004865dc is the word 1.
// supersampleMode at 0x004844ac is the byte 1. The other objects here are
// zero. One-byte objects are the ELF size; neighboring symbols begin at the
// next byte or after the recorded size.
extern "C" unsigned chatOn = 1u;
extern "C" unsigned commEcho = 0u;
extern "C" unsigned commEnable = 0u;
extern "C" unsigned commPlayback = 0u;
extern "C" unsigned commRecord = 0u;
extern "C" unsigned commThresh = 0u;
extern "C" unsigned damageDisplayOn = 0u;
extern "C" unsigned char debugPauseMenuOn = 0;
extern "C" unsigned g_BloodLevel = 0u;
extern "C" unsigned char hudNetWaiting = 0;
extern "C" unsigned subtitlesOn = 0u;
extern "C" unsigned char particlesPaused = 0;
extern "C" unsigned char recallOn = 0;
extern "C" unsigned char supersampleMode = 1;
extern "C" unsigned char waitNetExitCount = 0;
