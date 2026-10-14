// Observed unnamed .sdata run of eleven 8-byte name slots at 0x485580, each a C string
// padded with NULs to the slot width. Inferred extent: the only caller indexes 0..10.
// Unknown: the image continues with further 8-byte slots after the eleventh; they are not part of this view.
extern "C" const char norrath_ctors_ant_lantern_view_485580[11][8] = {
    "vaseweb",
    "hgurn",
    "goyle",
    "boxafay",
    "boxbfay",
    "boxcfay",
    "lavakeg",
    "hatebox",
    "uncrate",
    "dt_pota",
    "dt_potb"};
