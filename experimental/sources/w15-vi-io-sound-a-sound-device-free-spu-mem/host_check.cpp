// Host check only; not a build input.
#define NORRATH_HOST_ARENA 1
static unsigned char arena[0x10000];
extern "C" unsigned char* norrath_host_pointer(unsigned address) { return arena + address; }
#include <string>
#include <vector>
static std::vector<std::string> log_;
extern "C" unsigned RemHead__7VILList(unsigned char*) { log_.push_back("rem"); return 0x4000; }
extern "C" void InsertAfter__7VILNodeP7VILNode(unsigned node, unsigned after) { log_.push_back("insert" + std::to_string(node) + "," + std::to_string(after)); }
extern "C" void AddTail__7VILListP7VILNode(unsigned char*, unsigned node) { log_.push_back("tail" + std::to_string(node)); }
extern "C" void MergeFreeSpuMem__13VISoundDevice(unsigned char*) { log_.push_back("merge"); }
#include "free_spu_mem.cpp"
static unsigned& w(unsigned address) { return *reinterpret_cast<unsigned*>(arena + address); }
int main() {
    unsigned char* dev = arena;
    // free list: node A (0x5000, address 100) -> node B (0x5100, address 300) -> end node (0x5200)
    w(3312) = 0x5000;
    w(0x5000) = 0x5100; w(0x5004) = 0x4f00; w(0x5000 + 16) = 100;
    w(0x5100) = 0x5200; w(0x5104) = 0x5000; w(0x5100 + 16) = 300;
    w(0x5200) = 0;      w(0x5204) = 0x5100; w(0x5200 + 16) = 0;
    FreeSpuMem__13VISoundDeviceUii(dev, 200, 100);
    if (w(0x4000 + 16) != 200 || w(0x4000 + 20) != 128) return 1;
    if (log_.size() != 3 || log_[0] != "rem" || log_[1] != "insert16384,20480" || log_[2] != "merge") return 2;
    log_.clear();
    FreeSpuMem__13VISoundDeviceUii(dev, 50, 64);
    if (w(0x4000 + 20) != 64 || log_[1] != "insert16384," + std::to_string(0x4f00)) return 3;
    log_.clear();
    FreeSpuMem__13VISoundDeviceUii(dev, 1000, 1);
    if (w(0x4000 + 20) != 64 || log_.size() != 3 || log_[1] != "tail16384") return 4;
    log_.clear();
    FreeSpuMem__13VISoundDeviceUii(dev, 300, 0);
    if (w(0x4000 + 20) != 0 || log_[1] != "tail16384") return 5;
    log_.clear();
    FreeSpuMem__13VISoundDeviceUii(dev, 7, -1);
    if (static_cast<int>(w(0x4000 + 20)) != 0) return 6;
    FreeSpuMem__13VISoundDeviceUii(dev, 7, -200);
    if (static_cast<int>(w(0x4000 + 20)) != -128) return 7;
    return 0;
}
