#include "float_tags.hpp"
extern "C" void norrath_player_unpack_double(const unsigned char* input,unsigned char* output){
    using namespace norrath_float_tags_detail;
    const Wide value=wide(address(input));
    const Address a=address(output);
    const unsigned sign=unsigned(value>>63),exponent=unsigned(value>>52)&2047u;
    Wide fraction=value&0x000fffffffffffffULL;
    write_word(a+4u,sign);
    if(exponent==0u){
        write_word(a,2u); // Actual flush of all exponent-zero encodings.
        return; // Original leaves exponent/fraction/padding bytes untouched.
    }
    if(exponent==2047u){
        write_word(a+8u,1023u);
        if(fraction==0u){
            put_wide(a+16u,0x1000000000000000ULL);write_word(a,4u);
            return;
        }
        write_word(a,(fraction&0x0008000000000000ULL)?1u:0u);
        put_wide(a+16u,fraction);
        return;
    }
    fraction=(fraction<<8)|0x1000000000000000ULL;
    put_wide(a+16u,fraction);write_word(a+8u,exponent-1023u);write_word(a,3u);
}
extern "C" float norrath_player_pack_float(const unsigned char* parts){
    using namespace norrath_float_tags_detail;
    const Address a=address(parts);
    const unsigned category=read_word(a);
    unsigned fraction=read_word(a+12u),exponent=0u;
    const unsigned sign=read_word(a+4u);
    if(category==2u)fraction=0u;
    else if(fraction!=0u){
        const int power=signed_word(read_word(a+8u));
        if(power < -126)fraction=0u;
        else if(power>=129){exponent=255u;fraction=0u;}
        else{
            exponent=unsigned(power)+127u;
            if(power!=128 || signed_word(fraction)>=0)fraction>>=7;
            else{exponent=255u;fraction=0u;}
        }
    }
    // All original incoming A2 residue bits are overwritten by the masks.
    // No IEEE/host cast, rounding repair or exceptional-value substitution.
    const unsigned packed=(fraction&0x007fffffu)|((exponent&255u)<<23)|(sign<<31);
    return float_bits(packed);
}
extern "C" float norrath_player_make_float(unsigned category,int sign,int exponent,unsigned fraction){
    using namespace norrath_float_tags_detail;
    alignas(16) unsigned char parts[16];
    const Address a=address(parts);
    write_word(a,category);write_word(a+4u,unsigned(sign));write_word(a+8u,unsigned(exponent));write_word(a+12u,fraction);
    return norrath_player_pack_float(parts);
}
extern "C" float norrath_player_double_to_float(NorrathFormatUnsigned64 input){
    using namespace norrath_float_tags_detail;
    alignas(16) unsigned char parts[24]; // Native output leaves some fields uninitialized on exponent0.
    alignas(8) unsigned char raw[8];put_wide(address(raw),input);
    norrath_player_unpack_double(raw,parts);
    const Address a=address(parts);
    const Wide fraction=wide(a+16u);
    const unsigned shifted=unsigned((fraction<<2)>>32);
    const unsigned narrowed=shifted|((fraction&0x3fffffffu)!=0u?1u:0u);
    return norrath_player_make_float(read_word(a),signed_word(read_word(a+4u)),signed_word(read_word(a+8u)),narrowed);
}
