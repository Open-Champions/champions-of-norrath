// Observed objectCreateByName__FPcG6Point3sPPc at 0x00143d38, 96 bytes.
// Arguments: a name string, a Point3 position (by value), a short type and an array of
// option strings. Calls the function stored in the word userCreateByNameFunction
// (0x00486894, set by objectSetCreateByNameFunction) with the same four arguments,
// the position passed on as a fresh copy. A zero word returns 0 without a call. When the
// callee returns a nonzero object, the name pointer is stored in the object's word at
// +52; the callee's result is returned either way.
// Inferred: the callee returns a GameObject pointer (the setter's mangled name says so);
// the word at +52 is only known to receive the name pointer.
// Unknown: what the word at +52 is used for.
#include "../w8-object-list-math-common/object_types.hpp"

struct CreatedObject {
    unsigned char before_name[52];
    char* name;
};
#if defined(__mips__)
static_assert(__builtin_offsetof(CreatedObject, name) == 52 && sizeof(char*) == 4, "observed name offset");
#endif

using CreateByNameFunction = CreatedObject* (*)(char*, Point3, short, char**);
extern "C" CreateByNameFunction norrath_create_by_name_function_word;

extern "C" CreatedObject* objectCreateByName__FPcG6Point3sPPc(char* name, Point3 position, short type, char** options) {
    CreateByNameFunction create = norrath_create_by_name_function_word;
    if (create == nullptr) return nullptr;
    CreatedObject* object = create(name, position, type, options);
    if (object != nullptr) object->name = name;
    return object;
}
