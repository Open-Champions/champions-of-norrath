// Observed do_dyncast__C14__si_type_info... at 0x002fd708, 288 bytes. Arguments
// after this: offset (int), access path, target type_info, object pointer,
// source type_info, source subobject pointer, result record. Always returns 0 on
// the paths handled here; otherwise the base result.
//   1. If object == source subobject and this type equals the source type_info
//      (__eq__C9type_infoRC9type_info): result word 2 = access path, return 0.
//   2. Else if this type equals the target: result word 0 = object, word 1 = access
//      path, and word 3 is 1, except: for offset >= 0 word 3 = 6 when
//      (source subobject - object) == offset, otherwise 1; for offset == -2 word 3
//      = 1; for any other negative offset word 3 is left alone. Returns 0.
//   3. Else the call is forwarded to the base type_info in word 2 of this object:
//      slot 3 (offset 24) of the table at word 1 of the base, base pointer plus the
//      halfword adjustment, all other arguments unchanged; its result is returned.
// Inferred: word 2 is the base type_info (see __rtti_si).
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

struct NorrathDyncastResult {
    void* object;
    int access_path;
    int source_path;
    int found;
};

typedef int (*NorrathDyncastFn)(const void* self, int offset, int access_path, const void* target, void* object,
                                const void* source_type, void* source_subobject, NorrathDyncastResult* result);

int do_dyncast__C14__si_type_infoiQ216__user_type_info8sub_kindRC9type_infoPvT3T4RQ216__user_type_info14dyncast_result(
    const NorrathSiTypeInfo* self, int offset, int access_path, const void* target, void* object,
    const void* source_type, void* source_subobject, NorrathDyncastResult* result) {
    if (object == source_subobject && __eq__C9type_infoRC9type_info(self, source_type)) {
        result->source_path = access_path;
        return 0;
    }
    if (__eq__C9type_infoRC9type_info(self, target)) {
        result->access_path = access_path;
        result->object = object;
        if (offset >= 0) {
            result->found = (static_cast<int>(reinterpret_cast<char*>(source_subobject) -
                                              reinterpret_cast<char*>(object)) == offset)
                                ? 6
                                : 1;
        } else if (offset == -2) {
            result->found = 1;
        }
        return 0;
    }
    const NorrathSiTypeInfo* base = self->base;
    const NorrathTypeInfoSlot* slot = base->table + 3;
    return reinterpret_cast<NorrathDyncastFn>(slot->function)(
        reinterpret_cast<const char*>(base) + slot->adjustment, offset, access_path, target, object, source_type,
        source_subobject, result);
}
}
