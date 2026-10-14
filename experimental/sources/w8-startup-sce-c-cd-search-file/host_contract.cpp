#include <cstdio>
#include "search_file.cpp"
static void* seen_file;
static const char* seen_name;
static int seen_layer = -1, result = 0;
extern "C" int sceCdLayerSearchFile(void* file, const char* name, int layer) {
    seen_file = file; seen_name = name; seen_layer = layer;
    return result;
}
int main() {
    char file[4], name[] = "\\X.BIN;1";
    result = 1;
    if (sceCdSearchFile(file, name) != 1 || seen_file != file || seen_name != name || seen_layer != 0) return 1;
    result = 0; seen_layer = 9;
    if (sceCdSearchFile(nullptr, nullptr) != 0 || seen_layer != 0 || seen_file != nullptr) return 2;
    result = -5;
    if (sceCdSearchFile(file, name) != -5) return 3;
    std::puts("sceCdSearchFile contract passed");
    return 0;
}
