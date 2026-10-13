// Observed __throw_type_match_rtti_2 at 0x002fe0d0, 1292 bytes, pinned SLUS_205.65: decides whether a
// catch clause's type accepts a thrown type. Arguments: catch type_info, thrown type_info, the thrown
// object pointer and a pointer to a result word. Returns 1 for "matches with the object pointer
// unchanged", the result of the thrown type's upcast when it is a class type, else 0.
// The type_info records are the old-ABI layout read from the code: word 1 is the vtable pointer
// (the vtable holds a halfword this-adjustment at +0 and the dynamic type_info at +4); a pointer-type
// record keeps its pointee at +8, and a qualifier record keeps its pointee at +8 and its qualifier
// bits at +12 (bit 0 and bit 1, compared as small numbers). Class tests go through __dynamic_cast_2
// with the type_info accessor functions as targets, called here by hand because the build has no
// run-time type information. Read from the code, in this order:
//   1. *result = object; catch == thrown (by __eq__C9type_infoRC9type_info): return 1.
//   2. thrown is a class type (cast to __tf16__user_type_info): return upcast(thrown, catch, object, result).
//   3. otherwise both must be pointer types, else 0. Take both pointees; strip one qualifier record
//      from each (qualifier bits 0 when there is none). Fail (0) when the catch side has fewer bits set
//      than the thrown side for bit 0, then for bit 1. Remember "catch pointee const" (bit 0 of the catch
//      qualifiers) in a flag. Equal pointees: return 1.
//   4. catch pointee is void (== __tfv()): thrown pointee null or not a function type: return 1.
//   5. thrown pointee is a class type: return upcast(it, catch pointee, object, result).
//   6. otherwise both pointees must be pointer types and a loop walks them level by level: strip
//      qualifiers again with the same two failure tests; while the flag is zero also fail when the thrown
//      side has more bit 0 or more bit 1 than the catch side; equal pointees return 1; both next levels
//      must be pointer types (else 0); the flag is cleared when the catch qualifiers lacked bit 0.
// Unknown: the C++ rules this encodes beyond what the branches show, and the meaning of the qualifier
// bits. __dynamic_cast_2, the upcast, __tfv and the type_info accessors are separate routines and are
// imported by name. Nothing was executed on the EE.
extern "C" {
typedef const void* (*TypeInfoFunction)();

int __eq__C9type_infoRC9type_info(const void* a, const void* b);
void* __dynamic_cast_2(const void* dynamic_type, TypeInfoFunction target, int offset, void* most_derived,
                       TypeInfoFunction static_type, void* object);
void* upcast__C16__user_type_infoRC9type_infoPvPPv(void* self, const void* target, void* object, void** result);
const void* __tfv();
const void* __tf9type_info();
const void* __tf16__user_type_info();
const void* __tf19__pointer_type_info();
const void* __tf16__attr_type_info();
const void* __tf16__func_type_info();

static void* cast_to(const void* object, TypeInfoFunction target) {
    if (object == 0) return 0;
    const unsigned char* const vtable = *reinterpret_cast<const unsigned char* const*>(static_cast<const unsigned char*>(object) + 4);
    const int adjust = *reinterpret_cast<const short*>(vtable);
    const void* const dynamic_type = *reinterpret_cast<const void* const*>(vtable + 4);
    return __dynamic_cast_2(dynamic_type, target, 0, const_cast<unsigned char*>(static_cast<const unsigned char*>(object)) + adjust,
                            __tf9type_info, const_cast<void*>(object));
}

static const void* pointee(void* record) { return *reinterpret_cast<const void**>(static_cast<unsigned char*>(record) + 8); }
static int qualifiers(void* record) { return *reinterpret_cast<int*>(static_cast<unsigned char*>(record) + 12); }

// Strips one qualifier record: updates the type and returns its qualifier bits (0 without one).
static int strip(const void*& type) {
    void* const attr = cast_to(type, __tf16__attr_type_info);
    if (attr == 0) return 0;
    type = pointee(attr);
    return qualifiers(attr);
}

void* __throw_type_match_rtti_2(const void* catch_type, const void* thrown_type, void* object, void** result) {
    *result = object;
    if (__eq__C9type_infoRC9type_info(catch_type, thrown_type)) return reinterpret_cast<void*>(1);
    void* const thrown_class = cast_to(thrown_type, __tf16__user_type_info);
    if (thrown_class != 0) return upcast__C16__user_type_infoRC9type_infoPvPPv(thrown_class, catch_type, object, result);
    void* const thrown_pointer = cast_to(thrown_type, __tf19__pointer_type_info);
    if (thrown_pointer == 0) return 0;
    void* const catch_pointer = cast_to(catch_type, __tf19__pointer_type_info);
    if (catch_pointer == 0) return 0;
    const void* from = pointee(thrown_pointer);
    const void* to = pointee(catch_pointer);
    int from_quals = strip(from);
    int to_quals = strip(to);
    int to_const_flag = to_quals & 1;
    if ((to_quals & 1) < (from_quals & 1)) return 0;
    if ((to_quals & 2) < (from_quals & 2)) return 0;
    if (__eq__C9type_infoRC9type_info(to, from)) return reinterpret_cast<void*>(1);
    if (__eq__C9type_infoRC9type_info(to, __tfv())) {
        if (from == 0) return reinterpret_cast<void*>(1);
        if (cast_to(from, __tf16__func_type_info) == 0) return reinterpret_cast<void*>(1);
    }
    void* const from_class = cast_to(from, __tf16__user_type_info);
    if (from_class != 0) return upcast__C16__user_type_infoRC9type_infoPvPPv(from_class, to, object, result);
    void* from_next = cast_to(from, __tf19__pointer_type_info);
    if (from_next == 0) return 0;
    void* to_next = cast_to(to, __tf19__pointer_type_info);
    if (to_next == 0) return 0;
    to = pointee(to_next);
    from = pointee(from_next);
    for (;;) {
        from_quals = strip(from);
        to_quals = strip(to);
        if ((to_quals & 1) < (from_quals & 1)) return 0;
        if ((to_quals & 2) < (from_quals & 2)) return 0;
        if (to_const_flag == 0) {
            if ((from_quals & 1) < (to_quals & 1)) return 0;
            if ((from_quals & 2) < (to_quals & 2)) return 0;
        }
        if (__eq__C9type_infoRC9type_info(to, from)) return reinterpret_cast<void*>(1);
        void* const to_pointer = to == 0 ? 0 : cast_to(to, __tf19__pointer_type_info);
        void* const from_pointer = from == 0 ? 0 : cast_to(from, __tf19__pointer_type_info);
        if (to_pointer == 0) return 0;
        if (from_pointer == 0) return 0;
        to = pointee(to_pointer);
        from = pointee(from_pointer);
        if ((to_quals & 1) == 0) to_const_flag = 0;
    }
}
}
