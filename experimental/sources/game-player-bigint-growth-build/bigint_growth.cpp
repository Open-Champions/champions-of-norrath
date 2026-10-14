#include "bigint_growth.hpp"
using namespace norrath_bigint_growth_detail;

extern "C" unsigned char* norrath_player_bigint_multiply_add(unsigned char* reent,unsigned char* block,int factor,int addition){
    Word allocation=address(block),carry=Word(addition),cursor=allocation+20u;
    const Word limbs=read_word(allocation+16u);Word index=0u;
    // Actual first limb is processed even when the captured limb count is zero.
    do{
        const Word value=read_word(cursor);++index;
        const Word low=(value&0xffffu)*Word(factor)+carry;
        const Word high_part=(value>>16)*Word(factor)+(low>>16);
        carry=high_part>>16;
        write_word(cursor,(high_part<<16)+(low&0xffffu));cursor+=4u;
    }while(signed_word(index)<signed_word(limbs));
    if(carry!=0u){
        const Word capacity=read_word(allocation+8u);
        if(signed_word(limbs)>=signed_word(capacity)){
            const Word size_class=read_word(allocation+4u)+1u;
            const Word grown=address(norrath_player_bigint_allocate(reent,signed_word(size_class)));
            // Fresh original count read after allocation; copy sign/count and limbs.
            const Word copy_count=read_word(allocation+16u)*4u+8u;
            (void)norrath_copy_bytes(bytes(grown+12u),bytes(allocation+12u),copy_count);
            const Word previous=allocation;allocation=grown;
            norrath_player_bigint_release(reent,bytes(previous));
        }
        write_word(allocation+20u+limbs*4u,carry);
        write_word(allocation+16u,limbs+1u);
    }
    return bytes(allocation);
}

extern "C" unsigned char* norrath_player_bigint_from_text(unsigned char* reent,const char* text,int integer_digits,int total_digits,Word first_nine){
    Word cursor=address(text),capacity=1u,size_class=0u;
    const int groups=signed_word(Word(total_digits)+8u)/9;
    while(signed_word(capacity)<groups){capacity<<=1u;++size_class;}
    Word block=address(norrath_player_bigint_allocate(reent,signed_word(size_class)));
    write_word(block+20u,first_nine);write_word(block+16u,1u);
    Word index=9u;
    if(integer_digits>=10){
        cursor+=9u;
        do{
            const int digit=character(cursor)-48;++cursor;++index;
            block=address(norrath_player_bigint_multiply_add(reent,bytes(block),10,digit));
        }while(signed_word(index)<integer_digits);
        ++cursor; // Actual unconditional decimal-separator skip after integer loop.
    }else cursor+=10u;
    if(signed_word(index)<total_digits){
        Word left=Word(total_digits)-index;
        do{
            const int digit=character(cursor)-48;++cursor;--left;
            block=address(norrath_player_bigint_multiply_add(reent,bytes(block),10,digit));
        }while(left!=0u);
    }
    return bytes(block);
}

extern "C" unsigned char* norrath_player_bigint_shift(unsigned char* reent,unsigned char* block,int shift){
    const Word original=address(block),whole_words=arithmetic_right(Word(shift),5u);
    const Word old_limbs=read_word(original+16u);
    Word capacity=read_word(original+8u);
    Word wanted=whole_words+old_limbs+1u;
    Word size_class=read_word(original+4u);
    while(signed_word(capacity)<signed_word(wanted)){capacity<<=1u;++size_class;}
    const Word grown=address(norrath_player_bigint_allocate(reent,signed_word(size_class)));
    Word output=grown+20u;
    if(signed_word(whole_words)>0){
        Word left=whole_words;
        do{--left;write_word(output,0u);output+=4u;}while(left!=0u);
    }
    // Allocation may mutate original fields: native copy-end count is re-read now.
    Word input=original+20u;
    const Word end=input+read_word(original+16u)*4u;
    const Word low_bits=Word(shift)&31u;
    if(low_bits!=0u){
        const Word complementary=32u-low_bits;Word carry=0u;
        do{
            const Word value=read_word(input);
            write_word(output,(value<<low_bits)|carry);output+=4u;
            const Word fresh=read_word(input);input+=4u;carry=fresh>>complementary;
        }while(input<end);
        // Original branch delay stores this final word even when carry is zero.
        write_word(output,carry);if(carry!=0u)++wanted;
    }else{
        do{const Word value=read_word(input);input+=4u;write_word(output,value);output+=4u;}while(input<end);
    }
    write_word(grown+16u,wanted-1u);
    norrath_player_bigint_release(reent,block);
    return bytes(grown);
}

extern "C" unsigned char* norrath_player_bigint_multiply(unsigned char* reent,unsigned char* left,unsigned char* right){
    Word longer=address(left),shorter=address(right);
    Word long_count=read_word(longer+16u),short_count=read_word(shorter+16u);
    if(signed_word(long_count)<signed_word(short_count)){
        const Word previous=longer;longer=shorter;shorter=previous;
        long_count=short_count;short_count=read_word(shorter+16u);
    }
    const Word capacity=read_word(longer+8u),product_count=long_count+short_count;
    const Word size_class=read_word(longer+4u)+Word(signed_word(capacity)<signed_word(product_count));
    const Word product=address(norrath_player_bigint_allocate(reent,signed_word(size_class)));
    const Word product_start=product+20u,product_end=product_start+product_count*4u;
    for(Word cursor=product_start;cursor<product_end;cursor+=4u)write_word(cursor,0u);
    Word multiplier=shorter+20u;
    const Word multiplier_end=multiplier+short_count*4u;
    const Word multiplicand=longer+20u,multiplicand_end=multiplicand+long_count*4u;
    Word row=product_start;
    while(multiplier<multiplier_end){
        Word multiplier_word=read_word(multiplier),digit=multiplier_word&0xffffu;
        if(digit!=0u){
            Word source=multiplicand,destination=row,carry=0u;
            do{
                const Word value=read_word(source);source+=4u;
                const Word accumulated=read_word(destination);
                const Word low=(value&0xffffu)*digit+(accumulated&0xffffu)+carry;
                const Word high=(value>>16)*digit+(accumulated>>16);
                carry=low>>16;write_half(destination,low);
                const Word combined=high+carry;write_half(destination+2u,combined);destination+=4u;
                carry=combined>>16;
            }while(source<multiplicand_end);
            write_word(destination,carry);
            // Original re-reads the multiplier after the low-half destination writes.
            multiplier_word=read_word(multiplier);
        }
        digit=multiplier_word>>16;
        if(digit!=0u){
            Word source=multiplicand,destination=row,carry=0u;
            Word pending=read_word(destination),previous=pending;
            do{
                const Word value=read_word(source);source+=4u;
                write_half(destination,pending);previous>>=16;
                const Word low=(value&0xffffu)*digit+previous+carry;
                const Word high=(value>>16)*digit;
                write_half(destination+2u,low);destination+=4u;
                previous=read_word(destination);carry=low>>16;
                pending=high+(previous&0xffffu)+carry;carry=pending>>16;
            }while(source<multiplicand_end);
            write_word(destination,pending);
        }
        multiplier+=4u;row+=4u;
    }
    Word used=product_count,tail=product_end;
    while(signed_word(used)>0){tail-=4u;if(read_word(tail)!=0u)break;--used;}
    write_word(product+16u,used);return bytes(product);
}

extern "C" unsigned char* norrath_player_bigint_power5(unsigned char* reent,unsigned char* block,int exponent){
    Word value=address(block),power=Word(exponent);
    const Word remainder=power&3u;
    if(remainder!=0u){
        const Word factor=read_word(address(norrath_player_bigint_small_powers5_view)+(remainder-1u)*4u);
        value=address(norrath_player_bigint_multiply_add(reent,bytes(value),signed_word(factor),0));
    }
    power=arithmetic_right(power,2u);
    if(power==0u)return bytes(value);
    const Word context=address(reent);
    Word cached=read_word(context+72u);
    if(cached==0u){
        cached=address(norrath_player_bigint_from_integer(reent,625));
        write_word(context+72u,cached);write_word(cached,0u);
    }
    for(;;){
        const bool use=(power&1u)!=0u;power=arithmetic_right(power,1u);
        if(use){
            const Word previous=value;
            value=address(norrath_player_bigint_multiply(reent,bytes(previous),bytes(cached)));
            norrath_player_bigint_release(reent,bytes(previous));
        }
        if(power==0u)return bytes(value);
        const Word next=read_word(cached);
        if(next!=0u)cached=next;
        else{
            const Word next_power=address(norrath_player_bigint_multiply(reent,bytes(cached),bytes(cached)));
            write_word(cached,next_power);write_word(next_power,0u);cached=next_power;
        }
    }
}
