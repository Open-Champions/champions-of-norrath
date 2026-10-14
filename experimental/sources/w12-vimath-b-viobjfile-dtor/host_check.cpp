// Host check only; not a build input. Build with the host C++ compiler. Runs flags 0 to 3 with the
// word at offset 560 zero and nonzero and checks the call order, arguments and an untouched object.
#include "dtor.cpp"
#include <cstdio>
#include <cstring>
#include <string>

static std::string trace;
static void* where[3];
extern "C" {
void Close__9VIObjFile(void* self) { trace += "close;"; where[0] = self; }
void _$_6VIFile(void* self, int flags) { trace += "base" + std::to_string(flags) + ";"; where[1] = self; }
void __builtin_delete(void* pointer) { trace += "del;"; where[2] = pointer; }
}

int main() {
    bool ok = true;
    for (int open = 0; open < 2; ++open)
        for (int flags = 0; flags < 4; ++flags) {
            alignas(16) unsigned char object[1024];
            std::memset(object, 0x21, sizeof object);
            int word = open ? 0x1234 : 0;
            std::memcpy(object + 560, &word, 4);
            unsigned char before[1024];
            std::memcpy(before, object, sizeof object);
            trace.clear();
            where[0] = where[1] = where[2] = nullptr;
            _$_9VIObjFile(object, flags);
            std::string want = std::string(open ? "close;" : "") + "base2;" + ((flags & 1) ? "del;" : "");
            ok = ok && trace == want && where[1] == object && (!open || where[0] == object) &&
                 (!(flags & 1) || where[2] == object) && std::memcmp(before, object, sizeof object) == 0;
        }
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
