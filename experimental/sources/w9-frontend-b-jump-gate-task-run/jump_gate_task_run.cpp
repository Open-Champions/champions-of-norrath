// Observed: jumpGateTaskRun__Fv at 0x002832b0 is 1204 bytes with a 96-byte frame
// (a 64-byte-or-smaller text buffer at sp+0). It switches on jumpGateState (an
// unsigned byte read as a signed value; values 0 to 7 dispatch through a table
// at 0x00476ce0, anything else does nothing). Every state that advances sets
// jumpGateState to its old value plus one unless stated otherwise.
//  0: gameGetFullLevelName__FPc(buffer); cursorTime = 0; thisLevel = -1;
//     curDest = lastDest = 0; scan the 32-byte jumpGateEntries[0 ..
//     jumpGateDestCount) with strcmp against the buffer and set thisLevel to the
//     first match; gameControlPaused = gameControlDontResetPause = 1; if
//     jumpGateHurtDest is nonzero set the state to 4; otherwise, when
//     netNetworkMode is zero set particlesPaused = 1 and gamePause = 1, then
//     advance.
//  1: flareAlpha += 8; below 128 it stays in state 1, otherwise flareAlpha =
//     127 and advance.
//  2: cursorTime += a float constant of about 0.1 (0x3dcccccc), capped at 1.0.
//     If netPlayerIsLocal__Fi(jumpGateWho) and debugStartMenuIsOn__Fv(), call
//     netBlankController__Fii(jumpGateWho, 4) and stop. Let buttons be the word
//     at pad + 88 * jumpGateWho + 12. If it has padBackButton set, or
//     netPlayerIsConnected__Fi(jumpGateWho) is zero: play the sound
//     lumpFindResource__FPcT0("fx.lmp", "menu_main_accept.anm") with
//     soundPlaySequence__FP15AnimationHeaderP10GameObjectf(res, 0, 1.0f) and
//     advance. Else if curDest (as the signed byte) differs from thisLevel and
//     buttons has padAcceptButton: play the same sound, set jumping = 1 and
//     advance. Else with c = curDest: the word at pad + 88 * who + 16 having
//     0x101000 and c nonzero moves the selection to c - 1, or having 0x204000
//     and (signed) c < jumpGateDestCount - 1 moves it to c + 1; a move stores
//     curDest = new, lastDest = c, cursorTime = 0 and plays "menu_main_toggle.anm"
//     from "fx.lmp" the same way. (The up test is skipped when c is zero.)
//  3: flareAlpha -= 16; while it stays at least zero nothing else happens.
//     Otherwise flareAlpha = 0, gamePause = 0, particlesPaused = 0; if jumping
//     is nonzero: gameControlDontResetPause = gameControlPaused = 1,
//     debugClearStartMenu__Fv(), advance. If jumping is zero:
//     gameControlDontResetPause = 0, netBlankController__Fii(jumpGateWho, 5),
//     gameControlPaused = 0, and engineRemoveTask__FPFv_v is called for
//     jumpGateTaskRun__Fv and jumpGateTaskDraw__Fv.
//  4: initJumpEffect__Fv(); advance.
//  5: if jumping is at least 181: copy jumpGateHurtDest (when nonzero) or the
//     entry jumpGateEntries + 32 * curDest into levelName with strcpy, store
//     -1 at levelName + 80 and advance; otherwise jumpGateTime += a float
//     constant of about 0.05 (0x3d4ccccc), capped at 1.0, and jumping += 1.
//  6: advance.
//  7: gameControlPaused = 10; gameControlDontResetPause = 0; jumping = 0;
//     hudNoDraw = 0; engineRemoveTask__FPFv_v for jumpGateTaskRun__Fv and
//     jumpGateTaskDraw__Fv.
// Inferred: this is the jump-gate screen driver (fade in, choose, fade out, hand
// off the destination to levelName and the jump effect). The two float
// constants have no symbol; they sit in the image as the single-precision values
// 0x3dcccccc and 0x3d4ccccc and are written here as hexadecimal literals.
// Unknown: the meaning of levelName + 80, what jumping counting up to 181 is
// timing, and the units of jumpGateHurtDest beyond being a string pointer.

extern "C" char* strcpy(char* dst, const char* src);
extern "C" int strcmp(const char* a, const char* b);
extern "C" void gameGetFullLevelName__FPc(char* name);
extern "C" int netPlayerIsLocal__Fi(int player);
extern "C" int netPlayerIsConnected__Fi(int player);
extern "C" int debugStartMenuIsOn__Fv();
extern "C" void debugClearStartMenu__Fv();
extern "C" void netBlankController__Fii(int player, int mode);
extern "C" void* lumpFindResource__FPcT0(const char* lump, const char* name);
extern "C" void soundPlaySequence__FP15AnimationHeaderP10GameObjectf(void* header, void* object,
                                                                     float volume);
extern "C" void initJumpEffect__Fv();
extern "C" void engineRemoveTask__FPFv_v(void (*task)());
extern "C" void jumpGateTaskRun__Fv();
extern "C" void jumpGateTaskDraw__Fv();
extern "C" unsigned char jumpGateState;
extern "C" signed char jumpGateWho;
extern "C" char* jumpGateHurtDest;
extern "C" float jumpGateTime;
extern "C" float cursorTime;
extern "C" int thisLevel;
extern "C" int jumping;
extern "C" int flareAlpha;
extern "C" signed char curDest;
extern "C" signed char lastDest;
extern "C" signed char jumpGateDestCount;
extern "C" char jumpGateEntries[];
extern "C" char levelName[];
extern "C" unsigned char pad[];
extern "C" int padBackButton;
extern "C" int padAcceptButton;
extern "C" int gameControlPaused;
extern "C" int gameControlDontResetPause;
extern "C" int gamePause;
extern "C" signed char particlesPaused;
extern "C" signed char netNetworkMode;
extern "C" signed char hudNoDraw;

static inline int padWord(int who, int offset) {
    return *reinterpret_cast<int*>(pad + 88 * who + offset);
}

static void playSound(const char* name) {
    void* header = lumpFindResource__FPcT0("fx.lmp", name);
    soundPlaySequence__FP15AnimationHeaderP10GameObjectf(header, 0, 1.0f);
}

extern "C" void jumpGateTaskRun__Fv() {
    switch (static_cast<signed char>(jumpGateState)) {
    case 0: {
        char name[64];
        gameGetFullLevelName__FPc(name);
        cursorTime = 0.0f;
        thisLevel = -1;
        curDest = 0;
        lastDest = 0;
        char* entry = jumpGateEntries;
        for (int i = 0; i < jumpGateDestCount; ++i, entry += 32) {
            if (strcmp(entry, name) == 0) {
                thisLevel = i;
                break;
            }
        }
        gameControlPaused = 1;
        gameControlDontResetPause = 1;
        if (jumpGateHurtDest != 0) {
            jumpGateState = 4;
            return;
        }
        if (netNetworkMode == 0) {
            particlesPaused = 1;
            gamePause = 1;
        }
        jumpGateState = jumpGateState + 1;
        return;
    }
    case 1: {
        int alpha = flareAlpha + 8;
        if (alpha < 128) {
            flareAlpha = alpha;
            return;
        }
        flareAlpha = 127;
        jumpGateState = jumpGateState + 1;
        return;
    }
    case 2: {
        float t = cursorTime + 0x1.999998p-4f;
        if (1.0f < t) t = 1.0f;
        cursorTime = t;
        int who = jumpGateWho;
        if (netPlayerIsLocal__Fi(who) != 0) {
            if (debugStartMenuIsOn__Fv() != 0) {
                netBlankController__Fii(jumpGateWho, 4);
                return;
            }
            who = jumpGateWho;
        }
        if ((padWord(who, 12) & padBackButton) != 0 || netPlayerIsConnected__Fi(who) == 0) {
            playSound("menu_main_accept.anm");
            jumpGateState = jumpGateState + 1;
            return;
        }
        unsigned char c = static_cast<unsigned char>(curDest);
        if (static_cast<signed char>(c) != thisLevel && (padWord(jumpGateWho, 12) & padAcceptButton) != 0) {
            playSound("menu_main_accept.anm");
            jumping = 1;
            jumpGateState = jumpGateState + 1;
            return;
        }
        int next;
        if (c != 0 && (padWord(jumpGateWho, 16) & 0x101000) != 0) {
            next = c - 1;
        } else {
            if (!(static_cast<signed char>(c) < jumpGateDestCount - 1)) return;
            if ((padWord(jumpGateWho, 16) & 0x204000) == 0) return;
            next = c + 1;
        }
        curDest = static_cast<signed char>(next);
        lastDest = static_cast<signed char>(c);
        cursorTime = 0.0f;
        playSound("menu_main_toggle.anm");
        return;
    }
    case 3: {
        int alpha = flareAlpha - 16;
        if (alpha >= 0) {
            flareAlpha = alpha;
            return;
        }
        int wasJumping = jumping;
        flareAlpha = 0;
        gamePause = 0;
        particlesPaused = 0;
        if (wasJumping != 0) {
            gameControlDontResetPause = 1;
            gameControlPaused = 1;
            debugClearStartMenu__Fv();
            jumpGateState = jumpGateState + 1;
            return;
        }
        gameControlDontResetPause = 0;
        netBlankController__Fii(jumpGateWho, 5);
        gameControlPaused = 0;
        engineRemoveTask__FPFv_v(jumpGateTaskRun__Fv);
        engineRemoveTask__FPFv_v(jumpGateTaskDraw__Fv);
        return;
    }
    case 4:
        initJumpEffect__Fv();
        jumpGateState = jumpGateState + 1;
        return;
    case 5: {
        int count = jumping;
        if (count >= 181) {
            if (jumpGateHurtDest != 0) strcpy(levelName, jumpGateHurtDest);
            else strcpy(levelName, jumpGateEntries + 32 * curDest);
            levelName[80] = -1;
            jumpGateState = jumpGateState + 1;
            return;
        }
        float t = jumpGateTime + 0x1.999998p-5f;
        if (1.0f < t) t = 1.0f;
        jumpGateTime = t;
        jumping = count + 1;
        return;
    }
    case 6:
        jumpGateState = jumpGateState + 1;
        return;
    case 7:
        gameControlPaused = 10;
        gameControlDontResetPause = 0;
        jumping = 0;
        hudNoDraw = 0;
        engineRemoveTask__FPFv_v(jumpGateTaskRun__Fv);
        engineRemoveTask__FPFv_v(jumpGateTaskDraw__Fv);
        return;
    default:
        return;
    }
}
