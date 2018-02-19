#include <iostream>
#include <vector>
#include <fstream>
#include <iterator>
#include <assert.h>
#include <queue>
#include <limits>

#define REGISTER_COUNT 8

int main(int argc, const char* argv[]) {
    std::vector<uint32_t> registers(REGISTER_COUNT, 0);
    std::vector<std::vector<uint32_t >> collection;
    std::vector<std::uint8_t> console;

    if (argc < 2) {
        std::cout << "program aren't passed!\n";
        exit(-1);
    }

    std::ifstream file(argv[1], std::ios::in | std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "cannot open file " << argv[1] << std::endl;
        exit(-1);
    }
    std::vector<char> vec;
    std::copy(std::istreambuf_iterator<char>(file),
                std::istreambuf_iterator<char>(),
                std::back_inserter(vec));
    while (vec.size() % 4 != 0) {
        vec.push_back(0);
    }
    std::vector<uint32_t> platters;
    for (size_t i = 0; i < vec.size(); i += 4) {
        platters.push_back((((uint32_t)(uint8_t)vec[i]) << 24) |
                                   (((uint32_t)(uint8_t)vec[i + 1]) << 16) |
                                   (((uint32_t)(uint8_t)vec[i + 2]) << 8) |
                                   (uint32_t)(uint8_t)vec[i + 3]);
    }
    collection.push_back(platters);
    std::queue<uint32_t> freeMemory{};
    size_t platterIndex = 0;
    while (platterIndex < collection[0].size()) {
        uint32_t platter = collection[0][platterIndex];
        auto commandNumber = (uint8_t)((platter >> 28) & 0xf);
        auto regA = (uint8_t)((platter >> 6) & 0x7);
        auto regB = (uint8_t)((platter >> 3) & 0x7);
        auto regC = (uint8_t)(platter & 0x7);
        switch (commandNumber) {
            case 0: {
                if (registers[regC]) {
                    registers[regA] = registers[regB];
                }
                break;
            }
            case 1:
                registers[regA] = collection[registers[regB]][registers[regC]];
                break;
            case 2:
                collection[registers[regA]][registers[regB]] = registers[regC];
                break;
            case 3:
                registers[regA] = (uint32_t)((registers[regB] + registers[regC]) % (1ll << 32));
                break;
            case 4:
                registers[regA] = (uint32_t)((registers[regB] * registers[regC]) % (1ll << 32));
                break;
            case 5:
                registers[regA] = registers[regB] / registers[regC];
                break;
            case 6:
                registers[regA] = ~(registers[regB] & registers[regC]);
                break;
            case 7: return 0;
            case 8: {
                std::vector<uint32_t> newArray(registers[regC], 0);
                if (freeMemory.empty()) {
                    if (collection.size() > std::numeric_limits<uint32_t>::max()) {
                        std::cerr << "Cannot create more then UINT32_MAX array\n";
                        exit(-1);
                    }
                    registers[regB] = (uint32_t) collection.size() ;
                    collection.push_back(newArray);
                } else {
                    uint32_t index = freeMemory.front();
                    freeMemory.pop();
                    registers[regB] = index;
                    collection[index] = newArray;
                }
                break;
            }
            case 9:
                freeMemory.push(registers[regC]);
                break;
            case 10: {
                assert(registers[regC] >= 0 && registers[regC] <= 255);
                std::cout << (char)(registers[regC]);
                break;
            }
            case 11: {
                int c = getchar();
                if (c == EOF) {
                    registers[regC] = (uint32_t) ((1 << 32) - 1);
                } else {
                    assert(c >= 0 && c <= 255);
                    registers[regC] = (uint32_t) c;
                }
                break;
            }
            case 12: {
                collection[0] = collection[registers[regB]];
                platterIndex = registers[regC];
                continue;
            }
            case 13: {
                regA = (uint8_t)((platter >> 25) & 0x7);
                uint32_t value = platter & 0x1ffffff;
                registers[regA] = value;
                break;
            }
            default:
                std::cout << "wrong instruction indicator\n";
                break;
        }
        ++platterIndex;
    }
    return 0;
}