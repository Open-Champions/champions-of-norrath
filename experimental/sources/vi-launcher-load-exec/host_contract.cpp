#include "load_exec.cpp"

#include <cstring>

static int step;
static const char* seen_path;
static int seen_argc;
static char** seen_argv;
static unsigned seen_filename;
static int seen_exec_argc;
static unsigned seen_exec_arg;
static int error;

static void fail(int code) {
    if (error == 0) error = code;
}

extern "C" unsigned _kExecArg = 0;

extern "C" unsigned SetArg(const char* path, int argc, char** argv) {
    if (step != 0) fail(1);
    seen_path = path;
    seen_argc = argc;
    seen_argv = argv;
    ++step;
    return 0x44u;
}

extern "C" void TerminateLibrary() {
    if (step != 1) fail(2);
    ++step;
}

extern "C" void _LoadExecPS2(unsigned filename, int argc, unsigned exec_arg) {
    if (step != 2) fail(3);
    seen_filename = filename;
    seen_exec_argc = argc;
    seen_exec_arg = exec_arg;
    ++step;
}

int main() {
    char* argv[2] = {const_cast<char*>("language"), const_cast<char*>("1")};
    _kExecArg = 10;
    LoadExecPS2("cdrom0:\\NTGUI2.ELF;1", 2, argv);
    if (error != 0) return error;
    if (step != 3) return 4;
    if (seen_path == nullptr || std::strcmp(seen_path, "cdrom0:\\NTGUI2.ELF;1") != 0) return 5;
    if (seen_argc != 2 || seen_argv != argv) return 6;
    if (seen_filename != 0x44u || seen_exec_argc != 2 || seen_exec_arg != 14) return 7;
    return 0;
}
