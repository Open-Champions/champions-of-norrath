#include <array>
#include <cassert>
#include <cstdint>
#include <cstring>

namespace {
struct Point3 {
    std::int32_t x;
    std::int32_t y;
    std::int32_t z;
};

std::uint32_t call_count;
std::array<std::uint32_t, 5> calls;
}

extern "C" void __12ParticlePropG6Point3i(void*, const Point3*, std::int32_t);

extern "C" void __10GameObjectG6Point3PQ210GameObject11resourceMapPPc(
    void* self, const Point3* position, std::uint32_t resource_map,
    const char** names) {
    assert(call_count == 0);
    calls[0] = reinterpret_cast<std::uintptr_t>(self) & 0xffffffffu;
    calls[1] = static_cast<std::uint32_t>(position->x);
    calls[2] = static_cast<std::uint32_t>(position->y);
    calls[3] = static_cast<std::uint32_t>(position->z);
    calls[4] = resource_map | (names == nullptr ? 0u : 1u);
    ++call_count;
}

extern "C" void init__4Propi(void* self, std::int32_t argument) {
    auto* bytes = static_cast<unsigned char*>(self);
    assert(call_count == 1);
    std::uint32_t vtable;
    std::memcpy(&vtable, bytes + 72, sizeof(vtable));
    assert(vtable == 0x0045d018);
    assert(argument == -7);
    ++call_count;
}

int main() {
    alignas(4) std::array<unsigned char, 128> object;
    object.fill(0xa5);
    std::uint32_t initial_flags = 0x80010001;
    std::memcpy(object.data() + 12, &initial_flags, sizeof(initial_flags));
    Point3 position{0x12345678, -19, 0x10203040};

    __12ParticlePropG6Point3i(object.data(), &position, -7);

    assert(call_count == 2);
    assert(calls[0] == static_cast<std::uint32_t>(
                           reinterpret_cast<std::uintptr_t>(object.data())));
    assert(calls[1] == 0x12345678);
    assert(calls[2] == static_cast<std::uint32_t>(-19));
    assert(calls[3] == 0x10203040);
    assert(calls[4] == 0);

    std::uint32_t word;
    std::memcpy(&word, object.data() + 72, sizeof(word));
    assert(word == 0x0044cf88);
    std::memcpy(&word, object.data() + 12, sizeof(word));
    assert(word == (initial_flags | 0x4002));
    float scalar;
    std::memcpy(&scalar, object.data() + 28, sizeof(scalar));
    assert(scalar == 12.0f);
    for (std::size_t i = 0; i < object.size(); ++i) {
        if ((i >= 12 && i < 16) || (i >= 28 && i < 32) ||
            (i >= 72 && i < 76))
            continue;
        assert(object[i] == 0xa5);
    }
}
