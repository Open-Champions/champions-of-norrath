#include "services.hpp"
extern "C" void norrath_actor_animation_add(void* state,void* header,int flags){
    using namespace norrath_player_detail;
    if(header==nullptr)return;
    const Address a=address(state);
    const int count=signed_byte(bytes(a)[4]);
    const int maximum=signed_byte(bytes(a)[5]);
    const unsigned old_count=bytes(a)[4];
    const Address component=a+unsigned(count)*32u+8u;
    if(!(count<maximum))return;
    bytes(a)[4]=static_cast<unsigned char>(old_count+1u);
    const unsigned combined=half(a+6u)|unsigned(flags);
    const Address frame=read_word(component+4u);
    write_word(component,address(header));put_half(a+6u,combined);
    write_word(component+24u,0u);put_half(component+30u,0u);
    if(frame!=0u){write_word(frame+20u,0u);write_word(component+4u,0u);}
    put_half(component+28u,unsigned(flags));
    write_word(component+12u,0u);write_word(component+8u,0x3f800000u);
    write_word(component+16u,0x3f800000u);write_word(component+20u,0u);
}
extern "C" void norrath_animation_set_to_end(void* state){
    using namespace norrath_player_detail;
    const Address a=address(state),model=read_word(a);
    if(model!=0u){write_word(a,0u);write_word(model+1552u,0u);}
    const int count=signed_byte(bytes(a)[4]);
    if(count==0)return;
    const Address component=a+unsigned(count-1)*32u+8u;
    const Address animation=read_word(component);
    const unsigned flags=half(component+28u);
    const float duration=float(signed_word(read_word(animation+4u)));
    write_float(component+24u,duration);
    if(!(flags&0x800u))return;
    norrath_animation_update_component(pointer(component));
    const Address frame=read_word(component+4u);
    const float time=read_float(component+24u);
    const int previous=int(short_bits(half(frame+28u)));
    const float x_speed=read_float(frame+44u),y_speed=read_float(frame+48u);
    const float original_x=read_float(frame+32u),original_y=read_float(frame+36u);
    const float delta=time-float(previous);
    const float y_product=y_speed*delta,x_product=x_speed*delta;
    const float current_y=original_y+y_product,current_x=original_x+x_product;
    const float y_offset=original_y-current_y,x_offset=original_x-current_x;
    write_float(frame+36u,y_offset);write_float(frame+32u,x_offset);
}
