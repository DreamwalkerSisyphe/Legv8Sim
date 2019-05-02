#include <iostream>
#include "Instruction.h"

using namespace std;


bool legv8Flag [4] = {false, false, false, false};
vector<uint8_t> MEM(1000);


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

void CheckInstruction(Instruction i, vector<long long int> RFILE){
    if (i.type == "ADD")
        ADD(true, RFILE[i.regs[0]], RFILE[i.regs[1]], i.imed, RFILE[i.regs[2]], false);
    else if(i.type == "ADDS")
        ADD(true, RFILE[i.regs[0]], RFILE[i.regs[1]], i.imed, RFILE[i.regs[2]], true);
    else if(i.type == "ADDI")
        ADD(false, RFILE[i.regs[0]], RFILE[i.regs[1]], i.imed, RFILE[i.regs[1]], false);
    else if(i.type == "ADDIS")
        ADD(false, RFILE[i.regs[0]], RFILE[i.regs[1]], i.imed, RFILE[i.regs[1]], true);
    else if(i.type == "SUB")
        SUB(true, RFILE[i.regs[0]], RFILE[i.regs[1]], i.imed, RFILE[i.regs[2]], false);
    else if(i.type == "SUBS")
        SUB(true, RFILE[i.regs[0]], RFILE[i.regs[1]], i.imed, RFILE[i.regs[2]], true);
    else if(i.type == "SUBI")
        SUB(false, RFILE[i.regs[0]], RFILE[i.regs[1]], i.imed, RFILE[i.regs[1]], false);
    else if(i.type == "SUBIS")
        SUB(false, RFILE[i.regs[0]], RFILE[i.regs[1]], i.imed, RFILE[i.regs[1]], true);
    else if(i.type == "AND")
        AND(true, RFILE[i.regs[0]], RFILE[i.regs[1]], i.imed, RFILE[i.regs[2]], false);
    else if(i.type == "ANDS")
        AND(true, RFILE[i.regs[0]], RFILE[i.regs[1]], i.imed, RFILE[i.regs[2]], true);
    else if(i.type == "ANDI")
        AND(false, RFILE[i.regs[0]], RFILE[i.regs[1]], i.imed, RFILE[i.regs[1]], false);
    else if(i.type == "ANDIS")
        AND(false, RFILE[i.regs[0]], RFILE[i.regs[1]], i.imed, RFILE[i.regs[1]], true);

}

int main() {
    vector<long long int> RFILE(31);
    RFILE[0] = 0x00000000FFFFFF01;  RFILE[1] = 50;
    STUR(8, RFILE[2], 0, RFILE[0]);
    for(int i = 0; i < 1000; i++){
        cout << int(MEM[i]) << std::endl;
    }
    LDUR(8, RFILE[2], 0, RFILE[1]);
    cout << hex << RFILE[1] << std::endl;
    return 0;
}