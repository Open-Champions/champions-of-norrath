// Host comparison only. Not an EE build input.
// Strings reached through %s are 32-bit words on the target; here each word
// is an index into a table of host strings (see norrath_printf_host_text).
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>

static std::string output;
static std::vector<int> raw;
static int dintr_result = 0, eintr_calls = 0, eintr_result = 0;
static std::vector<unsigned long long> float_calls;
static std::vector<unsigned long long> printfloat_args;

static int capture(int character) { output.push_back(static_cast<char>(character)); raw.push_back(character); return 0; }
extern "C" int (*_putchar)(int) = capture;
extern "C" int DIntr() { return dintr_result; }
extern "C" int EIntr() { ++eintr_calls; return eintr_result; }
extern "C" float dptofp(unsigned long long bits) { double d; std::memcpy(&d, &bits, 8); float_calls.push_back(bits); return static_cast<float>(d); }
extern "C" unsigned long long norrath_entity_fptodp(float value) { double d = value; unsigned long long bits; std::memcpy(&bits, &d, 8); return bits; }
extern "C" void printfloat(unsigned long long bits) { printfloat_args.push_back(bits); }

#include "printf.cpp"

static int failures = 0;
static std::vector<std::string> strings;
extern "C" const char* norrath_printf_host_text(unsigned word) { return strings[word - 0x1000].c_str(); }
static unsigned low_string(const char* text) { strings.push_back(text); return 0x1000u + unsigned(strings.size() - 1); }

typedef unsigned long long Slot;
static void run(const char* format, const std::vector<Slot>& slots, const std::string& expect, const char* name) {
    output.clear(); raw.clear();
    std::vector<Slot> copy(slots);
    copy.push_back(0);
    _printf(format, copy.data());
    if (output != expect) {
        std::printf("%s: format \"%s\" produced \"%s\", expected \"%s\"\n", name, format, output.c_str(), expect.c_str());
        ++failures;
    }
}

int main() {
    run("abc", {}, "abc", "plain");
    run("", {}, "", "empty");
    run("%d", {42}, "42", "d");
    run("%d", {Slot(-42)}, "-42", "d negative");
    run("%d", {0}, "0", "d zero");
    run("%d", {0xdeadbeef00000007ull}, "7", "d reads the low word only");
    run("%d", {0x80000000ull}, "-2147483648", "d int min");
    run("%05d", {42}, "00042", "zero pad");
    run("%05d", {Slot(-42)}, "-00042", "zero pad after sign");
    run("%05d", {123456}, "123456", "wider than pad");
    run("%02d", {5}, "05", "pad 2");
    run("%010d", {7}, "0000000007", "two digit width");
    run("%0d", {7}, "7", "zero without digits");
    run("%00d", {7}, "7", "zero width");
    run("%099d", {7}, "000000000000000000000000000000" "7", "width limited to 31");
    run("%031d", {7}, "0000000000000000000000000000007", "width 31");
    run("%03d|%d", {7, 8}, "007|8", "pad resets after one conversion");
    run("%d%d", {1, 2}, "12", "two args");
    run("%u", {0xffffffffull}, "4294967295", "u word");
    run("%u", {0xdead0000ffffffffull}, "4294967295", "u ignores upper word");
    run("%u", {0}, "0", "u zero");
    run("%lu", {0xffffffffffffffffull}, "18446744073709551615", "lu");
    run("%hu", {0x12345}, "9029", "hu");
    run("%ld", {Slot(-9223372036854775807ll)}, "-9223372036854775807", "ld");
    run("%ld %d", {Slot(5000000000ll), 3}, "5000000000 3", "modifier resets per conversion");
    run("%hd", {0x12345}, "9029", "hd");
    run("%hd", {0xffff}, "-1", "hd negative");
    run("%x", {255}, "ff", "x");
    run("%x", {0}, "0", "x zero");
    run("%lx", {0xdeadbeefcafebabeull}, "deadbeefcafebabe", "lx");
    run("%08x", {0xabc}, "00000abc", "x pad");
    run("%hx", {0x12345}, "2345", "hx");
    run("%x", {0xdead00000000beefull}, "beef", "x ignores upper word");
    run("%o", {8}, "10", "o");
    run("%o", {0}, "0", "o zero");
    run("%lo", {0xffffffffffffffffull}, "1777777777777777777777", "lo");
    run("%c", {0x1234567841ull}, "A", "c low byte");
    run("%s", {low_string("hi there")}, "hi there", "s");
    run("%s|%s", {low_string("a"), low_string("b")}, "a|b", "two strings");
    run("%s", {low_string("")}, "(null)", "empty string prints (null)");
    run("%5d", {}, "d", "5 is not a width");
    run("a%qb", {}, "ab", "unknown conversion skipped");
    run("100%%", {}, "100", "percent is skipped");
    run("x=%d y=%d", {10, 20}, "x=10 y=20", "text between");
    run("Assertion \"%s\" failed in %s:%d  context:%s (%x)\n",
        {low_string("a>0"), low_string("game.cc"), 436, low_string("none"), 0xbeef},
        "Assertion \"a>0\" failed in game.cc:436  context:none (beef)\n", "assert format");

    // _putchar receives signed characters.
    run("\xe9", {}, "\xe9", "high plain byte");
    if (raw.size() != 1 || raw[0] != -23) { std::printf("plain high byte must be passed sign-extended\n"); ++failures; }
    run("%c", {0xe9}, "\xe9", "high c byte");
    if (raw.size() != 1 || raw[0] != -23) { std::printf("%%c high byte must be passed sign-extended\n"); ++failures; }

    // Floats: zero prints "0"; anything else goes through the float round trip.
    float_calls.clear(); printfloat_args.clear();
    double zero = 0.0, one_and_half = 1.5, third = 1.0 / 3.0;
    Slot zero_bits, half_bits, third_bits;
    std::memcpy(&zero_bits, &zero, 8); std::memcpy(&half_bits, &one_and_half, 8); std::memcpy(&third_bits, &third, 8);
    run("%f", {zero_bits}, "0", "float zero");
    if (!printfloat_args.empty()) { std::printf("float zero must not reach printfloat\n"); ++failures; }
    run("%f", {half_bits}, "", "float nonzero");
    run("%e", {third_bits}, "", "e same as f");
    double rounded = static_cast<float>(third);
    Slot rounded_bits; std::memcpy(&rounded_bits, &rounded, 8);
    if (printfloat_args.size() != 2 || printfloat_args[0] != half_bits || printfloat_args[1] != rounded_bits || float_calls.size() != 3) {
        std::printf("float round trip wrong (%zu calls)\n", printfloat_args.size());
        ++failures;
    }

    // Interrupt state.
    eintr_calls = 0; dintr_result = 0; eintr_result = 77;
    output.clear(); std::vector<Slot> none(2, 0);
    int result = _printf("x", none.data());
    if (result != 0 || eintr_calls != 0 || output != "x") { std::printf("interrupts were off: no EIntr, result 0\n"); ++failures; }
    dintr_result = 1;
    result = _printf("x", none.data());
    if (result != 77 || eintr_calls != 1) { std::printf("interrupts were on: EIntr once, its result returned\n"); ++failures; }
    result = _printf("", none.data());
    if (result != 77 || eintr_calls != 2) { std::printf("empty format still restores interrupts\n"); ++failures; }

    if (failures) return 1;
    std::printf("_printf cases passed\n");
    return 0;
}
