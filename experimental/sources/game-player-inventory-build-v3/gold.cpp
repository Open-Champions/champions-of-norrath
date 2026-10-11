#include "player_inventory.hpp"
extern "C" unsigned norrath_construct_Gold(void* object,const void* point,char** arguments){
    using namespace norrath_player_detail;
    base(object,point,arguments);
    const Address a=address(object),animation=a+88u;
    write_word(a+72u,address(norrath_player_view_44cd48));bytes(animation)[5]=1u;
    write_word(a+12u,read_word(a+12u)|0x2208u);
    float multiplier=1.0f;
    char* tag=text(address(norrath_player_view_483c08));
    if(norrath_actor_find_tag_string(object,tag,0))multiplier=norrath_player_find_tag_float(object,tag,0);
    const float ramp=norrath_player_treasure_ramp();
    const Address random=address(norrath_constructor_random_state_view);
    const unsigned seed=step(read_word(random));write_word(random,seed);
    float amount=float(int((seed>>16)%5u+10u))*multiplier;
    amount=amount*ramp;
    const int converted=static_cast<int>(amount); // Actual native CVT.W.S, no repair/clamp.
    write_word(a+128u,unsigned(converted));
    if(converted==0)write_word(a+128u,8u);
    bytes(a)[76]=1u;
    norrath_actor_animation_initialize(pointer(animation));
    write_word(a+80u,read_word(address(norrath_actor_view_4844a4)));
    void* header=norrath_player_resource_map_get(norrath_player_view_600d20);
    norrath_actor_animation_add(pointer(animation),header,516);
    norrath_animation_set_to_end(pointer(animation));
    for(unsigned index=0u;index<5u;++index){
        unsigned state=read_word(a+68u);
        float x,y,z;
        for(;;){
            state=step(state);x=float(int(state>>16))*(1.0f/65536.0f);x=x+x;x=x-1.0f;
            state=step(state);y=float(int(state>>16))*(1.0f/65536.0f);y=y+y;y=y-1.0f;
            state=step(state);z=float(int(state>>16))*(1.0f/65536.0f);z=z+z;z=z-1.0f;
            float squared=x*x;squared=squared+y*y;squared=squared+z*z;
            if(!(1.0f<squared))break; // Complete original rejection loop, no attempt cap.
        }
        const float original_x=read_float(a+16u);
        write_word(a+68u,state);
        x=x*8.0f;y=y*8.0f;z=z*8.0f;
        y=read_float(a+20u)+y;z=read_float(a+24u)+z;x=original_x+x;
        x=x+0.0f;z=z+8.0f;
        const Address output=a+132u+12u*index;
        write_float(output,x);write_float(output+4u,y);write_float(output+8u,z);
    }
    return 0u; // Last actual SLTI loop result, not a fabricated self pointer.
}
