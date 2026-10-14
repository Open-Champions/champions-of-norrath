// Host contract for objectSetFilterFunction. Not an EE plan input.
#include <cstdio>

using ObjectFilterFunction = int (*)(char*);
extern "C" ObjectFilterFunction norrath_user_filter_function_word = nullptr;
extern "C" void objectSetFilterFunction__FPFPc_i(ObjectFilterFunction);

static int accept(char*) { return 1; }
static int reject(char*) { return 0; }

int main() {
    int failures = 0;
    objectSetFilterFunction__FPFPc_i(accept);
    if (norrath_user_filter_function_word != accept) { std::fprintf(stderr, "fail: store\n"); ++failures; }
    objectSetFilterFunction__FPFPc_i(reject);
    if (norrath_user_filter_function_word != reject) { std::fprintf(stderr, "fail: replacement\n"); ++failures; }
    objectSetFilterFunction__FPFPc_i(nullptr);
    if (norrath_user_filter_function_word != nullptr) { std::fprintf(stderr, "fail: clear\n"); ++failures; }
    if (failures) return 1;
    std::printf("objectSetFilterFunction contract passed\n");
    return 0;
}
