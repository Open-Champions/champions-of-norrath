// Host contract for objectCreateByName__FPcG6Point3sPPc. Not an EE plan input.
#include <cstdio>
#include <cstring>
#include "../w8-object-list-math-common/object_types.hpp"

struct CreatedObject { unsigned char before_name[52]; char* name; };
using CreateByNameFunction = CreatedObject* (*)(char*, Point3, short, char**);
extern "C" CreateByNameFunction norrath_create_by_name_function_word = nullptr;
extern "C" CreatedObject* objectCreateByName__FPcG6Point3sPPc(char*, Point3, short, char**);

static int calls;
static char* seen_name; static Point3 seen_position; static short seen_type; static char** seen_options;
static CreatedObject made;
static CreatedObject* result = &made;
static CreatedObject* maker(char* n, Point3 p, short t, char** o) {
    ++calls; seen_name = n; seen_position = p; seen_type = t; seen_options = o; return result;
}

int main() {
    int failures = 0;
    auto expect = [&](bool ok, const char* m) { if (!ok) { std::fprintf(stderr, "fail: %s\n", m); ++failures; } };
    char name[] = "orc"; char* options[] = {name, nullptr};
    Point3 where = {1.5f, -2.0f, 3.25f};
    expect(objectCreateByName__FPcG6Point3sPPc(name, where, -7, options) == nullptr && calls == 0, "no function: returns 0, no call");
    norrath_create_by_name_function_word = maker;
    std::memset(&made, 0, sizeof made);
    CreatedObject* r = objectCreateByName__FPcG6Point3sPPc(name, where, -7, options);
    expect(r == &made && calls == 1, "returns the callee's object");
    expect(seen_name == name && seen_type == -7 && seen_options == options, "name, type and options passed through");
    expect(seen_position.x == 1.5f && seen_position.y == -2.0f && seen_position.z == 3.25f, "position passed through");
    expect(made.name == name, "name stored at +52");
    result = nullptr;
    made.name = nullptr;
    expect(objectCreateByName__FPcG6Point3sPPc(name, where, 5, nullptr) == nullptr && calls == 2 && made.name == nullptr, "callee returns 0: result 0, nothing stored");
    expect(seen_type == 5 && seen_options == nullptr, "second call arguments");
    if (failures) return 1;
    std::printf("objectCreateByName contract passed\n");
    return 0;
}
