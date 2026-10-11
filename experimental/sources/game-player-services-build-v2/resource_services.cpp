#include "services.hpp"
extern "C" void norrath_player_remove_quotations(char* source){
    if(static_cast<unsigned char>(source[0])!=34u)return;
    alignas(16) char workspace[512]; // Full actual non-save span, never zeroed/capped.
    (void)norrath_factory_strcpy(workspace,source+1);
    if(norrath_factory_strchr(workspace,34)){
        char* quote=norrath_factory_strchr(workspace,34); // Actual second real call.
        quote[0]=0;
    }
    (void)norrath_factory_strcpy(source,workspace);
}
extern "C" void* norrath_player_resource_map_get(void* resource_map){
    using namespace norrath_player_detail;
    const Address a=address(resource_map),frame=address(norrath_constructor_resource_frame_view);
    const unsigned current=read_word(frame);
    if(read_word(a)!=current){
        char* lump=text(read_word(a+4u));char* resource=text(read_word(a+8u));
        void* loaded=norrath_lump_find_resource(lump,resource);
        const unsigned fresh_frame=read_word(frame);
        write_word(a+12u,address(loaded));write_word(a,fresh_frame);
    }
    return pointer(read_word(a+12u));
}
extern "C" float norrath_player_treasure_ramp(){
    using namespace norrath_player_detail;
    const float coefficient=read_float(address(norrath_player_services_view_48068c));
    const float scale=read_float(address(norrath_creature_view_484cac));
    return scale*coefficient;
}
