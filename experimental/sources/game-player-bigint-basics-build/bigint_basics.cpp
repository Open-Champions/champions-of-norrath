#include "bigint_basics.hpp"
using namespace norrath_player_number_detail;

extern "C" unsigned char* norrath_player_calloc_reentrant(unsigned char* reent,Word count,Word width) {
    unsigned char* allocation=norrath_player_malloc_reentrant(reent,count*width);
    if(allocation==nullptr)return nullptr;
    const Word start=address(allocation);
    const Word available=(read_word(start-4u)&0xfffffffcu)-4u;
    if(available>=37u) {
        norrath_block_memory_set(allocation,0,available);
    } else {
        Word cursor=start;
        if(available>=20u) {
            write_word(start,0u);write_word(start+4u,0u);cursor=start+8u;
            if(available>=28u) {
                write_word(cursor,0u);cursor=start+16u;write_word(start+12u,0u);
                if(available>=36u) {
                    write_word(cursor,0u);cursor=start+24u;write_word(start+20u,0u);
                }
            }
        }
        write_word(cursor,0u);cursor+=4u;
        write_word(cursor+4u,0u);write_word(cursor,0u);
    }
    return allocation;
}

extern "C" unsigned char* norrath_player_bigint_allocate(unsigned char* reent,int size_class) {
    const Word context=address(reent), index=Word(size_class);
    Word heads=read_word(context+76u);
    if(heads==0u) {
        unsigned char* allocated=norrath_player_calloc_reentrant(reent,4u,16u);
        heads=address(allocated);
        write_word(context+76u,heads);
        if(heads==0u)return nullptr;
    }
    const Word slot=heads+index*4u;
    Word block=read_word(slot);
    if(block!=0u) {
        write_word(slot,read_word(block));
    } else {
        const Word capacity=1u<<(index&31u);
        unsigned char* allocated=norrath_player_calloc_reentrant(reent,1u,capacity*4u+20u);
        block=address(allocated);
        if(block==0u)return nullptr;
        write_word(block+4u,index);write_word(block+8u,capacity);
    }
    write_word(block+16u,0u);write_word(block+12u,0u);
    return object(block);
}

extern "C" void norrath_player_bigint_release(unsigned char* reent,unsigned char* allocation) {
    if(allocation==nullptr)return;
    const Word block=address(allocation);
    const Word index=read_word(block+4u);
    const Word slot=read_word(address(reent)+76u)+index*4u;
    const Word next=read_word(slot);
    write_word(block,next);write_word(slot,block);
}

extern "C" unsigned char* norrath_player_bigint_from_integer(unsigned char* reent,int integer) {
    unsigned char* allocation=norrath_player_bigint_allocate(reent,1);
    const Word block=address(allocation);
    write_word(block+20u,Word(integer));write_word(block+16u,1u);
    return allocation;
}

extern "C" int norrath_player_bigint_compare(unsigned char* left,unsigned char* right) {
    const Word l=address(left),r=address(right);
    const Word ln=read_word(l+16u),rn=read_word(r+16u);
    const Word difference=ln-rn;
    if(difference!=0u)return signed_word(difference);
    const Word begin=l+20u;
    Word lp=begin+rn*4u-4u,rp=r+20u+rn*4u;
    for(;;) {
        rp-=4u;
        const Word lw=read_word(lp),rw=read_word(rp);
        const bool another=begin<lp;
        if(lw!=rw)return lw<rw?-1:1;
        if(!another)return 0;
        lp-=4u;
    }
}

extern "C" Wide norrath_player_double_ulp(Wide value) {
    const Word exponent=(Word(value>>32)&0x7ff00000u)+0xfcc00000u;
    if(signed_word(exponent)>0)return Wide(exponent)<<32;
    // SRA20 of the wrapped low-word negation; subsequent SRAV/SLLV counts
    // are the architecture's low five bits even on exceptional input words.
    const Word negative=0u-exponent;
    const Word shifted=(negative>>20)|(negative&0x80000000u?0xfffff000u:0u);
    if(signed_word(shifted)<20)return Wide(0x00080000u>>(shifted&31u))<<32;
    const Word low_exponent=shifted-20u;
    if(signed_word(low_exponent)<31)return Wide(1u<<((~low_exponent)&31u));
    return 1ull;
}
