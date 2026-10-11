// Observed archive data. Directory A is layer0Dir at 0x006cc6a0 and directory B
// is layer1Dir at 0x006d26a8. Each is 768 rows of 32 bytes and a count word at
// offset 24576. currentLayerDir at 0x00483ff0 is file-initialized to directory A.
// isDual and dualLayer2Start are file-backed zeros. command at 0x006d86b0 is
// 16 bytes, ending at lumpLoadName. The format and extension bytes are the
// file contents at the addresses already named by the loader views.
extern "C" {
alignas(4) unsigned char norrath_archive_directory_a_view[24580];
alignas(4) unsigned char norrath_archive_directory_b_view[24580];
unsigned char* norrath_archive_directory_pointer_view = norrath_archive_directory_a_view;
unsigned char norrath_archive_directory_switch_view[4] = {};
unsigned char norrath_archive_layer_bias_view[4] = {};
unsigned char norrath_archive_command_view[16] = {};
unsigned char norrath_archive_completion_pointer_view[4] = {};
unsigned char norrath_archive_read_active_view[1] = {};
unsigned char norrath_archive_stream_active_view[1] = {};
unsigned char norrath_archive_stream_destination_view[4] = {};
unsigned char norrath_archive_stream_remaining_view[4] = {};
unsigned char norrath_archive_stream_restart_view[1] = {};
unsigned char norrath_archive_read_mode_view[4] = {};
unsigned char norrath_archive_permanent_halt_flag_view[1] = {};
unsigned char norrath_archive_no_new_vags_view[4] = {};
// Observed c_cnt.66 at 0x004460d4, four file-backed zero bytes. The dot in the
// original name is not a C identifier, so the read paths use this view.
unsigned char norrath_archive_cd_command_count_view[4] = {};
}
// const at namespace scope has internal linkage unless it is explicitly extern.
// The linkage block above does not supply that, so these are declared extern.
extern "C" const unsigned char norrath_archive_animation_extension_view[64] = {
    'a','n','m',0,0,0,0,0,
    'v','i','f',0,0,0,0,0,
    't','e','x',0,0,0,0,0,
    'f','n','t',0,0,0,0,0,
    'w','o','r','l','d',0,0,0,
    'v','a','g',0,0,0,0,0,
    'u','n','i',0,0,0,0,0,
    'e','t','e','x',0,0,0,0
};
extern "C" const unsigned char norrath_archive_check_file_view[] = {'l','u','m','p','.','c','c',0};
extern "C" const unsigned char norrath_archive_prefix_five_view[] = {'n','t','s','c','_',0};
extern "C" const unsigned char norrath_archive_prefix_four_view[] = {'p','a','l','_',0};
extern "C" const unsigned char norrath_archive_lookup_format_view[] = {'%','s',';','1',0};
extern "C" const unsigned char norrath_archive_directory_name_view[] = {'\\','B','G','\\','D','A','T','A',0};
extern "C" const unsigned char norrath_archive_load_failure_format_view[] = {
    '\'','%','s','\'',' ','n','o','t',' ','f','o','u','n','d','\n',0
};
extern "C" const unsigned char norrath_archive_zero_record_format_view[] = {
    '!','!','!','!','!','!','!','!','!','!',' ','%','s',' ','i','s',' ','f','u','c','k','e','d','\n',0
};
// These authored views are the original objects. cdBusy is 0x00483ffc,
// command is 0x006d86b0, cdResult is 0x00486924, and noNewVags is 0x00483f90.
#if defined(__APPLE__)
asm(
    ".globl _cdBusy\n.set _cdBusy, _norrath_archive_read_active_view\n"
    ".globl _command\n.set _command, _norrath_archive_command_view\n"
    ".globl _cdResult\n.set _cdResult, _norrath_archive_completion_pointer_view\n"
    ".globl _noNewVags\n.set _noNewVags, _norrath_archive_no_new_vags_view\n"
);
#else
asm(
    ".globl cdBusy\n.set cdBusy, norrath_archive_read_active_view\n"
    ".globl command\n.set command, norrath_archive_command_view\n"
    ".globl cdResult\n.set cdResult, norrath_archive_completion_pointer_view\n"
    ".globl noNewVags\n.set noNewVags, norrath_archive_no_new_vags_view\n"
);
#endif
