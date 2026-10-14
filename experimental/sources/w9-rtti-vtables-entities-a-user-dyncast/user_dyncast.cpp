// Observed do_dyncast__C16__user_type_info... at 0x002fd5a8, 152 bytes. Arguments
// after the type_info: int (unused), access path, target type_info, object
// pointer, source type_info, source subobject pointer, result record. It always
// returns 0. Two independent checks, using __eq__C9type_infoRC9type_info:
//   1. if the object pointer equals the source subobject pointer and this type
//      equals the source type_info: result word 2 = access path;
//   2. if this type equals the target: result word 0 = object pointer,
//      word 1 = access path, word 3 = 1.
// Both can apply in one call. When the pointers differ the first test is skipped
// without calling __eq__ for it.
extern "C" {
int __eq__C9type_infoRC9type_info(const void* left, const void* right);

struct NorrathDyncastResult {
    void* object;
    int access_path;
    int source_path;
    int found;
};

int do_dyncast__C16__user_type_infoiQ216__user_type_info8sub_kindRC9type_infoPvT3T4RQ216__user_type_info14dyncast_result(
    const void* self, int, int access_path, const void* target, void* object, const void* source_type,
    void* source_subobject, NorrathDyncastResult* result) {
    if (object == source_subobject && __eq__C9type_infoRC9type_info(self, source_type)) {
        result->source_path = access_path;
        return 0;
    }
    if (__eq__C9type_infoRC9type_info(self, target)) {
        result->object = object;
        result->access_path = access_path;
        result->found = 1;
    }
    return 0;
}
}
