// Observed: SetAppPtr__5VIWndPv at 0x010c34f8 is 8 bytes. It stores its second argument as a
// 32-bit word at this+24 and returns nothing.
// Unknown: what the application pointer is used for; no reader is part of this routine.
extern "C" void SetAppPtr__5VIWndPv(void* self, void* application) {
    *reinterpret_cast<unsigned*>(static_cast<unsigned char*>(self) + 24) =
        static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(application));
}
