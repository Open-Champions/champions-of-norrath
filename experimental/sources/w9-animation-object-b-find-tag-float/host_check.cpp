// Host contract for findTagFloat__10GameObjectPci. Not an EE plan input.
#include <cstdio>
#include <cstdlib>
#include <cstring>
struct TagObject { unsigned char before_tags[44]; char** tags; };
extern "C" float findTagFloat__10GameObjectPci(TagObject*, char*, int);
extern "C" unsigned char norrath_factory_ctype_minimum_view[384] = {0};
static int failures, strlen_calls, atof_calls;
static void expect(bool ok, const char* m) { if (!ok) { std::fprintf(stderr, "fail: %s\n", m); ++failures; } }
extern "C" unsigned norrath_factory_strlen(const char* t) { ++strlen_calls; return (unsigned)std::strlen(t); }
extern "C" int norrath_factory_strncmp(const char* a, const char* b, unsigned n) { return std::strncmp(a, b, n); }
// The host stand-ins carry the double as its value bits so the narrowing is visible.
extern "C" unsigned long long norrath_player_atof(const char* t) {
    ++atof_calls; double d = std::atof(t); unsigned long long bits; std::memcpy(&bits, &d, 8); return bits;
}
extern "C" float norrath_player_double_to_float(unsigned long long bits) { double d; std::memcpy(&d, &bits, 8); return (float)d; }

int main() {
    for (int c = 'A'; c <= 'Z'; ++c) norrath_factory_ctype_minimum_view[c + 128] = 1;
    char e0[] = "scale=2.5", e1[] = "scalefactor=9", e2[] = "scale=0.25", e3[] = "other=7", e4[] = "scale";
    char* list[] = {e1, e0, e4, e3, e2, nullptr};
    TagObject object;
    std::memset(&object, 0, sizeof object);
    char q[16];
    std::strcpy(q, "SCALE");
    expect(findTagFloat__10GameObjectPci(&object, q, 0) == 0.0f && strlen_calls == 0, "no list returns 0 before touching the name");
    expect(std::strcmp(q, "SCALE") == 0, "no list leaves the name unchanged");
    object.tags = list;
    expect(findTagFloat__10GameObjectPci(&object, q, 0) == 2.5f, "first hit parsed (prefix-only entry skipped)");
    expect(std::strcmp(q, "scale") == 0, "name lowercased in place");
    std::strcpy(q, "scale");
    expect(findTagFloat__10GameObjectPci(&object, q, 1) == 0.25f, "second hit; an entry with no '=' is not a hit");
    std::strcpy(q, "scale");
    expect(findTagFloat__10GameObjectPci(&object, q, 2) == 0.0f, "too few hits returns 0");
    std::strcpy(q, "other");
    atof_calls = 0;
    expect(findTagFloat__10GameObjectPci(&object, q, 0) == 7.0f && atof_calls == 1, "later entry; atof called once");
    std::strcpy(q, "nope");
    atof_calls = 0;
    expect(findTagFloat__10GameObjectPci(&object, q, 0) == 0.0f && atof_calls == 0, "absent name");
    char* empty[] = {nullptr};
    object.tags = empty;
    std::strcpy(q, "scale");
    expect(findTagFloat__10GameObjectPci(&object, q, 0) == 0.0f, "empty list");
    if (failures) return 1;
    std::printf("findTagFloat contract passed\n");
    return 0;
}
