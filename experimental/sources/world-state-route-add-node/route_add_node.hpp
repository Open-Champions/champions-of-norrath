#pragma once
static_assert(__CHAR_BIT__ == 8 && sizeof(unsigned) == 4 && sizeof(int) == 4 &&
              sizeof(float) == 4 && sizeof(short) == 2, "target scalar representation");
static_assert(__BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__, "target byte order");
extern "C" {
// Observed route tables. nodes is 32,768 bytes and clustercache is 256.
// nodeused, freeroot, world and drawFrameNm are four-byte words.
extern unsigned char norrath_route_nodes_bytes[];
extern unsigned char norrath_route_cache_bytes[];
extern unsigned char norrath_route_node_used_bytes[];
extern unsigned char norrath_route_free_root_bytes[];
extern unsigned char norrath_route_world_word_bytes[];
extern unsigned char norrath_route_draw_frame_bytes[];
void norrath_route_unroll_vnode(int index);
float norrath_world_find_height(unsigned char* world, const unsigned char* point, float radius);
// Observed A0 is the 12-byte point. The original passes Point3 by value.
void norrath_route_add_node(const unsigned char* point);
}
