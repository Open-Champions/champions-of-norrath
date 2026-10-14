#include "bigint_conversion.hpp"
using namespace norrath_bigint_conversion_detail;

extern "C" int norrath_player_bigint_leading_zeros(Word value) {
    Word count=0u;
    if((value&0xffff0000u)==0u){count=16u;value<<=16;}
    if((value&0xff000000u)==0u){count+=8u;value<<=8;}
    if((value&0xf0000000u)==0u){count+=4u;value<<=4;}
    if((value&0xc0000000u)==0u){count+=2u;value<<=2;}
    if(signed_word(value)<0)return signed_word(count);
    ++count;
    return (value&0x40000000u)==0u?32:signed_word(count);
}

extern "C" int norrath_player_bigint_trailing_zeros(unsigned char* storage) {
    const Word destination=address(storage);
    Word value=read_word(destination);
    if(value&7u) {
        if(value&1u)return 0;
        if(value&2u){write_word(destination,value>>1);return 1;}
        write_word(destination,value>>2);return 2;
    }
    Word count=0u;
    if((value&0xffffu)==0u){count=16u;value>>=16;}
    if((value&0xffu)==0u){count+=8u;value>>=8;}
    if((value&0xfu)==0u){count+=4u;value>>=4;}
    if((value&3u)==0u){count+=2u;value>>=2;}
    if((value&1u)==0u) {
        value>>=1;++count;
        if(value==0u)return 32; // Native zero path returns without writing storage.
    }
    write_word(destination,value);return signed_word(count);
}
