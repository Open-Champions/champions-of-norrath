#include "scanner.hpp"
extern "C" int norrath_player_stream_scan(unsigned char* stream,const char* format,const unsigned char* argument_cursor){
    using namespace norrath_scanner_detail;
    const Word file=address(stream);
    Word format_cursor=address(format),arguments=address(argument_cursor),read_count=0u,assignments=0u;
    // Original scanset256 and numeric350 scratch occupy one shared stack region.
    // The two bytes606..607 are untouched; no zero-fill or fabricated payload.
    alignas(16) unsigned char workspace[608];
    const Word character_class=address(workspace),number=character_class+256u;
    int integer_base=0;
    for(;;){
        int conversion=signed_byte(*bytes(format_cursor));++format_cursor;
        if(conversion==0)return signed_word(assignments);
        // The original multibyte test is BNE V0,V0: it never takes that path.
        if(classify(conversion)&8u){
            for(;;){
                if(available(file)<=0 && norrath_player_stream_refill(stream)!=0)return signed_word(assignments);
                Word input=read_word(file);unsigned byte=*bytes(input);
                if(!(classify(int(byte))&8u))break;
                const Word count=read_word(file+4u);
                write_word(file,input+1u);++read_count;write_word(file+4u,count-1u);
            }
            continue;
        }
        if(conversion!='%'){
            const Word literal=format_cursor-1u;
            if(available(file)<=0 && norrath_player_stream_refill(stream)!=0)return assignments?signed_word(assignments):-1;
            const Word input=read_word(file);
            // Original re-reads the signed literal after refill and after FILE cursor load.
            const int expected_literal=signed_byte(*bytes(literal));
            if(int(*bytes(input))!=expected_literal)return signed_word(assignments);
            const Word count=read_word(file+4u);
            write_word(file,input+1u);++read_count;write_word(file+4u,count-1u);
            continue;
        }
        Word flags=0u,width=0u;unsigned kind=3u;bool unsigned_integer=false;
        bool percent_literal=false,count_conversion=false;
        for(;;){
            conversion=signed_byte(*bytes(format_cursor));++format_cursor;
            if(conversion=='*'){flags|=8u;continue;}
            if(conversion=='l'){flags|=1u;continue;}
            if(conversion=='L'){flags|=2u;continue;}
            if(conversion=='h'){flags|=4u;continue;}
            if(conversion>='0'&&conversion<='9'){width=width*10u+Word(conversion-'0');continue;}
            switch(conversion){
            case 0:return -1;
            case '%':percent_literal=true;break;
            case 'D':flags|=1u;[[fallthrough]];
            case 'd':integer_base=10;break;
            case 'i':integer_base=0;break;
            case 'O':flags|=1u;[[fallthrough]];
            case 'o':integer_base=8;unsigned_integer=true;break;
            case 'u':integer_base=10;unsigned_integer=true;break;
            case 'X':case 'x':flags|=0x100u;integer_base=16;unsigned_integer=true;break;
            case 'p':flags|=0x110u;integer_base=16;unsigned_integer=true;break;
            case 'E':case 'G':case 'e':case 'f':case 'g':kind=4u;break;
            case 's':kind=2u;break;
            case '[':format_cursor=address(norrath_player_scan_character_class(bytes(character_class),text(format_cursor)));flags|=0x20u;kind=1u;break;
            case 'c':flags|=0x20u;kind=0u;break;
            case 'n':count_conversion=true;break;
            default:flags|=classify(conversion)&1u;integer_base=10;break;
            }
            break;
        }
        if(count_conversion){
            if(!(flags&8u)){
                const Word output=next_destination(arguments);
                if(flags&4u)write_half(output,read_count);
                else if(flags&1u)write_wide(output,extend_signed_word(read_count));
                else write_word(output,read_count);
            }
            continue;
        }
        if(percent_literal){
            const Word literal=format_cursor-1u;
            if(available(file)<=0 && norrath_player_stream_refill(stream)!=0)return assignments?signed_word(assignments):-1;
            const Word input=read_word(file);
            // %% uses the same fresh signed-literal load as an ordinary literal.
            const int expected_literal=signed_byte(*bytes(literal));
            if(int(*bytes(input))!=expected_literal)return signed_word(assignments);
            const Word count=read_word(file+4u);write_word(file,input+1u);++read_count;write_word(file+4u,count-1u);continue;
        }
        if(available(file)<=0 && norrath_player_stream_refill(stream)!=0)return assignments?signed_word(assignments):-1;
        if(!(flags&0x20u)){
            while(classify(int(*bytes(read_word(file))))&8u){
                ++read_count;
                if(!consume_to_refill(stream,file))return assignments?signed_word(assignments):-1;
            }
        }
        if(kind==0u){
            if(width==0u)width=1u;
            if(flags&8u){
                Word consumed=0u;
                for(;;){
                    const Word count=read_word(file+4u);
                    if(count>=width){
                        const Word input=read_word(file);write_word(file+4u,count-width);
                        consumed+=width;write_word(file,input+width);read_count+=consumed;break;
                    }
                    const Word input=read_word(file);consumed+=count;width-=count;
                    write_word(file,input+count);
                    if(norrath_player_stream_refill(stream)!=0){
                        if(consumed==0u)return assignments?signed_word(assignments):-1;
                        read_count+=consumed;break;
                    }
                }
            }else{
                const Word output=next_destination(arguments);
                Word consumed=norrath_player_stream_read_elements(bytes(output),1u,width,stream);
                if(consumed==0u)return assignments?signed_word(assignments):-1;
                read_count+=consumed;++assignments;
            }
            continue;
        }
        if(kind==1u||kind==2u){
            if(width==0u)width=0xffffffffu;
            Word consumed=0u,output=0u,start=0u;
            if(!(flags&8u)){output=next_destination(arguments);start=output;}
            for(;;){
                const Word input=read_word(file);const unsigned byte=*bytes(input);
                bool accepts=kind==1u?signed_byte(*bytes(character_class+byte))!=0:!(classify(int(byte))&8u);
                if(!accepts)break;
                if(flags&8u){
                    const Word count=read_word(file+4u)-1u;--width;
                    write_word(file,input+1u);++consumed;write_word(file+4u,count);
                    if(width==0u)break;
                    if(signed_word(count)>0)continue;
                }else{
                    const Word count=read_word(file+4u)-1u;--width;write_word(file+4u,count);
                    // Fresh byte after the count store retains real source/destination alias order.
                    *bytes(output)=*bytes(input);++output;write_word(file,input+1u);
                    if(width==0u)break;
                    if(available(file)>0)continue;
                }
                if(norrath_player_stream_refill(stream)==0)continue;
                if(kind==1u && ((flags&8u)?consumed==0u:output==start))return assignments?signed_word(assignments):-1;
                break;
            }
            if(!(flags&8u))consumed=output-start;
            if(kind==1u && consumed==0u)return signed_word(assignments);
            if(!(flags&8u)){*bytes(output)=0u;++assignments;}
            read_count+=consumed;continue;
        }
        // Exact original numeric width saturation, including width0 and wrapped width.
        width=width-1u<349u?width:349u;
        Word end=number;
        if(kind==3u){
            flags|=0x2c0u;
            while(width!=0u){
                const unsigned byte=*bytes(read_word(file));bool accepted=true;
                if(byte=='0'){
                    if(integer_base==0){integer_base=8;flags|=0x100u;}
                    flags&=(flags&0x200u)?~0x2c0u:~0x1c0u;
                }else if(byte>='1'&&byte<='7'){
                    integer_base=signed_half(address(norrath_player_scan_base_normalization_view)+Word(integer_base)*2u);flags&=~0x1c0u;
                }else if(byte=='8'||byte=='9'){
                    integer_base=signed_half(address(norrath_player_scan_base_normalization_view)+Word(integer_base)*2u);
                    if(integer_base<9)accepted=false;else flags&=~0x1c0u;
                }else if((byte>='A'&&byte<='F')||(byte>='a'&&byte<='f')){
                    if(integer_base<11)accepted=false;else flags&=~0x1c0u;
                }else if(byte=='+'||byte=='-'){
                    if(!(flags&0x40u))accepted=false;else flags&=~0x40u;
                }else if(byte=='x'||byte=='X'){
                    if(!(flags&0x100u)||end!=number+1u)accepted=false;
                    else{integer_base=16;flags&=~0x100u;}
                }else accepted=false;
                if(!accepted)break;
                *bytes(end)=static_cast<unsigned char>(byte);++end;
                if(!consume_to_refill(stream,file))break;
                --width;
            }
            if(flags&0x80u){
                if(end>number)(void)norrath_player_stream_unget(signed_byte(*bytes(end-1u)),stream);
                return signed_word(assignments);
            }
            int last=signed_byte(*bytes(end-1u));
            if(last=='x'||last=='X'){(void)norrath_player_stream_unget(last,stream);--end;}
            if(!(flags&8u)){
                *bytes(end)=0u;
                Wide value=unsigned_integer?norrath_player_strtoul(text(number),nullptr,integer_base)
                    :Wide(norrath_factory_strtol(text(number),nullptr,integer_base));
                const Word output=next_destination(arguments);
                if(flags&0x10u)write_word(output,Word(value));
                else if(flags&4u)write_half(output,Word(value));
                else if(flags&1u)write_wide(output,value);
                else write_word(output,Word(value));
                ++assignments;
            }
            read_count+=end-number;continue;
        }
        // Whole floating conversion scan. Leading zeroes are counted with actual
        // 64-bit DADDIU and intentionally do not consume field width or scratch.
        flags|=0x3c0u;
        Wide zeroes=0u,adjustment=0u;
        Wide decimal_zeroes; // Original stack616 only written after a decimal point.
        Word exponent_marker; // Original stack624 only read on a genuine correction path.
        while(width!=0u){
            const unsigned byte=*bytes(read_word(file));bool accepted=true,append=true;
            if(byte=='0'&&(flags&0x80u)){++zeroes;flags&=~0x40u;append=false;}
            else if(byte>='0'&&byte<='9')flags&=~0xc0u;
            else if(byte=='+'||byte=='-'){
                if(!(flags&0x40u))accepted=false;else flags&=~0x40u;
            }else if(byte=='.'){
                if(!(flags&0x100u))accepted=false;
                else{decimal_zeroes=zeroes;flags&=~0x140u;}
            }else if(byte=='e'||byte=='E'){
                if((flags&0x280u)!=0x200u && (!(flags&0x200u)||zeroes==0u))accepted=false;
                else{
                    if(!(flags&0x100u)){exponent_marker=end;adjustment=zeroes-decimal_zeroes;}
                    zeroes=0u;flags=(flags&~0x300u)|0xc0u;
                }
            }else accepted=false;
            if(!accepted)break;
            if(append){*bytes(end)=static_cast<unsigned char>(byte);++end;--width;}
            ++read_count;
            if(!consume_to_refill(stream,file))break;
        }
        if(zeroes!=0u)flags&=~0x80u;
        if(flags&0x80u){
            if(flags&0x200u){
                while(end>number){--end;(void)norrath_player_stream_unget(signed_byte(*bytes(end)),stream);}
                // Original does not subtract read_count for this full rollback.
                return signed_word(assignments);
            }
            --end;int last=signed_byte(*bytes(end));--read_count;
            if(last!='e'&&last!='E'){
                (void)norrath_player_stream_unget(last,stream);--end;
                last=signed_byte(*bytes(end));--read_count;
            }
            (void)norrath_player_stream_unget(last,stream);
        }
        if(flags&8u)continue;
        *bytes(end)=0u;
        Wide correction=0u;
        if((flags&0x300u)==0x200u){exponent_marker=end;adjustment=zeroes-decimal_zeroes;correction=0u-adjustment;}
        else if(adjustment!=0u)correction=Wide(norrath_player_atol(text(exponent_marker+1u)))-adjustment;
        if(adjustment!=0u){
            if(exponent_marker>=number+329u)exponent_marker=number+328u;
            // Genuine e%ld formatter and native raw64 variadic argumentA2.
            (void)norrath_factory_sprintf(text(exponent_marker),text(address(norrath_player_scan_exponent_format_view)),signed_wide(correction));
        }
        const Wide value=norrath_player_atof(text(number));
        const Word output=next_destination(arguments);
        if(flags&3u)write_wide(output,value);
        else store_float(output,norrath_player_double_to_float(value));
        ++assignments;
    }
}
