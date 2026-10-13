// Observed inventoryExit__Fiii at 0x001acc20, 356 bytes.
// Arguments: a screen index p (0 or 1 in practice), an integer `a` handed on to
// inventoryUnpack__Fiib, and a flag `announce`.
// `inventory` (0x00427288, 464 bytes) is an array of 116-byte screen records
// (screenStruct); `statsScreen` (0x004281a8, 232 bytes) is an array of 116-byte
// records too. Offsets below are bytes into one inventory record.
//   resetMenu__FP12screenStruct(record) first. Then the word at +112 is stored
//   at +24, and multiplied by the word buySellVertSpacing (0x00484920) into +36;
//   +76 is zeroed. inventoryUnpack__Fiib(a, p, 0) is called.
// If `announce` is nonzero: the word at +52 (read after the unpack call) is taken
// as a player index q. If netPlayerIsLocal__Fi(q) the resource
// "game_lvl_complet.anm" is looked up in the lump "fx.lmp" and played with
// soundPlaySequence__FP15AnimationHeaderP10GameObjectf (object null, rate 1.0).
// A three-byte message {9, q, byte at player[q].+89} is sent with
// netSendMessage__FiPUci(invMsgHandle, message, 3); the word at +80 is set to 5;
// player[q] byte +241 is set to q and byte +242 to zero (player records are 23984
// bytes apart); then, if the byte netNetworkMode (0x00483829) is nonzero, byte +16
// of statsScreen[0] is set to 1, else byte +16 of statsScreen[p] is set to 1.
// Without `announce` none of that happens. The meaning of the fields, of
// message type 9 and of the stored bytes is not established. The callees and
// objects are not defined here.
extern "C" {
extern unsigned char inventory[];
extern unsigned char statsScreen[];
extern unsigned char player[];
extern int buySellVertSpacing;
extern int invMsgHandle;
extern signed char netNetworkMode;
extern const unsigned char norrath_inventory_ability_sound_lump_view[];
extern const unsigned char norrath_inventory_exit_sound_resource_view[];
void resetMenu__FP12screenStruct(void*);
void inventoryUnpack__Fiib(int, int, bool);
int netPlayerIsLocal__Fi(int);
void* lumpFindResource__FPcT0(char* lump, char* name);
int soundPlaySequence__FP15AnimationHeaderP10GameObjectf(void* header, void* object, float rate);
void netSendMessage__FiPUci(int handle, unsigned char* message, int length);
}

extern "C" void inventoryExit__Fiii(int p, int a, int announce) {
    unsigned char* screen = inventory + p * 116;
    resetMenu__FP12screenStruct(screen);
    int base = *reinterpret_cast<int*>(screen + 112);
    *reinterpret_cast<int*>(screen + 76) = 0;
    *reinterpret_cast<int*>(screen + 24) = base;
    *reinterpret_cast<int*>(screen + 36) = base * buySellVertSpacing;
    inventoryUnpack__Fiib(a, p, false);
    if (announce == 0) return;
    int q = *reinterpret_cast<int*>(screen + 52);
    if (netPlayerIsLocal__Fi(q) != 0) {
        void* sound = lumpFindResource__FPcT0(
            const_cast<char*>(reinterpret_cast<const char*>(norrath_inventory_ability_sound_lump_view)),
            const_cast<char*>(reinterpret_cast<const char*>(norrath_inventory_exit_sound_resource_view)));
        soundPlaySequence__FP15AnimationHeaderP10GameObjectf(sound, nullptr, 1.0f);
    }
    unsigned char* record = player + q * 23984;
    unsigned char message[3];
    message[0] = 9;
    message[1] = static_cast<unsigned char>(q);
    message[2] = record[89];
    netSendMessage__FiPUci(invMsgHandle, message, 3);
    *reinterpret_cast<int*>(screen + 80) = 5;
    record[241] = static_cast<unsigned char>(q);
    record[242] = 0;
    statsScreen[(netNetworkMode != 0 ? 0 : p * 116) + 16] = 1;
}
