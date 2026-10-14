// Host contract for run__5SpellG6Point3s; not an EE input. The function must not touch the object or the point. The authored
// function was also compared with the original in a private R5900 run (four scenarios with random memory: same changed bytes,
// meaning none outside the stack frame).
#include "run.cpp"
#include <cstring>
int main() {
    unsigned char object[32], point[12];
    for (int i = 0; i < 32; ++i) object[i] = static_cast<unsigned char>(i + 1);
    for (int i = 0; i < 12; ++i) point[i] = static_cast<unsigned char>(0xa0 + i);
    unsigned char object_copy[32], point_copy[12];
    std::memcpy(object_copy, object, 32);
    std::memcpy(point_copy, point, 12);
    run__5SpellG6Point3s(object, point, -3);
    return std::memcmp(object, object_copy, 32) == 0 && std::memcmp(point, point_copy, 12) == 0 ? 0 : 1;
}
