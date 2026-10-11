#include "scan_stream.hpp"
using namespace norrath_stream_io_detail;

// Complete original LOCAL lflush callback, with its real flush operation.
static int norrath_scan_line_flush(unsigned char* stream) {
    if((half(address(stream)+12u)&9u)==9u)return norrath_factory_stream_flush(stream);
    return 0;
}

extern "C" int norrath_player_stream_refill(unsigned char* stream) {
    const Word file=address(stream);
    Word reent=read_word(file+84u);
    if(reent==0u) {
        reent=read_word(address(norrath_factory_impure_pointer_word));
        write_word(file+84u,reent);
    }
    if(read_word(reent+56u)==0u)norrath_factory_streams_init(bytes(reent));
    Word flags=half(file+12u);
    write_word(file+4u,0u);
    if(flags&0x20u)return -1;
    if((flags&4u)==0u) {
        if((flags&0x10u)==0u)return -1;
        if(flags&8u) {
            if(norrath_factory_stream_flush(stream)!=0)return -1;
            flags=half(file+12u);
            write_word(file+8u,0u);
            flags&=0xfff7u;
            write_word(file+24u,0u);write_half(file+12u,flags);
        }
        write_half(file+12u,flags|4u);
    } else {
        const Word pushback=read_word(file+48u);
        if(pushback!=0u) {
            if(pushback!=file+64u)norrath_player_free_reentrant(bytes(read_word(file+84u)),bytes(pushback));
            const Word restored=read_word(file+60u);
            write_word(file+48u,0u);write_word(file+4u,restored);
            if(restored!=0u) {
                const Word cursor=read_word(file+56u);
                write_word(file,cursor);return 0;
            }
        }
    }
    if(read_word(file+16u)==0u)norrath_player_stream_make_buffer(stream);
    if(half(file+12u)&3u)norrath_factory_walk_streams(bytes(read_word(file+84u)),&norrath_scan_line_flush);
    const Word buffer=read_word(file+16u);
    const Word cookie=read_word(file+28u);
    using Read=int (*)(unsigned char*,char*,int);
    Read driver=reinterpret_cast<Read>(static_cast<__UINTPTR_TYPE__>(read_word(file+32u)));
    write_word(file,buffer);
    const int count=driver(bytes(cookie),text(buffer),signed_word(read_word(file+20u)));
    flags=half(file+12u)&0xdfffu;
    write_word(file+4u,Word(count));write_half(file+12u,flags);
    if(count>0)return 0;
    if(count==0)flags|=0x20u;
    else {flags|=0x40u;write_word(file+4u,0u);}
    write_half(file+12u,flags);return -1;
}

extern "C" unsigned norrath_player_stream_read_elements(void* destination,Word size,Word count,unsigned char* stream) {
    const Word total=count*size;
    if(total==0u)return 0u;
    const Word file=address(stream);
    Word available=read_word(file+4u);
    if(signed_word(available)<0) {write_word(file+4u,0u);available=0u;}
    Word cursor=address(destination),remaining=total;
    while(available<remaining) {
        const Word input=read_word(file);
        remaining-=available;
        norrath_copy_bytes(bytes(cursor),bytes(input),available);
        const Word fresh_input=read_word(file);
        cursor+=available;write_word(file,fresh_input+available);
        if(norrath_player_stream_refill(stream)!=0)return (total-remaining)/size;
        available=read_word(file+4u);
    }
    const Word input=read_word(file);
    norrath_copy_bytes(bytes(cursor),bytes(input),remaining);
    const Word fresh_count=read_word(file+4u),fresh_input=read_word(file);
    write_word(file+4u,fresh_count-remaining);write_word(file,fresh_input+remaining);
    return count;
}

extern "C" int norrath_player_stream_unget(int character,unsigned char* stream) {
    if(character==-1)return -1;
    const Word file=address(stream);
    Word reent=read_word(file+84u);
    if(reent==0u) {reent=read_word(address(norrath_factory_impure_pointer_word));write_word(file+84u,reent);}
    if(read_word(reent+56u)==0u)norrath_factory_streams_init(bytes(reent));
    Word flags=half(file+12u)&0xffdfu;
    write_half(file+12u,flags);
    if((flags&4u)==0u) {
        if((flags&0x10u)==0u)return -1;
        if(flags&8u) {
            if(norrath_factory_stream_flush(stream)!=0)return -1;
            flags=half(file+12u);
            write_word(file+8u,0u);flags&=0xfff7u;
            write_word(file+24u,0u);write_half(file+12u,flags);
        }
        flags=half(file+12u);write_half(file+12u,flags|4u);
    }
    const Word pushback=read_word(file+48u),byte=Word(character)&0xffu;
    if(pushback!=0u) {
        const Word old_count=read_word(file+4u),capacity=read_word(file+52u);
        if(signed_word(old_count)>=signed_word(capacity)&&norrath_player_stream_extend_pushback(stream)!=0)return -1;
        const Word cursor=read_word(file)-1u;
        write_word(file,cursor);*bytes(cursor)=static_cast<unsigned char>(byte);
        const Word fresh_count=read_word(file+4u);
        write_word(file+4u,fresh_count+1u);return int(byte);
    }
    const Word base=read_word(file+16u),cursor=read_word(file);
    Word old_count;
    if(base!=0u) {
        old_count=read_word(file+4u);
        if(base<cursor&&Word(*bytes(cursor-1u))==byte) {
            write_word(file,cursor-1u);write_word(file+4u,old_count+1u);return int(byte);
        }
    } else old_count=read_word(file+4u);
    write_word(file+60u,old_count);write_word(file+56u,cursor);
    write_word(file+48u,file+64u);write_word(file+52u,3u);
    write_word(file,file+66u);*bytes(file+66u)=static_cast<unsigned char>(byte);
    write_word(file+4u,1u);return int(byte);
}

extern "C" const char* norrath_player_scan_character_class(unsigned char* table,const char* format) {
    const Word base=address(table);Word cursor=address(format);
    int current=norrath_core_format_detail::signed_byte(*bytes(cursor++));
    Word initial=0u;
    if(current==94) {current=norrath_core_format_detail::signed_byte(*bytes(cursor++));initial=1u;}
    for(Word n=256u;n!=0u;) {*bytes(base+--n)=static_cast<unsigned char>(initial);}
    if(current==0)return text(cursor-1u);
    const Word included=1u-initial;
mark_current:
    *bytes(base+Word(current))=static_cast<unsigned char>(included);
    {
        int next=norrath_core_format_detail::signed_byte(*bytes(cursor));cursor+=1u;
next_character:
        if(next==45) {
            const int endpoint=norrath_core_format_detail::signed_byte(*bytes(cursor));
            if(endpoint==93||endpoint<current) {current=45;goto mark_current;}
            cursor+=1u;
            do {current+=1;*bytes(base+Word(current))=static_cast<unsigned char>(included);}while(current<endpoint);
            next=norrath_core_format_detail::signed_byte(*bytes(cursor));cursor+=1u;
            goto next_character;
        }
        if(next==0)return text(cursor-1u);
        if(next==93)return text(cursor);
        current=next;goto mark_current;
    }
}
