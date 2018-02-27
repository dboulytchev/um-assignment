#include <bits/stdc++.h>

#define f       first
#define s       second
#define mp      make_pair
#define rank    something_which_is_supposed_to_be_rank
#define left    something_which_is_supposed_to_be_left
#define right   something_which_is_supposed_to_be_right

using namespace std;

typedef unsigned int uint32_t;
typedef long long ll;
typedef unsigned long long ull;
typedef long double ld;

vector<vector<uint32_t>> arrays;
queue<uint32_t> free_arrays;
uint32_t registers[8];
uint32_t execution_point;

bool is_little_endian() {
    short int number = 0x1;
    char *numPtr = (char*)&number;
    return (numPtr[0] == 1);
}

void next_step() {
    #ifdef DEBUG
    // cerr << "registers: ";
    // for (int i = 0; i < 8; ++i) {
    //     cerr << dec << registers[i] << ", ";
    // }
    // cerr << "\n";
    cerr << setw(8) << setfill('0') << hex << execution_point << ": ";
    #endif
    uint32_t op = arrays[0][execution_point++];
    #ifdef DEBUG
    cerr << setw(8) << setfill('0') << hex << op << " - ";
    #endif
    uint32_t op_id = op >> 28;
    if (op_id < 13) {
        uint32_t tmp = op;
        uint32_t c = tmp & ((1 << 3) - 1);
        tmp >>= 3;
        uint32_t b = tmp & ((1 << 3) - 1);
        tmp >>= 3;
        uint32_t a = tmp & ((1 << 3) - 1);
        switch (op_id) {
            case 0: {
                #ifdef DEBUG
                cerr << " CMOVE, ";
                #endif
                if (registers[c] != 0) {
                    registers[a] = registers[b];
                }
                break;
            }
            case 1: {
                #ifdef DEBUG
                cerr << " AREAD, ";
                #endif
                if (registers[b] >= arrays.size()) {
                    #ifdef DEBUG
                    cerr << "array id out of bounds\n";
                    #endif
                    exit(1);
                }
                if (registers[c] >= arrays[registers[b]].size()) {
                    #ifdef DEBUG
                    cerr << "array index out of bounds\n";
                    #endif
                    exit(1);
                }
                registers[a] = arrays[registers[b]][registers[c]]; // specification says "B" not "register B"
                break;
            }
            case 2: {
                #ifdef DEBUG
                cerr << "AWRITE, ";
                #endif
                if (registers[a] >= arrays.size()) {
                    #ifdef DEBUG
                    cerr << "array id out of bounds\n";
                    #endif
                    exit(1);
                }
                if (registers[b] >= arrays[registers[a]].size()) {
                    #ifdef DEBUG
                    cerr << "array index out of bounds\n";
                    #endif
                    exit(1);
                }
                arrays[registers[a]][registers[b]] = registers[c]; // same
                break;
            }
            case 3: {
                #ifdef DEBUG
                cerr << "   ADD, ";
                #endif
                registers[a] = registers[b] + registers[c];
                break;
            }
            case 4: {
                #ifdef DEBUG
                cerr << "   MUL, ";
                #endif
                registers[a] = registers[b] * registers[c];
                break;
            }
            case 5: {
                #ifdef DEBUG
                cerr << "   DIV, ";
                #endif
                registers[a] = registers[b] / registers[c];
                break;
            }
            case 6: {
                #ifdef DEBUG
                cerr << "  NAND, ";
                #endif
                registers[a] = ~(registers[b] & registers[c]);
                break;
            }
            case 7: {
                #ifdef DEBUG
                cerr << "  HALT, ";
                #endif
                exit(0);
            }
            case 8: {
                #ifdef DEBUG
                cerr << " ALLOC, ";
                #endif
                uint32_t size = registers[c];
                if (free_arrays.empty()) {
                    registers[b] = (uint32_t) arrays.size();
                    arrays.push_back(vector<uint32_t>());
                } else {
                    registers[b] = free_arrays.front();
                    free_arrays.pop();
                }
                if (size) {
                   arrays[registers[b]].assign(size, 0);
                }
                break;
            }
            case 9: {
                #ifdef DEBUG
                cerr << "   DEL, ";
                #endif
                if (registers[c] >= arrays.size()) {
                    #ifdef DEBUG
                    cerr << "array id out of bounds\n";
                    #endif
                    exit(1);
                }
                free_arrays.push(registers[c]);
                arrays[registers[c]].clear();
                break;
            }
            case 10: {
                #ifdef DEBUG
                cerr << " WRITE, ";
                #endif
                if (registers[c] > 255) {
                    #ifdef DEBUG
                    cerr << "register to output is not holding a character\n";
                    #endif
                    exit(1);
                }
                cout << (char) registers[c];
                cout.flush();
                break;
            }
            case 11: {
                #ifdef DEBUG
                cerr << "  READ, ";
                #endif
                char input_char;
                cin >> noskipws >> input_char;
                registers[c] = (cin.eof() ? -1 : (uint32_t) input_char);
                break;
            }
            case 12: {
                #ifdef DEBUG
                cerr << "  LOAD, ";
                #endif
                if (registers[b] >= arrays.size()) {
                    #ifdef DEBUG
                    cerr << "array id out of bounds\n";
                    #endif
                    exit(1);
                }
                if (registers[b] != 0) {
                    arrays[0] = arrays[registers[b]];
                }
                execution_point = registers[c]; // specification says "C" not "register C" :/
                break;
            }
            default: {}
        }
        #ifdef DEBUG
        cerr << setw(4) << setfill(' ') << hex << a << ", " << b << ", " << c;
        #endif
    } else if (op_id == 13) {
        uint32_t value = op & ((1 << 25) - 1);
        uint32_t a = (op >> 25) & ((1 << 3) - 1);
        #ifdef DEBUG
        cerr << " CONST, ";
        cerr << setw(4) << setfill(' ') << hex << a << ", " << setw(8) << setfill('0') << value;
        #endif
        registers[a] = value;
    } else {
        #ifdef DEBUG
        cerr << "UNKNOWN";
        #endif
    }
    #ifdef DEBUG
    cerr << "\n";
    #endif
}

int main (int argc, char *argv[]) {
    assert(argc == 2 && "Only argument -- file to process");
    #ifdef DEBUG
    if (is_little_endian()) {
        cerr << "current system is little-endian\n";
    }
    cerr << "loading " << argv[1] << "\n";
    #endif
    vector<uint32_t> program;
    auto file = fopen(argv[1], "rb");
    for (uint32_t op; fread(&op, 4, 1, file); ) {
        if (is_little_endian()) {
            uint8_t *buf = (uint8_t *) (&op);
            swap(buf[0], buf[3]);
            swap(buf[1], buf[2]);
        }
        program.push_back(op);
    }
    fclose(file);
    #ifdef DEBUG
    cerr << "loaded\n";
    #endif
    arrays.push_back(program);
    #ifdef DEBUG
    cerr << "running\n";
    #endif
    for (; execution_point < arrays[0].size(); ) {
        next_step();
    }
    cerr << "Time elapsed: " << (ld) clock() / CLOCKS_PER_SEC << "\n";
    return 0;
}