// Host check only; not a build input. Build: c++ -std=c++17 -Wall -Wextra -fno-builtin -DNORRATH_HOST_ARENA host_check.cpp
#include <cstdio>
#include <string>
#include "../w9-animation-object-c-common/host_arena.hpp"
#include "on_get_realm_stats.cpp"
static std::string trace;
static void* seenPage;
extern "C" int printf(const char* format, ...) {
    __builtin_va_list ap; __builtin_va_start(ap, format);
    int a = __builtin_va_arg(ap, int), b = __builtin_va_arg(ap, int);
    __builtin_va_end(ap);
    trace += std::string("printf:") + format + std::to_string(a) + "," + std::to_string(b) + ";";
    return 0;
}
extern "C" int sprintf(char* out, const char* format, ...) {
    __builtin_va_list ap; __builtin_va_start(ap, format);
    int v = __builtin_va_arg(ap, int);
    __builtin_va_end(ap);
    trace += std::string("sprintf:") + format + ";";
    return std::snprintf(out, 32, "%d", v);
}
extern "C" void SetStringValue__9VIGUIPagePCcT1b(void* page, const char* name, const char* value, int flag) {
    seenPage = page;
    trace += std::string("set:") + name + "=" + value + "," + std::to_string(flag) + ";";
}
int main() {
    using namespace norrath_aoc;
    bool ok = true;
    unsigned char* object = arena_at(0x100);
    unsigned char* renderer = arena_at(0x400);
    unsigned char* page = arena_at(0x800);
    store_pointer(object + 12, renderer);
    store_pointer(renderer + 52, page);
    for (int b : {0, 1, -1, 5}) {
        trace.clear(); seenPage = nullptr;
        onGetRealmStats__16VIRealmInterfaceUiiUiUiUiUiUiUiUiUiPv(object, 9, b, 1234, 0, 0, 0, 0, 0, 0, 0, nullptr);
        std::string want = "printf:VIRealmInterface::onGetRealmStats(%i, %i)\n9," + std::to_string(b) + ";";
        if (b == 0) want += "sprintf:%i;set:PlayersConnected=1234,0;";
        ok = ok && trace == want && (b == 0 ? seenPage == page : seenPage == nullptr);
    }
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
