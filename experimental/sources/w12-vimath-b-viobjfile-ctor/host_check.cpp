// Host check only; not a build input. Build with the host C++ compiler. Checks the call order and
// that both callees receive the object pointer.
#include "ctor.cpp"
#include <cstdio>
#include <string>

static std::string trace;
static void* seen[2];
extern "C" {
void __6VIFile(void* self) { trace += "base;"; seen[0] = self; }
void ResetState__9VIObjFile(void* self) { trace += "reset;"; seen[1] = self; }
}

int main() {
    alignas(16) unsigned char object[1024];
    __9VIObjFile(object);
    bool ok = trace == "base;reset;" && seen[0] == object && seen[1] == object;
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
