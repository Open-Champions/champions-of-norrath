#include "number_parser.hpp"
extern "C" NorrathFormatUnsigned64 norrath_player_strtod_reentrant(unsigned char* reent,const char* text,char** end){
    using namespace norrath_player_number_detail;
    const Word original=address(text);
    Word cursor=original,first_digit=0u,integer_digits=0u,digits=0u;
    Word first_nine=0u,fractional_places=0u,pending_zeros=0u;
    Wide remaining_digits=0u,exponent=0u,value=0u;
    bool negative=false,saw_zero=false;
    unsigned char *decimal_value=nullptr,*binary_value=nullptr,*decimal_copy=nullptr,*half_unit=nullptr,*difference=nullptr;
    alignas(4) unsigned char decomposition[8]; // Genuine d2b writes exponent/bits.
    int c;
    // Complete actual46-entry dispatch: NUL, six whitespace values and signs.
    for(;;){
        c=character(cursor);
        if(c==0){cursor=original;goto finish;}
        if(c==9||c==10||c==11||c==12||c==13||c==32){cursor+=1u;continue;}
        if(c==45||c==43){negative=c==45;cursor+=1u;c=character(cursor);if(c==0){cursor=original;goto finish;}}
        break;
    }
    if(c==48){
        saw_zero=true;
        do{cursor+=1u;c=character(cursor);}while(c==48);
        if(c==0)goto finish;
    }
    first_digit=cursor;
    while(decimal(c)){
        if(signed_word(digits)<9)first_nine=first_nine*10u+unsigned(c-48);
        else if(signed_word(digits)<16)remaining_digits=remaining_digits*10u+Wide(c-48);
        digits+=1u;cursor+=1u;c=character(cursor);
    }
    integer_digits=digits;
    if(c==46){
        cursor+=1u;c=character(cursor);
        if(digits==0u){
            while(c==48){cursor+=1u;c=character(cursor);pending_zeros+=1u;}
            if(unsigned(c-49)>=9u)goto exponent_marker;
            fractional_places=pending_zeros;pending_zeros=0u;first_digit=cursor;
        }
        while(decimal(c)){
            const unsigned digit=unsigned(c-48);pending_zeros+=1u;
            if(digit!=0u){
                fractional_places+=pending_zeros;
                if(signed_word(pending_zeros)>1){
                    Word zero_count=pending_zeros-1u;
                    do{
                        const Word old=digits;digits+=1u;
                        if(signed_word(old)<9)first_nine*=10u;
                        else if(signed_word(digits)<17)remaining_digits*=10u;
                        zero_count-=1u;
                    }while(zero_count!=0u);
                }
                const Word old=digits;digits+=1u;
                if(signed_word(old)<9)first_nine=first_nine*10u+digit;
                else if(signed_word(digits)<17)remaining_digits=remaining_digits*10u+digit;
                pending_zeros=0u;
            }
            cursor+=1u;c=character(cursor);
        }
    }
exponent_marker:
    if(c==101||c==69){
        if(digits==0u&&pending_zeros==0u&&!saw_zero){cursor=original;goto finish;}
        const Word marker=cursor;cursor+=1u;c=character(cursor);
        bool exponent_negative=false;
        if(c==43||c==45){exponent_negative=c==45;cursor+=1u;c=character(cursor);}
        if(!decimal(c)){cursor=marker;}
        else{
            while(c==48){cursor+=1u;c=character(cursor);}
            if(unsigned(c-49)<9u){
                const Word exponent_start=cursor;
                exponent=Wide(c-48);cursor+=1u;c=character(cursor);
                while(c>=48&&c<58){exponent=exponent*10u+Wide(c-48);cursor+=1u;c=character(cursor);}
                if(signed_word(cursor-exponent_start)>=9)exponent=9999999u;
                if(exponent_negative)exponent=0u-exponent;
            }
        }
    }
    if(digits==0u){if(pending_zeros==0u&&!saw_zero)cursor=original;goto finish;}
    {
        const Word kept=signed_word(digits)<17?digits:16u;
        if(integer_digits==0u)integer_digits=digits;
        exponent-=extend_signed_word(fractional_places);
        Word decimal_exponent=Word(exponent)+digits-kept;
        value=norrath_player_signed32_to_double(signed_word(first_nine));
        if(signed_word(first_nine)<0)value=add_double(value,0x41f0000000000000ull);
        if(signed_word(kept)>=10){
            value=multiply_double(table(norrath_player_number_tens_view,kept-9u),value);
            Wide tail;
            if((remaining_digits>>63)==0u)tail=norrath_player_signed64_to_double(signed_wide(remaining_digits));
            else{tail=norrath_player_signed64_to_double(signed_wide((remaining_digits>>1)|(remaining_digits&1u)));tail=add_double(tail,tail);}
            value=add_double(value,tail);
        }
        if(signed_word(decimal_exponent)>0){
            const Word small=decimal_exponent&15u;
            if(small!=0u)value=multiply_double(table(norrath_player_number_tens_view,small),value);
            decimal_exponent&=0xfffffff0u;
            if(decimal_exponent!=0u){
                if(signed_word(decimal_exponent)>=309)goto overflow;
                Word power=decimal_exponent>>4,index=0u;
                while(signed_word(power)>=2){const bool use=(power&1u)!=0u;power=unsigned(signed_word(power)>>1);if(use)value=multiply_double(table(norrath_player_number_big_tens_view,index),value);index+=1u;}
                value=replace_high(value,high(value)-0x03500000u);
                value=multiply_double(table(norrath_player_number_big_tens_view,index),value);
                const Word adjusted=high(value)&0x7ff00000u;
                if(adjusted>0x7ca00000u)goto overflow;
                if(adjusted>0x7c900000u)value=0x7fefffffffffffffull;
                else value=replace_high(value,high(value)+0x03500000u);
            }
        }else if(signed_word(decimal_exponent)<0){
            decimal_exponent=0u-decimal_exponent;
            const Word small=decimal_exponent&15u;
            if(small!=0u)value=norrath_player_double_divide(value,table(norrath_player_number_tens_view,small));
            // Original SRA4 sign-extends the masked low32 word, including its high bit.
            decimal_exponent=(decimal_exponent>>4) | ((decimal_exponent&0x80000000u)?0xf0000000u:0u);
            if(decimal_exponent!=0u){
                if(signed_word(decimal_exponent)>=32)goto underflow;
                Word index=0u;
                while(signed_word(decimal_exponent)>=2){const bool use=(decimal_exponent&1u)!=0u;decimal_exponent=unsigned(signed_word(decimal_exponent)>>1);if(use)value=multiply_double(table(norrath_player_number_tiny_tens_view,index),value);index+=1u;}
                const Wide last=table(norrath_player_number_tiny_tens_view,index),previous=value;
                value=multiply_double(last,value);
                if(compare_double(value,0u)==0){const Wide twice=add_double(previous,previous);const Wide retry=multiply_double(last,twice);value=1u;if(compare_double(retry,0u)==0)goto underflow;}
            }
        }
    }
    decimal_value=norrath_player_bigint_from_text(reent,string(first_digit),signed_word(integer_digits),signed_word(digits),first_nine);
    for(;;){
        decimal_copy=norrath_player_bigint_allocate(reent,signed_word(read_word(address(decimal_value)+4u)));
        (void)norrath_copy_bytes(object(address(decimal_copy)+12u),object(address(decimal_value)+12u),(read_word(address(decimal_value)+16u)<<2)+8u);
        binary_value=norrath_player_bigint_from_double(reent,value,decomposition,decomposition+4);
        half_unit=norrath_player_bigint_from_integer(reent,1);
        Word decimal_shift=0u,binary_shift=0u,decimal_fives=0u,binary_fives=0u;
        if((exponent>>63)==0u)decimal_shift=decimal_fives=Word(exponent);
        else binary_shift=binary_fives=0u-Word(exponent);
        const Word binary_exponent=read_word(address(decomposition)),binary_bits=read_word(address(decomposition)+4u);
        if(signed_word(binary_exponent)<0)decimal_shift-=binary_exponent;
        else binary_shift+=binary_exponent;
        Word extra=binary_exponent+1075u;
        if(signed_word(binary_exponent+binary_bits-1u)>=-1022)extra=54u-binary_bits;
        Word unit_shift=binary_shift;
        decimal_shift+=extra;binary_shift+=extra;
        Word common=signed_word(binary_shift)<signed_word(decimal_shift)?binary_shift:decimal_shift;
        if(signed_word(unit_shift)<signed_word(common))common=unit_shift;
        if(signed_word(common)>0){unit_shift-=common;binary_shift-=common;decimal_shift-=common;}
        if(signed_word(binary_fives)>0){
            half_unit=norrath_player_bigint_power5(reent,half_unit,signed_word(binary_fives));
            unsigned char* product=norrath_player_bigint_multiply(reent,half_unit,binary_value);
            norrath_player_bigint_release(reent,binary_value);binary_value=product;
        }
        if(signed_word(binary_shift)>0)binary_value=norrath_player_bigint_shift(reent,binary_value,signed_word(binary_shift));
        if(signed_word(decimal_fives)>0)decimal_copy=norrath_player_bigint_power5(reent,decimal_copy,signed_word(decimal_fives));
        if(signed_word(decimal_shift)>0)decimal_copy=norrath_player_bigint_shift(reent,decimal_copy,signed_word(decimal_shift));
        if(signed_word(unit_shift)>0)half_unit=norrath_player_bigint_shift(reent,half_unit,signed_word(unit_shift));
        difference=norrath_player_bigint_difference(reent,binary_value,decimal_copy);
        const Word below=read_word(address(difference)+12u);
        write_word(address(difference)+12u,0u);
        int comparison=norrath_player_bigint_compare(difference,half_unit);
        if(comparison<0){
            if(below!=0u||(value&0x000fffffffffffffull)!=0u)break;
            difference=norrath_player_bigint_shift(reent,difference,1);
            if(norrath_player_bigint_compare(difference,half_unit)<=0)break;
            value=replace_high(value,((high(value)&0x7ff00000u)-0x00100000u)|0x000fffffu)|0xffffffffull;break;
        }
        if(comparison==0){
            if(below!=0u){
                if((value&0x000fffffffffffffull)==0x000fffffffffffffull){value=Wide((high(value)&0x7ff00000u)+0x00100000u)<<32;break;}
            }else if((value&0x000fffffffffffffull)==0u){value=replace_high(value,((high(value)&0x7ff00000u)-0x00100000u)|0x000fffffu)|0xffffffffull;break;}
            if((Word(value)&1u)==0u)break;
            const Wide unit=norrath_player_double_ulp(value);
            if(below!=0u)value=add_double(unit,value);
            else{value=subtract_double(value,unit);if(compare_double(value,0u)==0)goto underflow;}
            break;
        }
        Wide adjustment=norrath_player_bigint_ratio(difference,half_unit),correction;
        if(compare_double(adjustment,0x4000000000000000ull)<=0){
            if(below!=0u){adjustment=correction=0x3ff0000000000000ull;}
            else if((value&0x000fffffffffffffull)!=0u){
                if(value==1u)goto underflow;
                adjustment=0x3ff0000000000000ull;correction=0xbff0000000000000ull;
            }else{
                if(compare_double(adjustment,0x3ff0000000000000ull)<0)adjustment=0x3fe0000000000000ull;
                else adjustment=multiply_double(adjustment,0x3fe0000000000000ull);
                correction=subtract_double(0u,adjustment);
            }
        }else{
            adjustment=multiply_double(adjustment,0x3fe0000000000000ull);
            correction=below!=0u?adjustment:subtract_double(0u,adjustment);
            correction=add_double(correction,0x3fe0000000000000ull);
        }
        const Word old_exponent=high(value)&0x7ff00000u;
        bool continue_correction=false;
        if(old_exponent==0x7fe00000u){
            const Wide previous=value;
            value=replace_high(value,high(value)-0x03500000u);
            const Wide unit=norrath_player_double_ulp(value);
            const Wide change=multiply_double(correction,unit);
            value=add_double(change,value);
            if((high(value)&0x7ff00000u)>0x7c9fffffu){
                if(previous==0x7fefffffffffffffull)goto overflow;
                value=0x7fefffffffffffffull;continue_correction=true;
            }else value=replace_high(value,high(value)+0x03500000u);
        }else{
            if(old_exponent<=0x03400000u&&compare_double(adjustment,0x3ff0000000000000ull)>=0){
                const Wide rounded=add_double(adjustment,0x3fe0000000000000ull);
                correction=norrath_player_signed32_to_double(norrath_player_double_to_signed32(rounded));
                if(below==0u)correction=subtract_double(0u,correction);
            }
            const Wide unit=norrath_player_double_ulp(value);
            const Wide change=multiply_double(correction,unit);
            value=add_double(change,value);
        }
        if(!continue_correction&&(high(value)&0x7ff00000u)==old_exponent){
            const NorrathFormatSigned64 whole=norrath_player_double_to_signed64(adjustment);
            const Wide integral=norrath_player_signed64_to_double(whole);
            const Wide fraction=subtract_double(adjustment,integral);
            if(below!=0u||(value&0x000fffffffffffffull)!=0u){
                if(compare_double(fraction,read_wide(address(norrath_player_number_half_lower_view)))<0)break;
                if(compare_double(fraction,read_wide(address(norrath_player_number_half_upper_view)))>0)break;
            }else if(compare_double(fraction,read_wide(address(norrath_player_number_quarter_lower_view)))<0)break;
        }
        norrath_player_bigint_release(reent,binary_value);
        norrath_player_bigint_release(reent,decimal_copy);
        norrath_player_bigint_release(reent,half_unit);
        norrath_player_bigint_release(reent,difference);
    }
    goto release_all;
overflow:
    write_word(address(reent),34u);
    value=read_wide(address(norrath_player_number_overflow_view));
    if(decimal_value!=nullptr)goto release_all;
    goto finish;
underflow:
    value=0u;write_word(address(reent),34u);
    if(decimal_value!=nullptr)goto release_all;
    goto finish;
release_all:
    norrath_player_bigint_release(reent,binary_value);
    norrath_player_bigint_release(reent,decimal_copy);
    norrath_player_bigint_release(reent,half_unit);
    norrath_player_bigint_release(reent,decimal_value);
    norrath_player_bigint_release(reent,difference);
finish:
    if(end!=nullptr)write_word(address(end),cursor);
    return negative?subtract_double(0u,value):value;
}
