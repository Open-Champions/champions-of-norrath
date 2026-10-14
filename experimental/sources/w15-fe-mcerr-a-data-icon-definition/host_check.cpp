// Host check only; not a build input.
#include "icon_definition.cpp"
#include <cstdio>

int main() {
    bool ok = sizeof sIconDefinition == 972;
    for (unsigned char v : sIconDefinition) ok = ok && v == 0;
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
