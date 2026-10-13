// Observed playerItemScan__6PlayerP3padi at 0x001a1e58, 2280 bytes: arguments the player object, a
// pad record (word +12 holds the button bits) and a flag, no result. idx is the word at object +960;
// record = player + idx * 23984.
//  Nothing happens when the word at object +1328 is nonzero, when the float at record +172 is not
//  above zero, or when the word at record +21872 (the player's world object P) is null.
//  1. A box is searched around P (floats at +16): low corner (x - 48, y - 48, z), high corner
//     (x + 48, y + 48, z + 60), up to 256 results (objectFindInBox__FG6Point3T0PP10GameObjecti, zero
//     terminated). Among results with bit 0x100000 in the word at +12 the one with the smallest squared
//     distance to P (below 1e7) is chosen; none chosen skips to step 5.
//  2. The chosen object's entry at (word at +72) + 104 is called as fn(object + halfword at entry,
//     buffer, idx) giving a kind and filling a UTF-16 buffer with its message. An empty message skips
//     to step 5; a message that is exactly one space skips step 3 and 4 but keeps the kind.
//  3. Otherwise pokeItemMagic[idx] is 0. With a nonzero kind, if the object's type_info (first
//     vtable function, word +72, at +4) equals __tf8ItemProp or __tf6PhProp then pokeItemMagic[idx]
//     is 1 when byte +361 is not 255, else 2 when bit 0x2000000000 of the 64-bit word at +336 is set,
//     else 0. The message is then the icon gameButtonToIcon(actionButton[idx]) (kind 1) or
//     gameButtonToIcon(16) followed by the text; with kind 0 the text alone. The message is cut one
//     character at a time while fontStringSizeu(mediumFont, text, 1000000) is 551 or more, and then
//     its last four characters become "...". With a zero flag pokeMessageAlpha[idx] rises by 7 up to
//     128. The message is marked as present.
//  4. With a zero flag: kind 1 and (pad word & actionButton[idx]) nonzero, or kind 2 and (pad word &
//     0x10) nonzero, set activePlayer to idx and call the entry at (word at +72) + 112 as
//     fn(object + halfword at entry, idx).
//  5. With a zero flag and no message present: pokeMessageAlpha[idx] falls by 7; at zero or below it,
//     pokeItemMagic[idx], pokeMessageAlpha[idx] and the first two message halfwords are cleared.
//     With a nonzero flag, a nonzero pokeMessageAlpha[idx] and the float at record +172 still above
//     zero, the message is drawn with the small font in colour (alpha << 24) | 0x464646: centred at
//     (320, 380) with a smooth border when there is one player or the game is networked (colour
//     0x5c2913 for magic 1, 0x4444 for magic 2), else at the x of the 12-entry table
//     {88,549,0,0, 88,549,238,0, 88,549,238,396} entry 4 * nmPlayers + idx - 8 as wrapped text above y 422.
// Inferred: this shows the "press a button to use" hint for the nearest usable world object and runs
// its action; kind 1 is the action button, kind 2 the button with bit 0x10.
// Unknown: the meaning of byte +361 / the 64-bit word at +336, the names of the two vtable entries.
extern "C" {
extern unsigned char player[];
extern int nmPlayers;
extern signed char netNetworkMode;
extern int activePlayer;
extern unsigned actionButton[4];
extern int pokeMessageAlpha[4];
extern int pokeItemMagic[4];
extern void* mediumFont;
extern void* smallFont;
struct Point3 { float x, y, z; };
int objectFindInBox__FG6Point3T0PP10GameObjecti(Point3 low, Point3 high, void** out, int max);
void __throw_bad_typeid();
void* __tf8ItemProp();
void* __tf6PhProp();
int __eq__C9type_infoRC9type_info(const void* left, const void* right);
int gameButtonToIcon__Fi(int button);
void ustrcpy__FPUsT0(unsigned short* to, const unsigned short* from);
void ustrcat__FPUsT0(unsigned short* to, const unsigned short* from);
int ustrlen__FPUs(const unsigned short* text);
int fontStringSizeu__FP4FontPUsi(void* font, unsigned short* text, int limit);
void fontStart__FP4FontiUii(void* font, int size, unsigned color, int flags);
void fontEnd__Fv();
void fontColor__FUiii(unsigned color, int a, int b);
void fontPrintCenteredu__FiiPUsi(int x, int y, unsigned short* text, int limit);
int fontPrintWrapCenteredu__FiiPUsiiii(int x, int y, unsigned short* text, int a, int b, int c, int d);
void textureSmoothBorder__FiiiiiUiiii(int x, int y, int w, int h, int a, unsigned color, int b, int c, int d);

// File-local in the image (msg.1197 at 0x007cace0, four 512-byte messages); the name is inferred.
unsigned short norrath_player_item_scan_message[4][256];
}

#ifdef W12_HUD_HOST
__UINTPTR_TYPE__ hostWordAddress(unsigned word);
#endif

namespace {
template <class T>
inline T load(const unsigned char* base, int offset) {
    T value;
    __builtin_memcpy(&value, base + offset, sizeof value);
    return value;
}
// The 32-bit pointer words of the image (the object's vtable word at +72, the world object word in
// the player record, vtable function words) become native pointers through this on the host check.
inline __UINTPTR_TYPE__ widen(unsigned word) {
#ifdef W12_HUD_HOST
    return hostWordAddress(word);
#else
    return word;
#endif
}
inline unsigned char* pointerAt(const unsigned char* base, int offset) {
    return reinterpret_cast<unsigned char*>(widen(load<unsigned>(base, offset)));
}
// A vtable entry is {short delta, short unused, function word}.
inline short entryDelta(const unsigned char* table, int offset) { return load<short>(table, offset); }
template <class F>
inline F entryFunction(const unsigned char* table, int offset) { return reinterpret_cast<F>(widen(load<unsigned>(table, offset + 4))); }
typedef int (*MessageFunction)(void* self, unsigned short* buffer, int player);
typedef void (*UseFunction)(void* self, int player);
typedef void* (*TypeFunction)();
inline void* typeOf(unsigned char* object) {
    if (object == nullptr) {
        __throw_bad_typeid();
        return nullptr;
    }
    return entryFunction<TypeFunction>(pointerAt(object, 72), 0)();
}
const int kTable[12] = {88, 549, 0, 0, 88, 549, 238, 0, 88, 549, 238, 396};
}

extern "C" void playerItemScan__6PlayerP3padi(void* self, void* pad, int flag) {
    unsigned char* object = static_cast<unsigned char*>(self);
    if (load<int>(object, 1328) != 0) return;
    const int index = load<int>(object, 960);
    const unsigned char* record = player + index * 23984;
    if (load<float>(record, 172) <= 0.0f) return;
    unsigned char* world = pointerAt(record, 21872);
    if (world == nullptr) return;
    const Point3 here = {load<float>(world, 16), load<float>(world, 20), load<float>(world, 24)};
    void* found[256];
    objectFindInBox__FG6Point3T0PP10GameObjecti(Point3{here.x - 48.0f, here.y - 48.0f, here.z}, Point3{here.x + 48.0f, here.y + 48.0f, here.z + 60.0f}, found, 256);
    int bestIndex = -1;
    float best = 10000000.0f;
    for (int i = 0; found[i] != nullptr; ++i) {
        unsigned char* candidate = static_cast<unsigned char*>(found[i]);
        if ((load<unsigned>(candidate, 12) & 0x100000u) == 0) continue;
        const float dx = load<float>(candidate, 16) - here.x;
        const float dy = load<float>(candidate, 20) - here.y;
        const float dz = load<float>(candidate, 24) - here.z;
        const float distance = dx * dx + dy * dy + dz * dz;
        if (distance < best) {
            best = distance;
            bestIndex = i;
        }
    }
    unsigned short* message = norrath_player_item_scan_message[index];
    bool present = false;      // a message was built this call
    bool actionStep = false;   // step 4 applies (a nonempty message, including the lone space)
    int kind = 0;
    unsigned char* target = nullptr;
    if (bestIndex != -1) {
        unsigned short buffer[200];
        target = static_cast<unsigned char*>(found[bestIndex]);
        buffer[0] = 0;
        const unsigned char* messageEntry = pointerAt(target, 72) + 104;
        kind = entryFunction<MessageFunction>(messageEntry, 0)(target + entryDelta(messageEntry, 0), buffer, index);
        if (buffer[0] != 0) {
            actionStep = true;
            if (!(buffer[0] == 32 && buffer[1] == 0)) {
                pokeItemMagic[index] = 0;
                if (kind != 0) {
                    bool prop = __eq__C9type_infoRC9type_info(typeOf(target), __tf8ItemProp()) != 0;
                    if (!prop) prop = __eq__C9type_infoRC9type_info(typeOf(target), __tf6PhProp()) != 0;
                    if (prop) {
                        if (target[361] != 255) pokeItemMagic[index] = 1;
                        else if ((load<unsigned long long>(target, 336) & 0x2000000000ull) != 0) pokeItemMagic[index] = 2;
                        else pokeItemMagic[index] = 0;
                    }
                    const int icon = kind == 1 ? gameButtonToIcon__Fi(static_cast<int>(actionButton[index])) : gameButtonToIcon__Fi(16);
                    message[0] = static_cast<unsigned short>(icon);
                    message[1] = 0;
                    ustrcat__FPUsT0(message, buffer);
                } else {
                    ustrcpy__FPUsT0(message, buffer);
                }
                if (fontStringSizeu__FP4FontPUsi(mediumFont, message, 1000000) >= 551) {
                    for (;;) {
                        if (fontStringSizeu__FP4FontPUsi(mediumFont, message, 1000000) < 551) break;
                        if (ustrlen__FPUs(message) <= 0) break;
                        message[ustrlen__FPUs(message) - 1] = 0;
                    }
                    unsigned short* end = message + ustrlen__FPUs(message);
                    end[-4] = 46;
                    end[-3] = 46;
                    end[-2] = 46;
                    end[-1] = 0;
                }
                if (flag == 0) {
                    pokeMessageAlpha[index] += 7;
                    if (pokeMessageAlpha[index] > 128) pokeMessageAlpha[index] = 128;
                }
                present = true;
            }
        }
    }
    if (actionStep && flag == 0) {
        const unsigned buttons = load<unsigned>(static_cast<unsigned char*>(pad), 12);
        if ((kind == 1 && (buttons & actionButton[index]) != 0) || (kind == 2 && (buttons & 0x10u) != 0)) {
            activePlayer = index;
            const unsigned char* useEntry = pointerAt(target, 72) + 112;
            entryFunction<UseFunction>(useEntry, 0)(target + entryDelta(useEntry, 0), index);
        }
    }
    if (flag == 0 && !present) {
        pokeMessageAlpha[index] -= 7;
        if (pokeMessageAlpha[index] <= 0) {
            pokeItemMagic[index] = 0;
            pokeMessageAlpha[index] = 0;
            message[0] = 0;
            message[1] = 0;
        }
    }
    if (pokeMessageAlpha[index] == 0) return;
    if (flag == 0) return;
    if (!(load<float>(record, 172) > 0.0f)) return;
    fontStart__FP4FontiUii(smallFont, 7, static_cast<unsigned>(pokeMessageAlpha[index]) << 24 | 0x464646u, -1);
    if (nmPlayers == 1 || netNetworkMode != 0) {
        const int size = fontStringSizeu__FP4FontPUsi(smallFont, message, 1000000);
        if (pokeItemMagic[index] == 1) fontColor__FUiii(static_cast<unsigned>(pokeMessageAlpha[index]) << 24 | 0x5c2913u, -1, 0);
        else if (pokeItemMagic[index] == 2) fontColor__FUiii(static_cast<unsigned>(pokeMessageAlpha[index]) << 24 | 0x4444u, -1, 0);
        fontPrintCenteredu__FiiPUsi(320, 380, message, 1000000);
        fontEnd__Fv();
        const int a = pokeMessageAlpha[index];
        textureSmoothBorder__FiiiiiUiiii(320 - ((size + static_cast<int>(static_cast<unsigned>(size) >> 31)) >> 1) - 20, 379, size + 40, 33, 7,
                                         static_cast<unsigned>((a + static_cast<int>(static_cast<unsigned>(a) >> 31)) >> 1) << 24, 0, 0, 0);
    } else {
        const int x = kTable[nmPlayers * 4 + index - 8];
        const int height = fontPrintWrapCenteredu__FiiPUsiiii(x, 422, message, 110, -100, -200, 17);
        const int top = 422 - height;
        const int extent = height + 14;
        fontPrintWrapCenteredu__FiiPUsiiii(x, top, message, 110, -100, 580, 17);
        fontEnd__Fv();
        const int a = pokeMessageAlpha[index];
        textureSmoothBorder__FiiiiiUiiii(x - 70, top - 2, 140, extent < 32 ? 32 : extent, 7,
                                         static_cast<unsigned>((a + static_cast<int>(static_cast<unsigned>(a) >> 31)) >> 1) << 24, 0, 0, 0);
    }
}
