# Skill ramp initialisation, part 1 (w13-skill-ramp-a)

Branch `agents/w13-skill-ramp-a`. Addresses and sizes are from the pinned private executable `SLUS-20565-sha256-9a60f13c5c31`. Nothing here ran on the EE or in the game. No game binary, disassembly or extracted data is included; the float values below are bit patterns read from the image and restated as literals.

## Observed

Each of the 29 functions is straight-line code: it takes the receiver in the first argument register and stores halfwords and single floats at fixed byte offsets, then returns. There are no calls, no loads from the receiver and no branches. The halfword stores use offsets 10-70 and the float stores use offsets 36-80; the base class initializer (`initRamps__5Skill`, w12-skill-c) writes the same offset families (+14.., +24.., +36..). A few float fields are stored with the integer register `sw zero`. The stored values are not round decimals (for example 6.9998956, 5.999881, 2.000002, 0.099999994 in Shock), so they are kept as exact hexadecimal float literals with the decimal in a comment. A decimal literal rounds differently under the reviewed compiler (it came out one ulp low in the first attempt), which is why the paired run mattered.

## Evidence per function

For each function: the authored source (`sources/<dir>/init_ramps.cpp`), a host contract (`host_check.cpp`, not a plan input) that fills a guarded buffer with 0xa5 and checks every byte against an independent raw bit-pattern table, and a paired run. The paired run executes the image's instructions and the freshly linked authored object in a small R5900 interpreter on four receiver fills (zeros and three random fills) and requires identical changed bytes. Zero mismatches in 29 functions x 4 fills, no interpreter halts, and every original word of every function was executed. The host contracts were rebuilt with `-Wall -Wextra -Werror`. `python3 -m unittest discover -s tools -p 'test_*.py'` passes (214 tests). The first paired run, with decimal literals, produced 92 mismatches (low float byte off by one), so the comparison does detect a wrong constant. The compiled sizes are the reviewed EE g++ `authored_cpp` objects and are not a byte-equality claim; they are larger than the originals because the compiler loads float constants differently.

| Function | Original address / bytes | Compiled bytes | Stores | Source directory |
|---|---:|---:|---:|---|
| `initRamps__10SkillDodge` | `0x002ae6e8` / 48 | 52 | 6 | `sources/w13-skill-ramp-a-init-ramps-skill-dodge` |
| `initRamps__10SkillShock` | `0x0028d840` / 108 | 128 | 15 | `sources/w13-skill-ramp-a-init-ramps-skill-shock` |
| `initRamps__11SkillCharge` | `0x00291fc8` / 112 | 132 | 15 | `sources/w13-skill-ramp-a-init-ramps-skill-charge` |
| `initRamps__12SkillArchery` | `0x002a2b68` / 48 | 52 | 6 | `sources/w13-skill-ramp-a-init-ramps-skill-archery` |
| `initRamps__12SkillBlessed` | `0x002a6e18` / 84 | 88 | 12 | `sources/w13-skill-ramp-a-init-ramps-skill-blessed` |
| `initRamps__12SkillCyclone` | `0x0029a7c8` / 84 | 100 | 12 | `sources/w13-skill-ramp-a-init-ramps-skill-cyclone` |
| `initRamps__12SkillHealing` | `0x002af238` / 84 | 100 | 12 | `sources/w13-skill-ramp-a-init-ramps-skill-healing` |
| `initRamps__12SkillLifeTap` | `0x002be1e8` / 48 | 56 | 6 | `sources/w13-skill-ramp-a-init-ramps-skill-life-tap` |
| `initRamps__12SkillRiposte` | `0x002a5088` / 84 | 88 | 12 | `sources/w13-skill-ramp-a-init-ramps-skill-riposte` |
| `initRamps__13SkillDarkness` | `0x00298180` / 100 | 120 | 15 | `sources/w13-skill-ramp-a-init-ramps-skill-darkness` |
| `initRamps__13SkillEntangle` | `0x002c6e00` / 80 | 92 | 12 | `sources/w13-skill-ramp-a-init-ramps-skill-entangle` |
| `initRamps__14SkillColdArrow` | `0x0029fb70` / 132 | 152 | 18 | `sources/w13-skill-ramp-a-init-ramps-skill-cold-arrow` |
| `initRamps__14SkillFireArrow` | `0x0029e2f8` / 112 | 128 | 15 | `sources/w13-skill-ramp-a-init-ramps-skill-fire-arrow` |
| `initRamps__14SkillFireStorm` | `0x0029d1c0` / 112 | 120 | 18 | `sources/w13-skill-ramp-a-init-ramps-skill-fire-storm` |
| `initRamps__14SkillHarmTouch` | `0x0029aed8` / 100 | 120 | 15 | `sources/w13-skill-ramp-a-init-ramps-skill-harm-touch` |
| `initRamps__14SkillMultiFire` | `0x002a1be0` / 148 | 172 | 21 | `sources/w13-skill-ramp-a-init-ramps-skill-multi-fire` |
| `initRamps__15SkillClericRoot` | `0x002c6188` / 76 | 84 | 12 | `sources/w13-skill-ramp-a-init-ramps-skill-cleric-root` |
| `initRamps__15SkillConeOfFire` | `0x002b6a10` / 92 | 100 | 12 | `sources/w13-skill-ramp-a-init-ramps-skill-cone-of-fire` |
| `initRamps__15SkillFireFlares` | `0x002a4888` / 88 | 104 | 12 | `sources/w13-skill-ramp-a-init-ramps-skill-fire-flares` |
| `initRamps__15SkillFrostStorm` | `0x002b3510` / 180 | 208 | 27 | `sources/w13-skill-ramp-a-init-ramps-skill-frost-storm` |
| `initRamps__15SkillHolyShield` | `0x002ae470` / 68 | 80 | 9 | `sources/w13-skill-ramp-a-init-ramps-skill-holy-shield` |
| `initRamps__15SkillHolyStrike` | `0x002b2d08` / 72 | 80 | 9 | `sources/w13-skill-ramp-a-init-ramps-skill-holy-strike` |
| `initRamps__15SkillPoisonRain` | `0x002bf1c8` / 104 | 112 | 18 | `sources/w13-skill-ramp-a-init-ramps-skill-poison-rain` |
| `initRamps__15SkillShieldBash` | `0x002c89d8` / 104 | 124 | 15 | `sources/w13-skill-ramp-a-init-ramps-skill-shield-bash` |
| `initRamps__15SkillWizardBeam` | `0x002b9398` / 88 | 96 | 12 | `sources/w13-skill-ramp-a-init-ramps-skill-wizard-beam` |
| `initRamps__16SkillColdWeapons` | `0x002ac360` / 116 | 124 | 15 | `sources/w13-skill-ramp-a-init-ramps-skill-cold-weapons` |
| `initRamps__16SkillConeOfFrost` | `0x002afcd0` / 84 | 100 | 12 | `sources/w13-skill-ramp-a-init-ramps-skill-cone-of-frost` |
| `initRamps__16SkillCriticalHit` | `0x002adba0` / 88 | 104 | 12 | `sources/w13-skill-ramp-a-init-ramps-skill-critical-hit` |
| `initRamps__16SkillDiseaseBolt` | `0x00293768` / 88 | 100 | 12 | `sources/w13-skill-ramp-a-init-ramps-skill-disease-bolt` |

## Imports closed

- `initRamps__10SkillDodge`
- `initRamps__10SkillShock`
- `initRamps__11SkillCharge`
- `initRamps__12SkillArchery`
- `initRamps__12SkillBlessed`
- `initRamps__12SkillCyclone`
- `initRamps__12SkillHealing`
- `initRamps__12SkillLifeTap`
- `initRamps__12SkillRiposte`
- `initRamps__13SkillDarkness`
- `initRamps__13SkillEntangle`
- `initRamps__14SkillColdArrow`
- `initRamps__14SkillFireArrow`
- `initRamps__14SkillFireStorm`
- `initRamps__14SkillHarmTouch`
- `initRamps__14SkillMultiFire`
- `initRamps__15SkillClericRoot`
- `initRamps__15SkillConeOfFire`
- `initRamps__15SkillFireFlares`
- `initRamps__15SkillFrostStorm`
- `initRamps__15SkillHolyShield`
- `initRamps__15SkillHolyStrike`
- `initRamps__15SkillPoisonRain`
- `initRamps__15SkillShieldBash`
- `initRamps__15SkillWizardBeam`
- `initRamps__16SkillColdWeapons`
- `initRamps__16SkillConeOfFrost`
- `initRamps__16SkillCriticalHit`
- `initRamps__16SkillDiseaseBolt`

(29 names. They are defined but not yet selected by the shared plan, so no post-integration count is claimed.)

## Imports opened

None. Every object has an empty undefined-symbol list.

## Build-plan entries needed

Profile `authored_cpp`, empty `header_closure`. Do not add `host_check.cpp`. The integrator assigns `input_order` and adds the source hashes to the `files` map.

```json
{"emitted_functions": [{"compiled_body_bytes": 52, "name": "initRamps__10SkillDodge"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "init_ramps.o", "profile": "authored_cpp", "sha256": "dc7087ce1b1b0710ae7b1c1e2df0cedf99b2e673885efb4a339655e1d93d6c3b", "source": "sources/w13-skill-ramp-a-init-ramps-skill-dodge/init_ramps.cpp"},
{"emitted_functions": [{"compiled_body_bytes": 128, "name": "initRamps__10SkillShock"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "init_ramps.o", "profile": "authored_cpp", "sha256": "034be5abdce72f579785edea69ab5f720252c771e8537786a734d41d77a53c5a", "source": "sources/w13-skill-ramp-a-init-ramps-skill-shock/init_ramps.cpp"},
{"emitted_functions": [{"compiled_body_bytes": 132, "name": "initRamps__11SkillCharge"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "init_ramps.o", "profile": "authored_cpp", "sha256": "484a86b5d2695411054ee4eff4ed4516c0b3ae3a796e7ca15de03203ff5ea3f1", "source": "sources/w13-skill-ramp-a-init-ramps-skill-charge/init_ramps.cpp"},
{"emitted_functions": [{"compiled_body_bytes": 52, "name": "initRamps__12SkillArchery"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "init_ramps.o", "profile": "authored_cpp", "sha256": "6fed43e046c1f075dfd185dd0dec3d3a90cb14b6d6a81c04a34afd00d6d2fb98", "source": "sources/w13-skill-ramp-a-init-ramps-skill-archery/init_ramps.cpp"},
{"emitted_functions": [{"compiled_body_bytes": 88, "name": "initRamps__12SkillBlessed"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "init_ramps.o", "profile": "authored_cpp", "sha256": "6fb5a0cb0ceb878e0a8fe5eda3ded3ed74ec252753d9ad6944cbc7a9172611ca", "source": "sources/w13-skill-ramp-a-init-ramps-skill-blessed/init_ramps.cpp"},
{"emitted_functions": [{"compiled_body_bytes": 100, "name": "initRamps__12SkillCyclone"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "init_ramps.o", "profile": "authored_cpp", "sha256": "c9664d6cea260a0856620a516ac69b9422f4d2736c8546831e446b8e1b60b723", "source": "sources/w13-skill-ramp-a-init-ramps-skill-cyclone/init_ramps.cpp"},
{"emitted_functions": [{"compiled_body_bytes": 100, "name": "initRamps__12SkillHealing"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "init_ramps.o", "profile": "authored_cpp", "sha256": "40c745586aa22e3be8f75d3d235fc331e4fd0fa7cd81ae6cd5e108a90fe5e48a", "source": "sources/w13-skill-ramp-a-init-ramps-skill-healing/init_ramps.cpp"},
{"emitted_functions": [{"compiled_body_bytes": 56, "name": "initRamps__12SkillLifeTap"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "init_ramps.o", "profile": "authored_cpp", "sha256": "5d342cf8aa0a423daa0783d739c0ec52007940f1531e47b3f74227a267f37948", "source": "sources/w13-skill-ramp-a-init-ramps-skill-life-tap/init_ramps.cpp"},
{"emitted_functions": [{"compiled_body_bytes": 88, "name": "initRamps__12SkillRiposte"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "init_ramps.o", "profile": "authored_cpp", "sha256": "c69d1b4cfa6c429b8e2e4d300964b488fb80aaa1250f8da5291054deb2647e14", "source": "sources/w13-skill-ramp-a-init-ramps-skill-riposte/init_ramps.cpp"},
{"emitted_functions": [{"compiled_body_bytes": 120, "name": "initRamps__13SkillDarkness"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "init_ramps.o", "profile": "authored_cpp", "sha256": "bf154d2ff1bf14a4092b8d01037130d3af3c135f1e267334a1a489cade073a07", "source": "sources/w13-skill-ramp-a-init-ramps-skill-darkness/init_ramps.cpp"},
{"emitted_functions": [{"compiled_body_bytes": 92, "name": "initRamps__13SkillEntangle"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "init_ramps.o", "profile": "authored_cpp", "sha256": "a6cafc1fc14e67bf49ffda0fc9528a30c34c4518a7c6a029b5ce87656f8760d7", "source": "sources/w13-skill-ramp-a-init-ramps-skill-entangle/init_ramps.cpp"},
{"emitted_functions": [{"compiled_body_bytes": 152, "name": "initRamps__14SkillColdArrow"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "init_ramps.o", "profile": "authored_cpp", "sha256": "0408a4f6a7a0b49777be751336c3d23e572ee0156c86359ad30e1254d8efcdb7", "source": "sources/w13-skill-ramp-a-init-ramps-skill-cold-arrow/init_ramps.cpp"},
{"emitted_functions": [{"compiled_body_bytes": 128, "name": "initRamps__14SkillFireArrow"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "init_ramps.o", "profile": "authored_cpp", "sha256": "d2bb79f5be2d644da3921d4b209fa3fac8831ad3db7b8615aab67f2a57180a24", "source": "sources/w13-skill-ramp-a-init-ramps-skill-fire-arrow/init_ramps.cpp"},
{"emitted_functions": [{"compiled_body_bytes": 120, "name": "initRamps__14SkillFireStorm"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "init_ramps.o", "profile": "authored_cpp", "sha256": "61f6859f94df7635a552c3f202cb9f655510050f08863da5eca064525ff9be2e", "source": "sources/w13-skill-ramp-a-init-ramps-skill-fire-storm/init_ramps.cpp"},
{"emitted_functions": [{"compiled_body_bytes": 120, "name": "initRamps__14SkillHarmTouch"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "init_ramps.o", "profile": "authored_cpp", "sha256": "aec22561f018297304c4b978799df9baeacf10e4e933b4d9ce8e9c1c912fe0a9", "source": "sources/w13-skill-ramp-a-init-ramps-skill-harm-touch/init_ramps.cpp"},
{"emitted_functions": [{"compiled_body_bytes": 172, "name": "initRamps__14SkillMultiFire"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "init_ramps.o", "profile": "authored_cpp", "sha256": "59786379b86e62ed072b7c26f4c8bf9eae63df6add40f636ff5cc44dbfbdbb49", "source": "sources/w13-skill-ramp-a-init-ramps-skill-multi-fire/init_ramps.cpp"},
{"emitted_functions": [{"compiled_body_bytes": 84, "name": "initRamps__15SkillClericRoot"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "init_ramps.o", "profile": "authored_cpp", "sha256": "32c06411b81f4cdc1e6aca221ff0fd024e868a700eb2d12a50662871d444a79b", "source": "sources/w13-skill-ramp-a-init-ramps-skill-cleric-root/init_ramps.cpp"},
{"emitted_functions": [{"compiled_body_bytes": 100, "name": "initRamps__15SkillConeOfFire"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "init_ramps.o", "profile": "authored_cpp", "sha256": "7f285f89b95999ea0451a5c029f4439762cbf99943fd77fbab5d23821a465cae", "source": "sources/w13-skill-ramp-a-init-ramps-skill-cone-of-fire/init_ramps.cpp"},
{"emitted_functions": [{"compiled_body_bytes": 104, "name": "initRamps__15SkillFireFlares"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "init_ramps.o", "profile": "authored_cpp", "sha256": "5e6120f0f89ea530afe52e00f0797f07b6de180f7582ecb2f947bab1c2adc419", "source": "sources/w13-skill-ramp-a-init-ramps-skill-fire-flares/init_ramps.cpp"},
{"emitted_functions": [{"compiled_body_bytes": 208, "name": "initRamps__15SkillFrostStorm"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "init_ramps.o", "profile": "authored_cpp", "sha256": "63d02a787670d59a9f3457e0917928047315a69c090a43becd4d75f39ab36e78", "source": "sources/w13-skill-ramp-a-init-ramps-skill-frost-storm/init_ramps.cpp"},
{"emitted_functions": [{"compiled_body_bytes": 80, "name": "initRamps__15SkillHolyShield"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "init_ramps.o", "profile": "authored_cpp", "sha256": "df02268c020f706974f16e535b55400c276c3a6c11b988e1bc3f07bd43f88c3a", "source": "sources/w13-skill-ramp-a-init-ramps-skill-holy-shield/init_ramps.cpp"},
{"emitted_functions": [{"compiled_body_bytes": 80, "name": "initRamps__15SkillHolyStrike"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "init_ramps.o", "profile": "authored_cpp", "sha256": "0658de6e87e6c3a2f70fff0cc3cf4f95f0eed6a566aac37558eea24ccb05eb72", "source": "sources/w13-skill-ramp-a-init-ramps-skill-holy-strike/init_ramps.cpp"},
{"emitted_functions": [{"compiled_body_bytes": 112, "name": "initRamps__15SkillPoisonRain"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "init_ramps.o", "profile": "authored_cpp", "sha256": "fca03eb1741ecd935626724e489343ff6801c966aad76be80bd7a34c5807f2e3", "source": "sources/w13-skill-ramp-a-init-ramps-skill-poison-rain/init_ramps.cpp"},
{"emitted_functions": [{"compiled_body_bytes": 124, "name": "initRamps__15SkillShieldBash"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "init_ramps.o", "profile": "authored_cpp", "sha256": "63de6e59ad666b50c840d98204510d7f96e3604fecece55fc6d1f61a47010948", "source": "sources/w13-skill-ramp-a-init-ramps-skill-shield-bash/init_ramps.cpp"},
{"emitted_functions": [{"compiled_body_bytes": 96, "name": "initRamps__15SkillWizardBeam"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "init_ramps.o", "profile": "authored_cpp", "sha256": "cf7355505e3f2c8c340744bdcf5148139be7de301c55f8e5d041e9ac9d7fef71", "source": "sources/w13-skill-ramp-a-init-ramps-skill-wizard-beam/init_ramps.cpp"},
{"emitted_functions": [{"compiled_body_bytes": 124, "name": "initRamps__16SkillColdWeapons"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "init_ramps.o", "profile": "authored_cpp", "sha256": "e695a05faf3bc52df3936ab804588998666b6271b3d9a112172102203417f5b5", "source": "sources/w13-skill-ramp-a-init-ramps-skill-cold-weapons/init_ramps.cpp"},
{"emitted_functions": [{"compiled_body_bytes": 100, "name": "initRamps__16SkillConeOfFrost"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "init_ramps.o", "profile": "authored_cpp", "sha256": "529da2429b44fb9e6e926e43548a05559a6e82ef1adec01356635b74d7fad0d5", "source": "sources/w13-skill-ramp-a-init-ramps-skill-cone-of-frost/init_ramps.cpp"},
{"emitted_functions": [{"compiled_body_bytes": 104, "name": "initRamps__16SkillCriticalHit"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "init_ramps.o", "profile": "authored_cpp", "sha256": "e4ff725aef43a19d971ab14277dcf5613158a0b9207406835ec0f1240f705069", "source": "sources/w13-skill-ramp-a-init-ramps-skill-critical-hit/init_ramps.cpp"},
{"emitted_functions": [{"compiled_body_bytes": 100, "name": "initRamps__16SkillDiseaseBolt"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "init_ramps.o", "profile": "authored_cpp", "sha256": "f6346e9fcbbc74584e7cb3fd564634ebee35f09f531608225e4480494863dac6", "source": "sources/w13-skill-ramp-a-init-ramps-skill-disease-bolt/init_ramps.cpp"}
```

## Authored but never executed

All 29 bodies on real hardware or in the game. They ran only inside the comparison interpreter and as host-contract programs.

## Unknown

- Which fields the offsets are (rank tables, costs, durations or others), and the class layout. No member names are proposed.
- Whether any field the initializers do not write matters to callers that read the ramps (the cast and damage code, and the per-class skill start-up) before evaluation.
- Why the float values sit slightly off round numbers (computed in the original source, or tuned). Only the bit patterns are known.
- Ramp evaluation (part 2 of the package) is not in this branch.
