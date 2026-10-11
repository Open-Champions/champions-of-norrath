#include "creature_animation.hpp"
using namespace creature_detail;
namespace {
// The original window is two actual little-endian halfwords joined high-half first.
// Every extraction needs this complete four-byte mapped window, even for one bit.
unsigned shifted_window(Address stream,unsigned bit_position){
    const Address a=stream+(sra(bit_position,4u)<<1);
    const unsigned joined=(half(a)<<16)|half(a+2u);
    return joined<<(bit_position&15u);
}
unsigned unsigned_bits(Address stream,unsigned at,unsigned width){return shifted_window(stream,at)>>(32u-width);}
int signed_bits(Address stream,unsigned at,unsigned width){return signed_word(sra(shifted_window(stream,at),32u-width));}
float encoded_scalar(Address stream,unsigned at,unsigned width,float factor){
    const float converted=static_cast<float>(signed_bits(stream,at,width));
    return converted*factor;
}
// Full genuine LOCAL updateAnimation2248 candidate. Internal source linkage retains
// its private identity; it is not admitted as an original public graph function.
void update_animation(void* component){
    const Address c=address(component);
    Address frame=word(c+4u);
    if(frame==0u){
        frame=address(norrath_animation_frame_state_acquire());
        put_word(frame+20u,c);put_word(c+4u,frame);
    }
    const unsigned initial_next=word(frame+16u);
    if(initial_next!=0u){
        const unsigned previous=word(frame+12u);
        if(previous==0u)put_word(address(norrath_creature_view_486910),initial_next);
        else put_word(previous+16u,initial_next);
        const unsigned tail=word(address(norrath_creature_view_48690c));
        const unsigned fresh_next=word(frame+16u);
        const unsigned fresh_previous=word(frame+12u);
        put_word(fresh_next+12u,fresh_previous);
        put_word(frame+12u,tail);put_word(tail+16u,frame);
        put_word(address(norrath_creature_view_48690c),frame);put_word(frame+16u,0u);
        frame=word(c+4u);
    }
    const unsigned target_time=unsigned(chop(scalar(c+24u)));
    const Address animation=word(c);
    const unsigned part_count=word(animation);
    if(signed_word(target_time)<signed_word(word(frame+8u))){
        put_word(frame+8u,0u);
        const Address stream=word(animation+8u);
        unsigned bit_position=0u;
        if(signed_word(part_count)>0){
            unsigned part=0u;
            do {
                const Address row=frame+part*60u;
                put_half(row+30u,0u);put_half(row+28u,0u);
                const unsigned translation_width=unsigned_bits(stream,bit_position,4u)+1u;
                bit_position+=4u;
                // First two translation values are consumed without reads/writes
                // for the original root-motion flag on part0.
                const bool skip_xy=(half(c+28u)&0x800u)!=0u && part==0u;
                if(!skip_xy){
                    const float x=encoded_scalar(stream,bit_position,translation_width,bits(0x3c800000u));
                    const float y=encoded_scalar(stream,bit_position+translation_width,translation_width,bits(0x3c800000u));
                    put_scalar(row+32u,x);put_scalar(row+36u,y);
                }
                bit_position+=translation_width*2u;
                const float z=encoded_scalar(stream,bit_position,translation_width,bits(0x3c800000u));
                bit_position+=translation_width;
                const unsigned quaternion_width=unsigned_bits(stream,bit_position,4u)+1u;
                bit_position+=4u;
                const float w=encoded_scalar(stream,bit_position,quaternion_width,bits(0x39800000u));
                const float x=encoded_scalar(stream,bit_position+quaternion_width,quaternion_width,bits(0x39800000u));
                const float y=encoded_scalar(stream,bit_position+quaternion_width*2u,quaternion_width,bits(0x39800000u));
                const float qz=encoded_scalar(stream,bit_position+quaternion_width*3u,quaternion_width,bits(0x39800000u));
                bit_position+=quaternion_width*4u;
                put_scalar(row+40u,z);
                put_word(row+52u,0u);put_word(row+48u,0u);put_word(row+44u,0u);
                // Packed quaternion order is W,X,Y,Z; memory is X,Y,Z,W.
                put_scalar(row+68u,w);put_scalar(row+56u,x);put_scalar(row+60u,y);put_scalar(row+64u,qz);
                put_word(row+80u,0u);put_word(row+76u,0u);put_word(row+72u,0u);put_word(row+84u,0u);
                part+=1u;
            } while(signed_word(part)<signed_word(part_count));
        }
        // The original uses the first row even for a nonpositive part count.
        const float x=scalar(frame+56u),y=scalar(frame+60u),z=scalar(frame+64u),w=scalar(frame+68u);
        const float y_squared=y*y;const float x_squared=x*x;
        const float xy=x*y;const float z_squared=z*z;const float zw=z*w;
        const float first_difference=x_squared-y_squared;
        const float w_squared=w*w;const float products=xy+zw;
        const float second_difference=first_difference-z_squared;
        const float numerator=products+products;
        const float denominator=second_difference+w_squared;
        put_word(frame+4u,bit_position);put_word(frame,stream);
        const int angle=norrath_integer_atan2(numerator,denominator);
        put_half(frame+24u,unsigned(angle));
    }
    const Address stream=word(frame);
    for(;;){
        unsigned bit_position=word(frame+4u);
        const unsigned duration=unsigned_bits(stream,bit_position,8u);
        const unsigned next_time=word(frame+8u)+duration;
        bit_position+=8u;
        if(signed_word(target_time)<signed_word(next_time))return;
        const unsigned event_start=bit_position;
        const unsigned kind=unsigned_bits(stream,event_start,1u);
        const unsigned part=unsigned_bits(stream,event_start+1u,6u);
        bit_position=event_start+7u;
        put_word(frame+8u,next_time);
        const Address row=frame+part*60u;
        if(kind!=0u){
            const float elapsed=static_cast<float>(signed_word(next_time-unsigned(signed_half(half(row+28u)))));
            const float dz=scalar(row+52u)*elapsed;
            const float dx=scalar(row+44u)*elapsed;
            const float dy=scalar(row+48u)*elapsed;
            const unsigned width=unsigned_bits(stream,bit_position,4u)+1u;
            bit_position+=4u;
            const float new_x=scalar(row+32u)+dx;
            const float old_z=scalar(row+40u),old_y=scalar(row+36u);
            put_scalar(row+32u,new_x);
            const float x=encoded_scalar(stream,bit_position,width,bits(0x3b800000u));
            const float y=encoded_scalar(stream,bit_position+width,width,bits(0x3b800000u));
            const float z=encoded_scalar(stream,bit_position+width*2u,width,bits(0x3b800000u));
            bit_position+=width*3u;
            const float new_y=old_y+dy;
            put_scalar(row+36u,new_y);
            put_half(row+28u,next_time);
            const float new_z=old_z+dz;
            put_scalar(row+40u,new_z);
            put_scalar(row+44u,x);put_scalar(row+48u,y);put_scalar(row+52u,z);
        } else {
            const float elapsed=static_cast<float>(signed_word(next_time-unsigned(signed_half(half(row+30u)))));
            const float dz=scalar(row+80u)*elapsed;
            const float dw=scalar(row+84u)*elapsed;
            const float dx=scalar(row+72u)*elapsed;
            const float dy=scalar(row+76u)*elapsed;
            const unsigned width=unsigned_bits(stream,bit_position,4u)+1u;
            bit_position+=4u;
            const float new_w=scalar(row+68u)+dw;
            const float old_x=scalar(row+56u),old_y=scalar(row+60u),old_z=scalar(row+64u);
            put_scalar(row+68u,new_w);
            const float w=encoded_scalar(stream,bit_position,width,bits(0x37000000u));
            const float x=encoded_scalar(stream,bit_position+width,width,bits(0x37000000u));
            const float y=encoded_scalar(stream,bit_position+width*2u,width,bits(0x37000000u));
            const float z=encoded_scalar(stream,bit_position+width*3u,width,bits(0x37000000u));
            bit_position+=width*4u;
            put_scalar(row+56u,old_x+dx);put_scalar(row+60u,old_y+dy);put_scalar(row+64u,old_z+dz);
            put_scalar(row+84u,w);put_scalar(row+72u,x);put_scalar(row+76u,y);put_scalar(row+80u,z);
            put_half(row+30u,next_time);
        }
        put_word(frame+4u,bit_position);
    }
}
}
extern "C" void norrath_actor_animation_randomize_time(void* state){
    const Address a=address(state);
    const int count=signed_byte(bytes(a)[4]);
    if(count==0)return;
    const Address component=a+(unsigned(count-1)<<5)+8u;
    const Address header=word(component);
    const unsigned flags=half(component+28u);
    const unsigned next=word(address(norrath_creature_view_483e58))*0x19660du+0x3c6ef35fu;
    const float duration=static_cast<float>(signed_word(word(header+4u)));
    const float sample=static_cast<float>(signed_word(next>>16));
    put_word(address(norrath_creature_view_483e58),next);
    const float fraction=sample*bits(0x37800000u);
    const float time=fraction*duration;
    put_scalar(component+24u,time);
    if((flags&0x800u)==0u)return;
    update_animation(pointer(component));
    const Address frame=word(component+4u);
    const float fresh_time=scalar(component+24u);
    const float previous=static_cast<float>(signed_half(half(frame+28u)));
    const float delta=fresh_time-previous;
    const float y_term=scalar(frame+48u)*delta;
    const float x_term=scalar(frame+44u)*delta;
    const float old_x=scalar(frame+32u),old_y=scalar(frame+36u);
    const float added_y=old_y+y_term;
    const float added_x=old_x+x_term;
    const float adjusted_y=old_y-added_y;
    const float adjusted_x=old_x-added_x;
    put_scalar(frame+36u,adjusted_y);put_scalar(frame+32u,adjusted_x);
}
