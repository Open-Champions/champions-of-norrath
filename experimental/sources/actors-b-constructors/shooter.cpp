#include "detail.hpp"
// __7ShooterG6Point3iPPc at 0x1e5ca8, 216 bytes.
// GameObject receives the point snapshot, a null map, and the argument list.
// When the first argument string is non-null, sscanf uses format address 0x4850f8
// and one int slot. The parsed half at object+84 is then replaced with 100.
// The int argument's low 16 bits are stored at half 82. The backward BGEZ loop
// clears bytes 80, 79, 78, and 77, and byte 76 is cleared separately. Object+12
// is ORed with 9. Low V0 is the medium-run-list result. _vt$7Shooter is 0x44a420.
extern "C" int sscanf(char* text, const char* format, int* first);
extern "C" unsigned norrath_construct_Shooter(void* object, const void* point, int type, char** arguments) {
    using namespace norrath_actors_b;
    const PointBytes local = snapshot_point(point);
    (void)norrath_game_object_construct(object, &local, nullptr, arguments);
    const Address a = address(object);
    const Address list = read_word(a + 44u);
    const Address text = read_word(address(pointer(list)));
    write_word(a + 72u, address_bits(norrath_actors_b_view_44a420));
    int parsed = 0;
    if (text != 0u) (void)sscanf(reinterpret_cast<char*>(pointer(text)),
                                reinterpret_cast<char*>(norrath_actors_b_view_4850f8), &parsed);
    write_half(a + 84u, static_cast<unsigned>(parsed));
    write_half(a + 82u, static_cast<unsigned>(type));
    write_half(a + 84u, 100u);
    for (unsigned index = 0u; index < 4u; ++index) bytes(a + 80u - index)[0] = 0u;
    bytes(a)[76] = 0u;
    write_word(a + 12u, read_word(a + 12u) | 9u);
    return norrath_actor_add_medium_run_list(object);
}
