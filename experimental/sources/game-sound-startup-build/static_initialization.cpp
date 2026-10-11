#include "sound_startup.hpp"
using namespace norrath_sound_startup_detail;

extern "C" void norrath_sound_static_initialization(int phase,unsigned priority) {
    if(priority!=0xffffu)return;
    const Address base=address(norrath_sound_sequence_records_view);
    if(phase==1){
        for(unsigned row=0;row<32u;++row){
            const Address sequence=base+row*76u;
            write_byte_ordered(sequence+5u,1u);
            write_word_ordered(sequence+40u,0u);
        }
    }
    if(priority!=0xffffu)return;
    if(phase==1)
        norrath_sound_resource_map_initialize(pointer(address(norrath_sound_global_dialog_map_view)),
             reinterpret_cast<char*>(norrath_sound_global_dialog_name_view),
             reinterpret_cast<char*>(norrath_sound_global_dialog_extension_view));
    if(phase!=0)return;
    if(base==0u)return;
    Address current=base+2432u;
    if(current==base)return;
    current-=76u;
    for(;;){
        norrath_list_destroy(pointer(current+40u));
        norrath_animation_state_destroy(pointer(current),2);
        const bool again=current!=base;
        current-=76u;
        if(!again)break;
    }
}
