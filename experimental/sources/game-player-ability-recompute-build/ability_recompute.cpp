#include "ability_recompute.hpp"
using namespace norrath_ability_recompute_detail;

extern "C" void norrath_player_recompute_ability_modifiers() {
    const Address count_word=address(norrath_player_active_count_word_view);
    int count=signed_word(read_word_fresh(count_word));
    if(count<=0)return;
    int player_index=0;
    Address record=address(norrath_player_progression_records_view);
    do {
        const Address captured_actor=read_word_fresh(record+21872u);
        if(captured_actor!=0u) {
            for(unsigned ability=0;ability<4u;++ability) {
                const unsigned value=read_word_fresh(record+120u+ability*4u);
                write_word_ordered(record+144u+ability*4u,value);
            }
            write_word_ordered(record+224u,0u);
            for(unsigned slot=0;slot<16u;++slot) {
                const Address item=read_word_fresh(record+21896u+slot*4u);
                if(item==0u)continue;
                // Each ID performs a separate first-match scan and fresh update.
                for(unsigned ability=0;ability<4u;++ability) {
                    const int modifier=first_item_modifier(item,int(7u+ability));
                    const Address field=record+144u+ability*4u;
                    write_word_ordered(field,read_word_fresh(field)+unsigned(modifier));
                }
                const int modifier=first_item_modifier(item,30);
                const float amount=native_signed_to_float(modifier);
                const float coefficient=read_float_fresh(address(norrath_player_ability_coefficients_view));
                const float accumulated=read_float_fresh(record+224u);
                const float product=native_multiply(amount,coefficient);
                write_float_ordered(record+224u,native_add(accumulated,product));
            }
            Address actor=captured_actor;
            for(unsigned ability=0;ability<4u;++ability) {
                const unsigned value=read_word_fresh(record+144u+ability*4u);
                write_byte_ordered(actor+1124u+ability,value-5u);
                if(ability==0u||ability==2u) {
                    const unsigned fresh_value=read_word_fresh(record+144u+ability*4u);
                    const Address fresh_actor=read_word_fresh(record+21872u);
                    write_byte_ordered(fresh_actor+1124u+ability,fresh_value-30u);
                }
                const Address clamp_actor=read_word_fresh(record+21872u);
                const Address field=clamp_actor+1124u+ability;
                if(read_signed_byte_fresh(field)<0)write_byte_ordered(field,0u);
                if(ability+1u<4u)actor=read_word_fresh(record+21872u);
            }
            const unsigned initial_class=read_word_fresh(record+80u);
            // These initial class stores really occur even though maximum
            // energy is recalculated again later in this same original method.
            if(initial_class==0u) {
                write_byte_ordered(record+192u,18u);write_float_ordered(record+200u,4.0f);
            } else if(initial_class==2u) {
                write_byte_ordered(record+192u,14u);write_float_ordered(record+200u,5.0f);
            } else if(initial_class==1u) {
                write_byte_ordered(record+192u,16u);write_float_ordered(record+200u,7.0f);
            } else if(initial_class==4u) {
                write_byte_ordered(record+192u,12u);write_float_ordered(record+200u,10.0f);
            } else if(initial_class==3u) {
                write_byte_ordered(record+192u,14u);write_float_ordered(record+200u,6.0f);
            }
            const int initial_level=read_signed_byte_fresh(record+89u);
            const int health_factor=read_signed_byte_fresh(record+192u);
            const int health_base=signed_word(unsigned(health_factor)*(unsigned(initial_level)+1u));
            write_float_ordered(record+176u,native_signed_to_float(health_base));
            const int health_rank=norrath_player_get_feat_rank(player_index,128ll);
            const int vitality=signed_word(read_word_fresh(record+156u));
            const int health_bonus=wrapped_product(health_rank,3u);
            const float health_maximum=read_float_fresh(record+176u);
            const float health_feat=native_signed_to_float(health_bonus);
            const int half_vitality=half_truncated(vitality);
            const unsigned energy_class=read_word_fresh(record+80u);
            const float health_ability=native_signed_to_float(half_vitality);
            const float health_updated=native_add(native_add(health_maximum,health_feat),health_ability);
            write_float_ordered(record+176u,health_updated);
            int energy_factor=0;
            if(energy_class==0u)energy_factor=6;
            else if(energy_class==2u)energy_factor=8;
            else if(energy_class==1u)energy_factor=10;
            else {
                if(energy_class==3u)energy_factor=9;
                if(energy_class==4u)energy_factor=14;
            }
            const int energy_level=read_signed_byte_fresh(record+89u);
            const Address energy_actor=read_word_fresh(record+21872u);
            const int energy_base=signed_word(unsigned(energy_factor)*unsigned(energy_level));
            const int energy_player=signed_word(read_word_fresh(energy_actor+960u));
            write_float_ordered(record+200u,native_signed_to_float(energy_base));
            const int energy_rank=norrath_player_get_feat_rank(energy_player,256ll);
            const int intelligence=signed_word(read_word_fresh(record+148u));
            const int energy_bonus=wrapped_product(energy_rank,5u);
            const float energy_maximum=read_float_fresh(record+200u);
            const float energy_feat=native_signed_to_float(energy_bonus);
            const unsigned strength=read_word_fresh(record+144u);
            const Address defense_actor=read_word_fresh(record+21872u);
            const int half_intelligence=half_truncated(intelligence);
            const int defense_base=signed_word(strength+125u);
            const float energy_ability=native_signed_to_float(half_intelligence);
            const int defense_player=signed_word(read_word_fresh(defense_actor+960u));
            const float energy_with_feat=native_add(energy_maximum,energy_feat);
            const float defense_value=native_signed_to_float(defense_base);
            const float energy_updated=native_add(energy_with_feat,energy_ability);
            write_float_ordered(record+312u,defense_value);
            write_float_ordered(record+200u,energy_updated);
            const int defense_rank=norrath_player_get_feat_rank(defense_player,64ll);
            const float defense_feat=native_signed_to_float(defense_rank);
            const unsigned cheat=read_word_fresh(address(norrath_player_ability_cheat_mode_word_view));
            const float current_defense=read_float_fresh(record+312u);
            const float defense_product=native_multiply(defense_feat,15.0f);
            const float defense_updated=native_add(current_defense,defense_product);
            write_float_ordered(record+312u,defense_updated);
            if(cheat!=0u) {
                const float extra=read_float_fresh(address(norrath_player_ability_coefficients_view)+4u);
                write_float_ordered(record+312u,native_add(defense_updated,extra));
            }
            const float fresh_defense=read_float_fresh(record+312u);
            const float ceiling=read_float_fresh(address(norrath_player_ability_coefficients_view)+8u);
            if(ceiling<fresh_defense)write_float_ordered(record+312u,ceiling);
            if(read_word_fresh(record+21872u)!=0u) {
                const int armor=norrath_player_recompute_paper_doll_modifiers(player_index);
                const Address fresh_actor=read_word_fresh(record+21872u);
                write_half_ordered(fresh_actor+232u,unsigned(armor));
            }
            const float maximum_energy=read_float_fresh(record+200u);
            const float current_energy=read_float_fresh(record+196u);
            if(maximum_energy<current_energy)write_float_ordered(record+196u,maximum_energy);
            const float maximum_health=read_float_fresh(record+176u);
            const float current_health=read_float_fresh(record+172u);
            if(maximum_health<current_health)write_float_ordered(record+172u,maximum_health);
            // Original null-actor skip bypasses this count reload completely.
            count=signed_word(read_word_fresh(count_word));
        }
        player_index=signed_word(unsigned(player_index)+1u);
        record+=23984u;
    } while(player_index<count);
}
