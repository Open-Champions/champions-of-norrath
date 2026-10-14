#include <cstdio>
#include <cstring>

extern "C" void* norrath_vt_tf_10GameObject();
extern "C" unsigned char norrath_ti_10GameObject[8];

static int failures = 0;
static int calls = 0;
static const char* seen = nullptr;
static void* seen_object = nullptr;

static void expect(bool ok, const char* message) {
    if (!ok) {
        std::fprintf(stderr, "fail: %s\n", message);
        ++failures;
    }
}

extern "C" void norrath_rtti_user(void* typeinfo, const char* name) {
    ++calls;
    seen = name;
    seen_object = typeinfo;
    auto* raw = static_cast<unsigned char*>(typeinfo);
    raw[0] = 1;
}

int main() {
    std::memset(norrath_ti_10GameObject, 0, 8);
    void* first = norrath_vt_tf_10GameObject();
    expect(first == norrath_ti_10GameObject, "returns the typeinfo object");
    expect(calls == 1 && seen_object == norrath_ti_10GameObject, "initializes a zero typeinfo");
    expect(seen && std::strcmp(seen, "10GameObject") == 0, "passes the observed class name");
    void* second = norrath_vt_tf_10GameObject();
    expect(second == first && calls == 1, "a stored first word skips initialization");

    calls = 0;
    norrath_ti_10GameObject[0] = 0;
    norrath_ti_10GameObject[4] = 9;
    norrath_ti_10GameObject[1] = 0;
    norrath_ti_10GameObject[2] = 0;
    norrath_ti_10GameObject[3] = 0;
    // First word is zero again. A nonzero later byte does not suppress the call.
    norrath_ti_10GameObject[7] = 4;
    void* third = norrath_vt_tf_10GameObject();
    expect(third == norrath_ti_10GameObject && calls == 1, "only the first word gates the call");
    return failures == 0 ? 0 : 1;
}
