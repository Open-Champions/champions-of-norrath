// Observed: GetMaterialPal__C14VISimpleSprite at 0x01167240, 8 bytes. With the simple sprite as
// this, it returns the 32-bit word at this + 72 and touches nothing else.
// Inferred: the material-palette word; the name suffix C shows the method is const.
// Unknown: the member name and the full set of values the word can hold.
extern "C" {
int GetMaterialPal__C14VISimpleSprite(const void* object) {
    return *reinterpret_cast<const int*>(static_cast<const unsigned char*>(object) + 72);
}
}
