#include "creature_animation.hpp"
using namespace creature_detail;
extern "C" unsigned norrath_actor_finish_construction(void* object){
    const Address a=address(object);
    using Unary=void(*)(void*);using Model=void*(*)(void*);using Argument=void(*)(void*,int);
    if((wide(a+160u)&1ull)!=0u){
        const Address entry=word(a+72u)+144u;
        const Address self=a+unsigned(signed_half(half(entry)));
        (void)reinterpret_cast<Unary>(pointer(word(entry+4u)))(pointer(self));
    }
    Address entry=word(a+72u)+128u;
    Address self=a+unsigned(signed_half(half(entry)));
    void* first_model=reinterpret_cast<Model>(pointer(word(entry+4u)))(pointer(self));
    if(first_model!=nullptr){
        // Original calls the virtual again and does not reuse the first result.
        entry=word(a+72u)+128u;self=a+unsigned(signed_half(half(entry)));
        void* fresh_model=reinterpret_cast<Model>(pointer(word(entry+4u)))(pointer(self));
        int bounds[6];
        norrath_model_bounding_box(fresh_model,-1,&bounds[0],&bounds[1],&bounds[2],&bounds[3],&bounds[4],&bounds[5]);
        const float height=static_cast<float>(signed_word(unsigned(bounds[5])-unsigned(bounds[2])));
        put_scalar(a+32u,height);
        if(height<60.0f)put_scalar(a+32u,60.0f);
        const unsigned rounded=unsigned(chop(scalar(a+32u)))+1u;
        bytes(a)[41]=static_cast<unsigned char>(rounded);
    }
    char* result=norrath_actor_find_tag_string(object,tag(norrath_creature_view_4837e0),0);
    if(result==nullptr)return 0u;
    entry=word(a+72u)+184u;
    const Wide updated=wide(a+160u)|0x100000000ull;
    const Address target=word(entry+4u);self=a+unsigned(signed_half(half(entry)));
    put_wide(a+160u,updated);
    (void)reinterpret_cast<Argument>(pointer(target))(pointer(self),10);
    return norrath_animation_set_to_end_fully_blended(pointer(a+288u));
}
