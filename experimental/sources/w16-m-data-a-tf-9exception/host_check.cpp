// Host check only; not an EE input. __rtti_user is a recording stub; the node is a local array.
#include "typeinfo.cpp"
#include <cstdio>
#include <cstring>
#include <string>

unsigned char __ti9exception[8] __attribute__((aligned(4)));
static std::string calls;
extern "C" void __rtti_user(void* t, const char* name) {
    calls += std::string("user(") + (t == __ti9exception ? "9exception" : "?") + "," + name + ");";
    static_cast<unsigned char*>(t)[0] = 1;
}

int main() {
    int failures = 0;
    // Zero first word: one registration call with this node and the class name, then the node is returned.
    std::memset(__ti9exception, 0, sizeof __ti9exception);
    calls.clear();
    if (__tf9exception() != __ti9exception) { std::fprintf(stderr, "fail: zero word return\n"); ++failures; }
    if (calls != "user(9exception,9exception);") { std::fprintf(stderr, "fail: zero word calls %s\n", calls.c_str()); ++failures; }
    // Nonzero first word, with other bytes set: returned at once, no call.
    const unsigned char set_words[3][4] = {{1, 0, 0, 0}, {0xff, 0xff, 0xff, 0xff}, {0, 0, 0, 0x80}};
    for (const auto& w : set_words) {
        std::memset(__ti9exception, 0x5a, sizeof __ti9exception);
        std::memcpy(__ti9exception, w, 4);
        calls.clear();
        if (__tf9exception() != __ti9exception) { std::fprintf(stderr, "fail: set word return\n"); ++failures; }
        if (!calls.empty()) { std::fprintf(stderr, "fail: set word calls %s\n", calls.c_str()); ++failures; }
    }
    if (!failures) std::puts("w16-m-data-a-tf-9exception ok");
    return failures ? 1 : 0;
}
