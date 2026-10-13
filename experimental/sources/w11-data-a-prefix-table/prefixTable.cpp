// _4item$prefixTable at 0x331e60, 2320 bytes of initialized .data: 58 rows of 40 bytes, the last row
// all zero. Observed: the pointer words are text keys (strings) that are relocated here to string
// literals; strings shared inside the table are written once. The remaining words keep their
// observed bits. Pointer identity with strings of other tables is not preserved.
// Inferred: item::getName reads word 0 as the key text of the attribute row indexed by an item's
// prefix or suffix byte. Unknown: the meaning of every other word.
struct Row {
    const char* p0;
    unsigned w1;
    unsigned w2;
    const char* p3;
    const char* p4;
    unsigned w5;
    unsigned w6;
    unsigned w7;
    unsigned w8;
    unsigned w9;
};
#if defined(__mips__)
static_assert(sizeof(Row) == 40, "prefixTable row");
#endif
static char s0[] = "ruined";
static char s1[] = "+damageincrease-75";
static char s2[] = "+armorbonus-75";
static char s3[] = "dirtied";
static char s4[] = "+armorbonus-50";
static char s5[] = "tattered";
static char s6[] = "+armorbonus-55";
static char s7[] = "rusty";
static char s8[] = "+damageincrease-65";
static char s9[] = "+armorbonus-65";
static char s10[] = "cracked";
static char s11[] = "+damageincrease-45";
static char s12[] = "+armorbonus-45";
static char s13[] = "worn";
static char s14[] = "+damageincrease-35";
static char s15[] = "+armorbonus-35";
static char s16[] = "torn";
static char s17[] = "+armorbonus75";
static char s18[] = "tarnished";
static char s19[] = "+damageincrease-25";
static char s20[] = "+armorbonus-25";
static char s21[] = "tainted";
static char s22[] = "+damageincrease-15";
static char s23[] = "+armorbonus-15";
static char s24[] = "battleworn";
static char s25[] = "+damageincrease-10";
static char s26[] = "+armorbonus-10";
static char s27[] = "charred";
static char s28[] = "+damageincrease-5";
static char s29[] = "+armorbonus-5";
static char s30[] = "woven";
static char s31[] = "+armorbonus150";
static char s32[] = "enlaced";
static char s33[] = "+armorbonus175";
static char s34[] = "bronzelined";
static char s35[] = "+armorbonus200";
static char s36[] = "bronze";
static char s37[] = "+damageincrease25";
static char s38[] = "+armorbonus25";
static char s39[] = "ironlined";
static char s40[] = "+armorbonus225";
static char s41[] = "castiron";
static char s42[] = "+damageincrease50";
static char s43[] = "+armorbonus50";
static char s44[] = "rugged";
static char s45[] = "reinforced";
static char s46[] = "+damageincrease75+weightmodifier-25";
static char s47[] = "+armorbonus100+weightmodifier-25";
static char s48[] = "shiny";
static char s49[] = "+armorbonus275";
static char s50[] = "padded";
static char s51[] = "+armorbonus125";
static char s52[] = "sturdy";
static char s53[] = "strong";
static char s54[] = "sharp";
static char s55[] = "+damageincrease100+critical5";
static char s56[] = "weighted";
static char s57[] = "+damageincrease125+attackspeed1";
static char s58[] = "forged";
static char s59[] = "+damageincrease150";
static char s60[] = "+armorbonus100";
static char s61[] = "polished";
static char s62[] = "+damageincrease175";
static char s63[] = "elven";
static char s64[] = "+damageincrease200+weightmodifier-50";
static char s65[] = "+armorbonus225+weightmodifier-50";
static char s66[] = "dwarven";
static char s67[] = "+damageincrease225+weightmodifier50";
static char s68[] = "+armorbonus250+weightmodifier50";
static char s69[] = "alloy";
static char s70[] = "+damageincrease250+weightmodifier-25";
static char s71[] = "masterwork";
static char s72[] = "+armorbonus300";
static char s73[] = "blackenediron";
static char s74[] = "+damageincrease275";
static char s75[] = "crystal";
static char s76[] = "+damageincrease300+weightmodifier25";
static char s77[] = "teirdal";
static char s78[] = "+damageincrease325+weightmodifier-25";
static char s79[] = "+armorbonus325+weightmodifier-25";
static char s80[] = "combine";
static char s81[] = "+damageincrease335+critical5";
static char s82[] = "+armorbonus335";
static char s83[] = "steellined";
static char s84[] = "+armorbonus375";
static char s85[] = "finesteel";
static char s86[] = "+damageincrease350";
static char s87[] = "+armorbonus350";
static char s88[] = "feirdal";
static char s89[] = "+damageincrease365+weightmodifier-50";
static char s90[] = "+armorbonus365+weightmodifier-50";
static char s91[] = "encrusted";
static char s92[] = "+damageincrease375";
static char s93[] = "+armorbonus400+costmodifier50";
static char s94[] = "embroidered";
static char s95[] = "+armorbonus450";
static char s96[] = "silverlined";
static char s97[] = "+armorbonus475+costmodifier100";
static char s98[] = "alliance";
static char s99[] = "+armorbonus500";
static char s100[] = "veliumlined";
static char s101[] = "+armorbonus525";
static char s102[] = "arcane";
static char s103[] = "+armorbonus550";
static char s104[] = "steelwar";
static char s105[] = "+damageincrease400";
static char s106[] = "+armorbonus415";
static char s107[] = "fortified";
static char s108[] = "+armorbonus425+weightmodifier25";
static char s109[] = "imbued";
static char s110[] = "+armorbonus435+weightmodifier-25";
static char s111[] = "wellbalanced";
static char s112[] = "+damageincrease415+attackspeed1";
static char s113[] = "silverywar";
static char s114[] = "+damageincrease425+costmodifier50";
static char s115[] = "+armorbonus450+costmodifier50";
static char s116[] = "serrated";
static char s117[] = "+damageincrease435+critical10";
static char s118[] = "silverish";
static char s119[] = "+damageincrease450+costmodifier100";
static char s120[] = "+armorbonus460+costmodifier25";
static char s121[] = "velium";
static char s122[] = "+damageincrease460+weightmodifier25";
static char s123[] = "+armorbonus475+weightmodifier25";
static char s124[] = "hulking";
static char s125[] = "+damageincrease475";
static char s126[] = "+armorbonus500+weightmodifier50";
static char s127[] = "ancient";
static char s128[] = "+damageincrease500+weightmodifier50";
static char s129[] = "+armorbonus550+weightmodifier50";
static char s130[] = "obsidian";
static char s131[] = "+damageincrease550";
static char s132[] = "+armorbonus565";
static char s133[] = "titan";
static char s134[] = "+damageincrease575+critical5";
static char s135[] = "+armorbonus575";
static char s136[] = "shadowed";
static char s137[] = "+damageincrease600";
static char s138[] = "+armorbonus600";

extern "C" {
Row _4item$prefixTable[58] = {
    {s0, 0x3010001u, 0x1u, s1, s2, 0x0u, 0x9c0000u, 0x0u, 0x0u, 0x0u},
    {s3, 0x2010001u, 0x1u, nullptr, s4, 0x0u, 0x80000u, 0x0u, 0x0u, 0x0u},
    {s5, 0x1010001u, 0x1u, nullptr, s6, 0x0u, 0x180000u, 0x0u, 0x0u, 0x0u},
    {s7, 0x4020001u, 0x1u, s8, s9, 0x0u, 0x40000u, 0x0u, 0x0u, 0x0u},
    {s10, 0x2030001u, 0x2u, s11, s12, 0x0u, 0x9c0000u, 0x0u, 0x0u, 0x0u},
    {s13, 0x3040001u, 0x3u, s14, s15, 0x0u, 0x9c0000u, 0x0u, 0x0u, 0x0u},
    {s16, 0x2010001u, 0x1u, nullptr, s17, 0x0u, 0x80000u, 0x0u, 0x0u, 0x0u},
    {s18, 0x1050001u, 0x1u, s19, s20, 0x0u, 0x1c0000u, 0x0u, 0x0u, 0x0u},
    {s21, 0x4060001u, 0x4u, s22, s23, 0x0u, 0x9c0000u, 0x0u, 0x0u, 0x0u},
    {s24, 0x1070001u, 0x5u, s25, s26, 0x0u, 0x9c0000u, 0x0u, 0x0u, 0x0u},
    {s27, 0x2080001u, 0x6u, s28, s29, 0x0u, 0x9c0000u, 0x0u, 0x0u, 0x0u},
    {s30, 0x3010001u, 0x1u, nullptr, s31, 0x0u, 0x80000u, 0x0u, 0x0u, 0x0u},
    {s32, 0x4010001u, 0x1u, nullptr, s33, 0x0u, 0x80000u, 0x0u, 0x0u, 0x0u},
    {s34, 0x1010001u, 0x1u, nullptr, s35, 0x0u, 0x80000u, 0x0u, 0x0u, 0x0u},
    {s36, 0x2040003u, 0x1u, s37, s38, 0x0u, 0x40000u, 0x0u, 0x0u, 0x0u},
    {s39, 0x2010001u, 0x1u, nullptr, s40, 0x0u, 0x80000u, 0x0u, 0x0u, 0x0u},
    {s41, 0x3090004u, 0x1u, s42, s43, 0x0u, 0x40000u, 0x0u, 0x0u, 0x0u},
    {s44, 0x4010001u, 0x1u, nullptr, s17, 0x0u, 0x940000u, 0x0u, 0x0u, 0x0u},
    {s45, 0x20a0004u, 0x7u, s46, s47, 0x0u, 0x9c0000u, 0x0u, 0x0u, 0x0u},
    {s48, 0x3010001u, 0x1u, nullptr, s49, 0x0u, 0x80000u, 0x0u, 0x0u, 0x0u},
    {s50, 0x2010001u, 0x1u, nullptr, s51, 0x0u, 0x140000u, 0x0u, 0x0u, 0x0u},
    {s52, 0x3010001u, 0x1u, nullptr, s31, 0x0u, 0x940000u, 0x0u, 0x0u, 0x0u},
    {s53, 0x4010001u, 0x1u, nullptr, s33, 0x0u, 0x940000u, 0x0u, 0x0u, 0x0u},
    {s54, 0x10b0005u, 0x1u, s55, nullptr, 0x0u, 0x40000u, 0x0u, 0xcu, 0x0u},
    {s56, 0x10c0005u, 0x8u, s57, nullptr, 0x0u, 0x840000u, 0x0u, 0x9u, 0x0u},
    {s58, 0x10d0006u, 0x1u, s59, s60, 0x0u, 0x40000u, 0x0u, 0x0u, 0x0u},
    {s61, 0x20e0006u, 0x1u, s62, s35, 0x0u, 0x40000u, 0x0u, 0x0u, 0x0u},
    {s63, 0x30f0007u, 0x9u, s64, s65, 0x0u, 0x9c0000u, 0x0u, 0x0u, 0x0u},
    {s66, 0x4100008u, 0xau, s67, s68, 0x0u, 0x940000u, 0x0u, 0x0u, 0x0u},
    {s69, 0x1110009u, 0x1u, s70, s49, 0x0u, 0x40000u, 0x0u, 0x0u, 0x0u},
    {s71, 0x2120001u, 0xbu, nullptr, s72, 0x0u, 0x940000u, 0x0u, 0x0u, 0x0u},
    {s73, 0x113000au, 0x1u, s74, nullptr, 0x0u, 0x40000u, 0x0u, 0x0u, 0x0u},
    {s75, 0x1140005u, 0x1u, s76, nullptr, 0x0u, 0x40000u, 0x0u, 0xcu, 0x0u},
    {s77, 0x315000bu, 0xcu, s78, s79, 0x0u, 0x9c0000u, 0x0u, 0x0u, 0x0u},
    {s80, 0x416000cu, 0x1u, s81, s82, 0x0u, 0x40000u, 0x0u, 0x0u, 0x0u},
    {s83, 0x40c0001u, 0x1u, nullptr, s84, 0x0u, 0x80000u, 0x0u, 0x0u, 0x0u},
    {s85, 0x117000du, 0x1u, s86, s87, 0x0u, 0x40000u, 0x0u, 0x0u, 0x0u},
    {s88, 0x218000eu, 0xdu, s89, s90, 0x0u, 0x9c0000u, 0x0u, 0x0u, 0x0u},
    {s91, 0x119000eu, 0xeu, s92, s93, 0x0u, 0x9c0000u, 0x0u, 0x0u, 0x0u},
    {s94, 0x4010001u, 0x1u, nullptr, s95, 0x0u, 0x80000u, 0x0u, 0x0u, 0x0u},
    {s96, 0x2010001u, 0x1u, nullptr, s97, 0x0u, 0x80000u, 0x0u, 0x0u, 0x0u},
    {s98, 0x3010001u, 0x1u, nullptr, s99, 0x0u, 0x80000u, 0x0u, 0x0u, 0x0u},
    {s100, 0x4010001u, 0x1u, nullptr, s101, 0x0u, 0x80000u, 0x0u, 0x0u, 0x0u},
    {s102, 0x5010001u, 0x1u, nullptr, s103, 0x0u, 0x80000u, 0x0u, 0x0u, 0x0u},
    {s104, 0x11a000fu, 0x1u, s105, s106, 0x0u, 0x40000u, 0x0u, 0x0u, 0x0u},
    {s107, 0x2010001u, 0x1u, nullptr, s108, 0x0u, 0x940000u, 0x0u, 0x0u, 0x0u},
    {s109, 0x3010001u, 0x1u, nullptr, s110, 0x0u, 0x940000u, 0x0u, 0x0u, 0x0u},
    {s111, 0x11b0010u, 0x1u, s112, nullptr, 0x0u, 0x40000u, 0x0u, 0x0u, 0x0u},
    {s113, 0x41c0011u, 0x1u, s114, s115, 0x0u, 0x40000u, 0x0u, 0x0u, 0x0u},
    {s116, 0x11d0012u, 0x1u, s117, nullptr, 0x0u, 0x40000u, 0x0u, 0x8u, 0x0u},
    {s118, 0x21e0013u, 0x1u, s119, s120, 0x0u, 0x40000u, 0x0u, 0x0u, 0x0u},
    {s121, 0x11f000eu, 0xfu, s122, s123, 0x0u, 0x940000u, 0x0u, 0x0u, 0x0u},
    {s124, 0x3200001u, 0x10u, s125, s126, 0x0u, 0x940000u, 0x0u, 0x0u, 0x0u},
    {s127, 0x4210001u, 0x11u, s128, s129, 0x0u, 0x9c0000u, 0x0u, 0x0u, 0x0u},
    {s130, 0x30a0014u, 0x6u, s131, s132, 0x0u, 0x40000u, 0x0u, 0x0u, 0x0u},
    {s133, 0x4220015u, 0x12u, s134, s135, 0x0u, 0x940000u, 0x0u, 0x0u, 0x0u},
    {s136, 0x5230015u, 0x13u, s137, s138, 0x0u, 0x9c0000u, 0x0u, 0x0u, 0x0u},
    {nullptr, 0x0u, 0x0u, nullptr, nullptr, 0x0u, 0x0u, 0x0u, 0x0u, 0x0u},
};
}
#if defined(__mips__)
static_assert(sizeof(_4item$prefixTable) == 2320, "prefixTable extent");
#endif
