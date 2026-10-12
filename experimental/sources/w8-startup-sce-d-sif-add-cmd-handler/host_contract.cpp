#define NORRATH_HOST_CONTRACT 1
#include "sif_add_cmd_handler.cpp"

#include <cstdio>
#include <cstddef>

unsigned norrath_host_gp = 0x00abcdefu;
DataTable _data_table;

int main() {
#if __SIZEOF_POINTER__ == 4
    static_assert(offsetof(DataTable, negative_ids) == 12, "offset");
    static_assert(offsetof(DataTable, nonnegative_ids) == 20, "offset");
    static_assert(sizeof(HandlerEntry) == 12, "entry");
#endif
    HandlerEntry sys[4] = {}, user[4] = {};
    _data_table.negative_ids = sys;
    _data_table.nonnegative_ids = user;
    int a, b, c;
    sceSifAddCmdHandler(static_cast<int>(0x80000002u), &a, &b);
    if (sys[2].handler != &a || sys[2].argument != &b || sys[2].gp != 0x00abcdefu) return 1;
    for (int i = 0; i < 4; ++i)
        if (i != 2 && (sys[i].handler || sys[i].argument || sys[i].gp)) return 2;
    for (int i = 0; i < 4; ++i)
        if (user[i].handler || user[i].argument || user[i].gp) return 3;
    norrath_host_gp = 0x00123456u;
    sceSifAddCmdHandler(3, &c, &a);
    if (user[3].handler != &c || user[3].argument != &a || user[3].gp != 0x00123456u) return 4;
    sceSifAddCmdHandler(0, nullptr, nullptr);
    if (user[0].handler || user[0].argument || user[0].gp != 0x00123456u) return 5;
    sceSifAddCmdHandler(static_cast<int>(0x80000000u), &b, &c);
    if (sys[0].handler != &b || sys[0].argument != &c) return 6;
    std::puts("sif add cmd handler contract passed");
    return 0;
}
