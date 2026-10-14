// Host check only; not a build input.
#include "product_title.cpp"
#include <cstdio>
#include <cstring>

int main() {
    bool ok = sizeof sProductTitle == 64 && std::strlen(sProductTitle) == 27 && std::strcmp(sProductTitle, "EverQuest Online\nAdventures") == 0;
    for (int i = 28; i < 64; ++i) ok = ok && sProductTitle[i] == 0;
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
