#include "close.cpp"

static VISoundDevice* seen;
static int calls;

extern "C" void Reset__13VISoundDevice(VISoundDevice* device) {
    seen = device;
    ++calls;
}

int main() {
    VISoundDevice* device = reinterpret_cast<VISoundDevice*>(0x4440);
    if (Close__13VISoundDevice(device) != 0) return 1;
    if (calls != 1 || seen != device) return 2;
    return 0;
}
