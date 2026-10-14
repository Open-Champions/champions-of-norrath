// Host check (not a plan input). Build:
//   c++ -O1 -Dstrdup=w16_strdup -o /tmp/w16_strdup host_check.cpp strdup.cpp && /tmp/w16_strdup
#include <cstdio>

extern "C" char* w16_strdup(const char* text);
void* norrath_factory_impure_pointer_word = reinterpret_cast<void*>(0x1234);
static void* seen_reent;
static const char* seen_text;
static int calls;
extern "C" char* _strdup_r(void* reent, const char* text) {
    ++calls;
    seen_reent = reent;
    seen_text = text;
    return reinterpret_cast<char*>(0x7777);
}

int main() {
    const char* text = "abc";
    if (w16_strdup(text) != reinterpret_cast<char*>(0x7777)) return 1;
    if (calls != 1 || seen_reent != reinterpret_cast<void*>(0x1234) || seen_text != text) return 2;
    norrath_factory_impure_pointer_word = reinterpret_cast<void*>(0x4321);
    w16_strdup(nullptr);
    if (calls != 2 || seen_reent != reinterpret_cast<void*>(0x4321) || seen_text != nullptr) return 3;
    std::printf("3 strdup checks passed\n");
    return 0;
}
