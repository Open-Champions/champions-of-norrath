#include "player_inventory.hpp"
extern "C" unsigned norrath_construct_Player1(void* object,const void* point,short type,char** arguments){
    using namespace norrath_player_detail;
    base(object,point,arguments);
    const Address a=address(object), mode=address(norrath_player_view_484ca8);
    unsigned result=read_word(a+12u)|3u;
    const int initial_mode=signed_byte(bytes(mode)[0]); // Captured BEFORE three child stores.
    write_word(a+72u,address(norrath_player_view_44d2e8));
    put_half(a+56u,unsigned(type));write_word(a+12u,result);
    if(initial_mode==3)return result;
    char* selected=norrath_actor_find_tag_string(object,text(address(norrath_player_view_483c10)),0);
    char* model=norrath_actor_find_tag_string(object,text(address(norrath_player_view_483ba8)),0);
    bool relocate=false;
    const int current=signed_byte(bytes(mode)[0]);result=unsigned(current);
    if(current<=0){relocate=true;bytes(mode)[0]=1u;}
    else{
        result=current<2?1u:0u;
        if(result){
            result=unsigned(norrath_game_object_find_tag_int(object,text(address(norrath_player_view_483c18)),0));
            if(result){relocate=true;bytes(mode)[0]=1u;}
        }
    }
    unsigned multiplayer;
    if(selected){
        result=unsigned(norrath_string_compare(text(address(norrath_player_view_34b752)),selected));
        multiplayer=bytes(address(norrath_player_view_483f3e))[0];
        if(result==0u){
            const int signed_multiplayer=signed_byte(bytes(address(norrath_player_view_483f3e))[0]);
            result=2u; // Actual branch delay writes V0 regardless of branch direction.
            if(signed_multiplayer==0){relocate=true;bytes(mode)[0]=2u;}
        }
    }else multiplayer=bytes(address(norrath_player_view_483f3e))[0];
    if(multiplayer!=0u&&selected){
        const int comparison=norrath_string_compare(selected,text(address(norrath_player_view_483c20)));
        result=3u; // The original compare branch has LI V0,3 in its delay slot.
        if(comparison==0){relocate=true;bytes(mode)[0]=3u;}
    }
    const int fresh_multiplayer=signed_byte(bytes(address(norrath_player_view_483f3e))[0]);
    result=unsigned(fresh_multiplayer); // Executed even when model is null.
    if(model&&fresh_multiplayer==0){
        const int comparison=norrath_string_compare(text(address(norrath_player_view_42a3f8)),model);
        result=3u;
        if(comparison==0){relocate=true;bytes(mode)[0]=3u;}
    }
    if(!relocate)return result;
    // Exact 48-byte original publication range 429410..429440: four points.
    for(unsigned index=0u;index<4u;++index){
        const NorrathPoint3 incoming=norrath_player_detail::point(a+16u);
        NorrathPoint3 found; // Genuine callee initializes all twelve result bytes.
        (void)norrath_object_find_empty_spot(&found,incoming,36,24,nullptr);
        publish_point(address(norrath_player_view_429410)+12u*index,found);
        const Address random=address(norrath_constructor_random_state_view);
        const unsigned seed=step(read_word(random));
        const unsigned orientation=half(a+56u);
        write_word(random,seed);
        result=(seed>>16)%4000u;
        put_half(address(norrath_player_view_484f38)+2u*index,orientation+result-2000u);
    }
    return result;
}
