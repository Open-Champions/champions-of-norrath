/* Observed channelInfo at 0x007d8f20. soundStopSound at 0x0024cf80 scans
 * 43 records of 20 bytes (slti bound 43, stride 20). The next symbol,
 * soundList, begins 864 bytes later, so 4 bytes after this array are not
 * part of the 43 records. Loader zeros.
 */
extern "C" {
alignas(32) unsigned char norrath_sound_channel_records_view[860];
}
