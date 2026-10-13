#include "always_loaded_page.cpp"
#include <cstring>
int main() {
    if (sizeof(alwaysLoadedPage) != 64) return 1;
    for (unsigned i = 0; i < sizeof(alwaysLoadedPage); i++) if (((const unsigned char*)&alwaysLoadedPage[0])[i] != 0) return 2;
    return 0;
}
