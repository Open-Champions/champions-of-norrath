#pragma once
#include "reentrant_free.hpp"
extern "C" {
unsigned char* norrath_player_malloc_reentrant(unsigned char*,unsigned);
void norrath_player_malloc_extend_top(unsigned char*,unsigned);
}
namespace norrath_malloc_detail {
using namespace norrath_core_format_detail;
// Original first takes wrapped32 magnitudes, then widens BEFORE signed64
// difference comparisons. A signed32 reinterpretation would change large sizes.
inline NorrathFormatSigned64 size_difference(Word size,Word requested) {
    return size<requested?-NorrathFormatSigned64(requested-size):NorrathFormatSigned64(size-requested);
}
inline Word bin_index(Word size) {
    const Word high=size>>9;
    if(high==0u)return size>>3;
    if(high<5u)return (size>>6)+56u;
    if(high<21u)return high+91u;
    if(high<85u)return (size>>12)+110u;
    if(high<341u)return (size>>15)+119u;
    if(high<1365u)return (size>>18)+124u;
    return 126u;
}
inline Word signed_group_shift(Word index) {
    if(signed_word(index)<0)index+=3u;
    return (index>>2)|(index&0x80000000u?0xc0000000u:0u);
}
inline void insert_old_remainder(Word bins,Word chunk,Word size) {
    Word front,back;
    if(size<512u) {
        const Word index=size>>3;
        back=bins+index*8u;
        const Word mask=read_word(bins+4u);
        front=read_word(back+8u);
        write_word(chunk+12u,back);write_word(chunk+8u,front);
        write_word(bins+4u,mask|Word(1ull<<((index>>2)&63u)));
    } else {
        const Word index=bin_index(size),head=bins+index*8u;
        back=head;front=read_word(head+8u);
        if(front==head) {
            const Word mask=read_word(bins+4u);
            write_word(bins+4u,mask|Word(1ull<<(signed_group_shift(index)&63u)));
        } else {
            while(size<(read_word(front+4u)&0xfffffffcu)) {
                front=read_word(front+8u);
                if(front==head)break;
            }
            back=read_word(front+12u);
        }
        write_word(chunk+12u,back);write_word(chunk+8u,front);
    }
    write_word(back+8u,chunk);write_word(front+12u,chunk);
}
}
