// Observed advanceFrame__Ff at 0x0018c078, 764 bytes, one float argument (the
// step) and a float result. It has two parts that share the step.
// Part one: for each index below nmActors it stores displayFrame plus the index
// into eRandState, then, when the word at the start of that 44-byte record of
// the table `actor` is nonzero, calls a virtual slot of that object: the object
// word at +72 is a table pointer, the entry is at table+32 (a signed 16-bit
// this-adjustment, then the function word at +4). It then calls
// worldPlotRouteProcess(1) and adds one to displayFrame.
// Part two: while displayActionPos is below nmActions and the float at the start
// of the 20-byte record `action[displayActionPos]` is not above the step, it
// takes the record, advances displayActionPos, and runs executeAction on it.
// With cutSkipMode zero, record word 4 equal to 0x3ff9d and cutPlayMode nonzero,
// it first runs a ready handshake: if netWeSentReady is zero and
// soundGetStreamPlayStatus(2) is zero it calls netSendMessage(cutMsgHandler, 0,
// 0) and sets netWeSentReady to 1; then if netNmReady is below
// max(netGetNmConnectedPartners(), 1) it undoes the step (displayFrame and
// displayActionPos each drop by one, cutTime drops by 1/60 and the result
// becomes the lowered displayFrame times 1/60 as a float) and leaves the loop; otherwise it
// clears netWeSentReady and netNmReady and continues to executeAction.
// Part three: when dialogActor is nonzero, animFindComponentWithFlags(
// dialogActor + 288, 32) returns a nonnegative index and
// soundGetStreamAmountPlayed(2) differs from a remembered earlier value, it
// compares a value derived from that amount with one derived from the float at
// dialogActor + 320 + 32 * index, remembers the amount, and sets a remembered
// correction to 0 (difference under 0.02), +0.1 or -0.1. With a nonzero
// correction it adds it to the float at +32 + 32 * i of the state for each i
// below the signed byte at state + 4. The two remembered values are
// function-local statics in the original and start at zero.
// Inferred: the 1/60 constant reads as a frame time and the three parts as
// cutscene playback; no structure names are claimed.
// Unknown: the records' field names, the meaning of 0x3ff9d, the action table
// owner, and what the constants 0.07, 7.2e-5 and 1024 stand for.
#if defined(NORRATH_HOST_CONTRACT)
extern "C" unsigned char* norrath_w9_host_base;
extern "C" void norrath_w9_host_call_slot(unsigned function, unsigned self);
#endif

extern "C" {
extern int nmActors;
extern unsigned actor;
extern int displayFrame;
extern int eRandState;
extern int nmActions;
extern unsigned action;
extern int displayActionPos;
extern unsigned dialogActor;
extern signed char cutSkipMode;
extern signed char cutPlayMode;
extern int netWeSentReady;
extern int netNmReady;
extern int cutMsgHandler;
extern float cutTime;
void worldPlotRouteProcess__Fi(int value);
void executeAction__FP6Action(unsigned char* record);
int soundGetStreamPlayStatus__Fi(int stream);
int soundGetStreamAmountPlayed__Fi(int stream);
void netSendMessage__FiPUci(int handler, unsigned char* data, int size);
int netGetNmConnectedPartners__Fv();
int animFindComponentWithFlags__FP14AnimationStatei(unsigned char* state, int flags);
}

namespace {
unsigned char* at(unsigned word) {
#if defined(NORRATH_HOST_CONTRACT)
    return norrath_w9_host_base + word;
#else
    return reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(word));
#endif
}

unsigned load_u32(const unsigned char* address) {
    return *reinterpret_cast<const unsigned*>(address);
}

float load_f32(const unsigned char* address) {
    return *reinterpret_cast<const float*>(address);
}

void store_f32(unsigned char* address, float value) {
    *reinterpret_cast<float*>(address) = value;
}

void call_slot(unsigned char* object) {
    unsigned char* entry = at(load_u32(object + 72)) + 32;
    int delta = *reinterpret_cast<const short*>(entry);
    unsigned function = load_u32(entry + 4);
#if defined(NORRATH_HOST_CONTRACT)
    norrath_w9_host_call_slot(function, static_cast<unsigned>(object + delta - norrath_w9_host_base));
#else
    reinterpret_cast<void (*)(unsigned char*)>(static_cast<__UINTPTR_TYPE__>(function))(object + delta);
#endif
}

float constant(unsigned bits) {
    return __builtin_bit_cast(float, bits);
}
}

extern "C" float advanceFrame__Ff(float step) {
    static int lastAmountPlayed;
    static float correction;

    for (int index = 0; index < nmActors; ++index) {
        eRandState = displayFrame + index;
        unsigned object = load_u32(at(actor) + index * 44);
        if (object != 0) call_slot(at(object));
    }
    worldPlotRouteProcess__Fi(1);
    displayFrame = displayFrame + 1;

    if (displayActionPos < nmActions && load_f32(at(action) + displayActionPos * 20) <= step) {
        do {
            int position = displayActionPos;
            displayActionPos = position + 1;
            unsigned char* record = at(action) + position * 20;
            if (cutSkipMode == 0 && load_u32(record + 4) == 0x3ff9du && cutPlayMode != 0) {
                if (netWeSentReady == 0 && soundGetStreamPlayStatus__Fi(2) == 0) {
                    netWeSentReady = 1;
                    netSendMessage__FiPUci(cutMsgHandler, 0, 0);
                }
                int partners = netGetNmConnectedPartners__Fv();
                int needed = partners < 1 ? 1 : partners;
                if (netNmReady < needed) {
                    int frame = displayFrame - 1;
                    displayFrame = frame;
                    displayActionPos = displayActionPos - 1;
                    cutTime = cutTime - constant(0x3c888888u);
                    step = static_cast<float>(frame) * constant(0x3c888888u);
                    break;
                }
                netWeSentReady = 0;
                netNmReady = 0;
            }
            executeAction__FP6Action(record);
        } while (displayActionPos < nmActions && load_f32(at(action) + displayActionPos * 20) <= step);
    }

    if (dialogActor != 0) {
        unsigned char* dialog = at(dialogActor);
        unsigned char* state = dialog + 288;
        int found = animFindComponentWithFlags__FP14AnimationStatei(state, 32);
        if (found >= 0) {
            int amount = soundGetStreamAmountPlayed__Fi(2);
            if (amount != lastAmountPlayed) {
                float fromAmount = static_cast<float>(amount - 1024) * constant(0x3898eadbu);
                float fromTable = load_f32(dialog + 320 + found * 32) * constant(0x3c888888u) - constant(0x3d8f5c28u);
                lastAmountPlayed = amount;
                float difference = __builtin_fabsf(fromAmount - fromTable);
                if (difference < constant(0x3ca3d70au)) {
                    correction = 0.0f;
                } else if (fromTable < fromAmount) {
                    correction = constant(0x3dccccccu);
                } else {
                    correction = constant(0xbdccccccu);
                }
            }
            if (correction != 0.0f) {
                for (int i = 0; i < static_cast<signed char>(state[4]); ++i) {
                    unsigned char* slot = state + 32 + i * 32;
                    store_f32(slot, load_f32(slot) + correction);
                }
            }
        }
    }
    return step;
}
