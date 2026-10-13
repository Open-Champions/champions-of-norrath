#include "delete_playback.cpp"
#include <vector>

struct Event { int kind; const void* a; const void* b; };
static std::vector<Event> events;
extern "C" void Remove__7VILNode(void* node) { events.push_back({1, node, nullptr}); }
extern "C" void AddTail__7VILListP7VILNode(void* list, void* node) { events.push_back({2, list, node}); }

int main() {
    alignas(8) static char object[256];
    alignas(8) static char record[64];
    DeletePlayback__19VISpellEffectSystemPQ219VISpellEffectSystem16VISpellEffectPbi(object, record);
    if (events.size() != 2) return 1;
    if (events[0].kind != 1 || events[0].a != record) return 2;
    if (events[1].kind != 2 || events[1].a != object + 112 || events[1].b != record) return 3;
    return 0;
}
