// Observed: resetMenu__FP12screenStruct at 0x001bbf28 is 32 bytes and returns nothing. It stores
// zero into seven 4-byte words of the record it is given, at byte offsets 20, 36, 40, 24, 28, 60
// and 64, in that order. No other byte is touched. The only observed caller passes an element of
// the 116-byte-stride array statsScreen (see the inventory-exit notes).
// Inferred: the argument is a screenStruct record, from the mangled name.
// Unknown: the meaning of each cleared field, and what else a screenStruct holds.

extern "C" void resetMenu__FP12screenStruct(void* screen) {
    unsigned char* base = static_cast<unsigned char*>(screen);
    *reinterpret_cast<int*>(base + 20) = 0;
    *reinterpret_cast<int*>(base + 36) = 0;
    *reinterpret_cast<int*>(base + 40) = 0;
    *reinterpret_cast<int*>(base + 24) = 0;
    *reinterpret_cast<int*>(base + 28) = 0;
    *reinterpret_cast<int*>(base + 60) = 0;
    *reinterpret_cast<int*>(base + 64) = 0;
}
