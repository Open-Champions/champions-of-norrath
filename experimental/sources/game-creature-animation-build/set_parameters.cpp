#include "creature_animation.hpp"
using namespace creature_detail;
extern "C" unsigned norrath_actor_set_parameters(void* object,float first,float second,float third,
                                                  float fourth,float fifth,int randomize) {
    const Address a=address(object);
    if((wide(a+160u)&1ull)!=0u){
        first=first*15.0f;third=third*view(norrath_creature_view_47fb58);
        fourth=fourth*3.0f;fifth=fifth*7.0f;
    }
    unsigned players=word(address(norrath_creature_view_484538));
    if(signed_word(players)>=2){
        if(*norrath_creature_view_483829!=0u && signed_word(players)>0){
            unsigned index=0u;
            do {
                const unsigned current=index;index+=1u;
                (void)norrath_network_player_connected(signed_word(current));
                players=word(address(norrath_creature_view_484538));
            } while(signed_word(index)<signed_word(players));
        }
        const float extra=static_cast<float>(signed_word(players-1u));
        const float increment=extra*view(norrath_creature_view_47fb5c);
        const float total=extra+1.0f;
        const float first_factor=increment+1.0f;
        fifth=fifth*total;first=first*first_factor;
    }
    if(randomize!=0){
        const unsigned random=norrath_actor_random(object);
        const float base=first*view(norrath_creature_view_484cac);
        const float power=norrath_native_powf(base,view(norrath_creature_view_47fb60));
        const float variation=static_cast<float>(signed_word(random%6u+18u));
        const float health=variation*power;
        const unsigned difficulty=word(address(norrath_creature_view_484cd8));
        put_scalar(a+560u,health);
        if(difficulty==2u)put_scalar(a+560u,health*1.5f);
        else if(difficulty==3u)put_scalar(a+560u,health+health);
        put_word(a+564u,word(a+560u));
    }
    const float doubled_second=second*12.0f;
    const float scaled_second=doubled_second*view(norrath_creature_view_484cac);
    const float scale=view(norrath_creature_view_484cac);
    const float exponent=view(norrath_creature_view_47fb64);
    put_half(a+232u,unsigned(chop(scaled_second)));
    const float third_power=norrath_native_powf(scale,exponent);
    const float scaled_third=third*third_power;
    put_half(a+200u,unsigned(chop(scaled_third)));
    const float third_power_again=norrath_native_powf(view(norrath_creature_view_484cac),exponent);
    const float doubled_third=third+third;
    const float upper_third=doubled_third*third_power_again;
    const unsigned converted_upper=unsigned(chop(upper_third));
    const int minimum=signed_half(half(a+200u))+1;
    const unsigned fresh_lower=half(a+200u);
    put_half(a+202u,converted_upper);
    if(signed_half(converted_upper&0xffffu)<minimum)put_half(a+202u,fresh_lower+1u);
    const float fresh_scale=view(norrath_creature_view_484cac);
    const float fourth_base=fresh_scale*12.0f;
    const float scaled_fourth=fourth_base*fourth;
    const float fifth_base=fifth*fresh_scale;
    const float fifth_exponent=view(norrath_creature_view_47fb68);
    put_half(a+204u,unsigned(chop(scaled_fourth)));
    const float fifth_power=norrath_native_powf(fifth_base,fifth_exponent);
    put_scalar(a+220u,fifth_power*8.0f);
    const int result=norrath_game_object_find_tag_int(object,tag(norrath_actor_view_4837b8),0);
    if(result!=0)put_word(a+560u,0u);
    return static_cast<unsigned>(result);
}
