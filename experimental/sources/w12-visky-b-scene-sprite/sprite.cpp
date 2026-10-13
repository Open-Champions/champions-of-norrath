// Observed: Sprite__7VIScenei at 0x01144ba8 is 28 bytes. With (scene, index) it reads the pointer
// stored at scene+1004, forms the address pointer + index * 12, and returns the 32-bit word at
// that address. There is no bounds check and no other access.
// Inferred: scene+1004 holds an array of 12-byte records whose first word is a sprite pointer
// (CreateSprite and the render loop use the same array).
// Unknown: the meaning of the other eight bytes of each record and the array's capacity.
// The macro is a seam for the host contract only: the EE's pointers are 32 bits wide.
#ifndef NORRATH_WORD_ADDRESS
#define NORRATH_WORD_ADDRESS(word) reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(word))
#endif
extern "C" {
unsigned Sprite__7VIScenei(void* scene, int index) {
    unsigned char* records = NORRATH_WORD_ADDRESS(*reinterpret_cast<unsigned*>(static_cast<unsigned char*>(scene) + 1004));
    return *reinterpret_cast<unsigned*>(records + index * 12);
}
}
