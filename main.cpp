#include <iostream>
#include <fstream>
#include <netinet/in.h>
#include <vector>
#include <unordered_set>

using ui = uint32_t;

union platter {
    struct {
        unsigned C:3, B:3, A:3, :23;
    } standard;

    struct {
        unsigned value:25, A:3, :4;
    } special;

    struct {
        unsigned :28, val:4;
    } op;

    ui value;
    char bytes[4];
};

int main(int argc, char ** argv) {

    ui registers[8] = {0, 0, 0, 0, 0, 0, 0 ,0};

    std::vector<std::vector<platter>> arrays;
    std::unordered_set<ui> freedArrays;

    std::ifstream is(argv[1], std::ios::in | std::ios::binary);
    std::vector<platter> zeroArray;

    while(is.peek() != EOF) {
        union platter p{};
        is.read(p.bytes, 4);
        p.value = ntohl(p.value);
        zeroArray.push_back(p);
    }
    arrays.push_back(zeroArray);

    size_t executionPoint = 0;
    while (executionPoint < arrays[0].size()) {
        union platter p = arrays[0][executionPoint++];

        ui OP     = p.op.val;
        ui A      = p.standard.A;
        ui B      = p.standard.B;
        ui C      = p.standard.C;
        ui SPCA   = p.special.A;
        ui SPCVAL = p.special.value;

        switch (OP) {
            case 0:
                if (registers[C] != 0) {
                    registers[A] = registers[B];
                }
                break;
            case 1:
                registers[A] = arrays[registers[B]][registers[C]].value;
                break;
            case 2:
                arrays[registers[A]][registers[B]].value = registers[C];
                break;
            case 3:
                registers[A] = registers[B] + registers[C];
                break;
            case 4:
                registers[A] = registers[B] * registers[C];
                break;
            case 5:
                registers[A] = registers[B] / registers[C];
                break;
            case 6:
                registers[A] = ~(registers[B] & registers[C]);
                break;
            case 7:
                exit(0);
            case 8: {
                ui id;
                if (freedArrays.empty()) {
                    std::vector<platter> newArray(registers[C]);
                    arrays.push_back(newArray);
                    id = static_cast<ui>(arrays.size() - 1);
                } else {
                    ui freeArray = *freedArrays.begin();
                    freedArrays.erase(freeArray);
                    arrays[freeArray].resize(registers[C]);
                    id = freeArray;
                }
                registers[B] = id;
                break;
            }
            case 9: {
                ui id = registers[C];
                arrays[id].clear();
                freedArrays.insert(id);
                break;
            }
            case 10:
                std::cout << (unsigned char) registers[C] << std::flush;
                break;
            case 11: {
                unsigned char c;
                if (std::cin >> c) {
                    registers[C] = c;
                } else {
                    registers[C] = 0xffffffff;
                }
                break;
            }
            case 12:
                arrays[0] = arrays[registers[B]];
                executionPoint = registers[C];
                break;
            case 13:
                registers[SPCA] = SPCVAL;
                break;
            default:
                exit(1);
        }

    }

    return 0;
}
