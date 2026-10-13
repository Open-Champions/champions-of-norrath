// Observed do_find_public_subobj__C14__si_type_info... at 0x002fd828, 152 bytes.
// If the object and subobject pointers are equal and the target type_info equals
// this one (__eq__C9type_infoRC9type_info, target first) the answer is 6.
// Otherwise the call is forwarded to the base type_info held in word 2 of this
// object: its table is word 1 of the base, and slot 4 (offset 32 in that table;
// a halfword adjustment at +0, the code address at +4) is called with the base
// pointer plus the adjustment and the remaining arguments unchanged.
// Inferred: word 2 is the base type_info (see __rtti_si). The forwarded result is
// returned as is.
extern "C" {
int __eq__C9type_infoRC9type_info(const void* left, const void* right);

struct NorrathTypeInfoSlot {
    short adjustment;
    short pad;
    void* function;
};

struct NorrathSiTypeInfo {
    const char* name;
    const NorrathTypeInfoSlot* table;
    const NorrathSiTypeInfo* base;
};

typedef int (*NorrathFindPublicFn)(const void* self, int offset, const void* target, void* object,
                                   void* subobject);

int do_find_public_subobj__C14__si_type_infoiRC9type_infoPvT3(const NorrathSiTypeInfo* self, int offset,
                                                              const void* target, void* object,
                                                              void* subobject) {
    if (subobject == object && __eq__C9type_infoRC9type_info(target, self)) return 6;
    const NorrathSiTypeInfo* base = self->base;
    const NorrathTypeInfoSlot* slot = base->table + 4;
    return reinterpret_cast<NorrathFindPublicFn>(slot->function)(
        reinterpret_cast<const char*>(base) + slot->adjustment, offset, target, object, subobject);
}
}
