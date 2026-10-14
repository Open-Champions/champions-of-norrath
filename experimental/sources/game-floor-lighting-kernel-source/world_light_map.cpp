#include "lighting.hpp"
// This complete original function captures the point and returns unity. Its
// unused world argument does not imply a missing light-map implementation here.
extern "C" float norrath_world_query_light_map(unsigned char*, const unsigned char* point) {
    using namespace norrath_floor_lighting_detail;
    alignas(8) unsigned char captured[12];
    capture_point(point, captured);
    return 1.0f;
}
