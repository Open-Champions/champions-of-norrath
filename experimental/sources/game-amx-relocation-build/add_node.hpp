#pragma once
static_assert(__CHAR_BIT__ == 8 && sizeof(unsigned) == 4 && sizeof(int) == 4 &&
              sizeof(float) == 4, "target scalar representation");
static_assert(sizeof(void*) == 4 && sizeof(__UINTPTR_TYPE__) == 4,
              "target32 byte-view address bridge");
static_assert(__BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__, "target byte order");
extern "C" {
extern unsigned char norrath_route_nodes_bytes[];
extern unsigned char norrath_route_cache_bytes[];
extern unsigned char norrath_route_node_used_bytes[];
extern unsigned char norrath_route_free_root_bytes[];
extern unsigned char norrath_route_world_word_bytes[];
extern unsigned char norrath_route_draw_frame_bytes[];
void norrath_route_unroll_vnode(int index);
float norrath_world_find_height(unsigned char* world,
                               const unsigned char* point, float radius);
// Observed A0 point-byte pointer; no inferred original Point3 class ABI.
void norrath_route_add_node(const unsigned char* point);
}
