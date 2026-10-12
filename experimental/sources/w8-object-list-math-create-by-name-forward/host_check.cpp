// Host contract for the createByName forwarder. Not an EE plan input.
#include <cstdio>
#include "../w8-object-list-math-common/object_types.hpp"

static char* seen_name;
static Point3 seen_position;
static short seen_type;
static char** seen_arguments;
static int calls;
static unsigned char answer[4];
extern "C" unsigned char* norrath_game_create_by_name(char* name, Point3 position, short type, char** arguments) {
    ++calls; seen_name = name; seen_position = position; seen_type = type; seen_arguments = arguments;
    return answer;
}
extern "C" unsigned char* createByName__FPcG6Point3sPPc(char*, Point3, short, char**);

int main() {
    int failures = 0;
    auto expect = [&](bool ok, const char* m) { if (!ok) { std::fprintf(stderr, "fail: %s\n", m); ++failures; } };
    char name[] = "Goblin";
    char* arguments[] = {name, nullptr};
    unsigned char* r = createByName__FPcG6Point3sPPc(name, Point3{1.5f, -2.5f, 3.5f}, static_cast<short>(-7), arguments);
    expect(calls == 1 && r == answer, "one call, result returned");
    expect(seen_name == name && seen_arguments == arguments && seen_type == -7, "name, type and argument list forwarded");
    expect(seen_position.x == 1.5f && seen_position.y == -2.5f && seen_position.z == 3.5f, "position forwarded by value");
    if (failures) return 1;
    std::printf("createByName forwarder contract passed\n");
    return 0;
}
