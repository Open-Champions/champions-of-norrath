// Observed: SetAnimSoundChannels__9VICSprite at 0x010b22b8, 320 bytes. It reads the word at this+404 and returns zero. When that word is 1 it
// stores 1 into nineteen words and otherwise zero into the same nineteen: this+472+52*k for k = 0..18. In both cases it then stores zero into
// thirty-six more words: this+1616+52*k for k = 0..24, then +3072, +3124, +3176, +3228, +3384, +3436, +3540, +3592, +3644, +3748, +3800.
// Inferred: the 52-byte stride is a table of per-animation records and the word at +404 (stored by Init__9VICSprite as its second argument) selects
// the skeleton kind; the value 1 gives all nineteen channel words the value 1.
// Unknown: what the skeleton kind 1 is, what the records hold and whether the word stored in the first group is a sound channel number
// (the name is the caller's); no other field is written.
extern "C" {
int SetAnimSoundChannels__9VICSprite(void* object) {
    unsigned char* bytes = static_cast<unsigned char*>(object);
    static const unsigned short kClear[] = {3072, 3124, 3176, 3228, 3384, 3436, 3540, 3592, 3644, 3748, 3800};
    int kind = *reinterpret_cast<int*>(bytes + 404);
    int value = kind == 1 ? 1 : 0;
    for (int k = 0; k < 19; ++k) *reinterpret_cast<int*>(bytes + 472 + 52 * k) = value;
    for (int k = 0; k < 25; ++k) *reinterpret_cast<int*>(bytes + 1616 + 52 * k) = 0;
    for (int i = 0; i < 11; ++i) *reinterpret_cast<int*>(bytes + kClear[i]) = 0;
    return 0;
}
}
