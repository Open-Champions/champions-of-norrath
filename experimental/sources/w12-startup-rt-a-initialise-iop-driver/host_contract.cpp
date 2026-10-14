// Host contract for InitialiseIopDriver. Not a build-plan input.
//   c++ -std=c++17 -DNORRATH_HOST_CONTRACT -o check host_contract.cpp && ./check
// The RPC call, the semaphore wait and printf are scripted and logged. The two never-ending loops are
// replaced by a thrown marker so the paths that reach them can be asserted.
#include <cstdio>
#include <cstring>
#include <string>
struct Spin {};
extern "C" {
unsigned char GLibData[0x200] __attribute__((aligned(8)));
unsigned char LocalData[0x400] __attribute__((aligned(64)));
void kdm_rpccall_end(void*) {}
void norrath_host_spin_forever() { throw Spin(); }
}
static std::string log_text;
static int rpc_result;
static unsigned short reply_version;
static int reply_status;
extern "C" int sceSifCallRpc(void* client, unsigned function, unsigned mode, void* send, int ssize, void* recv,
                             int rsize, void (*end)(void*), void* arg) {
    char b[160];
    std::snprintf(b, sizeof b, "rpc(%d,%u,%u,%d,%d,%d,%d,%d,%d);", client == GLibData, function, mode,
                  send == LocalData + 704, ssize, recv == LocalData + 704, rsize, end == kdm_rpccall_end, arg == LocalData + 704);
    log_text += b;
    std::memcpy(LocalData + 710, &reply_version, 2);
    std::memcpy(LocalData + 712, &reply_status, 4);
    return rpc_result;
}
extern "C" int WaitSema(int id) { log_text += "wait(" + std::to_string(id) + ");"; return 0; }
extern "C" int printf(const char* f, ...);
int printf(const char* f, ...) {
    // only the one format string used by the routine is expected
    __builtin_va_list ap; __builtin_va_start(ap, f);
    int a = __builtin_va_arg(ap, int), c = __builtin_va_arg(ap, int);
    __builtin_va_end(ap);
    char b[96]; std::snprintf(b, sizeof b, "printf(%s|%d|%d);", f, a, c);
    for (char* p = b; *p; ++p) if (*p == '\n') *p = '~';
    log_text += b;
    return 0;
}
#include "initialise_iop_driver.cpp"
static std::string run(int ret, unsigned short version, int status, bool* spun) {
    std::memset(LocalData, 0xcc, sizeof LocalData);
    std::memset(GLibData, 0, sizeof GLibData);
    unsigned short c1 = 0x1234, c2 = 0x5678; int sem = 99;
    std::memcpy(GLibData + 64, &c1, 2); std::memcpy(GLibData + 68, &c2, 2); std::memcpy(GLibData + 40, &sem, 4);
    log_text.clear(); rpc_result = ret; reply_version = version; reply_status = status; *spun = false;
    try { InitialiseIopDriver(); } catch (const Spin&) { *spun = true; }
    return log_text;
}
int main() {
    int bad = 0;
    bool spun;
    const std::string rpc = "rpc(1,3,1,1,16,1,16,1,1);";
    std::string r = run(0, 0x106, 0, &spun);
    if (spun || r != rpc + "wait(99);printf(driver version = %d.%02d~|1|6);") { std::puts(r.c_str()); ++bad; }
    unsigned short got; int st;
    std::memcpy(&got, LocalData + 704, 2); if (got != 0x1234) ++bad;
    std::memcpy(&got, LocalData + 706, 2); if (got != 0x5678) ++bad;
    std::memcpy(&got, LocalData + 708, 2); if (got != 0x106) ++bad;
    std::memcpy(&st, LocalData + 712, 4); if (st != 0) ++bad;
    r = run(-1, 0x106, 0, &spun);                       // negative result: no wait
    if (spun || r != rpc + "printf(driver version = %d.%02d~|1|6);") { std::puts(r.c_str()); ++bad; }
    r = run(0, 0x105, 0, &spun);                        // version differs: prints, then spins
    if (!spun || r != rpc + "wait(99);printf(driver version = %d.%02d~|1|5);") { std::puts(r.c_str()); ++bad; }
    r = run(0, 0x106, 7, &spun);                        // status set: spins before printing
    if (!spun || r != rpc + "wait(99);") { std::puts(r.c_str()); ++bad; }
    return bad;
}
