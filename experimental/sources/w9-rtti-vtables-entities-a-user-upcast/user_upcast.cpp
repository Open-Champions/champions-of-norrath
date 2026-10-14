// Observed do_upcast__C16__user_type_info... at 0x002fd548, 96 bytes. This is the
// upcast slot of the plain user type_info table: when the object's own type_info
// equals the target (compared through __eq__C9type_infoRC9type_info) it fills
// the result record and answers whether the access path is public.
// Observed result record words used: +0 the object pointer, +4 the access path
// value passed in, +8 the constant 8. The answer is 1 when (path & 6) == 4, so
// bit 1 and bit 2 of the path decide it; no sub_kind enumerator names are claimed.
// When the types differ nothing is stored and 0 is returned.
// Unknown: what the constant 8 in word 2 means.
extern "C" {
int __eq__C9type_infoRC9type_info(const void* left, const void* right);

struct NorrathUpcastResult {
    void* object;
    int access_path;
    int marker;
};

int do_upcast__C16__user_type_infoQ216__user_type_info8sub_kindRC9type_infoPvRQ216__user_type_info13upcast_result(
    const void* self, int access_path, const void* target, void* object, NorrathUpcastResult* result) {
    if (!__eq__C9type_infoRC9type_info(self, target)) return 0;
    result->object = object;
    result->marker = 8;
    result->access_path = access_path;
    return (access_path & 6) == 4;
}
}
