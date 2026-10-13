// Observed do_find_public_subobj__C16__user_type_info... at 0x002fd640, 20 bytes
// (no calls, no stack). The int and the target type_info are ignored. Returns 6
// when the two pointer arguments (object, subobject) are equal and 1 otherwise.
// Unknown: the enumerator names behind 6 and 1.
extern "C" {
int do_find_public_subobj__C16__user_type_infoiRC9type_infoPvT3(const void*, int, const void*, void* object,
                                                              void* subobject) {
    return object == subobject ? 6 : 1;
}
}
