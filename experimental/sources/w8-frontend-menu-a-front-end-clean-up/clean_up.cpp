// Observed frontEndCleanUp__Fi at 0x001ca8d0, 76 bytes. It stops the cut
// scene, removes the task whose entry is runFrontEnd__Fv (0x001c8098), puts
// the global camera into mode 1, clears frontEndRunning (word), stores the
// argument's low byte in frontEndSuspend, and clears hudNoDraw and
// attemptLoadGame (bytes). It returns nothing.
// Inferred: the argument is an int (Fi mangling); mode 1 is only a number here.
// Unknown: what mode 1 of the camera means.
extern "C" {
struct Camera;
void cutStop__Fv();
void runFrontEnd__Fv();
void engineRemoveTask__FPFv_v(void (*task)());
void SetMode__6Camerai(Camera* camera, int mode);
extern Camera* gameCam;
extern int frontEndRunning;
extern unsigned char frontEndSuspend;
extern unsigned char hudNoDraw;
extern unsigned char attemptLoadGame;

void frontEndCleanUp__Fi(int suspend) {
    cutStop__Fv();
    engineRemoveTask__FPFv_v(runFrontEnd__Fv);
    SetMode__6Camerai(gameCam, 1);
    frontEndRunning = 0;
    frontEndSuspend = static_cast<unsigned char>(suspend);
    hudNoDraw = 0;
    attemptLoadGame = 0;
}
}
