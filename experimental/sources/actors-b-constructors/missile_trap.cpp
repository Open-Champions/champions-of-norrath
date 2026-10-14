#include "detail.hpp"
// __11MissileTrapG6Point3iPPc at 0x1e5bc8, 172 bytes.
// GameObject receives the point snapshot, a null map, and the argument list.
// The first argument string, when non-null, is passed to sscanf with the format
// address 0x4850f0 and two int slots. Those slots are zero when sscanf is skipped.
// _vt$11MissileTrap (0x44a4b0) is stored at object+72. The low 16 bits of the int
// argument go to half 56. The low 16 bits of the two slots go to halves 76 and 78.
// Object+12 is ORed with 9. Low V0 is that updated flags word.
extern "C" int sscanf(char* text, const char* format, int* first, int* second);
extern "C" unsigned norrath_construct_MissileTrap(void* object, const void* point, int type, char** arguments) {
    using namespace norrath_actors_b;
    const PointBytes local = snapshot_point(point);
    (void)norrath_game_object_construct(object, &local, nullptr, arguments);
    const Address a = address(object);
    const Address list = read_word(a + 44u);
    const Address text = read_word(address(pointer(list)));
    write_word(a + 72u, address_bits(norrath_actors_b_view_44a4b0));
    int first = 0;
    int second = 0;
    if (text != 0u) {
        (void)sscanf(reinterpret_cast<char*>(pointer(text)),
                     reinterpret_cast<char*>(norrath_actors_b_view_4850f0), &first, &second);
    }
    const unsigned flags = read_word(a + 12u) | 9u;
    write_half(a + 56u, static_cast<unsigned>(type));
    write_word(a + 12u, flags);
    write_half(a + 76u, static_cast<unsigned>(first));
    write_half(a + 78u, static_cast<unsigned>(second));
    return flags;
}
