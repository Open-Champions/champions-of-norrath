// Host check for the find-animation naming bridge. Not an EE plan input.
#include <cstdio>
extern "C" void* norrath_object_find_animation(void*, char*);
static void *seen_object, *answer;
static char* seen_name;
extern "C" void* objectFindAnimation__FP10GameObjectPc(void* o, char* n) { seen_object = o; seen_name = n; return answer; }
int main() {
    int failures = 0, object, result;
    char name[] = "walk";
    answer = &result;
    void* r = norrath_object_find_animation(&object, name);
    if (r != &result || seen_object != &object || seen_name != name) { std::fprintf(stderr, "fail: forwarding\n"); ++failures; }
    answer = nullptr;
    if (norrath_object_find_animation(&object, name) != nullptr) { std::fprintf(stderr, "fail: null result\n"); ++failures; }
    if (failures) return 1;
    std::printf("find-animation bridge check passed\n");
    return 0;
}
