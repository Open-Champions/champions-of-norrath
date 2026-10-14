# Sound RPC views

Package notes for `agents/sound-rpc-views`. These thirty-one names are data.
This package defines no function. `soundInit__Ff`, `soundRestore__Fb`, and
`soundSleep__Fv` stay imports. None of these views is an alias of a SIF
symbol.

Addresses, sizes, and initial bytes below were read from the pinned
SLUS-20565 symbol table and file image. A zero `st_size` uses the span to
the next symbol in the same section. Host checks compare those sizes and
bytes. They are not EE plan inputs. The reviewed `authored_cpp` profile
compiled every unit. A private relocatable link of the thirty-one objects,
outside git, defined these thirty-one symbols and no others. It reported no
undefined symbol and no COMMON. `python3 -m unittest discover -s tools -p
'test_*.py'` passed, 214 tests. Nothing ran on the EE.

## Observed objects

| Published view | Original symbol | Address | Size | Initial bytes |
|---|---|---|---|---|
| `norrath_sound_rpc_client_view` | `gCd` | 0x007dd7e0 | 96 | BSS zeros. Bind and call use fields through offset 52. |
| `norrath_sound_rpc_receive_view` | stream `sbuff` | 0x007dd840 | 64 | BSS zeros. Receive length 64. |
| `norrath_sound_rpc_send_view` | `sbuffin` | 0x007dd880 | 64 | BSS zeros. Restore writes 12 bytes. |
| `norrath_sound_stream_heap_words_view` | `streamAllocation` | 0x007dd280 | 16 | BSS zeros. Sleep uses the first three words. |
| `norrath_sound_stereo_enable_word_view` | `soundStereoEnable` | 0x00485938 | 4 | `.sdata` word 1. |
| `norrath_sound_restore_formats_view` | rodata base | 0x00474640 | 163 | Four formats at offsets 0, 16, 128, and 152. |
| `norrath_sound_restore_allocation_condition_view` | sdata string | 0x00485970 | 7 | `buffer` |
| `norrath_sound_restore_slot_condition_view` | rodata string | 0x00474678 | 33 | `!streamAllocation[nmAllocations]` |
| `norrath_sound_restore_stream_count_condition_view` | rodata string | 0x004746a0 | 30 | `nmStreamsAllocated<=NMSTREAMS` |
| `norrath_sound_sequence_id_spinner_view` | `seqIdSpinner` | 0x00485940 | 2 | `.sdata` zeros. `st_size` is 2. |
| `norrath_sound_rpc_initialized_word_view` | `_sceSifInitCheck` | 0x00444968 | 4 | `.data` zero. |
| `norrath_sound_rpc_packet_buffer_view` | `_packet_buffer` | 0x008dec40 | 2048 | BSS zeros. |
| `norrath_sound_rpc_free_buffer_view` | `_free_buffer` | 0x008df440 | 2048 | BSS zeros. |
| `norrath_sound_rpc_free_buffer2_view` | `_free_buffer2` | 0x008dfc40 | 2048 | BSS zeros. |
| `norrath_sound_rpc_packet_pool_state_view` | RPC `_data_table` | 0x008e0440 | 64 | BSS zeros. Not the command table at 0x008dea18. |
| `norrath_sound_iop_heap_client_view` | `cd` | 0x008e1cc0 | 64 | BSS zeros. Offset 36 is the server word. |
| `norrath_sound_iop_heap_bind_word_view` | `_bind` | 0x00444a0c | 4 | `.data` word `0xffffffff`. |
| `norrath_sound_iop_heap_send_view` | heap `sdata` | 0x008e1d40 | 64 | BSS zeros. The call sends 4 bytes. |
| `norrath_sound_iop_heap_receive_view` | heap `rdata` | 0x008e1d00 | 64 | BSS zeros. The call receives 4 bytes. |
| `norrath_sound_remote_rpc_client_view` | `sceSd_gCd` | 0x008e5a40 | 64 | BSS zeros. Init polls offset 36. |
| `norrath_sound_remote_packet_view` | remote `sbuff` | 0x008e5a00 | 64 | BSS zeros. Not stream `sbuff`. |
| `norrath_sound_remote_bind_failure_format_view` | rodata string | 0x0047b9a8 | 35 | `sceSdRemoteInit() RPC bind error!\n` |
| `norrath_sound_remote_end_function_word_view` | `sceSdEndFunc` | 0x00446020 | 4 | `.data` zero. |
| `norrath_sound_remote_end_gp_word_view` | `sceSdEndGp` | 0x00446024 | 4 | `.data` zero. |
| `norrath_sound_remote_interrupt_functions_view` | three header words | 0x00446038 | 12 | `.data` zeros at 0, 4, and 8. |
| `norrath_sound_remote_interrupt_arguments_view` | three argument words | 0x00446044 | 12 | `.data` zeros at 0, 4, and 8. |
| `norrath_sound_remote_interrupt_gps_view` | three gp words | 0x00446050 | 12 | `.data` zeros. `_sce_sdr_cb_thid` at 0x0044605c is not included. |
| `norrath_sound_wake_top_id_word_view` | `topId` | 0x00444958 | 4 | `.data` zero. |
| `norrath_sound_wake_semaphore_word_view` | `topSema` | 0x008de0c0 | 4 | BSS zero. The unread word at 0x008de0c4 is not included. |
| `norrath_sound_wake_ring_view` | `topArg` | 0x008de0c8 | 1080 | BSS zeros. The cursor is the word at offset 4. |
| `norrath_sound_command_dma_destination_word_view` | command table + 8 | 0x008dea20 | 4 | BSS zero. See the overlap note below. |

The restore format block is one array because `soundRestore__Fb` indexes one
base. Its interior bytes include `sound.cc` and the two assert conditions so
offsets 128 and 152 stay on the later formats. Those strings also exist as
their own views. They are not aliases.

The command DMA destination word is byte 8 of the command `_data_table`
already published as `norrath_sound_command_state_view`. This package defines
a separate 4-byte object so the send path's import has a name. The two
symbols are not aliases and do not share storage.

All-zero objects that the image stores in `.data` or `.sdata` are still zero
here. The reviewed compiler places an all-zero definition in BSS.

## Imports closed

- `norrath_sound_command_dma_destination_word_view`
- `norrath_sound_iop_heap_bind_word_view`
- `norrath_sound_iop_heap_client_view`
- `norrath_sound_iop_heap_receive_view`
- `norrath_sound_iop_heap_send_view`
- `norrath_sound_remote_bind_failure_format_view`
- `norrath_sound_remote_end_function_word_view`
- `norrath_sound_remote_end_gp_word_view`
- `norrath_sound_remote_interrupt_arguments_view`
- `norrath_sound_remote_interrupt_functions_view`
- `norrath_sound_remote_interrupt_gps_view`
- `norrath_sound_remote_packet_view`
- `norrath_sound_remote_rpc_client_view`
- `norrath_sound_restore_allocation_condition_view`
- `norrath_sound_restore_formats_view`
- `norrath_sound_restore_slot_condition_view`
- `norrath_sound_restore_stream_count_condition_view`
- `norrath_sound_rpc_client_view`
- `norrath_sound_rpc_free_buffer2_view`
- `norrath_sound_rpc_free_buffer_view`
- `norrath_sound_rpc_initialized_word_view`
- `norrath_sound_rpc_packet_buffer_view`
- `norrath_sound_rpc_packet_pool_state_view`
- `norrath_sound_rpc_receive_view`
- `norrath_sound_rpc_send_view`
- `norrath_sound_sequence_id_spinner_view`
- `norrath_sound_stereo_enable_word_view`
- `norrath_sound_stream_heap_words_view`
- `norrath_sound_wake_ring_view`
- `norrath_sound_wake_semaphore_word_view`
- `norrath_sound_wake_top_id_word_view`

## Imports opened

None. These units reference no other symbol.

## Build-plan entries needed

Append these authored inputs after the current last input. On this worktree
the plan ends at `input_order` 846 and `default_authored_inputs` is 845.
The private trial used orders 847 through 877. Renumber them if the
integration tip has moved, and add 31 to `default_authored_inputs` when these
are the next authored inputs. Profile `authored_cpp`. Header closure empty.
Do not add any `host_contract.cpp`. `snapshot.json` must record every new
source and host check before `tools/check_source_snapshot.py` accepts the
tree. This branch does not edit the snapshot or the shared plan.

{ "header_closure": [], "input_order": 847, "kind": "independently_authored_source", "object_name": "rpc_client.o", "profile": "authored_cpp", "sha256": "26f8588192ea667b20f129f791b76ecc84dc710c4417c4183800be21d8716966", "source": "sources/sound-rpc-views-rpc-client/rpc_client.cpp", "emitted_functions": [], "private_object_sha256": "cc34152876f611f87472a4f615c2650fdd0cfb20b748361d314da7b27209ba0e" }
{ "header_closure": [], "input_order": 848, "kind": "independently_authored_source", "object_name": "rpc_receive.o", "profile": "authored_cpp", "sha256": "182033825be26deb011bf4f8cb81b18cd331e6ab5ccd17ad0d9229d8e9be0bde", "source": "sources/sound-rpc-views-rpc-receive/rpc_receive.cpp", "emitted_functions": [], "private_object_sha256": "357ec38f478ae4fcd74fbae75cfb3bcfa5998969baa2a8d7db7b612b8968e7fb" }
{ "header_closure": [], "input_order": 849, "kind": "independently_authored_source", "object_name": "rpc_send.o", "profile": "authored_cpp", "sha256": "17e9c4f5ca39f4159bf01f3352dd1f34f815ca8c207969e0c66758ec6b73a9f7", "source": "sources/sound-rpc-views-rpc-send/rpc_send.cpp", "emitted_functions": [], "private_object_sha256": "63d68bf6622c9d7e314e237793e2c4b1fac3e9dee18323149a84b10af0e21b2b" }
{ "header_closure": [], "input_order": 850, "kind": "independently_authored_source", "object_name": "stream_heap.o", "profile": "authored_cpp", "sha256": "6517eb970deeccb0f6b87c69451a370a4fe162579a9c9f85d76b84ea47c52f0a", "source": "sources/sound-rpc-views-stream-heap/stream_heap.cpp", "emitted_functions": [], "private_object_sha256": "aabf72ded143149d52868fe47e58f9ac33db13083cd19f08805b7ee56db7cc41" }
{ "header_closure": [], "input_order": 851, "kind": "independently_authored_source", "object_name": "stereo_enable.o", "profile": "authored_cpp", "sha256": "01507c15baeda199e6e0f5c6544f95da0f3a42abccd083c6603676f7dede7698", "source": "sources/sound-rpc-views-stereo-enable/stereo_enable.cpp", "emitted_functions": [], "private_object_sha256": "b67e436bb729fbe18b54984fe69054b36e75446ef1a64c2d98d5eba0612a6638" }
{ "header_closure": [], "input_order": 852, "kind": "independently_authored_source", "object_name": "restore_formats.o", "profile": "authored_cpp", "sha256": "0ced8fd6e1f8fcdfbab9388981468c1246ce25954fa108cf068822a017dfad8b", "source": "sources/sound-rpc-views-restore-formats/restore_formats.cpp", "emitted_functions": [], "private_object_sha256": "2070940bc88c2f8b3cc29042b6c76608f5a76ced0b01448d814c73adebb4403b" }
{ "header_closure": [], "input_order": 853, "kind": "independently_authored_source", "object_name": "restore_allocation.o", "profile": "authored_cpp", "sha256": "8000fa778a16f13bc8ee2597c507e73b9cce063da6607b80f65d2e453f077ec3", "source": "sources/sound-rpc-views-restore-allocation/restore_allocation.cpp", "emitted_functions": [], "private_object_sha256": "2aaa52370802cf584649ec358797a121983d0b74c8971f1a2ba09c51cc850d0e" }
{ "header_closure": [], "input_order": 854, "kind": "independently_authored_source", "object_name": "restore_slot.o", "profile": "authored_cpp", "sha256": "898feaf799ac21559035d2f2e81836ad83e85fd8cb0e41809e78b1f5078a13b2", "source": "sources/sound-rpc-views-restore-slot/restore_slot.cpp", "emitted_functions": [], "private_object_sha256": "b9a600d1fbace90fa4e10110ce546d7347132f431c5261db8a67b33cc2ae7777" }
{ "header_closure": [], "input_order": 855, "kind": "independently_authored_source", "object_name": "restore_stream_count.o", "profile": "authored_cpp", "sha256": "db55091e4d92846d5c2abebc074df4fefc142fb998d522cf76e27b43e8192e12", "source": "sources/sound-rpc-views-restore-stream-count/restore_stream_count.cpp", "emitted_functions": [], "private_object_sha256": "c506760bd243409395da68d91682cc97fdd3c36959d833d917bac99316f487df" }
{ "header_closure": [], "input_order": 856, "kind": "independently_authored_source", "object_name": "sequence_spinner.o", "profile": "authored_cpp", "sha256": "a2dd62ad1ea44f5aa38f5ab8adf44728a5efaeef48bc0ac06b1d559287df2a0b", "source": "sources/sound-rpc-views-sequence-spinner/sequence_spinner.cpp", "emitted_functions": [], "private_object_sha256": "277cbe040c75437b9bec666eeaa6ca96ab927160d3b3de84f48b472ab100af8b" }
{ "header_closure": [], "input_order": 857, "kind": "independently_authored_source", "object_name": "rpc_initialized.o", "profile": "authored_cpp", "sha256": "f62cb77b0ac50864e2a2ea9649a3d112ab143b4e9dba06adbfa69c49f613bd8a", "source": "sources/sound-rpc-views-rpc-initialized/rpc_initialized.cpp", "emitted_functions": [], "private_object_sha256": "71747f7dc5297f476085aa63e32562ae9f46a3c63fadbd6c8de74c152ca67831" }
{ "header_closure": [], "input_order": 858, "kind": "independently_authored_source", "object_name": "rpc_packet_buffer.o", "profile": "authored_cpp", "sha256": "f4af050b92eb579cf36739a86d18328235458f0af4c83c0addb9f53f6a047292", "source": "sources/sound-rpc-views-rpc-packet-buffer/rpc_packet_buffer.cpp", "emitted_functions": [], "private_object_sha256": "18dc62075f26c5e54f776cf80e3e4dc5838a70561e08a1b9124f61331ac272b2" }
{ "header_closure": [], "input_order": 859, "kind": "independently_authored_source", "object_name": "rpc_free_buffer.o", "profile": "authored_cpp", "sha256": "0ea6a37d1f97f96aa7ff56339103bcb5f8001955bae9e867a083162c78537750", "source": "sources/sound-rpc-views-rpc-free-buffer/rpc_free_buffer.cpp", "emitted_functions": [], "private_object_sha256": "5fce10afd2f50ca9e5a1601082b6689e5f17e8b7e3555742d8cb0ec867a47e76" }
{ "header_closure": [], "input_order": 860, "kind": "independently_authored_source", "object_name": "rpc_free_buffer2.o", "profile": "authored_cpp", "sha256": "f8dcec828fdf3b7fe407d6ea40f87ccc340a13f7ed4007b95ef8a7500a58e348", "source": "sources/sound-rpc-views-rpc-free-buffer2/rpc_free_buffer2.cpp", "emitted_functions": [], "private_object_sha256": "8e0631d8f7646b421224634965c70a5ec9bea7feb323b3c5121f52623ce91194" }
{ "header_closure": [], "input_order": 861, "kind": "independently_authored_source", "object_name": "rpc_pool_state.o", "profile": "authored_cpp", "sha256": "35e160127b7147f58779099c02adec4494e29fe558b2800f5daa2b13800826ca", "source": "sources/sound-rpc-views-rpc-pool-state/rpc_pool_state.cpp", "emitted_functions": [], "private_object_sha256": "1f6754d6bb7998483e67ce3c57030a15b2ae5d510aabb2c2c3be7237273c084b" }
{ "header_closure": [], "input_order": 862, "kind": "independently_authored_source", "object_name": "iop_heap_client.o", "profile": "authored_cpp", "sha256": "2b142de7d37132e6ccac2092d8fe876a5d6f03948e3ab0dd16403683ab7147ee", "source": "sources/sound-rpc-views-iop-heap-client/iop_heap_client.cpp", "emitted_functions": [], "private_object_sha256": "55ca2d9a75eafbff4c91e038f11946c9d767da40d2850f3691885361d76f8816" }
{ "header_closure": [], "input_order": 863, "kind": "independently_authored_source", "object_name": "iop_heap_bind.o", "profile": "authored_cpp", "sha256": "cfaa49752e9c8e68fb39d00f6b6681a767ed11b2d21501ac5624b14a234c17fb", "source": "sources/sound-rpc-views-iop-heap-bind/iop_heap_bind.cpp", "emitted_functions": [], "private_object_sha256": "822ea5c307cb134f7fe6defd78d88dec5349849bb2a1c66927238d15d89294d1" }
{ "header_closure": [], "input_order": 864, "kind": "independently_authored_source", "object_name": "iop_heap_send.o", "profile": "authored_cpp", "sha256": "836c16926195144dae6028ecf2ecdb8b010529e4d7f9781eeb8f7aea17c4d982", "source": "sources/sound-rpc-views-iop-heap-send/iop_heap_send.cpp", "emitted_functions": [], "private_object_sha256": "5a99b016a8ec11e1234efe07caaa874aeeac1fe0426fdb59d310c9f17fc452c0" }
{ "header_closure": [], "input_order": 865, "kind": "independently_authored_source", "object_name": "iop_heap_receive.o", "profile": "authored_cpp", "sha256": "c43a29c41f343308edd09ec7a8d8cf93f37b0570e754dacbda2b393433f1e6b0", "source": "sources/sound-rpc-views-iop-heap-receive/iop_heap_receive.cpp", "emitted_functions": [], "private_object_sha256": "03bbb49c1fa01d6b38b464fcf22675249dc445f4f80ca11361d1bd8db9437edb" }
{ "header_closure": [], "input_order": 866, "kind": "independently_authored_source", "object_name": "remote_client.o", "profile": "authored_cpp", "sha256": "f8252b24d04514b8eafeb994a9ae15f91d23fd27a4f218dc009c59c3cd74222b", "source": "sources/sound-rpc-views-remote-client/remote_client.cpp", "emitted_functions": [], "private_object_sha256": "8cbbdf7932d6b2a676fa1f7f8dcfe797da0b8b24404fe7562a40eaec2349340f" }
{ "header_closure": [], "input_order": 867, "kind": "independently_authored_source", "object_name": "remote_packet.o", "profile": "authored_cpp", "sha256": "baccaf0212e6531848678b329af4c2190d113147ebf1e5b8c9b9783e78f3e7be", "source": "sources/sound-rpc-views-remote-packet/remote_packet.cpp", "emitted_functions": [], "private_object_sha256": "33a290443e5e0ca52e5165738e342d24a44cbce8e19183986b1b6dfdf02d5079" }
{ "header_closure": [], "input_order": 868, "kind": "independently_authored_source", "object_name": "remote_bind_failure.o", "profile": "authored_cpp", "sha256": "9cad9b4b2926c150a8cd5d67dc601d4b223f2d036c4e1f718c8411efba9b7d28", "source": "sources/sound-rpc-views-remote-bind-failure/remote_bind_failure.cpp", "emitted_functions": [], "private_object_sha256": "c7b04e98a4dce08c3632d270422eddd7775bde39bb11f2d5aa98ffbc7db2f1a1" }
{ "header_closure": [], "input_order": 869, "kind": "independently_authored_source", "object_name": "remote_end_function.o", "profile": "authored_cpp", "sha256": "6f7ceb7aeb8df1816eaa0048998c3af56397a310117e8fad2c7a1a7a0d1bf0ef", "source": "sources/sound-rpc-views-remote-end-function/remote_end_function.cpp", "emitted_functions": [], "private_object_sha256": "e06aab9572df0dad2bc59df7df29a63e2376c6018ffe3d159f6f29f393f46346" }
{ "header_closure": [], "input_order": 870, "kind": "independently_authored_source", "object_name": "remote_end_gp.o", "profile": "authored_cpp", "sha256": "d2f4905bafdc98bdf027239ee2fdba00969cb8895390772102d6a8f89909ae62", "source": "sources/sound-rpc-views-remote-end-gp/remote_end_gp.cpp", "emitted_functions": [], "private_object_sha256": "881e9aa65411027452bdd20fa28260046fb7b3c4761d049359984a351f7f393f" }
{ "header_closure": [], "input_order": 871, "kind": "independently_authored_source", "object_name": "remote_interrupt_functions.o", "profile": "authored_cpp", "sha256": "a7383a4f591444d28941e5ef0321f1e67489c76a4677b5c55d6fb81b0ab8d3ec", "source": "sources/sound-rpc-views-remote-interrupt-functions/remote_interrupt_functions.cpp", "emitted_functions": [], "private_object_sha256": "b7e5bd298fb9250dac293663947d5a4215e5f50188730beeb650100c5a1e1018" }
{ "header_closure": [], "input_order": 872, "kind": "independently_authored_source", "object_name": "remote_interrupt_arguments.o", "profile": "authored_cpp", "sha256": "69f928cde7503ff328124031ec89a2c50043d1a73cd2dbf0cff457c9e5ca24a5", "source": "sources/sound-rpc-views-remote-interrupt-arguments/remote_interrupt_arguments.cpp", "emitted_functions": [], "private_object_sha256": "779ae40612c6f88aaef413d539cd8a8d25234f1fb35bc4b1ef61548d9d45f22e" }
{ "header_closure": [], "input_order": 873, "kind": "independently_authored_source", "object_name": "remote_interrupt_gps.o", "profile": "authored_cpp", "sha256": "41218617e7ff8e0f5397f42c8b7adfb0e69e8803f94dbe10e6f326046c5ac0cc", "source": "sources/sound-rpc-views-remote-interrupt-gps/remote_interrupt_gps.cpp", "emitted_functions": [], "private_object_sha256": "1ac10f87430abad042e0b809f47f946e42742a6ffea6f6a3261f47454495c81c" }
{ "header_closure": [], "input_order": 874, "kind": "independently_authored_source", "object_name": "wake_top_id.o", "profile": "authored_cpp", "sha256": "1923ed3d5fee31f3305499ded3c6d6025ededd14dce02b4e8e3647535d907dd9", "source": "sources/sound-rpc-views-wake-top-id/wake_top_id.cpp", "emitted_functions": [], "private_object_sha256": "d99235a18a91e9955bfb3734929855b69ff798cd08b0de086541aa9e9bbad204" }
{ "header_closure": [], "input_order": 875, "kind": "independently_authored_source", "object_name": "wake_semaphore.o", "profile": "authored_cpp", "sha256": "22fa3ff30d44c73801fc8e1ef573875ba91c54eda3585ca080144cb28b83dff6", "source": "sources/sound-rpc-views-wake-semaphore/wake_semaphore.cpp", "emitted_functions": [], "private_object_sha256": "ebd87e428dc4009c1dbd78bc638ee3e1e1060b75ce13b3829ad36592117a7d69" }
{ "header_closure": [], "input_order": 876, "kind": "independently_authored_source", "object_name": "wake_ring.o", "profile": "authored_cpp", "sha256": "fdb675ddab0cfdd6a345b9a4b2d11e4f99d07173092e6588cca2c9f2c4590ae6", "source": "sources/sound-rpc-views-wake-ring/wake_ring.cpp", "emitted_functions": [], "private_object_sha256": "4ea6e9648382ac7ab95325d7a61750cc84530b3a99147cfc37f813fa1a4709cc" }
{ "header_closure": [], "input_order": 877, "kind": "independently_authored_source", "object_name": "command_dma_destination.o", "profile": "authored_cpp", "sha256": "d073367397fbfad661ea0a99261f0b0c53f6aabe215f84737348b99fcb94abc9", "source": "sources/sound-rpc-views-command-dma-destination/command_dma_destination.cpp", "emitted_functions": [], "private_object_sha256": "6aaa792ba204984ba23f3509b86881eb31778878861b3ea6fc4f02425bc97fbe" }

## Authored but never executed

All thirty-one objects. Host checks passed on this Mac, and the reviewed EE
compiler produced the relocatable objects above. The private link checked
symbol closure only. No object ran on the EE, under the original process, or
as gameplay.

## Unknown

Bytes past the caller-proven prefix remain part of the symbol span and were
not given another name: RPC client bytes 56 through 95, RPC pool-state bytes
40 through 63, IOP heap send and receive bytes 4 through 63, and wake-ring
bytes 1031 through 1079. The unread word between `topSema` and `topArg` is
not owned. The 2048-byte packet and free buffers match a count of 32 stored
beside a 64-byte slot, which does not by itself prove an element type.
`sceSifInitRpc` writes the first 40 bytes of the pool state. The command DMA
word is a separate published object from byte 8 of the existing command state
view, so a store through one does not update the other. All-zero `.data` and
`.sdata` words are BSS in this compiler's output. Hardware, interrupts, DMA,
IOP heap, SPU, and gameplay remain unverified.
