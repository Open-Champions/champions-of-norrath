// Host contract for getModel__6Player; not an EE input. Includes the source.
#include "get_model.cpp"
#include <cstring>
int main() {
    alignas(8) unsigned char object[1400];
    std::memset(object, 0, sizeof object);
    void* values[] = {nullptr, object, object + 7, reinterpret_cast<void*>(sizeof(void*) == 4 ? 0xdeadbeefu : 0xdeadbeefull)};
    for (void* v : values) {
        std::memcpy(object + 1332, &v, sizeof v);
        if (getModel__6Player(object) != v) return 1;
    }
    return 0;
}
