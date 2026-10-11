#include "inventory_animation.hpp"
using namespace norrath_inventory_animation_detail;

extern "C" void norrath_inventory_set_animation(int player_index,void* animation_state,int mode) {
    const Address record=player_record(player_index);
    if(read_word_fresh(record+21872u)==0u)return;
    const unsigned appearance=read_word_fresh(record+84u);
    if(appearance>=10u)return;
    // Native112-byte frame reserves80 local bytes before saved registers/F20.
    // Only11/12/13 are copied; the rest remain unwritten, without a terminator
    // repair or any claimed native SP/input/heap alias equivalence.
    alignas(8) unsigned char local_workspace[80];
    const Address name=address(local_workspace),names=address(norrath_inventory_animation_lump_names_view);
    // Derived from the exact10 original code targets, not their copied payload.
    switch(appearance){
    case 0:copy_name12(name,names+16u);break;
    case 1:copy_name11(name,names);break;
    case 2:copy_name13(name,names+144u);break;
    case 3:copy_name13(name,names+96u);break;
    case 4:copy_name13(name,names+128u);break;
    case 5:copy_name13(name,names+80u);break;
    case 6:copy_name13(name,names+48u);break;
    case 7:copy_name12(name,names+112u);break;
    case 8:copy_name12(name,names+32u);break;
    case 9:copy_name13(name,names+64u);break;
    }
    // The native coefficient load occurs even when the mode replaces its value.
    float rate=read_float_fresh(address(norrath_inventory_animation_transition_rate_view));
    if(mode!=0)rate=0.0f;
    const int selection=read_signed_byte_fresh(record+23582u);
    unsigned offset;
    switch(selection){
    case 1:offset=0u;break;
    case 2:offset=16u;break;
    case 3:offset=32u;break;
    case 4:offset=48u;break;
    case 5:offset=64u;break;
    case 7:offset=80u;break;
    default:offset=96u;break;
    }
    char* fragment=text(address(norrath_inventory_animation_resource_names_view)+offset);
    void* header=norrath_lump_find_resource(text(name),fragment);
    if(norrath_animation_get_component(animation_state,header)!=nullptr)return;
    // Real lookup is repeated; no first result or external response is cached.
    header=norrath_lump_find_resource(text(name),fragment);
    norrath_animation_add_transition(animation_state,header,rate,67);
}
