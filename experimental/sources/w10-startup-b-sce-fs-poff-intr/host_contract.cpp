/* Host contract. Not a plan input. The global pointer is a host variable and the
 * callback records the global pointer value it observes. */
#define NORRATH_HOST_CONTRACT 1
#include "sce_fs_poff_intr.cpp"
#include <cstdio>
#include <vector>
extern "C" { unsigned norrath_host_gp = 0x1111u; unsigned sceCbPoff_Gp = 0x2222u; }
static std::vector<unsigned> events;
extern "C" void norrath_host_sync_ei() { events.push_back(0xe1000000u | norrath_host_gp); }
static void callback(unsigned argument) { events.push_back(argument); events.push_back(norrath_host_gp); }
int main() {
    unsigned block[2] = {0, 0x77};
    _sceFs_Poff_Intr(nullptr, block);
    if (events != std::vector<unsigned>{0xe1001111u}) { std::puts("no-callback path wrong"); return 1; }
    events.clear();
    block[0] = static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(callback));
    _sceFs_Poff_Intr(nullptr, block);
    if (events != std::vector<unsigned>{0x77u, 0x2222u, 0xe1001111u} || norrath_host_gp != 0x1111u) { std::puts("callback path wrong"); return 1; }
    std::puts("_sceFs_Poff_Intr contract passed");
    return 0;
}
