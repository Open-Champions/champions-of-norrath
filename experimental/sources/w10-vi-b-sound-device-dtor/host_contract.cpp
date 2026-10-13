#include "sound_device_dtor.cpp"

#include <string>
#include <vector>

static std::vector<std::string> trace;
static unsigned char* object_bytes;

extern "C" void _$_7VILList(VILList* list, int flags) {
    trace.push_back("L" + std::to_string(reinterpret_cast<unsigned char*>(list) - object_bytes) + "," +
                    std::to_string(flags));
}
extern "C" void __builtin_delete(void* pointer) {
    trace.push_back(pointer == object_bytes ? "D" : "D?");
}

int main() {
    static unsigned char object[4600];
    object_bytes = object;
    std::vector<std::string> lists = {"L3352,2", "L3340,2", "L3324,2", "L3312,2", "L3296,2", "L3284,2"};
    _$_13VISoundDevice(reinterpret_cast<VISoundDevice*>(object), 0);
    if (trace != lists) return 1;
    trace.clear();
    _$_13VISoundDevice(reinterpret_cast<VISoundDevice*>(object), 3);
    lists.push_back("D");
    if (trace != lists) return 2;
    trace.clear();
    _$_13VISoundDevice(reinterpret_cast<VISoundDevice*>(object), 2);
    lists.pop_back();
    if (trace != lists) return 3;
    return 0;
}
