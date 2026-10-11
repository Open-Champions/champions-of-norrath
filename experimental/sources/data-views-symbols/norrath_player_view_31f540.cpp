// Observed .data symbol _4item$globalItem at 0x31f540, 1696 bytes.
// 106 records of mesh pointer, texture pointer, and two zero words.
// Callers index with a 16-byte stride. Unknown: the legal range of the proxy id.
struct ProxyRecord {
    char* mesh;
    char* texture;
    unsigned unused0;
    unsigned unused1;
};
#if defined(__mips__)
static_assert(sizeof(ProxyRecord) == 16, "proxy record is two pointers and two zero words");
#endif
static char text_0[] = "arrow1.vif";
static char text_1[] = "arrow1.tex";
static char text_2[] = "arrowa.vif";
static char text_3[] = "arrowa.tex";
static char text_4[] = "quiver.vif";
static char text_5[] = "quiver.tex";
static char text_6[] = "dagger1.vif";
static char text_7[] = "dagger1.tex";
static char text_8[] = "short_sword1.vif";
static char text_9[] = "short_sword1.tex";
static char text_10[] = "broad_sword1.vif";
static char text_11[] = "broad_sword1.tex";
static char text_12[] = "long_sword1.vif";
static char text_13[] = "long_sword1.tex";
static char text_14[] = "rapier1.vif";
static char text_15[] = "rapier1.tex";
static char text_16[] = "bastard1.vif";
static char text_17[] = "bastard1.tex";
static char text_18[] = "dbl_hand_sword1.vif";
static char text_19[] = "dbl_hand_sword1.tex";
static char text_20[] = "claymore1.vif";
static char text_21[] = "claymore1.tex";
static char text_22[] = "falchion1.vif";
static char text_23[] = "falchion1.tex";
static char text_24[] = "scimitar1.vif";
static char text_25[] = "scimitar1.tex";
static char text_26[] = "flamberge1.vif";
static char text_27[] = "flamberge1.tex";
static char text_28[] = "cleaver1.vif";
static char text_29[] = "cleaver1.tex";
static char text_30[] = "axe1.vif";
static char text_31[] = "axe1.tex";
static char text_32[] = "battle_axe1.vif";
static char text_33[] = "battle_axe1.tex";
static char text_34[] = "dbl_battle_axe1.vif";
static char text_35[] = "dbl_battle_axe1.tex";
static char text_36[] = "halberd1.vif";
static char text_37[] = "halberd1.tex";
static char text_38[] = "spear1.vif";
static char text_39[] = "spear1.tex";
static char text_40[] = "club1.vif";
static char text_41[] = "club1.tex";
static char text_42[] = "mace1.vif";
static char text_43[] = "mace1.tex";
static char text_44[] = "dbl_hand_mace1.vif";
static char text_45[] = "dbl_hand_mace1.tex";
static char text_46[] = "morning_star1.vif";
static char text_47[] = "morning_star1.tex";
static char text_48[] = "pick1.vif";
static char text_49[] = "pick1.tex";
static char text_50[] = "hammer1.vif";
static char text_51[] = "hammer1.tex";
static char text_52[] = "dbl_hand_hammer1.vif";
static char text_53[] = "dbl_hand_hammer1.tex";
static char text_54[] = "rod1.vif";
static char text_55[] = "rod1.tex";
static char text_56[] = "dbl_hand_staff1.vif";
static char text_57[] = "dbl_hand_staff1.tex";
static char text_58[] = "great_staff1.vif";
static char text_59[] = "great_staff1.tex";
static char text_60[] = "bow1.vif";
static char text_61[] = "bow1.tex";
static char text_62[] = "composite_bow1.vif";
static char text_63[] = "composite_bow1.tex";
static char text_64[] = "shield1.vif";
static char text_65[] = "shield1.tex";
static char text_66[] = "shield_leather.vif";
static char text_67[] = "shield_leather.tex";
static char text_68[] = "pickup_leather_body.vif";
static char text_69[] = "pickup_leather_body.tex";
static char text_70[] = "pickup_leather_boots.vif";
static char text_71[] = "pickup_leather_boots.tex";
static char text_72[] = "pickup_leather_gloves.vif";
static char text_73[] = "pickup_leather_gloves.tex";
static char text_74[] = "pickup_leather_helmet.vif";
static char text_75[] = "pickup_leather_helmet.tex";
static char text_76[] = "pickup_leather_leggings.vif";
static char text_77[] = "pickup_leather_leggings.tex";
static char text_78[] = "pickup_chain_body.vif";
static char text_79[] = "pickup_chain_body.tex";
static char text_80[] = "pickup_chain_boots.vif";
static char text_81[] = "pickup_chain_boots.tex";
static char text_82[] = "pickup_chain_gloves.vif";
static char text_83[] = "pickup_chain_gloves.tex";
static char text_84[] = "pickup_chain_helmet.vif";
static char text_85[] = "pickup_chain_helmet.tex";
static char text_86[] = "pickup_chain_leggings.vif";
static char text_87[] = "pickup_chain_leggings.tex";
static char text_88[] = "pickup_plate_body.vif";
static char text_89[] = "pickup_plate_body.tex";
static char text_90[] = "pickup_plate_boots.vif";
static char text_91[] = "pickup_plate_boots.tex";
static char text_92[] = "pickup_plate_gloves.vif";
static char text_93[] = "pickup_plate_gloves.tex";
static char text_94[] = "pickup_plate_helmet.vif";
static char text_95[] = "pickup_plate_helmet.tex";
static char text_96[] = "pickup_plate_leggings.vif";
static char text_97[] = "pickup_plate_leggings.tex";
static char text_98[] = "potion.vif";
static char text_99[] = "potion.tex";
static char text_100[] = "potionmed.vif";
static char text_101[] = "potionmed.tex";
static char text_102[] = "potionlarge.vif";
static char text_103[] = "potionlarge.tex";
static char text_104[] = "potionmana.vif";
static char text_105[] = "potionmana.tex";
static char text_106[] = "potionmanamed.vif";
static char text_107[] = "potionmanamed.tex";
static char text_108[] = "potionmanalarge.vif";
static char text_109[] = "potionmanalarge.tex";
static char text_110[] = "recallscroll.vif";
static char text_111[] = "recallscroll.tex";
static char text_112[] = "potionfire.vif";
static char text_113[] = "potionfire.tex";
static char text_114[] = "potiongas.vif";
static char text_115[] = "potiongas.tex";
static char text_116[] = "potionexplode.vif";
static char text_117[] = "potionexplode.tex";
static char text_118[] = "venomsac.vif";
static char text_119[] = "venomsac.tex";
static char text_120[] = "fire_beetle_eye.vif";
static char text_121[] = "fire_beetle_eye.tex";
static char text_122[] = "bonechip.vif";
static char text_123[] = "bonechip.tex";
static char text_124[] = "poisongland.vif";
static char text_125[] = "poisongland.tex";
static char text_126[] = "mummytongue.vif";
static char text_127[] = "mummytongue.tex";
static char text_128[] = "mummyheart.vif";
static char text_129[] = "mummyheart.tex";
static char text_130[] = "stormshard.vif";
static char text_131[] = "stormshard.tex";
static char text_132[] = "soulgem.vif";
static char text_133[] = "soulgem.tex";
static char text_134[] = "vampirefang.vif";
static char text_135[] = "vampirefang.tex";
static char text_136[] = "bullrune.vif";
static char text_137[] = "bullrune.tex";
static char text_138[] = "starglyph.vif";
static char text_139[] = "starglyph.tex";
static char text_140[] = "ghoulflesh.vif";
static char text_141[] = "ghoulflesh.tex";
static char text_142[] = "bloodstone.vif";
static char text_143[] = "bloodstone.tex";
static char text_144[] = "regenstone.vif";
static char text_145[] = "regenstone.tex";
static char text_146[] = "manastone.vif";
static char text_147[] = "manastone.tex";
static char text_148[] = "carapace.vif";
static char text_149[] = "carapace.tex";
static char text_150[] = "mistyglobe.vif";
static char text_151[] = "mistyglobe.tex";
static char text_152[] = "wetstone.vif";
static char text_153[] = "wetstone.tex";
static char text_154[] = "ring1.vif";
static char text_155[] = "ring1.tex";
static char text_156[] = "amulet1.vif";
static char text_157[] = "amulet1.tex";
static char text_158[] = "earring1.vif";
static char text_159[] = "earring1.tex";
static char text_160[] = "choker1.vif";
static char text_161[] = "choker1.tex";
static char text_162[] = "scroll.vif";
static char text_163[] = "scroll.tex";
static char text_164[] = "hatesharda.vif";
static char text_165[] = "hatesharda.tex";
static char text_166[] = "hateshardb.vif";
static char text_167[] = "hateshardb.tex";
static char text_168[] = "elfdraught.vif";
static char text_169[] = "elfdraught.tex";
static char text_170[] = "raftpart1.vif";
static char text_171[] = "raftpart1.tex";
static char text_172[] = "raftpart2.vif";
static char text_173[] = "raftpart2.tex";
static char text_174[] = "raftpart3.vif";
static char text_175[] = "raftpart3.tex";
static char text_176[] = "raftpart4.vif";
static char text_177[] = "raftpart4.tex";
static char text_178[] = "trinketpart1.vif";
static char text_179[] = "trinketpart1.tex";
static char text_180[] = "trinketpart2.vif";
static char text_181[] = "trinketpart2.tex";
static char text_182[] = "trinketpart3.vif";
static char text_183[] = "trinketpart3.tex";
static char text_184[] = "trinketpart4.vif";
static char text_185[] = "trinketpart4.tex";
static char text_186[] = "trinketpart5.vif";
static char text_187[] = "trinketpart5.tex";
static char text_188[] = "trinketpart6.vif";
static char text_189[] = "trinketpart6.tex";
static char text_190[] = "trinketpart7.vif";
static char text_191[] = "trinketpart7.tex";
static char text_192[] = "bloodvine.vif";
static char text_193[] = "bloodvine.tex";
static char text_194[] = "exchanger.vif";
static char text_195[] = "exchanger.tex";
static char text_196[] = "cellkey.vif";
static char text_197[] = "cellkey.tex";
static char text_198[] = "shell.vif";
static char text_199[] = "shell.tex";
static char text_200[] = "battleplans.vif";
static char text_201[] = "battleplans.tex";
static char text_202[] = "horn.vif";
static char text_203[] = "horn.tex";
extern "C" {
ProxyRecord norrath_player_view_31f540[106] = {
    {text_0, text_1, 0u, 0u},
    {text_2, text_3, 0u, 0u},
    {text_4, text_5, 0u, 0u},
    {text_6, text_7, 0u, 0u},
    {text_8, text_9, 0u, 0u},
    {text_10, text_11, 0u, 0u},
    {text_12, text_13, 0u, 0u},
    {text_14, text_15, 0u, 0u},
    {text_16, text_17, 0u, 0u},
    {text_18, text_19, 0u, 0u},
    {text_20, text_21, 0u, 0u},
    {text_22, text_23, 0u, 0u},
    {text_24, text_25, 0u, 0u},
    {text_26, text_27, 0u, 0u},
    {text_28, text_29, 0u, 0u},
    {text_30, text_31, 0u, 0u},
    {text_32, text_33, 0u, 0u},
    {text_34, text_35, 0u, 0u},
    {text_36, text_37, 0u, 0u},
    {text_38, text_39, 0u, 0u},
    {text_40, text_41, 0u, 0u},
    {text_42, text_43, 0u, 0u},
    {text_44, text_45, 0u, 0u},
    {text_46, text_47, 0u, 0u},
    {text_48, text_49, 0u, 0u},
    {text_50, text_51, 0u, 0u},
    {text_52, text_53, 0u, 0u},
    {text_54, text_55, 0u, 0u},
    {text_56, text_57, 0u, 0u},
    {text_58, text_59, 0u, 0u},
    {text_60, text_61, 0u, 0u},
    {text_62, text_63, 0u, 0u},
    {text_64, text_65, 0u, 0u},
    {text_66, text_67, 0u, 0u},
    {text_68, text_69, 0u, 0u},
    {text_70, text_71, 0u, 0u},
    {text_72, text_73, 0u, 0u},
    {text_74, text_75, 0u, 0u},
    {text_76, text_77, 0u, 0u},
    {text_78, text_79, 0u, 0u},
    {text_80, text_81, 0u, 0u},
    {text_82, text_83, 0u, 0u},
    {text_84, text_85, 0u, 0u},
    {text_86, text_87, 0u, 0u},
    {text_88, text_89, 0u, 0u},
    {text_90, text_91, 0u, 0u},
    {text_92, text_93, 0u, 0u},
    {text_94, text_95, 0u, 0u},
    {text_96, text_97, 0u, 0u},
    {text_98, text_99, 0u, 0u},
    {text_100, text_101, 0u, 0u},
    {text_102, text_103, 0u, 0u},
    {text_104, text_105, 0u, 0u},
    {text_106, text_107, 0u, 0u},
    {text_108, text_109, 0u, 0u},
    {text_110, text_111, 0u, 0u},
    {text_112, text_113, 0u, 0u},
    {text_114, text_115, 0u, 0u},
    {text_116, text_117, 0u, 0u},
    {text_64, text_65, 0u, 0u},
    {text_118, text_119, 0u, 0u},
    {text_120, text_121, 0u, 0u},
    {text_122, text_123, 0u, 0u},
    {text_124, text_125, 0u, 0u},
    {text_126, text_127, 0u, 0u},
    {text_128, text_129, 0u, 0u},
    {text_130, text_131, 0u, 0u},
    {text_132, text_133, 0u, 0u},
    {text_134, text_135, 0u, 0u},
    {text_136, text_137, 0u, 0u},
    {text_138, text_139, 0u, 0u},
    {text_140, text_141, 0u, 0u},
    {text_142, text_143, 0u, 0u},
    {text_144, text_145, 0u, 0u},
    {text_146, text_147, 0u, 0u},
    {text_148, text_149, 0u, 0u},
    {text_150, text_151, 0u, 0u},
    {text_152, text_153, 0u, 0u},
    {text_154, text_155, 0u, 0u},
    {text_156, text_157, 0u, 0u},
    {text_158, text_159, 0u, 0u},
    {text_160, text_161, 0u, 0u},
    {text_162, text_163, 0u, 0u},
    {text_164, text_165, 0u, 0u},
    {text_166, text_167, 0u, 0u},
    {text_168, text_169, 0u, 0u},
    {text_170, text_171, 0u, 0u},
    {text_172, text_173, 0u, 0u},
    {text_174, text_175, 0u, 0u},
    {text_176, text_177, 0u, 0u},
    {text_178, text_179, 0u, 0u},
    {text_180, text_181, 0u, 0u},
    {text_182, text_183, 0u, 0u},
    {text_184, text_185, 0u, 0u},
    {text_186, text_187, 0u, 0u},
    {text_188, text_189, 0u, 0u},
    {text_190, text_191, 0u, 0u},
    {text_142, text_143, 0u, 0u},
    {text_192, text_193, 0u, 0u},
    {text_194, text_195, 0u, 0u},
    {text_196, text_197, 0u, 0u},
    {text_198, text_199, 0u, 0u},
    {text_200, text_201, 0u, 0u},
    {text_202, text_203, 0u, 0u},
    {text_128, text_129, 0u, 0u},
    {nullptr, nullptr, 0u, 0u},
};
}
