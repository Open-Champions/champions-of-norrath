#include "dtoa.hpp"
using namespace norrath_dtoa_detail;

extern "C" unsigned char* norrath_factory_dtoa_reentrant(unsigned char* reent,Wide incoming,int mode,int precision,unsigned char* decimal_position_word,unsigned char* sign_word,unsigned char* end_pointer_word) {
    const Word context=address(reent);
    Wide magnitude=incoming;
    const Word cached=read_word(context+64u);
    if(cached!=0u) {
        write_word(cached+4u,read_word(context+68u));
        write_word(cached+8u,1u<<(read_word(context+68u)&31u));
        release(context,cached);write_word(context+64u,0u);
    }
    if(high(magnitude)&0x80000000u) {
        write_word(address(sign_word),1u);magnitude&=0x7fffffffffffffffull;
    } else write_word(address(sign_word),0u);
    if((high(magnitude)&0x7ff00000u)==0x7ff00000u) {
        write_word(address(decimal_position_word),9999u);
        const unsigned char* text=(magnitude&0x000fffffffffffffull)==0u?norrath_dtoa_infinity_text_view:norrath_dtoa_nan_text_view;
        if(end_pointer_word!=nullptr) {
            const Word start=address(text);
            write_word(address(end_pointer_word),text[3]==0u?start+3u:start+8u);
        }
        return readonly_text(text);
    }
    if(compare_double(magnitude,0ull)==0) {
        write_word(address(decimal_position_word),1u);
        if(end_pointer_word!=nullptr)write_word(address(end_pointer_word),address(norrath_dtoa_zero_text_view)+1u);
        return readonly_text(norrath_dtoa_zero_text_view);
    }
    unsigned char binary_workspace[8];
    const Word scratch=address(binary_workspace);
    Word block=address(norrath_player_bigint_from_double(reent,magnitude,binary_workspace,binary_workspace+4u));
    Word exponent=(high(magnitude)>>20)&0x7ffu;
    Word binary_bits,binary_exponent,denormal;
    Wide normalized;
    if(exponent!=0u) {
        normalized=replace_high(magnitude,(high(magnitude)&0xfffffu)|0x3ff00000u);
        binary_exponent=exponent-1023u;denormal=0u;binary_bits=read_word(scratch+4u);
    } else {
        const Word low_exponent=read_word(scratch);
        binary_bits=read_word(scratch+4u);
        const Word span=binary_bits+low_exponent+1074u;
        Word leading;
        if(signed_word(span)>32)leading=(high(magnitude)<<((0u-span)&31u))|(Word(magnitude)>>((span-32u)&31u));
        else leading=Word(magnitude)<<((0u-span)&31u);
        normalized=integer(leading);
        if(signed_word(leading)<0)normalized=add_double(normalized,0x41f0000000000000ull);
        normalized=replace_high(normalized,high(normalized)+0xfe100000u);
        denormal=1u;binary_exponent=span-1075u;
    }
    Wide estimate=subtract_double(normalized,0x3ff8000000000000ull);
    estimate=multiply_double(estimate,table(norrath_dtoa_log_estimate_coefficients_view,0u));
    estimate=add_double(estimate,table(norrath_dtoa_log_estimate_coefficients_view,1u));
    const Wide exponent_term=multiply_double(integer(binary_exponent),table(norrath_dtoa_log_estimate_coefficients_view,2u));
    estimate=add_double(estimate,exponent_term);
    Word decimal_exponent=integer_part(estimate);
    if(compare_double(estimate,0ull)<0) {
        if(compare_double(estimate,integer(decimal_exponent))!=0)--decimal_exponent;
    }
    Word check_exponent=1u;
    if(decimal_exponent<23u) {
        check_exponent=0u;
        if(compare_double(magnitude,power_ten(decimal_exponent))<0)--decimal_exponent;
    }
    const Word binary_difference=binary_bits-binary_exponent-1u;
    Word numerator_twos,denominator_twos;
    if(signed_word(binary_difference)<0){numerator_twos=0u-binary_difference;denominator_twos=0u;}
    else {numerator_twos=0u;denominator_twos=binary_difference;}
    Word numerator_fives,denominator_fives;
    if(signed_word(decimal_exponent)<0){numerator_fives=0u-decimal_exponent;denominator_fives=0u;numerator_twos-=decimal_exponent;}
    else {numerator_fives=0u;denominator_fives=decimal_exponent;denominator_twos+=decimal_exponent;}
    Word chosen_mode=Word(mode),digits=Word(precision),quick_enabled=1u;
    if(chosen_mode>=10u)chosen_mode=0u;
    if(signed_word(chosen_mode)>=6){chosen_mode-=4u;quick_enabled=0u;}
    Word left_right=1u,limit=0xffffffffu,shorter_limit=0xffffffffu,allocation_digits;
    switch(chosen_mode) {
    case 0u:case 1u:allocation_digits=18u;digits=0u;break;
    case 2u:left_right=0u;[[fallthrough]];
    case 4u:
        if(signed_word(digits)<=0)digits=1u;
        allocation_digits=limit=shorter_limit=digits;break;
    case 3u:left_right=0u;[[fallthrough]];
    case 5u:
        shorter_limit=digits+decimal_exponent;limit=shorter_limit+1u;
        allocation_digits=signed_word(limit)>0?limit:1u;break;
    default:allocation_digits=18u;digits=0u;break;
    }
    write_word(context+68u,0u);
    if(allocation_digits>=24u) {
        Word capacity=4u,size_class=0u;
        do {capacity<<=1;++size_class;}while(!(allocation_digits<capacity+20u));
        write_word(context+68u,size_class);
    }
    const Word result=allocate(context,read_word(context+68u));
    write_word(context+64u,result);
    Word cursor=result,iteration=0u,denominator=0u,lower_margin=0u,upper_margin=0u;
    Wide value=magnitude,error,divisor;
    Word original_exponent=decimal_exponent,original_limit=limit;
    Word margin_twos=0u,margin_fives=0u,special_case=0u,round_digit=0u;
    int comparison=0,other_comparison=0;
    if(limit<15u&&quick_enabled!=0u) {
        Word operations=2u;
        if(signed_word(decimal_exponent)>0) {
            Word powers=arithmetic_right(decimal_exponent,4u);
            divisor=power_ten(decimal_exponent&15u);
            if(powers&16u){value=norrath_player_double_divide(magnitude,large_power_ten(4u));powers&=15u;operations=3u;}
            Word index=0u;
            while(powers!=0u) {
                if(powers&1u){divisor=multiply_double(divisor,large_power_ten(index));++operations;}
                powers=arithmetic_right(powers,1u);++index;
            }
            value=norrath_player_double_divide(value,divisor);
        } else {
            const Word negative=0u-decimal_exponent;
            if(negative!=0u) {
                value=multiply_double(power_ten(negative&15u),magnitude);
                Word powers=arithmetic_right(negative,4u),index=0u;
                while(powers!=0u) {
                    if(powers&1u){value=multiply_double(large_power_ten(index),value);++operations;}
                    powers=arithmetic_right(powers,1u);++index;
                }
            }
        }
        if(check_exponent!=0u&&compare_double(value,0x3ff0000000000000ull)<0&&signed_word(limit)>0) {
            if(signed_word(shorter_limit)<=0)goto quick_failure;
            limit=shorter_limit;--decimal_exponent;value=multiply_double(0x4024000000000000ull,value);++operations;
        }
        error=multiply_double(integer(operations),value);
        error=add_double(error,0x401c000000000000ull);
        error=replace_high(error,high(error)+0xfcc00000u);
        if(limit==0u) {
            lower_margin=upper_margin=denominator=0u;
            value=subtract_double(value,0x4014000000000000ull);
            if(compare_double(value,error)>0)goto one_digit;
            if(compare_double(value,subtract_double(0ull,error))<0)goto no_digits;
            goto quick_failure;
        }
        if(left_right!=0u) {
            error=subtract_double(norrath_player_double_divide(0x3fe0000000000000ull,power_ten(limit-1u)),error);
            iteration=0u;
            do {
                round_digit=integer_part(value);
                value=subtract_double(value,integer(round_digit));
                bytes(cursor)[0]=static_cast<unsigned char>(round_digit+48u);++cursor;
                if(compare_double(value,error)<0)goto finish_binary_only;
                if(compare_double(subtract_double(0x3ff0000000000000ull,value),error)<0)goto round_binary;
                ++iteration;
                if(! (signed_word(iteration)<signed_word(limit)))goto quick_failure;
                error=multiply_double(0x4024000000000000ull,error);
                value=multiply_double(0x4024000000000000ull,value);
            } while(true);
        } else {
            error=multiply_double(power_ten(limit-1u),error);iteration=1u;
            do {
                round_digit=integer_part(value);
                value=subtract_double(value,integer(round_digit));
                bytes(cursor)[0]=static_cast<unsigned char>(round_digit+48u);++cursor;
                if(iteration==limit) {
                    if(compare_double(value,add_double(0x3fe0000000000000ull,error))>0)goto round_binary;
                    if(compare_double(value,subtract_double(0x3fe0000000000000ull,error))<0) {
                        do {--cursor;}while(signed_byte(bytes(cursor)[0])==48);
                        ++cursor;goto finish_binary_only;
                    }
                    goto quick_failure;
                }
                value=multiply_double(0x4024000000000000ull,value);++iteration;
            }while(true);
        }
    }
quick_failure:
    value=magnitude;decimal_exponent=original_exponent;limit=original_limit;cursor=result;
    if(signed_word(read_word(scratch))>=0&&signed_word(decimal_exponent)<15) {
        divisor=power_ten(decimal_exponent);iteration=1u;
        if(signed_word(digits)<0&&signed_word(limit)<=0) {
            lower_margin=upper_margin=denominator=0u;
            if(signed_word(limit)<0)goto no_digits;
            if(compare_double(value,multiply_double(divisor,0x4014000000000000ull))<=0)goto no_digits;
            goto one_digit;
        }
        do {
            round_digit=integer_part(norrath_player_double_divide(value,divisor));
            value=subtract_double(value,multiply_double(integer(round_digit),divisor));
            bytes(cursor)[0]=static_cast<unsigned char>(round_digit+48u);++cursor;
            if(iteration==limit) {
                value=add_double(value,value);
                comparison=compare_double(value,divisor);
                if(comparison>0)goto round_binary;
                comparison=compare_double(value,divisor);
                if(comparison==0&&(round_digit&1u))goto round_binary;
                goto finish_binary_only;
            }
            value=multiply_double(0x4024000000000000ull,value);
            if(compare_double(value,0ull)==0)goto finish_binary_only;
            ++iteration;
        }while(true);
    }
    // The remaining genuine bigint generation/rounding body is authored below.
    margin_twos=numerator_twos;margin_fives=numerator_fives;
    if(left_right!=0u) {
        Word extra_twos;
        if(signed_word(chosen_mode)<2)extra_twos=denormal!=0u?read_word(scratch)+1075u:54u-read_word(scratch+4u);
        else {
            const Word reduction=limit-1u;
            margin_fives=numerator_fives-reduction;
            if(signed_word(numerator_fives)<signed_word(reduction)) {
                margin_fives=0u;
                const Word extra=reduction-numerator_fives;
                numerator_fives+=extra;denominator_fives+=extra;
            }
            extra_twos=limit;
            if(signed_word(extra_twos)<0){margin_twos=numerator_twos-extra_twos;extra_twos=0u;}
        }
        denominator_twos+=extra_twos;
        upper_margin=address(norrath_player_bigint_from_integer(reent,1));
        numerator_twos+=extra_twos;
    }
    if(signed_word(margin_twos)>0&&signed_word(denominator_twos)>0) {
        const Word cancelled=signed_word(margin_twos)<signed_word(denominator_twos)?margin_twos:denominator_twos;
        numerator_twos-=cancelled;denominator_twos-=cancelled;margin_twos-=cancelled;
    }
    if(signed_word(numerator_fives)>0) {
        if(left_right!=0u) {
            if(signed_word(margin_fives)>0) {
                upper_margin=power_five(context,upper_margin,margin_fives);
                const Word product=address(norrath_player_bigint_multiply(reent,object(upper_margin),object(block)));
                release(context,block);block=product;
            }
            const Word remaining=numerator_fives-margin_fives;
            if(remaining!=0u)block=power_five(context,block,remaining);
        } else block=power_five(context,block,numerator_fives);
    }
    denominator=address(norrath_player_bigint_from_integer(reent,1));
    if(signed_word(denominator_fives)>0)denominator=power_five(context,denominator,denominator_fives);
    if(signed_word(chosen_mode)<2&&(magnitude&0x000fffffffffffffull)==0u&&(high(magnitude)&0x7ff00000u)!=0u) {
        special_case=1u;++numerator_twos;++denominator_twos;
    }
    Word normalization;
    if(denominator_fives!=0u) {
        const Word count=read_word(denominator+16u);
        normalization=denominator_twos-Word(norrath_player_bigint_leading_zeros(read_word(denominator+count*4u+16u)));
    } else normalization=denominator_twos+1u;
    normalization&=31u;
    if(normalization!=0u)normalization=32u-normalization;
    if(signed_word(normalization)>=5) {
        normalization-=4u;numerator_twos+=normalization;denominator_twos+=normalization;margin_twos+=normalization;
    } else if(signed_word(normalization)<4) {
        normalization+=28u;numerator_twos+=normalization;denominator_twos+=normalization;margin_twos+=normalization;
    }
    if(signed_word(numerator_twos)>0)block=shift_block(context,block,numerator_twos);
    if(signed_word(denominator_twos)>0)denominator=shift_block(context,denominator,denominator_twos);
    if(check_exponent!=0u&&compare_blocks(block,denominator)<0) {
        block=multiply_add(context,block,10u,0u);--decimal_exponent;
        if(left_right!=0u)upper_margin=multiply_add(context,upper_margin,10u,0u);
        limit=shorter_limit;
    }
    if(signed_word(limit)<=0&&signed_word(chosen_mode)>=3) {
        if(signed_word(limit)<0)goto no_digits;
        denominator=multiply_add(context,denominator,5u,0u);
        if(compare_blocks(block,denominator)>0)goto one_digit;
        goto no_digits;
    }
    if(left_right!=0u) {
        if(signed_word(margin_twos)>0)upper_margin=shift_block(context,upper_margin,margin_twos);
        lower_margin=upper_margin;
        if(special_case!=0u) {
            const Word duplicate=allocate(context,read_word(upper_margin+4u));
            const Word copy_count=read_word(lower_margin+16u)*4u+8u;
            norrath_copy_bytes(bytes(duplicate+12u),bytes(lower_margin+12u),copy_count);
            upper_margin=shift_block(context,duplicate,1u);
        }
        const Word parity=Word(magnitude)&1u;
        iteration=1u;
        do {
            round_digit=Word(norrath_player_bigint_quotient(object(block),object(denominator)))+48u;
            comparison=compare_blocks(block,lower_margin);
            const Word difference=address(norrath_player_bigint_difference(reent,object(denominator),object(upper_margin)));
            other_comparison=1;
            if(read_word(difference+12u)==0u)other_comparison=compare_blocks(block,difference);
            release(context,difference);
            if(other_comparison==0&&chosen_mode==0u&&parity==0u) {
                if(round_digit==57u)goto emit_nine_and_round;
                if(other_comparison<comparison)++round_digit;
                bytes(cursor)[0]=static_cast<unsigned char>(round_digit);++cursor;goto finish_all;
            }
            if(comparison<0||(comparison==0&&chosen_mode==0u&&parity==0u)) {
                if(other_comparison>0) {
                    block=shift_block(context,block,1u);other_comparison=compare_blocks(block,denominator);
                    if(other_comparison>0||(other_comparison==0&&(round_digit&1u)))++round_digit;
                    if(round_digit==58u)goto emit_nine_and_round;
                }
                bytes(cursor)[0]=static_cast<unsigned char>(round_digit);++cursor;goto finish_all;
            }
            if(other_comparison>0) {
                if(round_digit==57u)goto emit_nine_and_round;
                ++round_digit;bytes(cursor)[0]=static_cast<unsigned char>(round_digit);++cursor;goto finish_all;
            }
            bytes(cursor)[0]=static_cast<unsigned char>(round_digit);++cursor;
            if(iteration==limit)break;
            block=multiply_add(context,block,10u,0u);
            if(lower_margin==upper_margin) {
                upper_margin=multiply_add(context,upper_margin,10u,0u);lower_margin=upper_margin;
            } else {
                const Word lower=multiply_add(context,lower_margin,10u,0u);
                upper_margin=multiply_add(context,upper_margin,10u,0u);lower_margin=lower;
            }
            ++iteration;
        }while(true);
    } else {
        iteration=1u;
        do {
            round_digit=Word(norrath_player_bigint_quotient(object(block),object(denominator)))+48u;
            bytes(cursor)[0]=static_cast<unsigned char>(round_digit);++cursor;
            if(!(signed_word(iteration)<signed_word(limit)))break;
            block=multiply_add(context,block,10u,0u);++iteration;
        }while(true);
    }
    block=shift_block(context,block,1u);comparison=compare_blocks(block,denominator);
    if(comparison>0||(comparison==0&&(round_digit&1u)))goto round_bigint;
    do {--cursor;}while(signed_byte(bytes(cursor)[0])==48);
    ++cursor;goto finish_all;
emit_nine_and_round:
    bytes(cursor)[0]=57u;++cursor;
round_bigint:
    --cursor;
    while(signed_byte(bytes(cursor)[0])==57) {
        if(cursor==result) {
            bytes(result)[0]=49u;++decimal_exponent;cursor=result+1u;goto finish_all;
        }
        --cursor;
    }
    bytes(cursor)[0]=static_cast<unsigned char>(bytes(cursor)[0]+1u);++cursor;goto finish_all;
round_binary:
    --cursor;
    while(signed_byte(bytes(cursor)[0])==57) {
        if(cursor==result) {
            bytes(result)[0]=48u;++decimal_exponent;break;
        }
        --cursor;
    }
    bytes(cursor)[0]=static_cast<unsigned char>(bytes(cursor)[0]+1u);++cursor;goto finish_binary_only;
no_digits:
    decimal_exponent=~digits;goto finish_all;
one_digit:
    bytes(result)[0]=49u;++decimal_exponent;cursor=result+1u;
finish_all:
    release(context,denominator);
    if(upper_margin!=0u) {
        if(lower_margin!=0u&&lower_margin!=upper_margin)release(context,lower_margin);
        release(context,upper_margin);
    }
finish_binary_only:
    release(context,block);
    bytes(cursor)[0]=0u;write_word(address(decimal_position_word),decimal_exponent+1u);
    if(end_pointer_word!=nullptr)write_word(address(end_pointer_word),cursor);
    return object(result);
}
