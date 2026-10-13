// Observed __dynamic_cast_2 at 0x002fe7a8, 140 bytes: the run-time cast helper called with type_info accessor
// functions instead of type_info objects. Six arguments: three accessors (from, to, sub), a small integer,
// an object pointer and a second object pointer. It calls the three accessors in the order from, to, sub
// (each with its own original argument registers left alone, no arguments of its own) and then finishes
// with a tail call to dyncast__C16__user_type_infoiRC9type_infoPvT2T3:
//   dyncast(from(), integer, to(), address, sub(), subobject).
// There is no null test on the object pointer here; the one observed caller tests before calling.
// Inferred: the integer is a require-public flag and the last pointer the sub-object, from how the callers
// and the dyncast routine use them; neither name is confirmed.
// Not authored here: dyncast__C16__user_type_infoiRC9type_infoPvT2T3 and the accessors.
extern "C" {
typedef const void* (*NorrathTypeInfoFunction)();

void* dyncast__C16__user_type_infoiRC9type_infoPvT2T3(const void* from, int require_public, const void* to,
                                                       void* address, const void* sub, void* subobject);

void* __dynamic_cast_2(NorrathTypeInfoFunction from, NorrathTypeInfoFunction to, int require_public, void* address,
                       NorrathTypeInfoFunction sub, void* subobject) {
    const void* const from_info = from();
    const void* const to_info = to();
    const void* const sub_info = sub();
    return dyncast__C16__user_type_infoiRC9type_infoPvT2T3(from_info, require_public, to_info, address, sub_info, subobject);
}
}
