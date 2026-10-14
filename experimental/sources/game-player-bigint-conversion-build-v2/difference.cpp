#include "bigint_conversion.hpp"
using namespace norrath_bigint_conversion_detail;

extern "C" unsigned char* norrath_player_bigint_difference(unsigned char* reent,unsigned char* left,unsigned char* right) {
    const int order=norrath_player_bigint_compare(left,right);
    if(order==0) {
        unsigned char* zero=norrath_player_bigint_allocate(reent,0);
        const Word output=address(zero);
        write_word(output+16u,1u);write_word(output+20u,0u);return zero;
    }
    Word sign=0u;
    if(order<0){unsigned char* previous=left;left=right;right=previous;sign=1u;}
    const Word l=address(left),r=address(right);
    const int size_class=signed_word(read_word(l+4u));
    unsigned char* difference=norrath_player_bigint_allocate(reent,size_class);
    const Word output=address(difference);
    write_word(output+12u,sign);
    Word left_cursor=l+20u,right_cursor=r+20u,output_cursor=output+20u,borrow=0u;
    Word count=read_word(l+16u);
    const Word right_count=read_word(r+16u);
    const Word left_end=left_cursor+count*4u,right_end=right_cursor+right_count*4u;
    // The original reads and subtracts at least one limb even for a zero count.
    do {
        const Word a=read_word(left_cursor);left_cursor+=4u;
        const Word b=read_word(right_cursor);right_cursor+=4u;
        const Word lower=(a&0xffffu)-(b&0xffffu)+borrow;
        const Word upper=(a>>16)-(b>>16)+arithmetic_right16(lower);
        write_half(output_cursor,lower);write_half(output_cursor+2u,upper);
        output_cursor+=4u;borrow=arithmetic_right16(upper);
    } while(right_cursor<right_end);
    while(left_cursor<left_end) {
        const Word a=read_word(left_cursor);left_cursor+=4u;
        const Word lower=(a&0xffffu)+borrow;
        const Word upper=(a>>16)+arithmetic_right16(lower);
        write_half(output_cursor,lower);write_half(output_cursor+2u,upper);
        output_cursor+=4u;borrow=arithmetic_right16(upper);
    }
    output_cursor-=4u;
    if(read_word(output_cursor)==0u) {
        do {output_cursor-=4u;const Word limb=read_word(output_cursor);--count;if(limb!=0u)break;}while(true);
    }
    write_word(output+16u,count);return difference;
}
