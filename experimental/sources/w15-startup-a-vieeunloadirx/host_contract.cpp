#include "unload_irx.cpp"

#include <cstdio>

static int order[4], norder, stop_id, stop_len, stop_args_null, stop_result_nonnull, unload_id;

int sceSifStopModule(int id, int length, const char* arguments, int* result) {
    order[norder++] = 1;
    stop_id = id;
    stop_len = length;
    stop_args_null = arguments == nullptr;
    stop_result_nonnull = result != nullptr;
    return 0;
}
int sceSifUnloadModule(int id) {
    order[norder++] = 2;
    unload_id = id;
    return 0;
}

int main() {
    VIEEUnloadIrx__Fi(77);
    if (norder != 2 || order[0] != 1 || order[1] != 2) return 1;
    if (stop_id != 77 || stop_len != 0 || !stop_args_null || !stop_result_nonnull) return 2;
    if (unload_id != 77) return 3;
    norder = 0;
    VIEEUnloadIrx__Fi(-3);
    if (stop_id != -3 || unload_id != -3) return 4;
    std::puts("VIEEUnloadIrx contract passed");
    return 0;
}
