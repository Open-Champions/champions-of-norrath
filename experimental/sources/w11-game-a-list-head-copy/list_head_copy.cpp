// Observed __8ListHeadRC8ListHead at 0x00194498, 8 bytes: the ListHead copy constructor. It stores
// zero in the first word of the destination and returns; the source is not read and no result
// register is set. Callers (the clone routines) use it to start an empty list head in a copied
// object. Inferred: the first word is the head's link pointer. Unknown: whether the type has other
// members (callers copy 4 bytes per head). Nothing here ran on the EE.
extern "C" void ListHead_copy(void* destination, const void* source) __asm__("__8ListHeadRC8ListHead");

extern "C" void ListHead_copy(void* destination, const void* source) {
    (void)source;
    *static_cast<unsigned*>(destination) = 0;
}
