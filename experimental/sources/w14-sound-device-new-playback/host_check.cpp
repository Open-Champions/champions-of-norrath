// Host check only; not a build input.
#include "new_playback.cpp"
#include <vector>
static std::vector<int> calls;
static void* head_result;
static void* last_add_list;
static void* last_add_node;
extern "C" void* RemHead__7VILList(void* list) { calls.push_back(1); last_add_list = list; return head_result; }
extern "C" void AddTail__7VILListP7VILNode(void* list, void* node) { calls.push_back(2); last_add_list = list; last_add_node = node; }
int main() {
    unsigned char dev[4096];
    int node = 0;
    head_result = &node;
    void* r = NewPlayback__13VISoundDevice(dev);
    if (r != &node) return 1;
    if (calls.size() != 2 || calls[0] != 1 || calls[1] != 2) return 2;
    if (last_add_list != dev + 3284 || last_add_node != &node) return 3;
    calls.clear();
    head_result = nullptr;
    r = NewPlayback__13VISoundDevice(dev);
    if (r != nullptr || calls.size() != 1 || calls[0] != 1) return 4;
    return 0;
}
