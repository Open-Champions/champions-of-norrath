#include "tag_queries.hpp"
extern "C" int norrath_has_light_or_particle(char** arguments) {
    using namespace norrath_tag_detail;
    if (first_fixed_integer(arguments, norrath_factory_particle_emitter_tag_bytes, 15u) != 0) return 1;
    if (first_fixed_integer(arguments, norrath_factory_dynamic_light_tag_bytes, 12u) != 0) return 1;
    if (first_fixed_integer(arguments, norrath_factory_halo_tag_bytes, 4u) != 0) return 1;
    for (int number = 0; number != 4; ++number) {
        if (norrath_has_light_or_particle_number(arguments, number) != 0) return 1;
    }
    return 0;
}
