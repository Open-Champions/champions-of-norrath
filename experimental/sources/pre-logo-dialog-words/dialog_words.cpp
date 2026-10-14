// Observed initial bytes. DLG_SwoopMode at 0x00485070 is four zero bytes.
// DLG_StartWithSwoopDone at 0x00485074 is four zero bytes.
// DLG_FireUpShopOnExit at 0x00485078 is one zero byte. boxTex begins at
// 0x0048507c. The three bytes between the shop flag and boxTex are not
// part of this symbol.
extern "C" int DLG_SwoopMode = 0;
extern "C" int DLG_StartWithSwoopDone = 0;
extern "C" char DLG_FireUpShopOnExit = 0;
