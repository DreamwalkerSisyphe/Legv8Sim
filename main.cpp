#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <algorithm>
#include "Instruction.h"

using namespace std;


bool legv8Flag [4] = {false, false, false, false}; // [Negative, Zero, Overflow, Carry]
vector<uint8_t> MEM(1000);
vector<string> types{"ADD", "ADDS", "ADDI", "ADDIS", "SUB", "SUBS", "SUBI", "SUBIS",
                     "LDUR", "LDURH", "LDURB", "STUR", "STURH", "STURB", "AND", "ANDS", "ANDI", "ANDIS"}; // Put all instruction types here so they can easily be checked




void SetFlags(bool sub, long long int source1, long long int source2, long long int result){
    long long int _source2;
    if(sub)  _source2 = -source2;
    else _source2 = source2;
    //NEGATIVE
    legv8Flag[0] = result < 0;
    //ZERO
    legv8Flag[1] = result == 0;
    //OVERFLOW
    if(source1 > 0 && _source2 > 0 && result < 0)
        legv8Flag[2] = true;
    else
        legv8Flag[2] = source1 < 0 && _source2 < 0 && result > 0;
    //CARRY
    if(source1 < 0 && _source2 < 0)
        legv8Flag[3] = true;
    else
        legv8Flag[3] = (source1 < 0 || _source2 < 0) && result >= 0;
}


void ADD(bool typeR, long long int &source1, long long int &source2, unsigned long long int immediate, long long int &destination, bool flag){
    if(typeR){
        destination = source1 + source2;
        if(flag)
            SetFlags(false, source1, source2, destination);
    }else{
        destination = source1 + immediate;
        if(flag)
            SetFlags(false, source1, immediate, destination);
    }

}

void SUB(bool typeR, long long int &source1, long long int &source2, unsigned long long int immediate, long long int &destination, bool flag){
    if(typeR){
        destination = source1 - source2;
        if(flag)
            SetFlags(true, source1, source2, destination);
    }else{
        destination = source1 - immediate;
        if(flag)
            SetFlags(true, source1, immediate, destination);
    }
}

void AND(bool typeR, long long int &source1, long long int &source2, unsigned long long int immediate, long long int &destination, bool flag){
    if(typeR){
        destination = source1 & source2;
    }else{
        destination = source1 & immediate;
    }
    if(flag){
        //NEGATIVE
        legv8Flag[0] = destination < 0;
        //ZERO
        legv8Flag[1] = destination == 0;
        //OVERFLOW
        legv8Flag[2] = false;
        //CARRY
        legv8Flag[3] = false;
    }
}

void ORR(bool typeR, long long int &source1, long long int &source2, unsigned long long int immediate, long long int &destination){
    if(typeR){
        destination = source1 | source2;
    }else{
        destination = source1 | immediate;
    }
}

void EOR(bool typeR, long long int &source1, long long int &source2, unsigned long long int immediate, long long int &destination){
    if(typeR){
        destination = source1 ^ source2;
    }else{
        destination = source1 ^ immediate;
    }
}

void LSL(long long int &source1, unsigned long long int shamt, long long int &destination){
    unsigned long long int temp = source1;
    destination = temp << shamt;
}

void LSR(long long int &source1, unsigned long long int shamt, long long int &destination){
    unsigned long long int temp = source1;
    destination = temp >> shamt;
}

void STUR(int size, long long int &source1, long long int addr, long long int &destination){
    uint8_t eightBit = 0;
    for(int i = 0; i < size; i++){
        eightBit = destination >> 8*i;
        MEM[source1 + addr + i] = eightBit;
    }
}

void LDUR(int size, long long int &source1, long long int addr, long long int &destination){
    destination = 0;
    unsigned long long int tempEight = 0;
    unsigned long long int temp = 0;
    for(int i = 0; i < size; i++){
        tempEight = MEM[source1 + addr + i];
        temp += tempEight << (8*i);
    }
    destination = temp;
}

bool BranchCond(string cond){
    if(cond == "EQ")
        return legv8Flag[1] == 1;
    else if(cond == "NE")
        return legv8Flag[1] == 0;
    //Add rest
}

void CheckInstruction(Instruction* i, vector<long long int> RFILE, long long int &pc){
    if (i->type == "ADD")
        ADD(true, RFILE[i->regs[1]], RFILE[i->regs[2]], i->imed, RFILE[i->regs[0]], false);
    else if(i->type == "ADDS")
        ADD(true, RFILE[i->regs[1]], RFILE[i->regs[2]], i->imed, RFILE[i->regs[0]], true);
    else if(i->type == "ADDI")
        ADD(false, RFILE[i->regs[1]], RFILE[i->regs[1]], i->imed, RFILE[i->regs[0]], false);
    else if(i->type == "ADDIS")
        ADD(false, RFILE[i->regs[1]], RFILE[i->regs[1]], i->imed, RFILE[i->regs[0]], true);
    else if(i->type == "SUB")
        SUB(true, RFILE[i->regs[1]], RFILE[i->regs[2]], i->imed, RFILE[i->regs[0]], false);
    else if(i->type == "SUBS")
        SUB(true, RFILE[i->regs[1]], RFILE[i->regs[2]], i->imed, RFILE[i->regs[0]], true);
    else if(i->type == "SUBI")
        SUB(false, RFILE[i->regs[1]], RFILE[i->regs[1]], i->imed, RFILE[i->regs[0]], false);
    else if(i->type == "SUBIS")
        SUB(false, RFILE[i->regs[1]], RFILE[i->regs[1]], i->imed, RFILE[i->regs[0]], true);
    else if(i->type == "AND")
        AND(true, RFILE[i->regs[1]], RFILE[i->regs[2]], i->imed, RFILE[i->regs[0]], false);
    else if(i->type == "ANDS")
        AND(true, RFILE[i->regs[1]], RFILE[i->regs[2]], i->imed, RFILE[i->regs[0]], true);
    else if(i->type == "ANDI")
        AND(false, RFILE[i->regs[1]], RFILE[i->regs[1]], i->imed, RFILE[i->regs[0]], false);
    else if(i->type == "ANDIS")
        AND(false, RFILE[i->regs[1]], RFILE[i->regs[1]], i->imed, RFILE[i->regs[0]], true);
    else if(i->type == "B")
        pc = i->labl;
    else if(i->type == "BL"){
        RFILE[30] = pc;
        pc = i->labl;
    } else if(i->type == "BR")
        pc = RFILE[30];
    else if(i->type.substr(0,2) == "B.")
        pc = BranchCond(i->type.substr(2,2)) ? i->labl-1 : pc;
    else if(i->type == "CBZ"){
        pc = RFILE[i->regs[0]] == 0 ? i->labl-1 : pc;
    }
    else if(i->type == "CBNZ"){
        pc = RFILE[i->regs[0]] != 0 ? i->labl-1 : pc;
    }
    else if(i->type == "LSL")
        LSL(RFILE[i->regs[1]], i->imed, RFILE[i->regs[0]]);
    else if(i->type == "LSR")
        LSR(RFILE[i->regs[1]], i->imed, RFILE[i->regs[0]]);
    else if(i->type == "EOR")
        EOR(false, RFILE[i->regs[1]], RFILE[i->regs[2]], i->imed, RFILE[i->regs[0]]);
    else if(i->type == "EORI")
        EOR(true, RFILE[i->regs[1]], RFILE[i->regs[2]], i->imed, RFILE[i->regs[0]]);
    else if(i->type == "ORR")
        ORR(false, RFILE[i->regs[1]], RFILE[i->regs[2]], i->imed, RFILE[i->regs[0]]);
    else if(i->type == "ORRI")
        ORR(true, RFILE[i->regs[1]], RFILE[i->regs[2]], i->imed, RFILE[i->regs[0]]);
    else if(i->type == "LDUR")
        LDUR(8,RFILE[i->regs[1]], i->imed, RFILE[i->regs[0]]);
    else if(i->type == "STUR")
        STUR(8,RFILE[i->regs[1]], i->imed, RFILE[i->regs[0]]);

}

int main(int argc, char* argv[]) {
    vector<long long int> RFILE(32);
    /*RFILE[0] = 0x00000000FFFFFF01;  RFILE[1] = 50;
    STUR(8, RFILE[2], 0, RFILE[0]);
    for(int i = 0; i < 1000; i++){
        cout << int(MEM[i]) << std::endl;
    }
    LDUR(8, RFILE[2], 0, RFILE[1]);
    cout << hex << RFILE[1] << std::endl;*/
    fstream tempfile;
    tempfile.open(argv[1]);
    string line;
    int lineNum = 0;
    map<string, int> labels;
    while(getline(tempfile, line)){ //Searches for and stores all labels.
        lineNum++;
        stringstream ss;
        ss << line;
        string word;
        ss >> word;
        if(word.back() == ':'){
            word.pop_back();
            labels[word] = lineNum;
        }
    }
    lineNum = 0;
    tempfile.close();
    fstream file;
    file.open(argv[1]); //Reopen the file in a new fstream so that the buffer resets back to the beginning.
    vector<Instruction*> program;
    while(getline(file, line)){ //Parsing each instruction
        lineNum++;
        stringstream ss;
        ss << line;
        string type;
        vector<int> regs;
        long long int imed = 0;
        int lblIndex = 0;
        while(!ss.eof()){
            string word;
            ss >> word;
            if(word.back() == ':') //Skip label markings.
                continue;
            //Sanitize the input
            if((word.back() == ',') || (word.back() == ']'))
                word.pop_back();
            if(word.front() == '[')
                word = word.substr(1, word.size()-1);
            if(find(types.begin(), types.end(), word) != types.end()){ //Checks to see if its in the list of valid instruction types.
                type = word;
                continue;
            }
            if(word.front() == '#'){ //Checks if its an imediate or address offse.
                word = word.substr(1, word.size()-1);
                imed = (long long int)stoi(word);
                continue;
            }
            if(labels.find(word) != labels.end()){ //Checks to see if its a label;
                lblIndex = labels[word];
                continue;
            }
            //Handle all special register names first. 
            if(word == "SP"){
                regs.push_back(28);
                continue;
            }
            if(word == "FP"){
                regs.push_back(29);
                continue;
            }
            if(word == "LR"){
                regs.push_back(30);
                continue;
            }
            if(word == "XZR"){
                regs.push_back(31);
                continue;
            }
            if(word.front() == 'X'){ //Check if its a register.
                word = word.substr(1, word.size()-1);
                regs.push_back(stoi(word));
            }
        }
        Instruction* I = new Instruction(type, regs, imed, lblIndex);
        program.push_back(I);
    }


    long long int pc = 0;
    while (pc < program.size()) {

        CheckInstruction(program[pc], RFILE, pc); //Execute instruction.
        RFILE[31] = 0; //Reset XZR to 0.
        pc++;
    }


    return 0;
}