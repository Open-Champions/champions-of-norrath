// Host check only; not a build input. Build: c++ -std=c++17 -Wall -Wextra -fno-builtin host_check.cpp
#include "on_disconnect.cpp"
#include <cstdio>
#include <string>
int connected = 1;
static std::string trace;
extern "C" int printf(const char* format, ...) {
    trace += std::string("printf:") + format + ";connected=" + std::to_string(connected) + ";";
    return 0;
}
int main() {
    unsigned char object[16] = {1, 2, 3};
    onDisconnect__16VIRealmInterface(object);
    bool ok = connected == 0 && object[0] == 1 && object[2] == 3 &&
              trace == "printf:Disconnected from realm interface\n;connected=0;";
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
