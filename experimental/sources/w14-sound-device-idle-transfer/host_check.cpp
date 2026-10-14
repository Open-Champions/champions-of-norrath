// Host check only; not a build input.
#define NORRATH_HOST_ARENA 1
#include <string>
#include <vector>
#include <cstring>
static unsigned char arena[0x10000];
extern "C" unsigned char* norrath_host_pointer(unsigned address) { return arena + address; }
extern "C" int norrath_overlay_sIopFunctionData_11edf08[2];
extern "C" int norrath_overlay_sRecv_11edd00[2];
int norrath_overlay_sIopFunctionData_11edf08[2];
int norrath_overlay_sRecv_11edd00[2];
static std::vector<std::string> log_;
static int stat_result;
static unsigned set_dma_result;
extern "C" int sceSifDmaStat(unsigned id) { log_.push_back("stat" + std::to_string(id)); return stat_result; }
extern "C" unsigned sceSifSetDma(void* d, int n) {
    const unsigned* p = static_cast<const unsigned*>(d);
    log_.push_back("setdma" + std::to_string(p[0]) + "," + std::to_string(p[1]) + "," + std::to_string(p[2]) + "," + std::to_string(p[3]) + "," + std::to_string(n));
    return set_dma_result;
}
extern "C" void FlushCache(int m) { log_.push_back("flush" + std::to_string(m)); }
extern "C" void SetIopFunction__FUii(unsigned f, int n) {
    log_.push_back("iop" + std::to_string(f) + "," + std::to_string(n) + "," + std::to_string(norrath_overlay_sIopFunctionData_11edf08[0]) + "," +
                   std::to_string(norrath_overlay_sIopFunctionData_11edf08[1]));
}
extern "C" void __builtin_delete(void* p) { log_.push_back("delete" + std::to_string(reinterpret_cast<unsigned long>(p))); }
extern "C" int printf(const char* f, ...) { log_.push_back(std::string("print:") + f); return 0; }
#include "idle_transfer.cpp"
int main() {
    alignas(8) static unsigned char dev[8192];
    unsigned* w = reinterpret_cast<unsigned*>(dev);
    auto reset = [&]() {
        std::memset(dev, 0, sizeof dev);
        std::memset(arena, 0, sizeof arena);
        log_.clear();
        norrath_overlay_sRecv_11edd00[1] = 0;
        norrath_overlay_sIopFunctionData_11edf08[0] = norrath_overlay_sIopFunctionData_11edf08[1] = 0;
    };
    // A: still running
    reset(); w[3408 / 4] = 9; stat_result = 3;
    if (IdleTransfer__13VISoundDevice(dev) != 0 || log_.size() != 1 || log_[0] != "stat9" || w[3408 / 4] != 9) return 1;
    // A: finished chunk, more to come, xm style destination (3388 != -1)
    reset(); w[3408 / 4] = 9; stat_result = -1; w[3388 / 4] = 100; w[3404 / 4] = 40; w[3384 / 4] = 500; w[3392 / 4] = 0xffffffffu;
    if (IdleTransfer__13VISoundDevice(dev) != 0 || w[3408 / 4] != 0 || w[3388 / 4] != 140) return 2;
    if (log_.size() != 2 || log_[1] != "iop1,2,40,100") return 3;
    if (w[3412 / 4] != 0) return 4;
    // A: finished everything, buffer, second iop call, result flag
    reset(); w[3408 / 4] = 9; stat_result = -1; w[3388 / 4] = 0xffffffffu; w[3384 / 4] = 0; w[3420 / 4] = 0x1234; w[3392 / 4] = 77; w[3396 / 4] = 5; w[3400 / 4] = 6;
    w[3416 / 4] = 0x800;
    if (IdleTransfer__13VISoundDevice(dev) != 0) return 5;
    if (w[3412 / 4] != 1 || w[3420 / 4] != 0 || *reinterpret_cast<unsigned*>(arena + 0x800) != 1) return 6;
    if (log_.size() != 3 || log_[1] != "delete4660" || log_[2] != "iop7,2,5,6") return 7;
    // A: finished everything, nothing optional
    reset(); w[3408 / 4] = 9; stat_result = -1; w[3388 / 4] = 0xffffffffu; w[3392 / 4] = 0xffffffffu;
    if (IdleTransfer__13VISoundDevice(dev) != 0 || w[3412 / 4] != 1 || log_.size() != 1) return 8;
    // B: nothing left, negative left
    reset();
    if (IdleTransfer__13VISoundDevice(dev) != 0 || !log_.empty()) return 9;
    reset(); w[3384 / 4] = 0xfffffff0u;
    if (IdleTransfer__13VISoundDevice(dev) != 0 || w[3384 / 4] != 0 || !log_.empty()) return 10;
    // B: busy receiver
    reset(); w[3384 / 4] = 100; norrath_overlay_sRecv_11edd00[1] = 1;
    if (IdleTransfer__13VISoundDevice(dev) != 0 || !log_.empty() || w[3384 / 4] != 100) return 11;
    // B: small chunk rounded to 64, adpcm destination (3388 != -1 -> +3368)
    reset(); w[3384 / 4] = 100; w[3380 / 4] = 0x1000; w[3388 / 4] = 7; w[3368 / 4] = 0x2000; w[3392 / 4] = 0x3000; set_dma_result = 5;
    if (IdleTransfer__13VISoundDevice(dev) != 0) return 12;
    if (log_.size() != 3 || log_[0] != "flush0" || log_[1] != "flush0" || log_[2] != "setdma4096,8192,128,0,1") return 13;
    if (w[3408 / 4] != 5 || w[3404 / 4] != 128 || w[3380 / 4] != 0x1000 + 128 || w[3384 / 4] != 0 || w[3392 / 4] != 0x3000) return 14;
    // B: large chunk, xm destination (3388 == -1 -> +3392 advances)
    reset(); w[3384 / 4] = 0x30000; w[3380 / 4] = 0x1000; w[3388 / 4] = 0xffffffffu; w[3368 / 4] = 0x2000; w[3392 / 4] = 0x3000; set_dma_result = 6;
    if (IdleTransfer__13VISoundDevice(dev) != 0 || log_[2] != "setdma4096,12288,65536,0,1") return 15;
    if (w[3404 / 4] != 0x10000 || w[3384 / 4] != 0x20000 || w[3392 / 4] != 0x13000) return 16;
    // B: exact boundary 0xffff and 0x10000
    reset(); w[3384 / 4] = 0xffff; w[3388 / 4] = 1; set_dma_result = 1;
    IdleTransfer__13VISoundDevice(dev);
    if (w[3404 / 4] != 0x10000 || w[3384 / 4] != 0) return 17;
    reset(); w[3384 / 4] = 0x10000; w[3388 / 4] = 1; set_dma_result = 1;
    IdleTransfer__13VISoundDevice(dev);
    if (w[3404 / 4] != 0x10000 || w[3384 / 4] != 0) return 18;
    // B: submission fails
    reset(); w[3384 / 4] = 100; w[3388 / 4] = 1; set_dma_result = 0;
    if (IdleTransfer__13VISoundDevice(dev) != -1) return 19;
    if (log_.back() != "print:Sample data transfer failed..\n" || w[3408 / 4] != 0 || w[3384 / 4] != 100 || w[3404 / 4] != 0) return 20;
    return 0;
}
