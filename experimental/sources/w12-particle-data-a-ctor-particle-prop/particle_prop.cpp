namespace {
struct Point3 {
    int x;
    int y;
    int z;
};
}

extern "C" void __10GameObjectG6Point3PQ210GameObject11resourceMapPPc(
    void*, const Point3*, unsigned int, const char**);
extern "C" void init__4Propi(void*, int);

extern "C" void __12ParticlePropG6Point3i(void* self, const Point3* position,
                                           int argument) {
    const Point3 position_copy = *position;
    __10GameObjectG6Point3PQ210GameObject11resourceMapPPc(
        self, &position_copy, 0, nullptr);

    *reinterpret_cast<unsigned int*>(static_cast<unsigned char*>(self) + 72) =
        0x0045d018;
    init__4Propi(self, argument);

    auto* bytes = static_cast<unsigned char*>(self);
    *reinterpret_cast<unsigned int*>(bytes + 72) = 0x0044cf88;
    *reinterpret_cast<float*>(bytes + 28) = 12.0f;
    *reinterpret_cast<unsigned int*>(bytes + 12) |= 0x4002;
}
