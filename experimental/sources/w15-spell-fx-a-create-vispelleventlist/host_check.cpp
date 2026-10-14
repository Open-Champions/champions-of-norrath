// Host contract for Create__16VISpellEventListiP7VIScene; not an EE input. Stubs record the calls. The authored function was
// also compared with the original in a private R5900 run.
#include "create.cpp"
#include <cstring>
#include <string>
static std::string trace;
static unsigned size_seen;
extern "C" {
void Destroy__16VISpellEventList(void*) { trace += "D;"; }
unsigned __builtin_vec_new(unsigned size) { trace += "N;"; size_seen = size; return 0x1234u; }
}
int main() {
    unsigned char list[16] = {};
    int scene = 0;
    void* const scene_pointer = &scene;
    if (Create__16VISpellEventListiP7VIScene(list, 7, &scene) != 0) return 1;
    unsigned word8, word4;
    std::memcpy(&word8, list + 8, 4);
    std::memcpy(&word4, list + 4, 4);
    unsigned stored;
    std::memcpy(&stored, list, sizeof stored);
    if (trace != "N;" || size_seen != 308u || word4 != 7u || word8 != 0x1234u || stored != static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(scene_pointer))) return 2;
    trace.clear();
    if (Create__16VISpellEventListiP7VIScene(list, 3, &scene) != 0) return 3;
    return trace == "D;N;" && size_seen == 132u ? 0 : 4;
}
