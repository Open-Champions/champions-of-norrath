// Host check only; not an EE input. __rtti_user is a recording stub; the node is a local array.
#include "typeinfo.cpp"
#include <cstdio>
#include <cstring>
#include <string>

unsigned char __ti10VIResource[8] __attribute__((aligned(4)));
static std::string calls;
extern "C" void __rtti_user(void* t, const char* name) {
    calls += std::string("user(") + (t == __ti10VIResource ? "10VIResource" : "?") + "," + name + ");";
    static_cast<unsigned char*>(t)[0] = 1;
}

int main() {
    int failures = 0;
    // Zero first word: one registration call with this node and the class name, then the node is returned.
    std::memset(__ti10VIResource, 0, sizeof __ti10VIResource);
    calls.clear();
    if (__tf10VIResource() != __ti10VIResource) { std::fprintf(stderr, "fail: zero word return\n"); ++failures; }
    if (calls != "user(10VIResource,10VIResource);") { std::fprintf(stderr, "fail: zero word calls %s\n", calls.c_str()); ++failures; }
    // Nonzero first word, with other bytes set: returned at once, no call.
    const unsigned char set_words[3][4] = {{1, 0, 0, 0}, {0xff, 0xff, 0xff, 0xff}, {0, 0, 0, 0x80}};
    for (const auto& w : set_words) {
        std::memset(__ti10VIResource, 0x5a, sizeof __ti10VIResource);
        std::memcpy(__ti10VIResource, w, 4);
        calls.clear();
        if (__tf10VIResource() != __ti10VIResource) { std::fprintf(stderr, "fail: set word return\n"); ++failures; }
        if (!calls.empty()) { std::fprintf(stderr, "fail: set word calls %s\n", calls.c_str()); ++failures; }
    }
    if (!failures) std::puts("w16-m-data-a-tf-10viresource ok");
    return failures ? 1 : 0;
}
