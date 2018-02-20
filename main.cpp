#include <iostream>
#include <fstream>
#include <vector>

uint32_t registers[8];

enum instr {
    CMOVE = 0,
    ARRAY_INDEX,
    ARRAY_AMENDMENT,
    ADD,
    MUL,
    DIV,
    NAND,
    HALT,
    ALLOC,
    FREE,
    OUT,
    IN,
    LOAD,
    CONST
};

struct Code {
    void set_code(uint32_t code) {
        _type = (instr) ((code & 0xF0000000) >> 28);
        if (_type == CONST) {
            _A = static_cast<uint8_t>((code >> 25) & 0x7);
            _value = code & 0x1FFFFFF;
        } else {
            _A = (code >> 6) & 0x7;
            _B = (code >> 3) & 0x7;
            _C = code & 0x7;
        }
    }

    instr get_type() const {
        return _type;
    }

    uint32_t get_A() const {
        return _A;
    }

    uint32_t get_B() const {
        return _B;
    }

    uint32_t get_C() const {
        return _C;
    }

    uint32_t get_value() const {
        return _value;
    }

private:
    instr _type;
    uint32_t _A;
    uint32_t _B;
    uint32_t _C;
    uint32_t _value;
};

template <class T>
void end_swap(T *objp)
{
    auto *memp = reinterpret_cast<unsigned char*>(objp);
    std::reverse(memp, memp + sizeof(T));
}

int main(int argc, const char* argv[]) {
    std::vector<std::vector<uint32_t>> alloc_memory;
    std::vector<uint32_t> free_idx;
    if (argc < 2) {
        std::cerr << "Ooops!" << std::endl;
        return -1;
    }
    std::ifstream file;
    file.open(argv[1], std::ios_base::binary);
    std::vector<uint32_t> platters;
    uint32_t current_platter;
    while (file.read(reinterpret_cast<char *>(&current_platter), sizeof(current_platter))) {
        end_swap(&current_platter);
        platters.push_back(current_platter);
    }
    alloc_memory.push_back(platters);
    uint32_t ip = 0;
    Code c{};
    while (ip < alloc_memory[0].size()) {
        c.set_code(alloc_memory[0][ip++]);
        switch (c.get_type()) {
            case CMOVE:
                registers[c.get_A()] = registers[c.get_C()] ? registers[c.get_B()] :  registers[c.get_A()];
                break;
            case ARRAY_INDEX:
                registers[c.get_A()] = alloc_memory[registers[c.get_B()]][registers[c.get_C()]];
                break;
            case ARRAY_AMENDMENT:
                alloc_memory[registers[c.get_A()]][registers[c.get_B()]] = registers[c.get_C()];
                break;
            case ADD:
                registers[c.get_A()] = static_cast<uint32_t>((registers[c.get_B()] + registers[c.get_C()]) % (1ll << 32));
                break;
            case MUL:
                registers[c.get_A()] = static_cast<uint32_t>((registers[c.get_B()] * registers[c.get_C()]) % (1ll << 32));
                break;
            case DIV:
                registers[c.get_A()] = registers[c.get_B()] / registers[c.get_C()];
                break;
            case NAND:
                registers[c.get_A()] = ~(registers[c.get_B()] & registers[c.get_C()]);
                break;
            case HALT: return 0;
            case ALLOC: {
                std::vector<uint32_t> buffer(registers[c.get_C()], 0);
                if (free_idx.empty()) {
                    registers[c.get_B()] = static_cast<uint32_t>(alloc_memory.size());
                    alloc_memory.push_back(buffer);
                } else {
                    uint32_t index = free_idx.back();
                    free_idx.pop_back();
                    registers[c.get_B()] = index;
                    alloc_memory[index] = buffer;
                }
                break;
            }
            case FREE:
                free_idx.push_back(registers[c.get_C()]);
                break;
            case OUT:
                std::cout << static_cast<char>(registers[c.get_C()]);
                break;
            case IN: {
                int ch = getchar();
                registers[c.get_C()] = ch == EOF ? static_cast<uint32_t>((1 << 32) - 1) : static_cast<uint32_t>(ch);
                break;
            }
            case LOAD:
                alloc_memory[0] = alloc_memory[registers[c.get_B()]];
                ip = registers[c.get_C()];
                break;
            case CONST:
                registers[c.get_A()] = c.get_value();
                break;
        }
    }
    return 0;
}