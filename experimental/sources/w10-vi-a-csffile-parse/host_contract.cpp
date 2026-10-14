#define printf norrath_host_printf
#include "parse.cpp"
#undef printf

#include <cstdarg>
#include <cstring>

static int order[8];
static int count;
static void* file_seen;
static const char* path_seen;
static int open_result;
static int parse_result;
static void* parse_args[4];
static int open_mode;
static int dtor_flags;
static const char* printed_format;
static const char* printed_arg;

extern "C" void __6VIFile(void* file) { file_seen = file; order[count++] = 1; }
extern "C" int norrath_host_printf(const char* format, ...) {
    printed_format = format;
    va_list args;
    va_start(args, format);
    printed_arg = va_arg(args, const char*);
    va_end(args);
    order[count++] = 2;
    return 0;
}
extern "C" int Open__6VIFilePCci(void* file, const char* path, int mode) {
    if (file != file_seen) order[count++] = 99;
    path_seen = path;
    open_mode = mode;
    order[count++] = 3;
    return open_result;
}
extern "C" int Parse__9VICSFFileR6VIFileR10VIESFParseR7VIScene(void* object, void* file, void* parse, void* scene) {
    parse_args[0] = object; parse_args[1] = file; parse_args[2] = parse; parse_args[3] = scene;
    order[count++] = 4;
    return parse_result;
}
extern "C" void _$_6VIFile(void* file, int flags) {
    if (file != file_seen) order[count++] = 99;
    dtor_flags = flags;
    order[count++] = 5;
}

int main() {
    int object, parse, scene;
    open_result = -1;
    int result = Parse__9VICSFFilePCcR10VIESFParseR7VIScene(&object, "a.csf", &parse, &scene);
    if (result != -1 || count != 4 || order[0] != 1 || order[1] != 2 || order[2] != 3 || order[3] != 5) return 1;
    if (open_mode != 33 || dtor_flags != 2 || std::strcmp(path_seen, "a.csf") != 0) return 2;
    if (std::strcmp(printed_format, "Decompressing %s\n") != 0 || printed_arg != path_seen) return 3;

    count = 0;
    open_result = 0;
    parse_result = -4;
    result = Parse__9VICSFFilePCcR10VIESFParseR7VIScene(&object, "b.csf", &parse, &scene);
    if (result != -1 || count != 5 || order[3] != 4 || order[4] != 5) return 4;
    if (parse_args[0] != &object || parse_args[1] != file_seen || parse_args[2] != &parse || parse_args[3] != &scene) return 5;

    count = 0;
    parse_result = 6;
    result = Parse__9VICSFFilePCcR10VIESFParseR7VIScene(&object, "c.csf", &parse, &scene);
    if (result != 0 || count != 5) return 6;
    count = 0;
    open_result = 3;  // any nonnegative open result continues
    parse_result = 0;
    result = Parse__9VICSFFilePCcR10VIESFParseR7VIScene(&object, "d.csf", &parse, &scene);
    if (result != 0 || count != 5) return 7;
    return 0;
}
