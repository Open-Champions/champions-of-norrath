// Observed: _$_16VIRealmInterface at 0x01103228 is 416 bytes. With (this, flags), word offsets are in this:
//  1. The word at +8 = the address of _vt$16VIRealmInterface (0x01210298).
//  2. Two arrays of 100 string data pointers, at +4684 and then at +4284 (the second range is released after the
//     first, each from its last entry to its first), are released when the address of the range start plus its
//     offset is nonzero in 32-bit arithmetic. Release of a pointer p of the old reference-counted library:
//     rep = p - 16; the word at rep+8 (the reference count) is decreased by one; when it reaches zero the
//     capacity at rep+4 decides: if capacity + 16 is below 129 the block goes to the shared free list
//     _t24__default_alloc_template2b0i0$_S_free_list at index ((capacity + 23) >> 3) - 1 (the first word of rep
//     takes the old list head, the list takes rep), else __wrap_free(rep) is called.
//  3. __11VIUDPSocket destructor _$_11VIUDPSocket(this + 3396, 2); then the base destructor
//     _$_Q212RealmService15RealmGatewayAPI(this, 0).
//  4. When bit 0 of flags is set, __builtin_delete(this) is tail-called.
// Inferred: the mirror of the constructor: members destroyed in reverse order of construction. The two checks on
// the range start are the compiler's guard for an absent object and cannot be true for a real object.
// Unknown: whether the allocator in use is the standard library's default allocator (the 8-byte rounding and the
// 128-byte limit are read from the arithmetic only), and the return type (void).
extern "C" {
struct StringRep {
    unsigned length;
    unsigned capacity;
    int references;
    int shared;
};
void _$_11VIUDPSocket(void* self, int flags);
void _$_Q212RealmService15RealmGatewayAPI(void* self, int flags);
void __wrap_free(void* pointer);
void __builtin_delete(void* pointer);
extern unsigned char vt_realm_interface[] __asm__("_vt$16VIRealmInterface");
extern unsigned free_list[] __asm__("_t24__default_alloc_template2b0i0$_S_free_list");

static inline unsigned word_of(const void* pointer) {
    return static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(pointer));
}
static inline void* pointer_of(unsigned word) {
    return reinterpret_cast<void*>(static_cast<__UINTPTR_TYPE__>(word));
}

static void release_strings(unsigned* begin, unsigned* end) {
    while (end != begin) {
        --end;
        StringRep* rep = static_cast<StringRep*>(pointer_of(*end - 16));
        rep->references = rep->references - 1;
        if (rep->references == 0) {
            const unsigned capacity = rep->capacity;
            if (capacity + 16 < 129) {
                const unsigned index = ((capacity + 23) >> 3) - 1;
                *reinterpret_cast<unsigned*>(rep) = free_list[index];
                free_list[index] = word_of(rep);
            } else {
                __wrap_free(rep);
            }
        }
    }
}

void dtor_16VIRealmInterface(void* self, int flags) __asm__("_$_16VIRealmInterface");
void dtor_16VIRealmInterface(void* self, int flags) {
    unsigned char* bytes = static_cast<unsigned char*>(self);
    *reinterpret_cast<unsigned*>(bytes + 8) = word_of(vt_realm_interface);
    if (word_of(self) + 4684u != 0) {
        release_strings(reinterpret_cast<unsigned*>(bytes + 4684), reinterpret_cast<unsigned*>(bytes + 5084));
    }
    if (word_of(self) + 4284u != 0) {
        release_strings(reinterpret_cast<unsigned*>(bytes + 4284), reinterpret_cast<unsigned*>(bytes + 4684));
    }
    _$_11VIUDPSocket(bytes + 3396, 2);
    _$_Q212RealmService15RealmGatewayAPI(self, 0);
    if ((flags & 1) != 0) __builtin_delete(self);
}
}
