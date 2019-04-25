#include <vector>
#include <string>

using namespace std;

class Instruction{
    
    public:

    string type; //Instruction name or type
    vector<int> regs; //Vector of integers representing the registers to be used (in order)
    long long int imed; //Value to be used for instructions with an imediate or address value (otherwise NULL)
    int labl; //Integer representing the index of the the instruction to jump to for branch instructions (otherwise NULL)

    Instruction(string t, vector<int> r, long long int i, int l){
        type = t;
        regs = r;
        imed = i;
        labl = l;
    }
}