#include <cstdio>
#include "fs_init_state.cpp"
int main() {
    static_assert(sizeof norrath_fs_rcv_adr_view == 8, "span to ip0.64");
    static_assert(sizeof _rcv_data_cmd == 0x880, "span to _iob");
    static_assert(sizeof _sif_FsRcv_Data == 64 && sizeof _sif_FsPoff_Data == 64, "spans");
    for (unsigned w : norrath_fs_rcv_adr_view) if (w) return 1;
    for (unsigned char b : _rcv_data_cmd) if (b) return 2;
    for (unsigned char b : _sif_FsRcv_Data) if (b) return 3;
    for (unsigned char b : _sif_FsPoff_Data) if (b) return 4;
    if (_fs_rcv_bufdbl != 0) return 5;
    std::puts("fs init state contract passed");
    return 0;
}
