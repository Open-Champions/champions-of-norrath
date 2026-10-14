#include "load_irx.cpp"

#include <cstdio>
#include <cstring>

static int prints, formats, loads, result_script[8];
static char printed[64], formatted[64], loaded_path[3][64];
static int format_size, loaded_len[3];
static const char* loaded_args[3];

int printf(const char* format, ...) {
    ++prints;
    __builtin_va_list ap;
    __builtin_va_start(ap, format);
    std::snprintf(printed, sizeof printed, "%s|%s", format, __builtin_va_arg(ap, const char*));
    __builtin_va_end(ap);
    return 0;
}
int snprintf__8VIStringPciPCce(char* buffer, int size, const char* format, ...) {
    ++formats;
    format_size = size;
    __builtin_va_list ap;
    __builtin_va_start(ap, format);
    const char* name = __builtin_va_arg(ap, const char*);
    __builtin_va_end(ap);
    std::snprintf(buffer, static_cast<size_t>(size), "%s|%s", format, name);
    std::strcpy(formatted, buffer);
    return 0;
}
int sceSifLoadModule(const char* path, int length, const char* arguments) {
    if (loads < 3) {
        std::strcpy(loaded_path[loads], path);
        loaded_len[loads] = length;
        loaded_args[loads] = arguments;
    }
    return result_script[loads++];
}

int main() {
    static const char args[] = "x";
    result_script[0] = 9;
    if (VIEELoadIrx__FPCciT0("SIO2MAN.IRX", 5, args) != 9 || loads != 1) return 1;
    if (prints != 1 || std::strcmp(printed, "Loading module %s\n|SIO2MAN.IRX") != 0) return 2;
    if (formats != 1 || format_size != 256) return 3;
    if (std::strcmp(formatted, "cdrom0:\\MODULES\\%s;1|SIO2MAN.IRX") != 0) return 4;
    if (std::strcmp(loaded_path[0], formatted) != 0 || loaded_len[0] != 5 || loaded_args[0] != args) return 5;
    loads = prints = formats = 0;
    result_script[0] = -1; result_script[1] = -200; result_script[2] = 0;
    if (VIEELoadIrx__FPCciT0("A", 0, nullptr) != 0 || loads != 3 || prints != 1 || formats != 1) return 6;
    if (std::strcmp(loaded_path[2], loaded_path[0]) != 0 || loaded_args[2] != nullptr) return 7;
    std::puts("VIEELoadIrx contract passed");
    return 0;
}
