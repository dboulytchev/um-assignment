#include <iostream>
#include <vector>
#include <fstream>
#include <netinet/in.h>

#define FOR_BYTECODES(DO)                                                                                                   \
        DO(CMOV, "The register A receives the value in register B, unless the register C contains 0.")                      \
        DO(AIDX, "The register A receives the value stored at offset in register C in the array identified by B.")          \
        DO(AAMEND, "The array identified by A is amended at the offset in register B to store the value in register C.")    \
        DO(ADD, "The register A receives the value in register B plus the value in register C, modulo 2^32.")               \
        DO(MUL, "The register A receives the value in register B times the value in register C, modulo 2^32.")              \
        DO(DIV, "The register A receives the value in register B divided by the value in register C, if any, where each quantity is treated treated as an unsigned 32 bit number.") \
        DO(NAND, "Each bit in the register A receives the 1 bit if either register B or register C has a 0 bit in that position.  Otherwise the bit in register A receives the 0 bit.") \
        DO(HALT, "The universal machine stops computation.")                                                                \
        DO(ALLOC, "A new array is created with a capacity of platters commensurate to the value in the register C. This new array is initialized entirely with platters holding the value 0. A bit pattern not consisting of exclusively the 0 bit, and that identifies no other active allocated array, is placed in the B register.") \
        DO(FREE, "The array identified by the register C is abandoned. Future allocations may then reuse that identifier.") \
        DO(OUT, "The value in the register C is displayed on the console immediately. Only values between and including 0 and 255 are allowed.") \
        DO(IN, "The universal machine waits for input on the console. When input arrives, the register C is loaded with the input, which must be between and including 0 and 255. If the end of input has been signaled, then the register C is endowed with a uniform value pattern where every place is pregnant with the 1 bit.") \
        DO(LOAD, "The array identified by the B register is duplicated and the duplicate shall replace the '0' array,regardless of size. The execution finger is placed to indicate the platter of this array that is described by the offset given in C, where the value 0 denotes the first platter, 1 the second, et cetera. The '0' array shall be the most sublime choice for loading, and shall be handled with the utmost velocity.") \
        DO(VALUE, "The value indicated is loaded into the register A forthwith.")

enum Instruction {
#define ENUM_ELEM(b, d) BC_##b,
    FOR_BYTECODES(ENUM_ELEM)
#undef ENUM_ELEM
};

union Platter {
    uint32_t raw;
    char raw_bytes[4];

    struct {
        uint32_t c : 3, b : 3, a : 3, : 19, operator_number : 4;
    } standard;

    struct {
        uint32_t value : 25, a : 3, operator_number : 4;
    } special;
};

class UniversalMachine {
public:

    UniversalMachine() : _registers(REGISTERS_COUNT) {
    }

    void load_program(std::ifstream &file) {
        _memory.clear();
        _free_memory_idxs.clear();
        std::vector<Platter> platters;
        Platter current_platter = {};
        while (file.read(current_platter.raw_bytes, sizeof(current_platter.raw_bytes))) {
            current_platter.raw = ntohl(current_platter.raw);
            platters.push_back(current_platter);
        }
        _memory.push_back(platters);
    }

    void exec_program() {
        if (_memory.empty()) {
            throw std::invalid_argument("Program not loaded");
        }

        uint32_t ip = 0;
        while (ip < _memory[0].size()) {
            Platter platter = _memory[0][ip++];
            uint32_t operator_number = platter.standard.operator_number;
            uint32_t a = operator_number < BC_VALUE ? platter.standard.a : platter.special.a;
            uint32_t b = platter.standard.b;
            uint32_t c = platter.standard.c;

            switch (operator_number) {
                case BC_CMOV:
                    if (_registers[c] != 0) {
                        _registers[a] = _registers[b];
                    }
                    break;
                case BC_AIDX:
                    _registers[a] = _memory[_registers[b]][_registers[c]].raw;
                    break;
                case BC_AAMEND:
                    _memory[_registers[a]][_registers[b]].raw = _registers[c];
                    break;
                case BC_ADD:
                    _registers[a] = _registers[b] + _registers[c];
                    break;
                case BC_MUL:
                    _registers[a] = _registers[b] * _registers[c];
                    break;
                case BC_DIV:
                    _registers[a] = _registers[b] / _registers[c];
                    break;
                case BC_NAND:
                    _registers[a] = ~(_registers[b] & _registers[c]);
                    break;
                case BC_HALT:
                    return;
                case BC_ALLOC: {
                    uint32_t idx = 0;
                    if (_free_memory_idxs.empty()) {
                        idx = static_cast<uint32_t >(_memory.size());
                        uint32_t size = _registers[c];
                        std::vector<Platter> array(size);
                        _memory.push_back(array);
                    } else {
                        idx = _free_memory_idxs.back();
                        _free_memory_idxs.pop_back();
                        uint32_t size = _registers[c];
                        _memory[idx].resize(size);
                    }
                    _registers[b] = idx;
                    break;
                }
                case BC_FREE: {
                    uint32_t idx = _registers[c];
                    _memory[idx].clear();
                    _free_memory_idxs.push_back(idx);
                    break;
                }
                case BC_OUT:
                    std::cout << static_cast<char>(_registers[c]) << std::flush;
                    break;
                case BC_IN:
                    uint8_t ch;
                    _registers[c] = std::cin >> std::noskipws >> ch ? ch : 0xFFFFFFFF;
                    break;
                case BC_LOAD:
                    _memory[0] = _memory[_registers[b]];
                    ip = _registers[c];
                    break;
                case BC_VALUE:
                    _registers[a] = platter.special.value;
                    break;
                default:
                    throw std::invalid_argument("Invalid bytecode");
            }
        }
    }

public:
    static const uint32_t REGISTERS_COUNT = 8;

private:
    std::vector<uint32_t> _registers;
    std::vector<std::vector<Platter>> _memory;
    std::vector<uint32_t> _free_memory_idxs;
};

int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cerr << "USAGE: interpreter <filename>" << std::endl;
        return EXIT_FAILURE;
    }

    std::ifstream file(argv[1], std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Failed to open file " << argv[1] << std::endl;
        return EXIT_FAILURE;
    }

    UniversalMachine um;
    um.load_program(file);
    um.exec_program();

    return EXIT_SUCCESS;
}
