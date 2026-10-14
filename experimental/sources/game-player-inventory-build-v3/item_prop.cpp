#include "player_inventory.hpp"
extern "C" unsigned norrath_construct_ItemProp(void* object,const void* point,short type,char** arguments){
    using namespace norrath_player_detail;
    base(object,point,arguments);
    const Address a=address(object),description=a+328u,matrix=a+272u;
    write_word(a+72u,address(norrath_player_view_44d018));write_word(a+12u,read_word(a+12u)|0x1809u);
    norrath_object_update_in_grid(object);
    put_half(a+56u,unsigned(type));put_half(a+320u,half(a+320u)|1u);write_word(a+28u,0x40800000u);
    RawPoint initial_position=snapshot(a+16u);
    norrath_matrix_make_translation_yaw(pointer(matrix),&initial_position,short_bits(unsigned(type)+16384u));
    write_word(a+72u,address(norrath_player_view_44cef8));
    for(unsigned index=0u;index<4u;++index)put_half(a+388u-8u*index,65535u);
    bytes(description)[33]=255u;bytes(description)[32]=255u;
    for(unsigned index=0u;index<4u;++index){
        put_half(description+16u+2u*index,0u);put_half(description+24u+2u*index,0u);
    }
    char* item_tag=text(address(norrath_player_view_483c28));
    if(norrath_actor_find_tag_string(object,item_tag,0)){
        char* item=norrath_actor_find_tag_string(object,item_tag,0);
        norrath_player_create_item(item,pointer(description),-1,0);
    }else norrath_player_create_item(text(address(norrath_player_view_4623b0)),pointer(description),-1,0);
    const Address prefix=address(norrath_player_view_4623a0);
    const RawPoint name_prefix=snapshot(prefix);
    const unsigned char name_tail=bytes(prefix)[12];
    publish(a+84u,name_prefix);bytes(a)[96]=name_tail; // All thirteen real source bytes.
    const Address proxy_table=address(norrath_player_view_31f540);
    const unsigned first_proxy=unsigned(norrath_player_proxy_mesh_id(pointer(description)));
    (void)norrath_factory_strcpy(text(a+100u),text(read_word(proxy_table+(first_proxy<<4))));
    const unsigned second_proxy=unsigned(norrath_player_proxy_mesh_id(pointer(description)));
    (void)norrath_factory_strcpy(text(a+164u),text(read_word(proxy_table+(second_proxy<<4)+4u)));
    write_word(a+76u,address(norrath_lump_find_resource(text(a+84u),text(a+100u))));
    void* colors=norrath_lump_find_resource(text(a+84u),text(a+164u));
    unsigned flags=(read_word(a+12u)|0x102000u)&0xffffe7ffu;
    const unsigned long long description_flag=wide(description+8u)&0x8000000000ULL;
    write_word(a+80u,address(colors));put_wide(a+232u,0xffffffffffffffffULL);write_word(a+12u,flags);
    if(description_flag)write_word(a+12u,flags&0xffffcfffU);
    norrath_object_update_in_grid(object);
    // Capture the final original halfword before matrix writes. No later reload.
    const unsigned final_yaw=half(a+56u);
    RawPoint final_position=snapshot(a+16u);
    norrath_matrix_make_translation_yaw(pointer(matrix),&final_position,short_bits(final_yaw+16384u));
    // Complete original matrix356 leaves this pure cosine-quadrant mask in V0.
    // Factory ignores constructor residues; no self/64-bit result is claimed.
    return (final_yaw^(final_yaw<<1))&0x8000u;
}
