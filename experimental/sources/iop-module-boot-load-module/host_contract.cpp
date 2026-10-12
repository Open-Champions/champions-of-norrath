#include "load_module.cpp"

#include <cstdio>
#include <cstring>

static const char* seen_path;
static int seen_length;
static const void* seen_argument;
static int* seen_result;
static int seen_operation;
static int calls;

extern "C" int _sceSifLoadModule(const char* path, int argument_length, const void* argument,
                                 int* result_word, int operation) {
    ++calls;
    seen_path = path;
    seen_length = argument_length;
    seen_argument = argument;
    seen_result = result_word;
    seen_operation = operation;
    *result_word = 99;
    return 7;
}

int main() {
    char argument[] = {'h', 'u', 'b', 0};
    const int result = sceSifLoadModule("cdrom0:\\bg\\0mod\\usbd.irx;1", 43, argument);
    if (calls != 1 || result != 7 || seen_operation != 0) return 1;
    if (seen_length != 43 || seen_argument != argument || seen_result == nullptr) return 2;
    if (std::strcmp(seen_path, "cdrom0:\\bg\\0mod\\usbd.irx;1") != 0) return 3;
    if (*seen_result != 99) return 4;
    if (sceSifLoadModule("only", 0, nullptr) != 7 || seen_length != 0 || seen_argument != nullptr) return 5;
    std::puts("sceSifLoadModule passed");
    return 0;
}
