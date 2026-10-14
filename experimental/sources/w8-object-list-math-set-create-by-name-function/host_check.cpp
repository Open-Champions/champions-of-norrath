// Host contract for objectSetCreateByNameFunction. Not an EE plan input.
#include <cstdio>
#include "../w8-object-list-math-common/object_types.hpp"

struct GameObject;
using CreateByNameFunction = GameObject* (*)(char*, Point3, short, char**);
extern "C" CreateByNameFunction norrath_create_by_name_function_word = nullptr;
extern "C" void objectSetCreateByNameFunction__FPFPcG6Point3sPPc_P10GameObject(CreateByNameFunction);

static GameObject* first(char*, Point3, short, char**) { return nullptr; }
static GameObject* second(char*, Point3, short, char**) { return nullptr; }

int main() {
    int failures = 0;
    objectSetCreateByNameFunction__FPFPcG6Point3sPPc_P10GameObject(first);
    if (norrath_create_by_name_function_word != first) { std::fprintf(stderr, "fail: first store\n"); ++failures; }
    objectSetCreateByNameFunction__FPFPcG6Point3sPPc_P10GameObject(second);
    if (norrath_create_by_name_function_word != second) { std::fprintf(stderr, "fail: replacement\n"); ++failures; }
    objectSetCreateByNameFunction__FPFPcG6Point3sPPc_P10GameObject(nullptr);
    if (norrath_create_by_name_function_word != nullptr) { std::fprintf(stderr, "fail: clear\n"); ++failures; }
    if (failures) return 1;
    std::printf("objectSetCreateByNameFunction contract passed\n");
    return 0;
}
