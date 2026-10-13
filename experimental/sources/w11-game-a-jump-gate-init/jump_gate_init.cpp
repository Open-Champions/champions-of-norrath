// Observed jumpGateInit__FiPcc at 0x002845f0, 144 bytes. Starts a jump-gate transition. Arguments:
// who (an int, stored as the signed byte jumpGateWho), a text pointer stored whole in
// jumpGateHurtDest, and a flag stored in the byte jumpHidden. Then, in image order: jumpGateState
// becomes 0 and `jumping` becomes 0; engineAddTask__FPFv_viPci registers jumpGateTaskRun__Fv with
// priority 20, the task name "jumpgate" and flag 0, and jumpGateTaskDraw__Fv with priority 20, the
// name "jumpgatedraw" and flag 1; gameNetKick__Fi(0) runs; and the animation header found by
// lumpFindResource__FPcT0 in the lump "fx.lmp" under the name "spel_gatescroll.anm" is played with
// soundPlaySequence__FP15AnimationHeaderP10GameObjectf (no object, volume 1.0f). There is no result.
// The image passes its own writable copy of "fx.lmp" (an unnamed 7-byte string in the .data region
// near `jumping`); here the literal is a different object with the same contents.
// Inferred: the sequence is the gate's scroll sound. Unknown: why `jumping` is cleared here, and
// the task priorities' meaning beyond ordering. Nothing here ran on the EE.
extern "C" {
extern signed char jumpGateWho;
extern char* jumpGateHurtDest;
extern signed char jumpHidden;
extern unsigned char jumpGateState;
extern int jumping;
void jumpGateTaskRun__Fv();
void jumpGateTaskDraw__Fv();
void engineAddTask__FPFv_viPci(void (*callback)(), int priority, char* name, int flag);
void gameNetKick__Fi(int value);
void* lumpFindResource__FPcT0(char* lump, char* name);
int soundPlaySequence__FP15AnimationHeaderP10GameObjectf(void* header, void* object, float volume);
}

extern "C" void jumpGateInit__FiPcc(int who, char* hurt_destination, char hidden) {
    jumpGateWho = static_cast<signed char>(who);
    jumpGateHurtDest = hurt_destination;
    jumpHidden = hidden;
    jumpGateState = 0;
    jumping = 0;
    engineAddTask__FPFv_viPci(jumpGateTaskRun__Fv, 20, const_cast<char*>("jumpgate"), 0);
    engineAddTask__FPFv_viPci(jumpGateTaskDraw__Fv, 20, const_cast<char*>("jumpgatedraw"), 1);
    gameNetKick__Fi(0);
    soundPlaySequence__FP15AnimationHeaderP10GameObjectf(
        lumpFindResource__FPcT0(const_cast<char*>("fx.lmp"), const_cast<char*>("spel_gatescroll.anm")), nullptr, 1.0f);
}
