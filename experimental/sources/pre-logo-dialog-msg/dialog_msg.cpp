// Observed dialogMsgCallback__FPUci at 0x1d1c80, 2292 bytes, pinned SLUS_205.65.
// The length is not read. The signed first byte selects one of five entries
// in the jump table at 0x46cdc0; any other byte returns.
//
// Case 0 stores a dialog offer. It returns while recallOn is set, else sends
// gameNetKick(0). The message is: a zero selector byte, a signed name length,
// the name bytes, pad bytes until the cursor is four-byte aligned, the dialog
// packet word, the player byte, two more bytes, a zero pad byte, three camera
// floats, and the yaw/pitch halfwords. The name copy stores no terminator
// when the cursor is already aligned; that quirk is kept. Push, SetYaw,
// SetPitch, and BuildMatrix run on gameCam in that order with a null point.
// Records in the packet have stride 1096; byte 1 set runs scriptStart on the
// bytes at record+1, and the first nonzero result index is kept for
// startPacket. A non-negative player byte below nmPlayers reads the entity
// word at that player record plus 21104; a null word, or an out-of-range
// byte, uses the single-object search instead. Otherwise up to 100 listed
// objects are scanned for the nearest one by full float distance, with the
// first entry forced, and bit 0x20 of its word at offset 12 selects the
// creature block. A null entity there would fault on the word load before
// the explicit empty check, which calls the bad-typeid throw; both are kept
// in that order. The slot at (entity+72)+4 is called with the name; its
// second argument is the pitch-extended leftover on the plain single-object
// path, the end of the list on the multi-object path, and the list base when
// the list comes back empty (the original then also leaves call-clobbered
// bytes in the first argument, which cannot be reproduced here, so the name
// is passed). Only the slot result is consumed, compared against the NPC
// type before setExtraParams. dialogIdleAnimation comes from objectFindAnimation
// with "esc", falling back to "idle". The tag lookup uses the empty string,
// the name formats with "%s.anm", and the two 2048-rounded buffers use the
// eight unnamed zero bytes at 0x485040 as their tag; no ELF symbol covers
// those bytes, so this unit keeps its own zero block for the same reads.
// Case 1 advances text: letterAtATime with a fresh page clears both flags.
// Otherwise showOptions clear, or a cursor tween away from 1, runs the
// shared scroll tail (showMore required, scrollY must equal scrollEnd, then
// showMore clears and scrollTarget becomes the smaller of scrollY+72 and
// scrollEnd with scrollSpan as its float distance). Otherwise stream 2
// resets and a nonzero optionCount compares the option name at
// curDlg+84*opt+136 against the tag; a mismatch starts the bytes at
// tag+84*opt+136 (the original adds to the post-strcmp first argument, which
// is unadvanced exactly when the first bytes differ, the only path that
// reaches the call). A record word at +168 other than -1 resets the body and
// starts that packet; otherwise the fade rises once through flaredOut.
// Case 2 moves the option by the signed second byte within optionCount.
// Offline play only restarts the tween; networked play also stores the index
// in both option words. Case 4 stores -1 in swooping and falls into case 3.
// Case 3 tears the dialog down: it waits on animLoadingComplete through
// cdProcess, frees the two buffers, restores the camera mode, clears the
// dialog words, restores a low creature flag through animAdd, wakes
// monsters, sets the far clip to 1800, adopts DLG_NewWorld into levelName
// with 0xff at byte 80 on mismatch, runs the post action, starts the shop
// for the first local player when the shop flag is set, and switches scenes
// when sceneTransitionOK is set. Inferred types cover accessed fields only;
// animation, sound, shop, and hardware behavior are untested.
extern "C" {
extern signed char recallOn;
extern int nmPlayers, activePlayer;
extern signed char netNetworkMode, hudNoDraw;
extern unsigned char* gameCam;
extern unsigned char* dialogCreature;
extern void* dialogIdleAnimation;
extern signed char waitForSound, animLoadingComplete;
extern int DLG_SwoopMode;
extern char DLG_FireUpShopOnExit;
extern int dialogMutex;
extern int sceneTransitionOK asm("sceneTransitionOK.1041");
extern int showOptions, showMore, letterAtATime, textPageDone;
extern int optionIndex, nextIndex, checkBlock;
extern int scrollTarget, scrollY, scrollEnd;
extern int flaredIn, flaredOut, nameChars, sentEndMsg, optionCount, swooping;
extern float cursorTween, scrollSpan;
// curDlg is `unsigned` in dialog-hud-controls; unsigned long is also 32 bits
// under the reviewed EE mlong32 profile, and it carries full pointers in
// host contracts. dlgArrayPtr has no other definition yet.
extern unsigned long dlgArrayPtr, curDlg;
extern unsigned short dlgName[];
extern unsigned optTbl[];
extern void* anim[];
extern unsigned char player[];
extern char levelName[], DLG_NewWorld[];
int gameNetKick__Fi(int value);
void Push__6Camera(unsigned char* camera);
void SetYaw__6Camerai(unsigned char* camera, int yaw);
void SetPitch__6Camerai(unsigned char* camera, int pitch);
void BuildMatrix__6CamerafffP6Point3(unsigned char* camera, float x, float y, float z, void* point);
void SetMode__6Camerai(unsigned char* camera, int mode);
void StartDialogSwoop__6CameraP10GameObjecti(unsigned char* camera, void* idle, int option);
int netPlayerIsLocal__Fi(int index);
void* objectFindObject__FPc(unsigned char* name);
int objectFindObject__FPcPP10GameObjecti(unsigned char* name, void** list, int max);
void* objectFindAnimation__FP10GameObjectPc(unsigned char* object, char* name);
char* findTagString__10GameObjectPci(unsigned char* object, char* tag, int extra);
unsigned short* textGetString__FPc(char* key);
void ustrcpy__FPUsT0(unsigned short* dest, unsigned short* src);
int ustrlen__FPUs(unsigned short* text);
void sleepMonsters__8creaturev(unsigned char* self);
void wakeMonsters__8creaturev();
void resetTextBody__Fv();
int soundGetDialogSize__FPc(char* path);
void* memAlloc__FiPc(int size, char* tag);
void memFree__FPv(void* block);
void startPacket__Fi(int packet);
int coreStartSwitchScene__Fi(int scene);
int coreInterfaceFadeUp__Fi(int fade);
void soundResetStream__Fi(int stream);
int scriptStart__FPc(char* script);
void scriptActivatePostAction__Fv();
int cdProcess__Fv();
void animAdd__FP14AnimationStateP15AnimationHeaderi(unsigned char* state, void* header, int flag);
void worldSetFarClip__Ff(float distance);
int strcmp(const char* first, const char* second);
char* strcpy(char* dest, const char* src);
int sprintf(char* dest, const char* format, ...);
void shopStart__Fiii(int player, int first, int second);
void setExtraParams__FP8CreaturePc(unsigned char* creature, char* params);
void* __tf3NPC();
int __eq__C9type_infoRC9type_info(void* first, void* second);
void __throw_bad_typeid();
}
static unsigned load_word(const unsigned char* p) {
    return unsigned(p[0]) | unsigned(p[1]) << 8 | unsigned(p[2]) << 16 | unsigned(p[3]) << 24;
}
static void* load_pointer(const unsigned char* p) {
#ifdef NORRATH_HOST_CONTRACT
    void* v = nullptr;
    unsigned char* out = reinterpret_cast<unsigned char*>(&v);
    for (unsigned i = 0; i < sizeof v; ++i) out[i] = p[i];
    return v;
#else
    return reinterpret_cast<void*>(static_cast<unsigned long>(load_word(p)));
#endif
}
static void copy_bytes(void* dest, const void* src, unsigned size) {
    auto* out = static_cast<unsigned char*>(dest);
    auto* in = static_cast<const unsigned char*>(src);
    for (unsigned i = 0; i < size; ++i) out[i] = in[i];
}
// Eight zero bytes standing in for the unnamed tag at 0x485040. The original
// reads byte 0, compares and allocates with the address, and adds option
// offsets to it; all of those behave the same on zeros here.
static char dialogTag[256] = {0};
static short load_half(const unsigned char* p) {
    return static_cast<short>(unsigned(p[0]) | unsigned(p[1]) << 8);
}
static void scroll_advance() {
    if (showMore == 0) return;
    if (scrollY != scrollEnd) return;
    showMore = 0;
    int target = scrollY + 72;
    if (target >= scrollEnd) target = scrollEnd;
    scrollTarget = target;
    scrollSpan = static_cast<float>(target - scrollY);
}
extern "C" void dialogMsgCallback__FPUci(unsigned char* message, int length) {
    (void)length;
    signed char selector = static_cast<signed char>(message[0]);
    if (selector < 0 || selector > 4) return;
    switch (selector) {
    case 0: {
        if (recallOn != 0) return;
        gameNetKick__Fi(0);
        unsigned char name[256];
        unsigned char* cursor = message + 2;
        int nameLength = static_cast<signed char>(message[1]);
        int copied = 0;
        if (nameLength > 0) {
            while (copied < nameLength) {
                name[copied] = *cursor;
                ++cursor;
                ++copied;
            }
        }
        if (((cursor - message) & 3) != 0) name[copied] = 0;
        while (((cursor - message) & 3) != 0) ++cursor;
        unsigned char* packet;
        {
            unsigned char* at = cursor;
#ifdef NORRATH_HOST_CONTRACT
            void* v = nullptr;
            unsigned char* out = reinterpret_cast<unsigned char*>(&v);
            for (unsigned i = 0; i < sizeof v; ++i) out[i] = at[i];
            packet = static_cast<unsigned char*>(v);
            cursor += sizeof v;
#else
            packet = reinterpret_cast<unsigned char*>(static_cast<unsigned long>(load_word(at)));
            cursor += 4;
#endif
        }
        int playerIndex = static_cast<signed char>(cursor[0]);
        int swoopFlag = static_cast<signed char>(cursor[1]);
        int checkByte = static_cast<signed char>(cursor[2]);
        cursor += 4;
        float camX, camY, camZ;
        copy_bytes(&camX, cursor, 4);
        copy_bytes(&camY, cursor + 4, 4);
        copy_bytes(&camZ, cursor + 8, 4);
        cursor += 12;
        int yaw = load_half(cursor);
        cursor += 2;
        Push__6Camera(gameCam);
        SetYaw__6Camerai(gameCam, yaw);
        int pitch = load_half(cursor);
        SetPitch__6Camerai(gameCam, pitch);
        BuildMatrix__6CamerafffP6Point3(gameCam, camX, camY, camZ, nullptr);
        int picked = 0;
        int scripts = packet[0];
        for (int i = 0; i < scripts; ++i) {
            unsigned char* record = packet + i * 1096;
            if (record[1] == 0) continue;
            if (scriptStart__FPc(reinterpret_cast<char*>(record + 1)) != 0) {
                picked = i;
                break;
            }
        }
        unsigned char* entity = nullptr;
        unsigned char* slotAux = reinterpret_cast<unsigned char*>(static_cast<long>(pitch));
        if (playerIndex >= 0 && playerIndex < nmPlayers) {
            unsigned char* focus = player + playerIndex * 23984 + 21104;
            entity = static_cast<unsigned char*>(load_pointer(focus));
            if (entity != nullptr) {
                void* found[100];
                for (int i = 0; i < 100; ++i) found[i] = nullptr;
                objectFindObject__FPcPP10GameObjecti(name, found, 100);
                if (found[0] == nullptr) {
                    entity = static_cast<unsigned char*>(objectFindObject__FPc(name));
                    slotAux = reinterpret_cast<unsigned char*>(found);
                } else {
                    float best = -1.0f;
                    bool first = true;
                    unsigned char* nearest = nullptr;
                    int last = 0;
                    for (; last < 100 && found[last] != nullptr; ++last) {
                        auto* candidate = static_cast<unsigned char*>(found[last]);
                        float px, py, pz, ex, ey, ez;
                        copy_bytes(&px, focus + 16, 4);
                        copy_bytes(&py, focus + 20, 4);
                        copy_bytes(&pz, focus + 24, 4);
                        copy_bytes(&ex, candidate + 16, 4);
                        copy_bytes(&ey, candidate + 20, 4);
                        copy_bytes(&ez, candidate + 24, 4);
                        float dx = ex - px, dy = ey - py, dz = ez - pz;
                        float dist = __builtin_sqrtf(dx * dx + dy * dy + dz * dz);
                        if (first || dist < best) {
                            best = dist;
                            nearest = candidate;
                        }
                        first = false;
                    }
                    entity = nearest;
                    slotAux = reinterpret_cast<unsigned char*>(&found[last]);
                }
            } else {
                entity = static_cast<unsigned char*>(objectFindObject__FPc(name));
            }
        } else {
            entity = static_cast<unsigned char*>(objectFindObject__FPc(name));
        }
        // The original loads the flags word before the empty check; the check
        // comes first here so the host contract stays within sanitizer rules.
        // Observable behavior matches on every path where the original does
        // not fault.
        if (entity == nullptr) __throw_bad_typeid();
        unsigned flags = load_word(entity + 12);
        if ((flags & 0x20u) != 0u) {
            auto* inner = static_cast<unsigned char*>(load_pointer(entity + 72));
            typedef void* (*Slot4)(unsigned char*, unsigned char*);
            Slot4 slot = reinterpret_cast<Slot4>(load_pointer(inner + 4));
            void* probed = slot(name, slotAux);
            if (__eq__C9type_infoRC9type_info(probed, __tf3NPC()) != 0)
                setExtraParams__FP8CreaturePc(entity, reinterpret_cast<char*>(entity + 960));
            else
                setExtraParams__FP8CreaturePc(entity, static_cast<char*>(load_pointer(entity + 36)));
        }
        void* idle = objectFindAnimation__FP10GameObjectPc(dialogCreature, const_cast<char*>("esc"));
        if (idle == nullptr)
            idle = objectFindAnimation__FP10GameObjectPc(dialogCreature, const_cast<char*>("idle"));
        dialogIdleAnimation = idle;
        char* tag = findTagString__10GameObjectPci(entity, const_cast<char*>(""), 0);
        ustrcpy__FPUsT0(dlgName, textGetString__FPc(tag));
        sleepMonsters__8creaturev(reinterpret_cast<unsigned char*>(dlgName));
        resetTextBody__Fv();
        dlgArrayPtr = reinterpret_cast<unsigned long>(packet);
        nameChars = ustrlen__FPUs(dlgName);
        flaredIn = 0;
        hudNoDraw = 1;
        DLG_NewWorld[0] = dialogTag[0];
        DLG_FireUpShopOnExit = 0;
        DLG_SwoopMode = swoopFlag;
        letterAtATime = 1;
        flaredOut = 0;
        checkBlock = checkByte;
        sentEndMsg = 0;
        int count = static_cast<signed char>(packet[0]);
        if (count > 0) {
            int peak = static_cast<int>(reinterpret_cast<unsigned long>(entity));
            for (int k = 0; k < count; ++k) {
                char path[1100];
                sprintf(path, "%s.anm", reinterpret_cast<char*>(packet + 49 + k * 1096));
                int size = soundGetDialogSize__FPc(path);
                if (size >= peak) peak = size;
            }
            unsigned rounded = (static_cast<unsigned>(peak) + 2047u) & ~2047u;
            anim[0] = memAlloc__FiPc(static_cast<int>(rounded), dialogTag);
            anim[4] = memAlloc__FiPc(static_cast<int>(rounded), dialogTag);
        }
        startPacket__Fi(picked);
        if (netPlayerIsLocal__Fi(activePlayer) == 0) sceneTransitionOK = 1;
        if (sceneTransitionOK != 0) coreStartSwitchScene__Fi(20);
        if (DLG_SwoopMode == 0) return;
        StartDialogSwoop__6CameraP10GameObjecti(gameCam, dialogIdleAnimation, checkBlock);
        swooping = 1;
        return;
    }
    case 1: {
        if (curDlg == 0u) return;
        if (letterAtATime != 0) {
            if (textPageDone == 0) {
                letterAtATime = 0;
                textPageDone = 1;
                return;
            }
        }
        if (showOptions == 0) {
            scroll_advance();
            return;
        }
        if (cursorTween != 1.0f) {
            scroll_advance();
            return;
        }
        soundResetStream__Fi(2);
        if (optionCount != 0) {
            unsigned opt = optTbl[optionIndex];
            unsigned long optBase = static_cast<unsigned long>(curDlg) + opt * 84u;
            if (strcmp(dialogTag, reinterpret_cast<char*>(optBase + 136u)) != 0)
                scriptStart__FPc(dialogTag + opt * 84u + 136u);
            if (optionCount != 0) {
                unsigned long record = static_cast<unsigned long>(curDlg) + opt * 84u;
                if (static_cast<int>(load_word(reinterpret_cast<unsigned char*>(record + 168u))) != -1) {
                    resetTextBody__Fv();
                    startPacket__Fi(static_cast<int>(load_word(reinterpret_cast<unsigned char*>(record + 168u))));
                    return;
                }
            }
        }
        if (flaredOut == 0) {
            coreInterfaceFadeUp__Fi(0);
            flaredOut = 1;
        }
        return;
    }
    case 2: {
        if (optionCount < 2) return;
        if (showMore != 0) return;
        if (showOptions == 0) return;
        int next = optionIndex + static_cast<signed char>(message[1]);
        if (next < 0) return;
        if (next >= optionCount) return;
        if (netNetworkMode == 0) {
            cursorTween = 0.0f;
            return;
        }
        nextIndex = next;
        cursorTween = 1.0f;
        optionIndex = next;
        return;
    }
    case 4:
        swooping = -1;
        // Falls through to case 3.
    case 3: {
        if (curDlg == 0u) return;
        while (animLoadingComplete == 0) cdProcess__Fv();
        void** slot = anim;
        for (int i = 0; i < 2; ++i, slot += 4) {
            if (*slot != nullptr) {
                memFree__FPv(*slot);
                *slot = nullptr;
            }
        }
        hudNoDraw = 0;
        curDlg = 0u;
        dialogMutex = 0;
        SetMode__6Camerai(gameCam, 1);
        soundResetStream__Fi(2);
        waitForSound = 0;
        if (dialogCreature != nullptr) {
            int flag = static_cast<signed char>(dialogCreature[292]);
            // The original reloads the word below after storing the flag;
            // no store touches it in between, so one read is equivalent.
            if (flag < 2) dialogCreature[292] = 1;
            unsigned idleNow = static_cast<unsigned>(reinterpret_cast<unsigned long>(dialogIdleAnimation));
            if (load_word(dialogCreature + 296) != idleNow) {
                dialogCreature[292] = 0;
                animAdd__FP14AnimationStateP15AnimationHeaderi(dialogCreature + 288, dialogIdleAnimation, 1);
            }
        }
        dialogCreature = nullptr;
        wakeMonsters__8creaturev();
        worldSetFarClip__Ff(1800.0f);
        if (strcmp(dialogTag, DLG_NewWorld) != 0) {
            strcpy(levelName, DLG_NewWorld);
            levelName[80] = static_cast<char>(0xff);
        }
        scriptActivatePostAction__Fv();
        if (DLG_FireUpShopOnExit != 0) {
            if (netNetworkMode == 0) {
                shopStart__Fiii(activePlayer, 0, 1);
            } else {
                gameNetKick__Fi(0);
                for (int i = 0; i < nmPlayers; ++i) {
                    if (netPlayerIsLocal__Fi(i) != 0) {
                        shopStart__Fiii(i, 0, 1);
                        break;
                    }
                }
            }
        }
        if (sceneTransitionOK != 0) coreStartSwitchScene__Fi(20);
        return;
    }
    }
}
