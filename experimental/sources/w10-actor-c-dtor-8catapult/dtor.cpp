// Observed _$_8Catapult at 0x0028bc08, 252 bytes. In order:
//   1. store the 8Catapult vtable (0x4506b0) at object+72;
//   2. walk the singly linked list whose head word is at object+248 (next word at node+0):
//      for every node whose word at node+12 is a nonzero object address, call that object's
//      virtual function at vtable byte offset 8 with the integer 3, then clear the word at
//      node+12. The next link is read before the call;
//   3. when the word at object+88 is nonzero and the signed byte cutMode is nonzero, call that
//      object's virtual function at vtable byte offset 8 with 3;
//   4. listDestroy on the list head at object+248;
//   5. the AnimationState destructor on object+92 with flag 0;
//   6. the GameObject destructor on the object with flag 0;
//   7. when bit 0 of the incoming flag is set, blockFree on the object.
// A virtual call reads the table address at the callee's +72, takes the entry at the given byte
// offset (a signed halfword adjustment at +0, the function address at +4) and calls the function
// with the callee address plus the adjustment and the argument.
// Unknown: what the list nodes, the +12 and +88 objects and the slot at offset 8 are. Inferred:
// the destructor, from the symbol name. Not executed on the EE.
// The host check defines NORRATH_W10AC_HOST so that 32-bit address words map to host memory.

extern "C" {
void listDestroy__FP8ListHead(void* head);
void animation_state_destructor(void* state, int flags) asm("_$_14AnimationState");
void norrath_vt_dtor_10GameObject(void* object, int flags);
void norrath_block_free(void* object);
extern const unsigned char norrath_ctors_ant_lantern_view_4506b0[];
extern signed char cutMode;
#if defined(NORRATH_W10AC_HOST)
extern unsigned char* norrath_w10ac_host_base;
void* norrath_w10ac_host_function(unsigned word);
#endif
}

namespace {
unsigned low_pointer(const void* pointer) {
    return static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(pointer));
}
unsigned read_word(const unsigned char* p) {
    return unsigned(p[0]) | (unsigned(p[1]) << 8) | (unsigned(p[2]) << 16) | (unsigned(p[3]) << 24);
}
void write_word(unsigned char* p, unsigned value) {
    p[0] = static_cast<unsigned char>(value);
    p[1] = static_cast<unsigned char>(value >> 8);
    p[2] = static_cast<unsigned char>(value >> 16);
    p[3] = static_cast<unsigned char>(value >> 24);
}
#if defined(NORRATH_W10AC_HOST)
unsigned char* address_of(unsigned word) { return norrath_w10ac_host_base + word; }
void* function_of(unsigned word) { return norrath_w10ac_host_function(word); }
#else
unsigned char* address_of(unsigned word) {
    return reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(word));
}
void* function_of(unsigned word) { return reinterpret_cast<void*>(static_cast<__UINTPTR_TYPE__>(word)); }
#endif
void call_slot_8(unsigned char* target, int argument) {
    const unsigned char* entry = address_of(read_word(target + 72u)) + 8;
    typedef void (*Function)(void*, int);
    Function function = reinterpret_cast<Function>(function_of(read_word(entry + 4)));
    short adjustment = static_cast<short>(entry[0] | (entry[1] << 8));
    function(target + adjustment, argument);
}
}

extern "C" void dtor_8Catapult(void* object_storage, int flags) asm("_$_8Catapult");
extern "C" void dtor_8Catapult(void* object_storage, int flags) {
    unsigned char* object = static_cast<unsigned char*>(object_storage);
    write_word(object + 72u, low_pointer(norrath_ctors_ant_lantern_view_4506b0));
    for (unsigned node_word = read_word(object + 248u); node_word != 0u;) {
        unsigned char* node = address_of(node_word);
        unsigned next_word = read_word(node);
        unsigned target = read_word(node + 12u);
        if (target != 0u) {
            call_slot_8(address_of(target), 3);
            write_word(node + 12u, 0u);
        }
        node_word = next_word;
    }
    unsigned partner = read_word(object + 88u);
    if (partner != 0u && cutMode != 0) call_slot_8(address_of(partner), 3);
    listDestroy__FP8ListHead(object + 248u);
    animation_state_destructor(object + 92u, 0);
    norrath_vt_dtor_10GameObject(object, 0);
    if ((flags & 1) != 0) norrath_block_free(object);
}
