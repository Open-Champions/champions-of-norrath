// Observed: _$_16VIRealmInterface at 0x01103228 is 416 bytes and returns nothing. In
// order it: stores the address of the data symbol _vt$16VIRealmInterface (0x01210298)
// at offset 8; releases the 100 string words at 4684..5083 from the last to the first;
// releases the 100 string words at 4284..4683 the same way; calls _$_11VIUDPSocket(
// this + 3396, 2); calls _$_Q212RealmService15RealmGatewayAPI(this, 0); and if bit 0
// of the second argument is set tail-calls __builtin_delete(this).
// Releasing a string word w (the data pointer of the library string class): rep = w - 16;
// the reference word at rep + 8 is decremented; if it reaches zero and the capacity word
// at rep + 4, called c, satisfies c + 16 <= 128, rep is pushed onto the small-block free
// list _t24__default_alloc_template2b0i0$_S_free_list (0x011f2910, 16 pointers) at index
// ((c + 23) >> 3) - 1 (rep word 0 receives the old list head); otherwise rep is passed to
// __wrap_free. The original also guards each array loop with a test that the array
// address is nonzero, which is never false for a real object; it is not reproduced.
// Unknown: the string class' source; only its inlined release step is reconstructed.
extern "C" {
extern char norrath_realm_vt[] __asm__("_vt$16VIRealmInterface");
extern int* norrath_string_free_list[] __asm__("_t24__default_alloc_template2b0i0$_S_free_list");

void _$_11VIUDPSocket(void* socket, int flags);
void _$_Q212RealmService15RealmGatewayAPI(void* object, int flags);
void __wrap_free(void* pointer);
void __builtin_delete(void* pointer);

static void release_string(char* data) {
    int* rep = reinterpret_cast<int*>(data - 16);
    int references = rep[2] - 1;
    rep[2] = references;
    if (references != 0) return;
    unsigned capacity = static_cast<unsigned>(rep[1]);
    if (capacity + 16 < 129u) {
        int** slot = &norrath_string_free_list[((capacity + 23) >> 3) - 1];
        rep[0] = reinterpret_cast<int>(*slot);
        *slot = rep;
    } else {
        __wrap_free(rep);
    }
}

void _$_16VIRealmInterface(void* object, int flags) {
    char* base = static_cast<char*>(object);
    *reinterpret_cast<char**>(base + 8) = norrath_realm_vt;
    char** strings = reinterpret_cast<char**>(base + 4684);
    for (int index = 99; index >= 0; --index) release_string(strings[index]);
    strings = reinterpret_cast<char**>(base + 4284);
    for (int index = 99; index >= 0; --index) release_string(strings[index]);
    _$_11VIUDPSocket(base + 3396, 2);
    _$_Q212RealmService15RealmGatewayAPI(object, 0);
    if ((flags & 1) != 0) __builtin_delete(object);
}
}
