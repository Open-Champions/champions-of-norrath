#include "player_inventory.hpp"
extern "C" unsigned norrath_construct_UserParamProp(void* object,const void* point,short type,char** arguments){
    using namespace norrath_player_detail;
    base(object,point,arguments);
    const Address a=address(object),animation=a+84u;
    write_word(a+72u,address(norrath_player_view_44c9e8));bytes(animation)[5]=8u;put_half(a+56u,unsigned(type));
    if(norrath_actor_find_tag_string(object,text(address(norrath_player_view_483ba0)),0))write_word(a+12u,read_word(a+12u)|0x100000u);
    if(norrath_game_object_find_tag_int(object,text(address(norrath_player_view_462350)),0))write_word(a+12u,read_word(a+12u)|1u);
    write_word(a+12u,read_word(a+12u)|8u);
    char* name=norrath_actor_find_tag_string(object,text(address(norrath_player_view_483ba8)),0);
    // Preserve the original shared workspace: buffers at16/144/272, scan output400.
    // Neither the stack workspace nor failed-scan output is initialized here.
    alignas(16) unsigned char workspace[416];
    char* lump=reinterpret_cast<char*>(workspace+272u);
    char* mesh=reinterpret_cast<char*>(workspace+16u);
    char* colors=reinterpret_cast<char*>(workspace+144u);
    if(name==nullptr){
        name=norrath_actor_find_tag_string(object,text(address(norrath_player_view_483bb0)),0);
        if(name)norrath_player_remove_quotations(name);
    }
    if(name){
        (void)norrath_factory_sprintf(lump,text(address(norrath_player_view_483bb8)),name);
        (void)norrath_factory_sprintf(mesh,text(address(norrath_player_view_483bc0)),name);
        (void)norrath_factory_sprintf(colors,text(address(norrath_player_view_483bc8)),name);
        write_word(a+76u,address(norrath_lump_find_resource(lump,mesh)));
        write_word(a+80u,address(norrath_lump_find_resource(lump,colors)));
    }
    if(read_word(a+76u)==0u){
        char* resource_lump=norrath_actor_find_tag_string(object,text(address(norrath_player_view_483bd0)),0);
        char* resource=norrath_actor_find_tag_string(object,text(address(norrath_player_view_483bd8)),0);
        write_word(a+76u,address(norrath_lump_find_resource(resource_lump,resource)));
    }
    if(read_word(a+80u)==0u){
        char* resource_lump=norrath_actor_find_tag_string(object,text(address(norrath_player_view_483bd0)),0);
        char* resource=norrath_actor_find_tag_string(object,text(address(norrath_player_view_483be0)),0);
        write_word(a+80u,address(norrath_lump_find_resource(resource_lump,resource)));
    }
    norrath_actor_animation_initialize(pointer(animation));
    char* animation_tag=text(address(norrath_player_view_483be8));
    if(norrath_actor_find_tag_string(object,animation_tag,0)){
        char* resource_lump=norrath_actor_find_tag_string(object,text(address(norrath_player_view_483bd0)),0);
        char* resource=norrath_actor_find_tag_string(object,animation_tag,0);
        void* header=norrath_lump_find_resource(resource_lump,resource);
        norrath_actor_animation_add(pointer(animation),header,1);
        norrath_actor_animation_randomize_time(pointer(animation));
        (void)norrath_actor_add_slow_run_list(object);
    }
    char* mask_tag=text(address(norrath_player_view_462360));
    unsigned mask=0u;
    if(norrath_actor_find_tag_string(object,mask_tag,0)){
        char* source=norrath_actor_find_tag_string(object,mask_tag,0);
        (void)norrath_player_sscanf(source,text(address(norrath_player_view_483bf0)),workspace+400u);
        mask=read_word(address(workspace)+400u); // Original read remains if genuine scan fails.
    }
    const unsigned long long active=norrath_actor_model_active_chunks(pointer(read_word(a+76u)),signed_word(mask));
    put_wide(a+352u,active);
    return unsigned(active); // Canonical constructor result is the low32 GPR lane.
}
