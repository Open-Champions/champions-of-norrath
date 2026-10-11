"""Host execution of the animation sound-name mapper, plus the EE compile."""

import subprocess
import unittest
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
SOURCE = ROOT / "experimental/sources/save-net-anim-map-sounds/map_sounds.cpp"
DRIVER = Path(__file__).with_suffix(".cpp")
EE = Path(
    "/Users/syndicatemike/Documents/Codex/projects/open-champions/"
    "champions-of-norrath/work/toolchains/ee/bin/mips64r5900el-ps2-elf-g++"
)
EE_FLAGS = [
    "-G0", "-O2", "-ffreestanding", "-fno-asynchronous-unwind-tables",
    "-fno-exceptions", "-fno-pic", "-fno-rtti", "-fno-stack-protector",
    "-fno-unwind-tables", "-fomit-frame-pointer", "-fwrapv", "-mabi=eabi",
    "-march=r5900", "-mfp32", "-mgp64", "-mlong32", "-mno-abicalls",
    "-msingle-float", "-std=c++17", "-nostdinc", "-nostdinc++",
]


class MapSoundsTests(unittest.TestCase):
    def test_host_behavior(self):
        binary = Path("/tmp/norrath-save-net-anim-map-sounds")
        subprocess.run(
            ["c++", "-std=c++17", "-Wall", "-Wextra", "-Werror",
             "-DNORRATH_HOST_ARENA", str(SOURCE), str(DRIVER), "-o", str(binary)],
            check=True)
        subprocess.run([str(binary)], check=True)

    def test_ee_object_exports_only_the_mapper(self):
        object_path = Path("/tmp/norrath-save-net-anim-map-sounds.o")
        subprocess.run(
            [str(EE), *EE_FLAGS, "-c", str(SOURCE), "-o", str(object_path)],
            check=True)
        nm = EE.with_name("mips64r5900el-ps2-elf-nm")
        text = subprocess.run([str(nm), str(object_path)], check=True,
                              capture_output=True, text=True).stdout
        defined = [line.split()[-1] for line in text.splitlines() if " T " in line]
        undefined = [line.split()[-1] for line in text.splitlines() if " U " in line]
        self.assertEqual(defined, ["norrath_archive_map_animation_sounds"])
        self.assertEqual(undefined, [
            "norrath_string_compare",
            "soundComputeSequenceLength__FP15AnimationHeader",
            "soundMapSound__FPci",
        ])


if __name__ == "__main__":
    unittest.main()
