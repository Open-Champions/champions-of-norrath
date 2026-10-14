// Observed __7Weatheri at 0x0021cd98, 96 bytes. Calls the GameObject constructor
// (__10GameObjectG6Point3PQ210GameObject11resourceMapPPc) with this, a point of three zero
// floats, a null resource map and a null argument list. After it returns, stores the integer
// argument at object+76 and the Weather table pointer (_vt$7Weather) at object+72, in that
// order, and returns whatever the GameObject constructor returned (the machine word in the
// result register is not touched).
// Unknown: what the integer means. Inferred: the Weather class name from the symbol. Nothing
// here ran on the EE.
#include "../w10-actor-p-common/access.hpp"

extern "C" {
void* gameobject_construct(void* self, Point3 position, void* resource_map, char** arguments)
    __asm__("__10GameObjectG6Point3PQ210GameObject11resourceMapPPc");
extern unsigned char vt_Weather[] __asm__("_vt$7Weather");
}

extern "C" void* weather_construct(void* self, int argument) __asm__("__7Weatheri");
extern "C" void* weather_construct(void* self, int argument) {
    using namespace w10p;
    unsigned char* const o = static_cast<unsigned char*>(self);
    const Point3 origin = {0.0f, 0.0f, 0.0f};
    void* const result = gameobject_construct(self, origin, nullptr, nullptr);
    sword(o, 76) = argument;
    word(o, 72) = to_word(vt_Weather);
    return result;
}
