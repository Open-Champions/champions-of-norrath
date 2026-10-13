// Observed do_upcast__C14__si_type_info... at 0x002fd658, 176 bytes. If this type
// equals the target (__eq__C9type_infoRC9type_info) it fills the result record
// exactly as the plain user-type_info version does (word 0 object pointer, word 1
// access path, word 2 the constant 8) and returns 1 when (path & 6) == 4. If the
// types differ the call is forwarded to the base type_info in word 2 of this
// object: slot 2 (offset 16) of the table at word 1 of the base, whose halfword
// adjustment at +0 is added to the base pointer and whose code address is at +4.
// The forwarded call gets the access path, target, object pointer and result
// record unchanged and its result is returned.
// Inferred: word 2 is the base type_info (see __rtti_si). Unknown: meaning of 8.
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

struct NorrathUpcastResult {
    void* object;
    int access_path;
    int marker;
};

typedef int (*NorrathUpcastFn)(const void* self, int access_path, const void* target, void* object,
                               NorrathUpcastResult* result);

int do_upcast__C14__si_type_infoQ216__user_type_info8sub_kindRC9type_infoPvRQ216__user_type_info13upcast_result(
    const NorrathSiTypeInfo* self, int access_path, const void* target, void* object,
    NorrathUpcastResult* result) {
    if (__eq__C9type_infoRC9type_info(self, target)) {
        result->object = object;
        result->marker = 8;
        result->access_path = access_path;
        return (access_path & 6) == 4;
    }
    const NorrathSiTypeInfo* base = self->base;
    const NorrathTypeInfoSlot* slot = base->table + 2;
    return reinterpret_cast<NorrathUpcastFn>(slot->function)(
        reinterpret_cast<const char*>(base) + slot->adjustment, access_path, target, object, result);
}
}
