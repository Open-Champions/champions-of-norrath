// Observed: IsPanEnabled__C13VISoundSprite at 0x0117c488, 8 bytes. With the sound sprite as
// this, it returns the 32-bit word at this + 80 and touches nothing else.
// Inferred: the pan-enabled word; the name suffix C shows the method is const.
// Unknown: the member name and the full set of values the word can hold.
extern "C" {
int IsPanEnabled__C13VISoundSprite(const void* object) {
    return *reinterpret_cast<const int*>(static_cast<const unsigned char*>(object) + 80);
}
}
