#include "float_tags.hpp"
extern "C" float norrath_player_find_tag_float(void* object,char* tag,int occurrence){
    using namespace norrath_float_tags_detail;
    const Address a=address(object),name=address(tag);
    if(read_word(a+44u)==0u)return 0.0f; // Original returns before any tag access.
    Address cursor=name;
    if(bytes(cursor)[0]!=0u){
        do{
            const int c=signed_byte(bytes(cursor)[0]);
            const unsigned char classification=norrath_factory_ctype_minimum_view[c+128];
            bytes(cursor)[0]=static_cast<unsigned char>((classification&1u)?c+32:c);
            cursor+=1u;
        }while(bytes(cursor)[0]!=0u);
    }
    const unsigned length=norrath_factory_strlen(tag);
    Address table=read_word(a+44u),offset=0u;
    if(read_word(table)==0u)return 0.0f;
    unsigned selected=unsigned(occurrence);
    for(;;){
        char* candidate=text(read_word(table+offset));
        const int comparison=norrath_factory_strncmp(candidate,tag,length);
        table=read_word(a+44u); // Genuine comparison effects precede this reload.
        if(comparison==0){
            const Address delimiter=read_word(table+offset)+length;
            offset+=4u;
            if(signed_byte(bytes(delimiter)[0])==61){
                selected-=1u;
                if(selected==0xffffffffu){
                    const Wide parsed=norrath_player_atof(text(delimiter+1u));
                    return norrath_player_double_to_float(parsed);
                }
            }
        }else offset+=4u;
        if(read_word(table+offset)==0u)return 0.0f;
    }
}
