#include <iostream>
#include <vector>
#include "Instruction.h"

using namespace std;

bool legv8Flag[false, false, false, false];

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
    destination = source1 << shamt;
}

void LSR(long long int &source1, unsigned long long int shamt, long long int &destination){
    destination = source1 >> shamt;
}

void STUR(int size, long long int &source1, long long int addr, long long int &destination){

}

void LDUR(int size, long long int &source1, long long int addr, long long int &destination){

}


int main() {
    vector<long long int> RFILE;
    vector<int8_t> MEM;
    RFILE.push_back(20);
    RFILE.push_back(50);
    RFILE.push_back(0);
    ADD(true, RFILE[0], RFILE[0], 0, RFILE[2], false);
    std::cout << "Hello, World!" << std::endl;
    return 0;
}