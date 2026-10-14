// Host check only; not a build input.
#define NORRATH_HOST_ARENA 1
#include <string>
#include <vector>
#include <cstring>
#include <cstdarg>
static unsigned char arena[0x10000];
extern "C" unsigned char* norrath_host_pointer(unsigned address) { return arena + address; }
static std::vector<std::string> log_;
static unsigned records[2] = {0x100, 0x200};
static int nrecords;
extern "C" int printf(const char* f, ...) { log_.push_back(std::string("print:") + f); return 0; }
extern "C" void __builtin_delete(void* p) { log_.push_back("delete" + std::to_string(reinterpret_cast<unsigned long>(p))); }
extern "C" unsigned FirstPlayback__13VISoundDevice(void*) { log_.push_back("first"); return nrecords ? records[0] : 0; }
extern "C" unsigned NextPlayback__13VISoundDeviceP10VIPlayback(void*, unsigned p) {
    log_.push_back("next" + std::to_string(p));
    for (int i = 0; i < nrecords; ++i) if (records[i] == p) return i + 1 < nrecords ? records[i + 1] : 0;
    return 0;
}
extern "C" unsigned Sound__13VISoundDevicei(void*, int i) { log_.push_back("sound" + std::to_string(i)); return 0x4000 + 0x100 * i; }
extern "C" int Type__C7VISound(unsigned s) { log_.push_back("type" + std::to_string(s)); return *reinterpret_cast<int*>(arena + s + 8); }
extern "C" void Stop__13VISoundDevicei(void*, int v) { log_.push_back("stop" + std::to_string(v)); }
extern "C" void StopAllBgm__13VISoundDevice(void*) { log_.push_back("stopallbgm"); }
extern "C" int BgmBufferSize__13VISoundDevice(void*) { log_.push_back("bgmsize"); return 0x1000; }
extern "C" void FreeSpuMem__13VISoundDeviceUii(void*, unsigned a, int n) { log_.push_back("free" + std::to_string(a) + "," + std::to_string(n)); }
extern "C" void Init__7VILList(void* l) { log_.push_back("initlist" + std::to_string(reinterpret_cast<unsigned long>(l))); }
extern "C" void AddTail__7VILListP7VILNode(void* l, void* n) { log_.push_back("addtail" + std::to_string((unsigned char*)n - (unsigned char*)l)); }
extern "C" void norrath_host_call_destructor(unsigned self, unsigned fn, int flags) {
    log_.push_back("dtor" + std::to_string(self) + "," + std::to_string(fn) + "," + std::to_string(flags));
}
#include "reset.cpp"
int main() {
    alignas(8) static unsigned char dev[8192];
    std::memset(dev, 0xaa, sizeof dev);
    unsigned* w = reinterpret_cast<unsigned*>(dev);
    nrecords = 2;
    for (int i = 0; i < 2; ++i) {
        unsigned* r = reinterpret_cast<unsigned*>(arena + records[i]);
        r[28 / 4] = 3 + i;
        r[32 / 4] = 50 + i;
    }
    *reinterpret_cast<int*>(arena + 0x4000 + 0x100 * 3 + 8) = 1;
    *reinterpret_cast<int*>(arena + 0x4000 + 0x100 * 4 + 8) = 0;
    // two sounds in slots 2 and 127; the other slots are empty
    std::memset(dev + 2584, 0, 512);
    w[2584 / 4 + 2] = 0x6000;
    w[2584 / 4 + 127] = 0x6100;
    for (unsigned s : {0x6000u, 0x6100u}) {
        unsigned* snd = reinterpret_cast<unsigned*>(arena + s);
        snd[3] = 0x7000 + (s - 0x6000);
        short* e = reinterpret_cast<short*>(arena + 0x7000 + (s - 0x6000) + 8);
        e[0] = 8; e[1] = 0;
        *reinterpret_cast<unsigned*>(arena + 0x7000 + (s - 0x6000) + 12) = 0x9000 + (s - 0x6000);
    }
    w[3420 / 4] = 0x5555;
    w[4472 / 4] = 0x300000;
    unsigned char before[8192];
    std::memcpy(before, dev, sizeof dev);
    if (Reset__13VISoundDevice(dev) != 0) return 1;
    std::vector<std::string> want = {"print:qaw!\n", "delete21845", "print:qaw!1\n", "first", "next256", "sound3", "type16640", "stop50",
        "next512", "sound4", "type16896", "print:qaw!2\n", "stopallbgm", "print:qaw!3\n",
        "dtor24584,36864,3", "dtor24840,37120,3", "print:qaw!4\n", "bgmsize", "free3145728,8192", "print:qaw!5\n", "print:qaw!6\n"};
    want[14] = "dtor" + std::to_string(0x6000 + 8) + "," + std::to_string(0x9000) + ",3";
    want[15] = "dtor" + std::to_string(0x6100 + 8) + "," + std::to_string(0x9100) + ",3";
    want[6] = "type" + std::to_string(0x4000 + 0x300);
    want[10] = "type" + std::to_string(0x4000 + 0x400);
    want.push_back("initlist3284");
    want.push_back("initlist3296");
    for (int i = 0; i < 46; ++i) want.push_back("addtail" + std::to_string(8 + 56 * i - 3296));
    want.push_back("print:qaw!7\n");
    want.push_back("print:qaw!8\n");
    want.push_back("print:qaw!9\n");
    // init list log holds pointer differences from a null base: base + offset
    std::vector<std::string> got = log_;
    for (auto& s : got) if (s.rfind("initlist", 0) == 0) s = "initlist" + std::to_string(std::stoul(s.substr(8)) - reinterpret_cast<unsigned long>(dev));
    if (got != want) {
        for (size_t i = 0; i < got.size() && i < want.size(); ++i) if (got[i] != want[i]) return 100 + static_cast<int>(i);
        return 2;
    }
    // memory effects
    auto isZero = [&](int lo, int hi) { for (int i = lo; i < hi; ++i) if (dev[i]) return false; return true; };
    if (!isZero(0, 8)) return 3;
    if (!isZero(2584, 3096 + 4)) return 4;  // 2584..3092 cleared, 3096 cleared by the final store
    if (!isZero(3100, 3284)) return 5;
    if (w[3432 / 4] != 1 || w[3384 / 4] != 0 || w[3408 / 4] != 0 || w[3404 / 4] != 0 || w[3412 / 4] != 1 || w[4472 / 4] != 0 || w[3420 / 4] != 0) return 6;
    if (dev[3436] || dev[3692] || dev[3948] || dev[4204]) return 7;
    if (dev[3437] != 0xaa || dev[3693] != 0xaa) return 8;
    float f;
    std::memcpy(&f, dev + 4468, 4); if (f != 1.0f) return 9;
    std::memcpy(&f, dev + 3424, 4); if (f != 1.0f) return 10;
    std::memcpy(&f, dev + 3428, 4); if (f != 1.0f) return 11;
    if (w[4460 / 4] || w[4464 / 4]) return 12;
    // everything else untouched
    for (int i = 0; i < 8192; ++i) {
        bool touched = i < 8 || (i >= 2584 && i < 3100) || (i >= 3100 && i < 3284) || (i >= 3384 && i < 3388) || (i >= 3404 && i < 3416) || (i >= 3420 && i < 3424) ||
                       (i >= 3424 && i < 3436) || i == 3436 || i == 3692 || i == 3948 || i == 4204 || (i >= 4460 && i < 4476);
        if (!touched && dev[i] != before[i]) return 13;
    }
    return 0;
}
