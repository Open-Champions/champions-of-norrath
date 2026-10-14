#define NORRATH_HOST_CONTRACT
#include "dialog_msg.cpp"
#include <cassert>
#include <cstdarg>
#include <cstdio>
#include <cstring>

extern "C" {
signed char recallOn; int nmPlayers, activePlayer;
signed char netNetworkMode, hudNoDraw;
unsigned char* gameCam; unsigned char* dialogCreature; void* dialogIdleAnimation;
signed char waitForSound, animLoadingComplete;
int DLG_SwoopMode; char DLG_FireUpShopOnExit; int dialogMutex;
int sceneTransitionOK;
int showOptions, showMore, letterAtATime, textPageDone;
int optionIndex, nextIndex, checkBlock;
int scrollTarget, scrollY, scrollEnd;
int flaredIn, flaredOut, nameChars, sentEndMsg, optionCount, swooping;
float cursorTween, scrollSpan;
unsigned long dlgArrayPtr, curDlg;
unsigned short dlgName[64]; unsigned optTbl[8]; void* anim[8];
unsigned char player[4 * 23984]; char levelName[322]; char DLG_NewWorld[64];
}

static int kicks, kicksArg, switches, fades, fadeArg, resets, resetArg;
static int packets, packetArg, tbody, frees, allocCount, freedCount;
static int postActions, cdProcs, wakes, sleeps, animAdds, farClips;
static float farClip;
static int shopN, shopP, shopQ, shopR;
static int findsSingle, findsMulti, dlgSizes, textGets;
static int pushOk, yawArg, pitchArg, modeArg, swoopOpt;
static float buildXYZ[3];
static void *buildCam, *modeCam, *swoopCam, *swoopIdle, *sleepSelf;
static void *slotAuxSeen = nullptr;
static unsigned char slotName[16];
static char sizePath[1100];
static int sizeResult = 0, scriptCalls = 0, scriptResults[8];
static void* scriptPtrs[8];
static void* singleResult = nullptr;
static void* multiEntries[8];
static int multiCount = 0;
static int localOf[4];
static void* escResult = nullptr, *idleResult = nullptr;
static void* slotResult = nullptr, *npcToken = (void*)0x1111;
static int eqResult = 0;
static unsigned char* extraArgs[4];
static int extraCount = 0;
static void* allocResults[4];
static int allocSizes[4], allocTags[4];
static void* freedPtrs[4];
static int strcpyCalls = 0;
static unsigned char findName[16];

static void reset_stubs() {
    kicks = kicksArg = switches = fades = fadeArg = resets = resetArg = 0;
    packets = packetArg = tbody = frees = allocCount = freedCount = 0;
    postActions = cdProcs = wakes = sleeps = animAdds = farClips = 0;
    shopN = shopP = shopQ = shopR = 0;
    findsSingle = findsMulti = dlgSizes = textGets = 0;
    pushOk = yawArg = pitchArg = modeArg = swoopOpt = 0;
    buildCam = modeCam = swoopCam = swoopIdle = sleepSelf = nullptr;
    slotAuxSeen = nullptr;
    buildXYZ[0] = buildXYZ[1] = buildXYZ[2] = 0;
    farClip = 0;
    scriptCalls = 0; extraCount = 0; strcpyCalls = 0;
    memset(scriptPtrs, 0, sizeof scriptPtrs);
    memset(scriptResults, 0, sizeof scriptResults);
    memset(extraArgs, 0, sizeof extraArgs);
    memset(findName, 0, sizeof findName);
    memset(slotName, 0, sizeof slotName);
}
static void reset_state() {
    recallOn = 0; nmPlayers = 0; activePlayer = 0; netNetworkMode = 0; hudNoDraw = 0;
    dialogCreature = nullptr; dialogIdleAnimation = nullptr;
    waitForSound = 0; animLoadingComplete = 0; DLG_SwoopMode = 0;
    DLG_FireUpShopOnExit = 0; dialogMutex = 0; sceneTransitionOK = 0;
    showOptions = showMore = letterAtATime = textPageDone = 0;
    optionIndex = nextIndex = checkBlock = 0;
    scrollTarget = scrollY = scrollEnd = 0;
    flaredIn = flaredOut = nameChars = sentEndMsg = optionCount = swooping = 0;
    cursorTween = scrollSpan = 0; dlgArrayPtr = curDlg = 0;
    memset(dlgName, 0, sizeof dlgName); memset(optTbl, 0, sizeof optTbl);
    memset(anim, 0, sizeof anim); memset(player, 0, sizeof player);
    memset(levelName, 0, sizeof levelName); memset(DLG_NewWorld, 0, sizeof DLG_NewWorld);
    singleResult = nullptr; multiCount = 0;
    escResult = idleResult = slotResult = nullptr;
    eqResult = 0; sizeResult = 0;
    memset(localOf, 0, sizeof localOf);
    memset(multiEntries, 0, sizeof multiEntries);
    memset(allocResults, 0, sizeof allocResults);
    memset(allocSizes, 0, sizeof allocSizes);
    memset(allocTags, 0, sizeof allocTags);
    memset(freedPtrs, 0, sizeof freedPtrs);
    reset_stubs();
}

extern "C" {
int gameNetKick__Fi(int v) { ++kicks; kicksArg = v; return 0; }
void Push__6Camera(unsigned char* c) { pushOk = (c == gameCam); }
void SetYaw__6Camerai(unsigned char* c, int y) { buildCam = c; yawArg = y; }
void SetPitch__6Camerai(unsigned char* c, int p) { pitchArg = p; }
void BuildMatrix__6CamerafffP6Point3(unsigned char* c, float x, float y, float z, void* p) {
    buildXYZ[0] = x; buildXYZ[1] = y; buildXYZ[2] = z; assert(p == nullptr);
}
void SetMode__6Camerai(unsigned char* c, int m) { modeCam = c; modeArg = m; }
void StartDialogSwoop__6CameraP10GameObjecti(unsigned char* c, void* i, int o) {
    swoopCam = c; swoopIdle = i; swoopOpt = o;
}
int netPlayerIsLocal__Fi(int i) { return (i >= 0 && i < 4) ? localOf[i] : 0; }
void* objectFindObject__FPc(unsigned char* n) {
    ++findsSingle; strncpy((char*)findName, (char*)n, 15); return singleResult;
}
int objectFindObject__FPcPP10GameObjecti(unsigned char*, void** l, int m) {
    ++findsMulti; int k = 0;
    for (; k < multiCount && k < m; ++k) l[k] = multiEntries[k];
    if (k < m) l[k] = nullptr;
    return 0;
}
void* objectFindAnimation__FP10GameObjectPc(unsigned char*, char* n) {
    return n[0] == 'e' ? escResult : idleResult;
}
char* findTagString__10GameObjectPci(unsigned char*, char*, int) { return (char*)"t"; }
unsigned short* textGetString__FPc(char*) {
    ++textGets; static unsigned short s[] = {'H', 'i', 0}; return s;
}
void ustrcpy__FPUsT0(unsigned short* d, unsigned short* s) {
    int i = 0; do { d[i] = s[i]; } while (s[i++] && i < 63);
}
int ustrlen__FPUs(unsigned short* t) { int i = 0; while (t[i]) ++i; return i; }
void sleepMonsters__8creaturev(unsigned char* s) { ++sleeps; sleepSelf = s; }
void wakeMonsters__8creaturev() { ++wakes; }
void resetTextBody__Fv() { ++tbody; }
int soundGetDialogSize__FPc(char* p) {
    ++dlgSizes; strncpy(sizePath, p, sizeof(sizePath) - 1); return sizeResult;
}
void* memAlloc__FiPc(int s, char* t) {
    allocSizes[allocCount] = s; allocTags[allocCount] = t[0];
    return allocResults[allocCount++];
}
void memFree__FPv(void* b) { freedPtrs[freedCount++] = b; }
void startPacket__Fi(int p) { ++packets; packetArg = p; }
int coreStartSwitchScene__Fi(int s) { ++switches; return s; }
int coreInterfaceFadeUp__Fi(int f) { ++fades; fadeArg = f; return 0; }
void soundResetStream__Fi(int s) { ++resets; resetArg = s; }
int scriptStart__FPc(char* s) {
    scriptPtrs[scriptCalls] = s; return scriptResults[scriptCalls++];
}
void scriptActivatePostAction__Fv() { ++postActions; }
int cdProcess__Fv() { ++cdProcs; animLoadingComplete = 1; return 0; }
void animAdd__FP14AnimationStateP15AnimationHeaderi(unsigned char*, void*, int) { ++animAdds; }
void worldSetFarClip__Ff(float d) { ++farClips; farClip = d; }
void shopStart__Fiii(int p, int a, int b) { ++shopN; shopP = p; shopQ = a; shopR = b; }
void setExtraParams__FP8CreaturePc(unsigned char* c, char* p) {
    if (extraCount < 4) extraArgs[extraCount++] = (unsigned char*)p; (void)c;
}
void* __tf3NPC() { return npcToken; }
int __eq__C9type_infoRC9type_info(void*, void*) { return eqResult; }
void __throw_bad_typeid() { assert(!"bad typeid path taken"); }
int strcmp(const char* a, const char* b) {
    while (*a && *a == *b) { ++a; ++b; }
    return (unsigned char)*a - (unsigned char)*b;
}
char* strcpy(char* d, const char* s) {
    ++strcpyCalls; char* o = d; while ((*d++ = *s++)) {} return o;
}
int sprintf(char* d, const char* f, ...) {
    va_list ap; va_start(ap, f); int n = vsnprintf(d, 1099, f, ap); va_end(ap); return n;
}
}

static void* test_slot(unsigned char* n, unsigned char* a) {
    memcpy(slotName, n, 8); slotAuxSeen = a; return slotResult;
}

static void store_ptr(unsigned char* at, void* p) { memcpy(at, &p, sizeof p); }
static void store_float(unsigned char* at, float f) { memcpy(at, &f, sizeof f); }

// Builds a case-0 message: [0][len][name][0xff pad][ptr][pIdx][sw][ck][0][cam][yaw][pitch].
static void build_offer(unsigned char* msg, const char* nm, void* pkt, int pIdx,
                        int sw, int ck, float cx, float cy, float cz, int yaw, int pitch) {
    int len = (int)strlen(nm);
    unsigned char* c = msg;
    *c++ = 0; *c++ = (unsigned char)len;
    memcpy(c, nm, len); c += len;
    while (((c - msg) & 3) != 0) *c++ = 0xff;
    memcpy(c, &pkt, sizeof pkt); c += sizeof pkt;
    *c++ = (unsigned char)pIdx; *c++ = (unsigned char)sw; *c++ = (unsigned char)ck; *c++ = 0;
    memcpy(c, &cx, 4); c += 4; memcpy(c, &cy, 4); c += 4; memcpy(c, &cz, 4); c += 4;
    c[0] = (unsigned char)yaw; c[1] = (unsigned char)(yaw >> 8); c += 2;
    c[0] = (unsigned char)pitch; c[1] = (unsigned char)(pitch >> 8); c += 2;
}

static unsigned char entA[1024], entB[1024], entC[1024], innerB[16];
static unsigned char camBuf[64];

int main() {
    gameCam = camBuf;
    // T1: out-of-range selectors return without touching anything.
    {
        reset_state();
        unsigned char m[2] = {5, 0};
        dialogMsgCallback__FPUci(m, 99);
        assert(kicks == 0);
        m[0] = (unsigned char)-1;
        dialogMsgCallback__FPUci(m, 99);
        assert(kicks == 0);
    }
    // T2: recall blocks case 0.
    {
        reset_state(); recallOn = 1;
        unsigned char m[2] = {0, 0};
        dialogMsgCallback__FPUci(m, 2);
        assert(kicks == 0);
    }
    // T3: full single-path offer ending in a swoop plus a remote switch.
    {
        reset_state();
        static unsigned char pkt[2200];
        memset(pkt, 0, sizeof pkt);
        unsigned char msg[64];
        build_offer(msg, "abcd", pkt, 5, 1, 7, 1.0f, 2.0f, 3.0f, 10, 20);
        nmPlayers = 2; activePlayer = 0; localOf[0] = 0;
        static unsigned char ent[32];
        memset(ent, 0, sizeof ent);
        singleResult = ent; // flags word zero: no creature block
        idleResult = (void*)0x51;
        dialogMsgCallback__FPUci(msg, 64);
        assert(kicks == 1 && kicksArg == 0);
        assert(pushOk && yawArg == 10 && pitchArg == 20);
        assert(buildXYZ[0] == 1.0f && buildXYZ[1] == 2.0f && buildXYZ[2] == 3.0f);
        assert(sleeps == 1 && sleepSelf == (void*)dlgName);
        assert(dlgName[0] == 'H' && dlgName[1] == 'i');
        assert(dlgArrayPtr == (unsigned long)pkt && nameChars == 2 && flaredIn == 0);
        assert(hudNoDraw == 1 && DLG_NewWorld[0] == 0 && DLG_FireUpShopOnExit == 0);
        assert(DLG_SwoopMode == 1 && letterAtATime == 1 && flaredOut == 0);
        assert(checkBlock == 7 && sentEndMsg == 0 && allocCount == 0);
        assert(tbody == 1 && packets == 1 && packetArg == 0);
        assert(sceneTransitionOK == 1 && switches == 1);
        assert(swoopCam == gameCam && swoopIdle == (void*)0x51 && swoopOpt == 7 && swooping == 1);
    }
    // T3b: local player skips the switch and a zero swoop flag returns early.
    {
        reset_state();
        static unsigned char pkt[2200];
        memset(pkt, 0, sizeof pkt);
        unsigned char msg[64];
        build_offer(msg, "abcd", pkt, 5, 0, 7, 0, 0, 0, 0, 0);
        nmPlayers = 2; activePlayer = 0; localOf[0] = 1;
        static unsigned char ent[32];
        memset(ent, 0, sizeof ent);
        singleResult = ent;
        dialogMsgCallback__FPUci(msg, 64);
        assert(sceneTransitionOK == 0 && switches == 0 && swooping == 0);
    }
    // T4: indexed path picks the nearer listed object; empty list falls back.
    {
        reset_state();
        static unsigned char pkt[2200];
        memset(pkt, 0, sizeof pkt);
        unsigned char msg[64];
        build_offer(msg, "abcd", pkt, 0, 0, 0, 0, 0, 0, 0, 0);
        nmPlayers = 2;
        memset(entA, 0, sizeof entA);
        memset(entB, 0, sizeof entB);
        memset(entC, 0, sizeof entC);
        store_ptr(player + 21104, entA);
        store_float(entB + 16, 10.0f);
        store_float(entC + 16, 1.0f);
        store_ptr(entC + 72, innerB);
        entC[12] = 0x20;
        multiEntries[0] = entB; multiEntries[1] = entC; multiCount = 2;
        eqResult = 1; slotResult = npcToken;
        void* fn = (void*)test_slot;
        memcpy(innerB + 4, &fn, sizeof fn);
        static unsigned char params[8];
        store_ptr(entC + 36, params);
        singleResult = entA;
        dialogMsgCallback__FPUci(msg, 64);
        assert(findsMulti == 1 && findsSingle == 0);
        assert(extraCount == 1 && extraArgs[0] == entC + 960);
        assert(*(void**)slotAuxSeen == nullptr); // end-of-list terminator
        reset_stubs();
        multiCount = 0;
        dialogMsgCallback__FPUci(msg, 64);
        assert(findsSingle == 1);
    }
    // T4b: non-NPC result uses the word at entity+36; single path aux is pitch.
    {
        reset_state();
        static unsigned char pkt[2200];
        memset(pkt, 0, sizeof pkt);
        unsigned char msg[64];
        build_offer(msg, "abcd", pkt, 9, 0, 0, 0, 0, 0, 7, 20);
        nmPlayers = 0; // out of range -> single path
        static unsigned char ent[1024];
        memset(ent, 0, sizeof ent);
        singleResult = ent;
        store_ptr(ent + 72, innerB);
        ent[12] = 0x20;
        eqResult = 0;
        void* fn = (void*)test_slot;
        memcpy(innerB + 4, &fn, sizeof fn);
        static unsigned char params[8];
        store_ptr(ent + 36, params);
        dialogMsgCallback__FPUci(msg, 64);
        assert(extraCount == 1 && extraArgs[0] == (unsigned char*)params);
        assert(memcmp(slotName, "abcd", 5) == 0);
        assert(slotAuxSeen == (void*)(long)20);
    }
    // T5: script records keep the first nonzero index for startPacket.
    {
        reset_state();
        static unsigned char pkt[2200];
        memset(pkt, 0, sizeof pkt);
        pkt[0] = 2; pkt[1] = 0; pkt[1096 + 1] = 9;
        unsigned char msg[64];
        build_offer(msg, "abcd", pkt, 5, 0, 0, 0, 0, 0, 0, 0);
        nmPlayers = 0;
        static unsigned char ent[32];
        memset(ent, 0, sizeof ent);
        singleResult = ent;
        scriptResults[0] = 3;
        dialogMsgCallback__FPUci(msg, 64);
        assert(scriptCalls == 1);
        assert(scriptPtrs[0] == (void*)(pkt + 1096 + 1));
        assert(packetArg == 1);
    }
    // T6: dialog sizes round up to 2048 and fill both buffers.
    {
        reset_state();
        static unsigned char pkt[2200];
        memset(pkt, 0, sizeof pkt);
        pkt[0] = 1; pkt[1] = 0;
        strcpy((char*)pkt + 49, "snd");
        unsigned char msg[64];
        build_offer(msg, "abcd", pkt, 5, 0, 0, 0, 0, 0, 0, 0);
        nmPlayers = 0;
        static unsigned char ent[32];
        memset(ent, 0, sizeof ent);
        singleResult = ent;
        sizeResult = 0x7fffffff;
        static unsigned char b0[8], b1[8];
        allocResults[0] = b0; allocResults[1] = b1;
        dialogMsgCallback__FPUci(msg, 64);
        assert(dlgSizes == 1 && strcmp(sizePath, "snd.anm") == 0);
        unsigned expect = (0x7fffffffu + 2047u) & ~2047u;
        assert(allocCount == 2 && allocSizes[0] == (int)expect && allocTags[0] == 0);
        assert(anim[0] == b0 && anim[4] == b1);
    }
    // T7: aligned names store no terminator (two back-to-back offers).
    {
        reset_state();
        static unsigned char pkt[2200];
        memset(pkt, 0, sizeof pkt);
        nmPlayers = 0;
        static unsigned char ent[32];
        memset(ent, 0, sizeof ent);
        singleResult = ent;
        unsigned char m1[64], m2[64];
        build_offer(m1, "abcdefg", pkt, 5, 0, 0, 0, 0, 0, 0, 0);
        build_offer(m2, "hi", pkt, 5, 0, 0, 0, 0, 0, 0, 0);
        dialogMsgCallback__FPUci(m1, 64);
        assert(strcmp((char*)findName, "abcdefg") == 0);
        dialogMsgCallback__FPUci(m2, 64);
        assert(strcmp((char*)findName, "hicdefg") == 0);
    }
    // T8: case 1 guards and the shared scroll tail.
    {
        reset_state();
        curDlg = 0;
        unsigned char m[2] = {1, 0};
        letterAtATime = 1;
        dialogMsgCallback__FPUci(m, 2);
        assert(resets == 0);
        reset_state();
        curDlg = 0x40; letterAtATime = 1; textPageDone = 0;
        dialogMsgCallback__FPUci(m, 2);
        assert(letterAtATime == 0 && textPageDone == 1 && resets == 0);
        reset_state();
        curDlg = 0x40; letterAtATime = 0; showOptions = 0;
        showMore = 0; scrollY = 10; scrollEnd = 100; scrollTarget = 10;
        dialogMsgCallback__FPUci(m, 2);
        assert(scrollTarget == 10 && showMore == 0 && fades == 0);
        showMore = 1; scrollY = 28; scrollEnd = 100;
        dialogMsgCallback__FPUci(m, 2);
        assert(showMore == 1 && scrollTarget == 10 && fades == 0);
        scrollY = 100; scrollEnd = 100; showMore = 1; scrollTarget = 10;
        dialogMsgCallback__FPUci(m, 2);
        assert(showMore == 0 && scrollTarget == 100 && scrollSpan == 0.0f);
        scrollY = 50; scrollEnd = 100; showMore = 1; scrollTarget = 60;
        dialogMsgCallback__FPUci(m, 2);
        assert(scrollTarget == 60 && showMore == 1 && fades == 0);
        reset_state();
        curDlg = 0x40; showOptions = 1; cursorTween = 0.5f;
        showMore = 1; scrollY = 50; scrollEnd = 50; scrollTarget = 7;
        dialogMsgCallback__FPUci(m, 2);
        assert(scrollTarget == 50 && showMore == 0 && scrollSpan == 0.0f && resets == 0);
    }
    // T9: case 1 option records: mismatch starts tag+84n+136, -1 fades, else starts.
    {
        reset_state();
        static unsigned char dlg[512];
        memset(dlg, 0, sizeof dlg);
        curDlg = (unsigned long)dlg;
        showOptions = 1; cursorTween = 1.0f; optionCount = 2;
        optTbl[0] = 1;
        dlg[1 * 84 + 136] = 'x';
        *(int*)(dlg + 1 * 84 + 168) = -1;
        unsigned char m[2] = {1, 0};
        dialogMsgCallback__FPUci(m, 2);
        assert(scriptCalls == 1);
        assert(fades == 1 && fadeArg == 0 && flaredOut == 1);
        dialogMsgCallback__FPUci(m, 2);
        assert(fades == 1);
        reset_stubs(); flaredOut = 0;
        dlg[1 * 84 + 136] = 0;
        dialogMsgCallback__FPUci(m, 2);
        assert(scriptCalls == 0 && fades == 1);
        reset_stubs(); flaredOut = 0;
        *(int*)(dlg + 1 * 84 + 168) = 7;
        dialogMsgCallback__FPUci(m, 2);
        assert(tbody == 1 && packetArg == 7 && fades == 0);
        // Quirk address check across two option numbers.
        reset_state();
        memset(dlg, 0, sizeof dlg);
        curDlg = (unsigned long)dlg;
        showOptions = 1; cursorTween = 1.0f; optionCount = 3;
        optTbl[0] = 0;
        dlg[0 * 84 + 136] = 'y';
        *(int*)(dlg + 0 * 84 + 168) = -1;
        dialogMsgCallback__FPUci(m, 2);
        void* p0 = scriptPtrs[0];
        reset_stubs(); flaredOut = 0;
        optTbl[0] = 2;
        dlg[2 * 84 + 136] = 'y';
        dialogMsgCallback__FPUci(m, 2);
        assert((unsigned char*)scriptPtrs[0] - (unsigned char*)p0 == 2 * 84);
    }
    // T10: case 2 bounds and the offline/networked split.
    {
        reset_state();
        unsigned char m[2] = {2, 1};
        optionCount = 1; showOptions = 1;
        dialogMsgCallback__FPUci(m, 2);
        assert(cursorTween == 0.0f && optionIndex == 0);
        reset_state();
        optionCount = 3; showOptions = 1; showMore = 1;
        dialogMsgCallback__FPUci(m, 2);
        assert(optionIndex == 0);
        reset_state();
        optionCount = 3; showOptions = 0;
        dialogMsgCallback__FPUci(m, 2);
        assert(optionIndex == 0);
        reset_state();
        optionCount = 3; showOptions = 1; optionIndex = 0;
        m[1] = (unsigned char)-1;
        dialogMsgCallback__FPUci(m, 2);
        assert(optionIndex == 0);
        m[1] = 5;
        dialogMsgCallback__FPUci(m, 2);
        assert(optionIndex == 0);
        reset_state();
        optionCount = 3; showOptions = 1; optionIndex = 1; netNetworkMode = 0;
        m[1] = 1;
        dialogMsgCallback__FPUci(m, 2);
        assert(optionIndex == 1 && nextIndex == 0 && cursorTween == 0.0f);
        reset_state();
        optionCount = 3; showOptions = 1; optionIndex = 1; netNetworkMode = 1;
        dialogMsgCallback__FPUci(m, 2);
        assert(optionIndex == 2 && nextIndex == 2 && cursorTween == 1.0f);
    }
    // T11: case 3 teardown with buffers, camera, clip, and scene switch.
    {
        reset_state();
        curDlg = 0x44; animLoadingComplete = 1;
        static unsigned char b0[8];
        anim[0] = b0;
        unsigned char m[2] = {3, 0};
        sceneTransitionOK = 1;
        dialogMsgCallback__FPUci(m, 2);
        assert(freedCount == 1 && freedPtrs[0] == b0 && anim[0] == nullptr && anim[4] == nullptr);
        assert(modeCam == gameCam && modeArg == 1);
        assert(hudNoDraw == 0 && curDlg == 0 && dialogMutex == 0 && waitForSound == 0);
        assert(resets == 1 && resetArg == 2 && wakes == 1 && farClips == 1 && farClip == 1800.0f);
        assert(strcpyCalls == 0 && postActions == 1 && switches == 1 && shopN == 0);
        reset_stubs();
        dialogMsgCallback__FPUci(m, 2);
        assert(cdProcs == 0 && wakes == 0);
    }
    // T12: case 3 waits for the loader, adopts new worlds, and shops.
    {
        reset_state();
        curDlg = 0x44; animLoadingComplete = 0;
        unsigned char m[2] = {3, 0};
        strcpy(DLG_NewWorld, "lava3");
        DLG_FireUpShopOnExit = 1; netNetworkMode = 0; activePlayer = 2;
        dialogMsgCallback__FPUci(m, 2);
        assert(cdProcs == 1 && animLoadingComplete == 1);
        assert(strcmp(levelName, "lava3") == 0 && levelName[80] == (char)0xff);
        assert(shopN == 1 && shopP == 2 && shopQ == 0 && shopR == 1 && kicks == 0);
        reset_state();
        curDlg = 0x44; animLoadingComplete = 1;
        DLG_FireUpShopOnExit = 1; netNetworkMode = 1; nmPlayers = 2;
        localOf[0] = 0; localOf[1] = 1;
        dialogMsgCallback__FPUci(m, 2);
        assert(kicks == 1 && kicksArg == 0 && shopN == 1 && shopP == 1);
    }
    // T13: case 3 creature restore calls animAdd only on mismatch.
    {
        reset_state();
        curDlg = 0x44; animLoadingComplete = 1;
        static unsigned char cre[336];
        memset(cre, 0, sizeof cre);
        cre[292] = 0;
        store_ptr(cre + 296, (void*)0x1234);
        dialogCreature = cre;
        dialogIdleAnimation = (void*)0x5678;
        unsigned char m[2] = {3, 0};
        dialogMsgCallback__FPUci(m, 2);
        assert(cre[292] == 0 && animAdds == 1 && dialogCreature == nullptr);
        reset_state();
        curDlg = 0x44; animLoadingComplete = 1;
        memset(cre, 0, sizeof cre);
        store_ptr(cre + 296, (void*)0x5678);
        dialogCreature = cre;
        dialogIdleAnimation = (void*)0x5678;
        dialogMsgCallback__FPUci(m, 2);
        assert(cre[292] == 1 && animAdds == 0);
    }
    // T14: case 4 flags the swoop and runs the same teardown.
    {
        reset_state();
        curDlg = 0x44; animLoadingComplete = 1; swooping = 5;
        unsigned char m[2] = {4, 0};
        dialogMsgCallback__FPUci(m, 2);
        assert(swooping == -1 && curDlg == 0u && wakes == 1);
    }
    return 0;
}
