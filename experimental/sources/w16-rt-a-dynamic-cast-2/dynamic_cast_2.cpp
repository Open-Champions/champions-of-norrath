// Observed __dynamic_cast_2 at 0x002fe7a8, 140 bytes. Six arguments: three zero-argument
// functions that each return a type-descriptor pointer (first, second and fifth argument),
// an integer (third), and two object pointers (fourth and sixth). The first, second and
// fifth arguments are called in that order, then the body tail-calls
// dyncast__C16__user_type_infoiRC9type_infoPvT2T3 (0x002fd400) with
//   (first result, integer, second result, fourth, fifth result, sixth).
// No null test on either object pointer. Inferred only: the integer is a flag and the
// callees' roles; no name is confirmed.
extern "C" {
typedef const void* (*TypeDescriptorFunction)();

void* dyncast__C16__user_type_infoiRC9type_infoPvT2T3(const void* first, int flag, const void* second,
                                                       void* object, const void* third, void* other);

void* __dynamic_cast_2(TypeDescriptorFunction first, TypeDescriptorFunction second, int flag, void* object,
                       TypeDescriptorFunction third, void* other) {
    const void* const first_result = first();
    const void* const second_result = second();
    const void* const third_result = third();
    return dyncast__C16__user_type_infoiRC9type_infoPvT2T3(first_result, flag, second_result, object,
                                                           third_result, other);
}
}
