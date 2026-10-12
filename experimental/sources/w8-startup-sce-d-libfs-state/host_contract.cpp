#include "libfs_state.cpp"

#include <cstdio>

int main() {
    static_assert(sizeof _send_data == 0x8e1100 - 0x8e04c0, "span to _rcv_data_rpc");
    static_assert(sizeof _rcv_data_rpc == 0x8e1140 - 0x8e1100, "span to _rcv_data_cmd");
    static_assert(sizeof _iob == 0x8e1bc0 - 0x8e19c0, "span to _cd");
    static_assert(sizeof _cd == 0x8e1be8 - 0x8e1bc0, "span to _fsversion");
    static_assert(sizeof _sceFs_q == 128, "symbol size");
    for (unsigned char b : _send_data) if (b) return 1;
    for (unsigned char b : _rcv_data_rpc) if (b) return 2;
    for (unsigned char b : _iob) if (b) return 3;
    for (unsigned char b : _cd) if (b) return 4;
    for (int w : _sceFs_q) if (w != -1) return 5;
    if (_fs_fsq_semid != -1 || _fs_iob_semid != -1) return 6;
    std::puts("libfs state contract passed");
    return 0;
}
