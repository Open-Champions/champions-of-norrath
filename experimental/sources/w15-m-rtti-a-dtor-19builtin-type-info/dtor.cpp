// Observed _$_19__builtin_type_info at 0x0031afa0, 100 bytes. It stores the table address of
// _vt$19__builtin_type_info (0x004538c8) into word 1 of the object, then calls the base destructor _$_9type_info
// with flag 0. When bit 0 of the incoming flag is set it tail-calls __builtin_delete on the object.
// Inferred: the destructor of a class derived from type_info. Unknown: the layout of the object beyond word 1.
extern "C" {
void __builtin_delete(void* object);
void dtor_9type_info(void* object, int flags) __asm__("_$_9type_info");
extern const unsigned char vt_19builtin_type_info[] __asm__("_vt$19__builtin_type_info");
}

extern "C" void dtor_19builtin_type_info(void* object, int flags) __asm__("_$_19__builtin_type_info");
extern "C" void dtor_19builtin_type_info(void* object, int flags) {
    const bool delete_after = (flags & 1) != 0;
    reinterpret_cast<const void**>(object)[1] = vt_19builtin_type_info;
    dtor_9type_info(object, 0);
    if (delete_after) __builtin_delete(object);
}
