#include "reentrant_free.hpp"
using namespace norrath_core_format_detail;

extern "C" void norrath_player_free_reentrant(unsigned char* reent,unsigned char* allocation) {
    if(allocation==nullptr)return;
    norrath_player_malloc_lock(reent);
    Word chunk=address(allocation)-8u;
    const Word header=read_word(chunk+4u);
    Word size=header&0xfffffffeu;
    const Word bins=address(norrath_player_malloc_bins_view);
    const Word following=chunk+size;
    const Word top=read_word(bins+8u);
    const Word following_size=read_word(following+4u)&0xfffffffcu;
    if(following==top) {
        size+=following_size;
        if((header&1u)==0u) {
            const Word previous_size=read_word(chunk);
            chunk-=previous_size;size+=previous_size;
            const Word back=read_word(chunk+12u),front=read_word(chunk+8u);
            write_word(front+12u,back);write_word(back+8u,front);
        }
        write_word(bins+8u,chunk);write_word(chunk+4u,size|1u);
        const Wide threshold=read_wide(address(norrath_player_malloc_trim_threshold_view));
        if(Wide(size)>=threshold)norrath_player_malloc_trim_reentrant(reent,read_word(address(norrath_player_malloc_top_pad_view)));
        norrath_player_malloc_unlock(reent);return;
    }
    write_word(following+4u,following_size);
    bool in_remainder=false;
    if((header&1u)==0u) {
        const Word previous_size=read_word(chunk);
        chunk-=previous_size;size+=previous_size;
        const Word front=read_word(chunk+8u);
        if(front==bins+8u)in_remainder=true;
        else {
            const Word back=read_word(chunk+12u);
            write_word(front+12u,back);write_word(back+8u,front);
        }
    }
    const Word next_header=read_word(following+following_size+4u);
    if((next_header&1u)==0u) {
        size+=following_size;
        const Word front=read_word(following+8u);
        if(!in_remainder&&front==bins+8u) {
            write_word(front+12u,chunk);in_remainder=true;
            write_word(front+8u,chunk);write_word(chunk+8u,front);write_word(chunk+12u,front);
        } else {
            const Word back=read_word(following+12u);
            write_word(front+12u,back);write_word(back+8u,front);
        }
    }
    const Word footer=chunk+size;
    write_word(chunk+4u,size|1u);write_word(footer,size);
    if(!in_remainder) {
        Word front,back;
        if(size<512u) {
            const Word index=size>>3;
            back=bins+index*8u;
            const Word mask=read_word(bins+4u);
            front=read_word(back+8u);
            write_word(chunk+12u,back);write_word(chunk+8u,front);
            write_word(bins+4u,mask|Word(1ull<<((index>>2)&63u)));
        } else {
            const Word high=size>>9;
            Word index=size>>3;
            if(high!=0u) {
                if(high<5u)index=(size>>6)+56u;
                else if(high<21u)index=high+91u;
                else if(high<85u)index=(size>>12)+110u;
                else if(high<341u)index=(size>>15)+119u;
                else if(high<1365u)index=(size>>18)+124u;
                else index=126u;
            }
            const Word head=bins+index*8u;
            back=head;front=read_word(head+8u);
            if(front==head) {
                Word adjusted=index;
                if(signed_word(index)<0)adjusted+=3u;
                const Word shifted=(adjusted>>2)|(adjusted&0x80000000u?0xc0000000u:0u);
                const Word mask=read_word(bins+4u);
                write_word(bins+4u,mask|Word(1ull<<(shifted&63u)));
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
    norrath_player_malloc_unlock(reent);
}
