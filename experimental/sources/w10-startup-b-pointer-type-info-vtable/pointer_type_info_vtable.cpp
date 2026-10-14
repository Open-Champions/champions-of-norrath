// Observed _vt$19__pointer_type_info at 0x004538f8, 24 bytes of initialised data (the
// next object starts at 0x00453910). It is three 8-byte pairs of a zero adjustment word
// and a function address, the last pair zero:
//   0x0031ae98 __tf19__pointer_type_info
//   0x0031ae30 _$_19__pointer_type_info
//   (zero pair)
// __rtti_ptr (emitted as norrath_rtti_ptr) stores the address of this object in word 1
// of the pointer type_info it initialises. Inferred: the slot meanings follow the symbol
// names only. Unknown: neither target is authored yet; both stay imports under their
// original names. This is published as the view the callers import, not at the original
// address.
extern "C" {
void __tf19__pointer_type_info();
void _$_19__pointer_type_info();
}

struct NorrathLegacyPointerTypeInfoSlot {
    int adjustment;
    void (*function)();
};

#if defined(__mips__)
static_assert(sizeof(NorrathLegacyPointerTypeInfoSlot) == 8, "offset and 32-bit code address");
#endif

extern "C" const NorrathLegacyPointerTypeInfoSlot norrath_legacy_pointer_type_info_vtable_view[3] = {
    {0, __tf19__pointer_type_info},
    {0, _$_19__pointer_type_info},
    {0, nullptr},
};
