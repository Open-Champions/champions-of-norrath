// Host comparison only. Not an EE build input.
#include <cstdio>
#include <cstring>

static int strcmp_calls = 0;
extern "C" int strcmp(const char* left, const char* right) {
    ++strcmp_calls;
    for (;; ++left, ++right) {
        const unsigned char a = static_cast<unsigned char>(*left), b = static_cast<unsigned char>(*right);
        if (a != b || a == 0) return int(a) - int(b);
    }
}
#include "type_info_eq.cpp"

struct Fake { const char* name; unsigned pad[2]; };

int main() {
    char first_text[] = "10GameObject";
    char same_text[] = "10GameObject";
    char other_text[] = "9ItemProp";
    Fake a{first_text, {}}, b{same_text, {}}, c{other_text, {}}, prefix{"10GameObjec", {}};
    int failures = 0;
    if (__eq__C9type_infoRC9type_info(&a, &a) != 1 || strcmp_calls != 0) { std::printf("identity must not compare names\n"); ++failures; }
    if (__eq__C9type_infoRC9type_info(&a, &b) != 1) { std::printf("equal names must match\n"); ++failures; }
    if (__eq__C9type_infoRC9type_info(&a, &c) != 0) { std::printf("different names must differ\n"); ++failures; }
    if (__eq__C9type_infoRC9type_info(&c, &a) != 0) { std::printf("different names must differ (swapped)\n"); ++failures; }
    if (__eq__C9type_infoRC9type_info(&a, &prefix) != 0) { std::printf("prefix must differ\n"); ++failures; }
    if (failures) return 1;
    std::printf("type_info equality cases passed\n");
    return 0;
}
