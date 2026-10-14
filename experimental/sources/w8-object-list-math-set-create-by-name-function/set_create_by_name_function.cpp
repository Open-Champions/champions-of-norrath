// Observed objectSetCreateByNameFunction__FPFPcG6Point3sPPc_P10GameObject at
// 0x143d28, extent 8. Stores its argument in the word userCreateByNameFunction at
// 0x486894 (gp-relative) and returns nothing. The word is read by
// objectCreateByName; this function does not call it. The authored view of that
// word is norrath_create_by_name_function_word (object-infra-state).
#include "../w8-object-list-math-common/object_types.hpp"

struct GameObject;
using CreateByNameFunction = GameObject* (*)(char*, Point3, short, char**);

extern "C" CreateByNameFunction norrath_create_by_name_function_word;

extern "C" void objectSetCreateByNameFunction__FPFPcG6Point3sPPc_P10GameObject(CreateByNameFunction function) {
    norrath_create_by_name_function_word = function;
}
