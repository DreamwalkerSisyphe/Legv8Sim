#include <iostream>
#include <vector>

using namespace std;

bool legv8Flag[false, false, false, false];

void ADD(bool typeR, long long int &source1, long long int &source2, long long int immediate, long long int &destination, bool flag){
    if(typeR){
        destination = source1 + source2;
    }else{
        destination = source1 + immediate;
    }
}

void SUB(bool typeR, long long int &source1, long long int &source2, long long int immediate, long long int &destination, bool flag){
    if(typeR){
        destination = source1 - source2;
    }else{
        destination = source1 - immediate;
    }
}

void AND(bool typeR, long long int &source1, long long int &source2, long long int immediate, long long int &destination, bool flag){
    if(typeR){
        destination = source1 & source2;
    }else{
        destination = source1 & immediate;
    }
}

void ORR(bool typeR, long long int &source1, long long int &source2, long long int immediate, long long int &destination, bool flag){
    if(typeR){
        destination = source1 | source2;
    }else{
        destination = source1 | immediate;
    }
}

void EOR(bool typeR, long long int &source1, long long int &source2, long long int immediate, long long int &destination, bool flag){
    if(typeR){
        destination = source1 ^ source2;
    }else{
        destination = source1 ^ immediate;
    }
}

void LSL(long long int &source1, long long int shamt, long long int &destination){
    destination = source1 << shamt;
}

void LSR(long long int &source1, long long int shamt, long long int &destination){
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