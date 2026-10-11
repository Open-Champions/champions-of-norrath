#include "numeric_conversion.hpp"
using namespace norrath_numeric_conversion_detail;

extern "C" Wide norrath_player_signed32_to_double(int input) {
    unsigned char parts[24];
    const Word p=address(parts),bits=Word(input),sign=bits>>31;
    write_word(p,3u);write_word(p+4u,sign);
    if(bits==0u) {
        write_word(p,2u);
        // Original leaves exponent, fraction and padding untouched on zero.
        return norrath_player_pack_double(parts);
    }
    write_word(p+8u,60u);
    if(sign!=0u&&bits==0x80000000u)return 0xc1e0000000000000ull;
    Wide fraction=sign==0u?extend_signed_word(bits):extend_signed_word(0u-bits);
    write_wide(p+16u,fraction);
    if(fraction<=0x0fffffffffffffffull) {
        Word exponent=read_word(p+8u);
        do {fraction<<=1;--exponent;}while(fraction<=0x0fffffffffffffffull);
        write_word(p+8u,exponent);write_wide(p+16u,fraction);
    }
    return norrath_player_pack_double(parts);
}

extern "C" Wide norrath_player_signed64_to_double(NorrathFormatSigned64 input) {
    const Wide bits=Wide(input),factor=0x40f0000000000000ull;
    Wide upper=norrath_player_signed32_to_double(signed_word(Word(bits>>32)));
    upper=norrath_player_double_multiply(upper,factor);
    upper=norrath_player_double_multiply(upper,factor);
    const int low=signed_word(Word(bits));
    Wide lower=norrath_player_signed32_to_double(low);
    if(low<0)lower=norrath_player_double_add(lower,0x41f0000000000000ull);
    return norrath_player_double_add(upper,lower);
}

extern "C" int norrath_player_double_to_signed32(Wide input) {
    unsigned char raw[8],parts[24];
    write_wide(address(raw),input);
    norrath_player_unpack_double(raw,parts);
    const Word p=address(parts),category=read_word(p);
    if(category==2u||category<2u)return 0;
    if(category==4u)return read_word(p+4u)==0u?0x7fffffff:signed_word(0x80000000u);
    const Word exponent=read_word(p+8u);
    if(signed_word(exponent)<0)return 0;
    if(signed_word(exponent)>=31)return read_word(p+4u)==0u?0x7fffffff:signed_word(0x80000000u);
    const Wide fraction=read_wide(p+16u);
    const Word sign=read_word(p+4u);
    const Word magnitude=Word(fraction>>((60u-exponent)&63u));
    return signed_word(sign==0u?magnitude:0u-magnitude);
}

extern "C" Wide norrath_player_double_to_unsigned32_bits(Wide input) {
    unsigned char raw[8],parts[24];
    write_wide(address(raw),input);
    norrath_player_unpack_double(raw,parts);
    const Word p=address(parts),category=read_word(p);
    if(category==2u||category<2u)return 0ull;
    if(read_word(p+4u)!=0u)return 0ull;
    if(category==4u)return 0xffffffffffffffffull;
    const Word exponent=read_word(p+8u);
    if(signed_word(exponent)<0)return 0ull;
    if(signed_word(exponent)>=32)return 0xffffffffffffffffull;
    // Preserve both original shift paths even though the earlier exponent
    // bound prevents the >=61 path for a canonical signed32 exponent.
    Wide magnitude;
    if(signed_word(exponent)>=61)magnitude=read_wide(p+16u)<<((exponent-60u)&63u);
    else magnitude=read_wide(p+16u)>>((60u-exponent)&63u);
    return extend_signed_word(Word(magnitude));
}

extern "C" Wide norrath_player_double_to_unsigned64(Wide input) {
    if(norrath_factory_double_compare(input,0ull)<0)return 0ull;
    const Wide scaled=norrath_player_double_multiply(input,0x3df0000000000000ull);
    const Wide high_bits=norrath_player_double_to_unsigned32_bits(scaled)<<32;
    Wide represented;
    if(signed_wide(high_bits)<0) {
        const Wide half=(high_bits>>1)|(high_bits&1ull);
        represented=norrath_player_signed64_to_double(signed_wide(half));
        represented=norrath_player_double_add(represented,represented);
    } else represented=norrath_player_signed64_to_double(signed_wide(high_bits));
    const Wide remainder=norrath_factory_double_subtract(input,represented);
    if(norrath_factory_double_compare(remainder,0ull)<0) {
        const Wide magnitude=norrath_factory_double_subtract(0ull,remainder);
        const Word low=Word(norrath_player_double_to_unsigned32_bits(magnitude));
        return high_bits-Wide(low);
    }
    const Word low=Word(norrath_player_double_to_unsigned32_bits(remainder));
    return high_bits+Wide(low);
}

extern "C" NorrathFormatSigned64 norrath_player_double_to_signed64(Wide input) {
    if(norrath_factory_double_compare(input,0ull)<0) {
        const Wide magnitude=norrath_factory_double_subtract(0ull,input);
        const Wide converted=norrath_player_double_to_unsigned64(magnitude);
        return signed_wide(0ull-converted);
    }
    return signed_wide(norrath_player_double_to_unsigned64(input));
}
