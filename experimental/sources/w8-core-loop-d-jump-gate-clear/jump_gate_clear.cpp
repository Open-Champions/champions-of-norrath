// Observed jumpGateClear__Fv at 0x00284680, 52 bytes. When the word jumping
// (0x00486030) is zero it removes two scheduler tasks, jumpGateTaskRun__Fv
// (0x002832b0) first and jumpGateTaskDraw__Fv (0x00283768) second, through
// engineRemoveTask__FPFv_v. When jumping is nonzero nothing happens.
// Unknown: why a nonzero jumping leaves both tasks registered.
extern "C" int jumping;
extern "C" void jumpGateTaskRun__Fv();
extern "C" void jumpGateTaskDraw__Fv();
extern "C" void engineRemoveTask__FPFv_v(void (*)());

extern "C" void jumpGateClear__Fv() {
    if (jumping == 0) {
        engineRemoveTask__FPFv_v(jumpGateTaskRun__Fv);
        engineRemoveTask__FPFv_v(jumpGateTaskDraw__Fv);
    }
}
