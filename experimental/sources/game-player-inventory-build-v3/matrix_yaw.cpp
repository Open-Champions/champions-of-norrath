#include "player_inventory.hpp"
namespace {
int signed_field(unsigned word,unsigned width){
    const unsigned mask=(1u<<width)-1u;
    word&=mask;
    return word<(1u<<(width-1u))?int(word):int(word)-int(1u<<width);
}
float polynomial(float angle,float scale,float third,float fifth,float seventh){
    float x=angle*scale;
    const float square=x*x;
    float odd=x*square;
    const float cubic=odd*third;
    odd=odd*square;
    x=x-cubic;
    const float next=odd*square;
    const float quintic=odd*fifth;
    const float heptic=next*seventh;
    x=x+quintic;x=x-heptic;
    const float twice=x+x;
    const float product=twice*x;
    return 1.0f-product;
}
}
extern "C" void norrath_matrix_make_translation_yaw(void* output,const void* point,short yaw){
    using namespace norrath_player_detail;
    const RawPoint position=snapshot(address(point)); // Original full12 before first output store.
    const Address a=address(output);
    const unsigned raw=unsigned(yaw);
    unsigned shifted=unsigned(short_bits(raw-16384u));
    const unsigned first_mask=(raw^(raw<<1))&0x8000u;
    const unsigned second_mask=(shifted^(shifted<<1))&0x8000u;
    int first=int(yaw),second=int(short_bits(shifted));
    float first_sign=1.0f,second_sign=1.0f;
    if(first_mask){first_sign=-1.0f;first=signed_field(raw,15u);}
    const float scale=read_float(address(norrath_player_view_47fed4));
    const float third=read_float(address(norrath_player_view_47fed8));
    const float fifth=read_float(address(norrath_player_view_47fedc));
    const float seventh=read_float(address(norrath_player_view_47fee0));
    const float first_value=polynomial(float(first),scale,third,fifth,seventh)*first_sign;
    if(second_mask){second_sign=-1.0f;second=signed_field(shifted,15u);}
    write_float(a,first_value);write_float(a+16u,first_value);write_float(a+32u,1.0f);
    write_word(a+8u,0u);write_word(a+20u,0u);write_word(a+24u,0u);write_word(a+28u,0u);
    write_word(a+44u,position.z);write_word(a+36u,position.x);write_word(a+40u,position.y);
    const float second_value=polynomial(float(second),scale,third,fifth,seventh)*second_sign;
    write_float(a+4u,second_value);write_float(a+12u,-second_value);
}
