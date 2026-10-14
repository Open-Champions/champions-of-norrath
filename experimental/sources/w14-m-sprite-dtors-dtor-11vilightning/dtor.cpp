// Observed _$_11VILightning at 0x0106fcc0, 164 bytes.
// The object pointer is compared with -112, sign-extended to 64 bits as the EE compares it. When it is equal, the
// array loop is skipped. Otherwise the loop calls _$_10VIMatrix44(element, 2) for four elements of 1552 bytes, from
// object+6256 down to object+1600, and stops when the pointer reaches object+112. After the loop, and also in the
// skipped case, it calls _$_10VIMatrix44(object+32, 2). When bit 0 of the incoming flags is set, it tail-calls
// __builtin_delete(object). No vtable is stored in this function.
// Inferred: the element stride and count are from the constants in the image. Unknown: what the -112 sentinel means,
// and what the array elements are (the callee is another package's matrix destructor). Nothing here ran on the EE.
extern "C" {
void matrix_destructor(void* self, int flags) __asm__("_$_10VIMatrix44");
void free_object(void* pointer) __asm__("__builtin_delete");
}

extern "C" void destructor_11VILightning(void* self, int flags) __asm__("_$_11VILightning");
extern "C" void destructor_11VILightning(void* self, int flags) {
    unsigned char* object = static_cast<unsigned char*>(self);
    if (static_cast<long long>(reinterpret_cast<__INTPTR_TYPE__>(self)) != -112LL) {
        unsigned char* stop = object + 112;
        if (stop != object + 6320) {
            unsigned char* element = object + 6320 - 1552;
            for (;;) {
                matrix_destructor(element + 1488, 2);
                if (stop == element) break;
                element -= 1552;
            }
        }
    }
    matrix_destructor(object + 32, 2);
    if ((flags & 1) != 0) free_object(object);
}
