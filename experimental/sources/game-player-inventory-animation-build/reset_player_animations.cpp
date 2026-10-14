#include "inventory_animation.hpp"
using namespace norrath_inventory_animation_detail;

extern "C" void norrath_player_reset_animations(int player_index) {
    const Address record=player_record(player_index);
    Address actor=read_word_fresh(record+21872u);
    if(actor==0u)return;
    if(read_float_fresh(record+172u)<=0.0f)return;
    const int captured_selection=read_signed_byte_fresh(record+23582u);
    const int named_animation=read_signed_byte_fresh(actor+1106u);
    write_word_ordered(actor+312u,0x3f800000u);
    write_half_ordered(actor+324u,4613u);
    write_word_ordered(actor+304u,0x3f800000u);
    const unsigned row=unsigned(captured_selection)*104u;
    if(named_animation!=0) {
        // This selector is read after the early actor stores. Its first case
        // uses the captured actor; every other native case reloads the actor.
        const int selection=read_signed_byte_fresh(record+23582u);
        unsigned offset;
        switch(selection){
        case 1:offset=0u;break;
        case 2:offset=16u;break;
        case 3:offset=32u;break;
        case 4:offset=48u;break;
        case 7:offset=64u;break;
        default:offset=80u;break;
        }
        const Address lookup_actor=selection==1?actor:read_word_fresh(record+21872u);
        const Address map=read_word_fresh(lookup_actor+36u);
        const Address lump_name=read_word_fresh(map);
        const Address resource=address(norrath_inventory_reset_resource_names_view)+offset;
        const Address header=address(norrath_lump_find_resource(text(lump_name),text(resource)));
        actor=read_word_fresh(record+21872u);
        write_word_ordered(actor+296u,header);
    } else {
        const Address map=read_word_fresh(actor+36u);
        const Address header=read_word_fresh(map+row+68u);
        write_word_ordered(actor+296u,header);
    }
    // Map capture precedes these stores, but the three actual field reads
    // follow them and remain ordered92,76,84 before any of their output stores.
    const Address map=read_word_fresh(actor+36u);
    write_half_ordered(actor+420u,4613u);
    write_word_ordered(actor+336u,0u);
    const unsigned third=read_word_fresh(map+row+92u);
    const unsigned first=read_word_fresh(map+row+76u);
    const unsigned second=read_word_fresh(map+row+84u);
    write_word_ordered(actor+328u,first);
    write_word_ordered(actor+360u,second);
    write_word_ordered(actor+392u,third);
    write_word_ordered(actor+344u,0u);
    write_half_ordered(actor+356u,4613u);
    write_word_ordered(actor+368u,0u);
    write_word_ordered(actor+376u,0u);
    write_half_ordered(actor+388u,4613u);
    write_word_ordered(actor+400u,0u);
    write_word_ordered(actor+408u,0u);
    write_word_ordered(actor+432u,0u);
    write_word_ordered(actor+440u,0u);
    norrath_animation_reset_frame_states(pointer(actor+288u));
}
