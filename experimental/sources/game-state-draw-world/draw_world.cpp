// Observed gameDrawWorld__Fv at 0x001d0c50, 116 bytes.
// A nonzero gameNoDraw word returns before any call. Otherwise
// worldSetViewCenter runs. drawAutoMap runs only when the
// coreLoadingScreenRunning byte, the hudNoDraw byte, and the world word are
// all zero for the pointer. worldDrawWorld then receives world.
// drawDome runs only when coreLoadingScreenRunning is zero after
// worldDrawWorld. gameLineDebugDraw always follows that test.
// Nothing here was executed.
extern "C" int gameNoDraw;
extern "C" unsigned char coreLoadingScreenRunning;
extern "C" unsigned char hudNoDraw;
extern "C" unsigned char* world;
extern "C" void worldSetViewCenter__Fv();
extern "C" void drawAutoMap__Fv();
extern "C" void worldDrawWorld__FP12_worldHeader(unsigned char* header);
extern "C" void drawDome__Fv();
extern "C" void gameLineDebugDraw__Fv();

extern "C" void gameDrawWorld__Fv() {
    if (gameNoDraw != 0) return;
    worldSetViewCenter__Fv();
    if (coreLoadingScreenRunning == 0 && hudNoDraw == 0 && world != 0) drawAutoMap__Fv();
    worldDrawWorld__FP12_worldHeader(world);
    if (coreLoadingScreenRunning == 0) drawDome__Fv();
    gameLineDebugDraw__Fv();
}
