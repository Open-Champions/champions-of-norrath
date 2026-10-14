# Lava-wraith constructor name tags (c)

Package `w9-ctors-wraith-c`, branch `agents/w9-ctors-wraith-c`. Private reference:
`work/private/SLUS_205.65`; addresses are that image's. Nothing from it is committed.

## Evidence

- Observed: all 38 addresses lie in `.rodata` (`0x454300`..), have no ELF symbol, and each holds
  a short printable NUL-terminated ASCII string (orc tags, `showName`, `lumpname`, `change%d`,
  mummy and skeleton variant tags, wood-elf tags). Each view is the characters plus the NUL.
- Check performed per unit: the source compiled with the reviewed EE `authored_cpp` flags, the
  emitted bytes were compared with the original string bytes including the NUL, and a host test
  (`host/`, not a plan input) compares the object with the expected text.
- Inferred: that the callers (`ctors-lava-wraith-detail`, `-mummy`, `-wood-elf-soldier`) pass
  these addresses as name text. Unknown: which strings the original pool shares between
  neighbours, pointer identity with other copies, and the meaning of the short mummy tags.
- `change%d` is a format string; its use is not verified here.

## Imports closed

All 38 `norrath_ctors_lava_wraith_view_*` names in the scope: `471968 471978 471988 471998
472040 472050 472060 472c10 472c28 472c38 472c50 472c60 472c78 472c88 472ca0 472cb0 472cc8
473f20 473f30 473f40 473f58 473f70 473f88 473fa0 473fb0 473fc0 473fd8 473ff0 474000 474010
474028 474040 474050 474060 474540 474550 474560 474570`.

## Imports opened

None. The units reference nothing.

## Build-plan entries needed

One entry per row: kind `independently_authored_source`, profile `authored_cpp`, empty
`header_closure`, no emitted functions, object name `<name>.o`, source
`sources/w9-ctors-wraith-c-views/<name>.cpp`, `input_order` by the integrator; the same sha in
`files`.

| Unit | sha256 |
|---|---|
| `norrath_ctors_lava_wraith_view_471968` | `5af72966b635bfe86ad09b76c508604d4bd2087ab332cda4ac0f8359e7991142` |
| `norrath_ctors_lava_wraith_view_471978` | `9b204b8c3f61e462b3a8108aae2a26f754b2602f257cb1b04674387dacbc1a60` |
| `norrath_ctors_lava_wraith_view_471988` | `1cb2486bb4fd5b03ed7b7325da9104d7882b94e37ba3876f143c37bce2289d7c` |
| `norrath_ctors_lava_wraith_view_471998` | `9b2e620b29e1e5ca0b612aa412b9431a5ef10527393a70a0006095886fb6969d` |
| `norrath_ctors_lava_wraith_view_472040` | `4f9e7dfdd4a1bae825fafadd55582f097abf6331347141b751b5f1f98a1b639a` |
| `norrath_ctors_lava_wraith_view_472050` | `2aea6a7ae8a55f94f76af615638452c76f82efb85eaf185989d599986c4fca69` |
| `norrath_ctors_lava_wraith_view_472060` | `a0036ce8ddcaff7faa682afdf4fd16868a2b4bce4948f740c325bbfb3d87c67f` |
| `norrath_ctors_lava_wraith_view_472c10` | `59babafeb17e0abdc98b3233aca2685c6c4908b06022ec880fa91580f7176ca4` |
| `norrath_ctors_lava_wraith_view_472c28` | `957562d9c702f06c84e4cdc7791c2479cf7ed2b4c4c0faf3d4630ab8ba3d8f7d` |
| `norrath_ctors_lava_wraith_view_472c38` | `8c0bd867179059f2e5d68226526ade349c3d57e2efab7dd7ab3b98d2ad5efe80` |
| `norrath_ctors_lava_wraith_view_472c50` | `ca5b0167046ed224c585b003786a1459690f93c4075c68430f967e5907987693` |
| `norrath_ctors_lava_wraith_view_472c60` | `725b6f7a3184485c34e8feb2af7354b4f0d1df5eef24b20e0594bdaa53c2e9a6` |
| `norrath_ctors_lava_wraith_view_472c78` | `527c364016170a5de94db7c8688ff83584253fdece0e0846e458bcf9b8d8b15f` |
| `norrath_ctors_lava_wraith_view_472c88` | `4064b2d4bcd25e571b668f6e1fc601a45e7a5bce61d97f22b2204ef4806236e4` |
| `norrath_ctors_lava_wraith_view_472ca0` | `242244b1d0b2aede42462f7bcc3c1558c58d17cb2ad82c1c5517589ed335542a` |
| `norrath_ctors_lava_wraith_view_472cb0` | `27e055dac58029ac202d894225a10694f678d2b304da2d60b8d4f439000ce4c4` |
| `norrath_ctors_lava_wraith_view_472cc8` | `671ec4b3b4f6a367d2a2a652928ee8416b2b6a88e64a5cb203644ddf47a03396` |
| `norrath_ctors_lava_wraith_view_473f20` | `6e722fe8d4fdbf647af2c151a3477184b2695604b19d51ea0dcdc9ac5642843f` |
| `norrath_ctors_lava_wraith_view_473f30` | `d5177a7cce21888efd1e0099dd0560f66934deac54ba8c4484e8b2795078e458` |
| `norrath_ctors_lava_wraith_view_473f40` | `a2744ba5e23deb00c8e795bd8554468bda9763eef98c061d3f177da831f17cdd` |
| `norrath_ctors_lava_wraith_view_473f58` | `6be19eaac1561ad58c739dfe5ad9ce9f651c39408b3d28d98247d9c63295d579` |
| `norrath_ctors_lava_wraith_view_473f70` | `60f1ceb458f5d4bfcba3762c535261779a973021b6aa6bf56edcf1235fad2b58` |
| `norrath_ctors_lava_wraith_view_473f88` | `d921e2e11f9aa78dbf39df453991d8c93706ec3664e55029f627e2d9b70d354d` |
| `norrath_ctors_lava_wraith_view_473fa0` | `f46d5166179a4f369afc0ad9fed106a36e6b4f4b03c24a5a1e5ab785759b4425` |
| `norrath_ctors_lava_wraith_view_473fb0` | `ecdcefeb1920b05e7cf80bd36900cec3e64728e94817d6f732c98f4192001be2` |
| `norrath_ctors_lava_wraith_view_473fc0` | `6fbae33f1c18d5a9d347805847276728c8ed0fb7ed1bb92da269364b6ee3157b` |
| `norrath_ctors_lava_wraith_view_473fd8` | `72af29c09914771a0c1d69335b729e0642058a929e82bc2bbc93a6ef54d63962` |
| `norrath_ctors_lava_wraith_view_473ff0` | `930a73fa14e4431d3a2769f135a7fd958e44f8e7a38fe1d74a6cd41923292738` |
| `norrath_ctors_lava_wraith_view_474000` | `606aab013901ed3f088169b89b7abab9eab59642732ec636bf0e30cd31952921` |
| `norrath_ctors_lava_wraith_view_474010` | `427c7a5828db9de0c053f25494286deecca080121fa8431a5c631a15401e67e5` |
| `norrath_ctors_lava_wraith_view_474028` | `0cf47720372e1d41d2968d046958c6cbca5b098c9cc624fca41a5926f54c1673` |
| `norrath_ctors_lava_wraith_view_474040` | `22cb18ac0d4046d5ca5a04792af1ae1fac08694650a1b55d8c58795978f95e6f` |
| `norrath_ctors_lava_wraith_view_474050` | `86c7d635453f5c99c768de95d6086781471dac25dcf4009fc152cef022026133` |
| `norrath_ctors_lava_wraith_view_474060` | `4ae5fb20a748a09d89ae27b239da0f16af456ac12f9c0b532760913a85b44432` |
| `norrath_ctors_lava_wraith_view_474540` | `51a2b743b395db806202e7323a474c709f9b061a46baabfdc10d514f56c8561e` |
| `norrath_ctors_lava_wraith_view_474550` | `a8307b6688fa8afa5251513a16c7e668a932880871992f59c2cd29c27217e413` |
| `norrath_ctors_lava_wraith_view_474560` | `f154d236712a75a124f7af17724c1fab0fbed111b1982d49a3c94539f29294b1` |
| `norrath_ctors_lava_wraith_view_474570` | `67c90cf394d9aa6bd62d5a94919175b804dd0c9df18b8369358a30172169f9e5` |

## Authored but never executed

All of it. Host tests and EE-profile compiles only; no partial link was run on this branch
(the full plan was not rebuilt), and nothing ran on the EE or as gameplay.

## Unknown

Dispatch of the callers on these strings, string-pool sharing in the original, and the
meaning of abbreviations such as `mummytso`.
