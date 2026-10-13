// Observed msg_poke__10DoorSecreti at 0x213058, 260 bytes. The int is a player index.
// First the string tag "key" is looked up on the object (findTagString, index 0). If
// there is one, the tag is looked up a second time and handed to scriptStart; if that
// returns zero the door stays shut: playerPlaySound(index, 7) and a 180-frame info
// message with the text-table entry for "DoorLocked" (player argument -1, last argument
// 0), and nothing else happens. Otherwise (no tag, or the script returned nonzero) the door
// opens: bit 20 (0x100000) of the flag word at object+12 is cleared, the byte at +88 is
// set to 1, the object is added to the run list, the sequence "door_stn_open.anm" from the
// "fx.lmp" lump is played on the object at rate 1.0, and the global word frameNm is
// stored at object+84.
// The strings sit in the image's string areas (0x485368, 0x470610, 0x485380, 0x4705d0);
// this source holds its own copies.
// Unknown: what the key script does, and what +84 and +88 mean beyond "opened at frame".

extern "C" {
char* findTagString__10GameObjectPci(void* object, const char* tag, int index);
int scriptStart__FPc(char* script);
void playerPlaySound__Fii(int index, int sound);
unsigned short* textGetString__FPc(const char* key);
void hudSetInfoMessage__FPUsiic(unsigned short* text, int duration, int player, char extra);
void objectAddToRunList__FP10GameObject(void* object);
char* lumpFindResource__FPcT0(const char* lump, const char* name);
int soundPlaySequence__FP15AnimationHeaderP10GameObjectf(void* header, void* object, float rate);
extern unsigned frameNm;
}

typedef unsigned Word __attribute__((may_alias));

extern "C" void msg_poke__10DoorSecreti(void* self, int player) {
    unsigned char* object = static_cast<unsigned char*>(self);
    if (findTagString__10GameObjectPci(self, "key", 0) != 0) {
        if (scriptStart__FPc(findTagString__10GameObjectPci(self, "key", 0)) == 0) {
            playerPlaySound__Fii(player, 7);
            hudSetInfoMessage__FPUsiic(textGetString__FPc("DoorLocked"), 180, -1, 0);
            return;
        }
    }
    object[88] = 1;
    *reinterpret_cast<Word*>(object + 12) &= 0xffefffffu;
    objectAddToRunList__FP10GameObject(self);
    soundPlaySequence__FP15AnimationHeaderP10GameObjectf(lumpFindResource__FPcT0("fx.lmp", "door_stn_open.anm"), self, 1.0f);
    *reinterpret_cast<Word*>(object + 84) = frameNm;
}
