// Pinned __do_global_ctors at 0x002f9ee8. Ordinary tables have a count or
// all-ones sentinel in slot zero and a null terminator after the callbacks.
using Callback = void (*)();
extern "C" {
extern Callback __CTOR_LIST__[];
extern unsigned char __EH_FRAME_BEGIN__[];
void __register_frame_info(const void*, void*);
void __do_global_dtors();
int atexit(Callback);
void __do_global_ctors() {
    // The reviewed EE frame registrar initializes six 32-bit fields.
    static unsigned frame_object[6];
    __register_frame_info(__EH_FRAME_BEGIN__, frame_object);
    unsigned count = static_cast<unsigned>(
        reinterpret_cast<__UINTPTR_TYPE__>(__CTOR_LIST__[0]));
    if (count == ~0u) {
        count = 0;
        while (__CTOR_LIST__[count + 1] != nullptr) ++count;
    }
    while (count != 0) {
        __CTOR_LIST__[count--]();
    }
    (void)atexit(__do_global_dtors);
}
}
