//
// Created by Lambda-C on 2/20/17.
//

#include <iostream>
#include <map>
#include <queue>
#include <vector>
#include <stack>
#include <fstream>
#include <string>


#define DATA_SEGMENT_SIZE 30000

class brainF_vm{
private:
    enum brainF_token{end, forward, backward, inc, dec, in, out, sloop, eloop};
    typedef  std::deque<brainF_token> BFCode;
    typedef std::stack<BFCode::iterator> BFStack;
    std::map<const char, brainF_token> sTable;
    BFCode brainF_code; // store the code(token), a pointer to show the location of current instruction
    BFStack brainF_stack; // store the info - where to go after the current function finished, in BF, for loop
    unsigned int brainF_data[DATA_SEGMENT_SIZE]; // where the code edit data

    BFCode::iterator ip; // pointer for the code, instruction pointer
    //BFStack::iterator sp; // pointer for the stack, loop
    int dp; // the data pointer

    void default_sTable(){
        sTable['>'] = forward;
        sTable['<'] = backward;
        sTable['+'] = inc;
        sTable['-'] = dec;
        sTable['.'] = out;
        sTable[','] = in;
        sTable['['] = sloop;
        sTable[']'] = eloop;
    }

    void init_segments(){
        std::memset(brainF_data,0,DATA_SEGMENT_SIZE);
        brainF_code.clear();
        while (!brainF_stack.empty()){
            brainF_stack.pop();
        }
    }

protected:
    void aStep(brainF_token t){
        switch(t){
            case forward:
                dp++;
                break;
            case backward:
                dp--;
                break;
            case inc:
                brainF_data[dp]++;
                break;
            case dec:
                brainF_data[dp]--;
                break;
            case in:
                brainF_data[dp]=getchar();
                break;
            case out:
                putchar(brainF_data[dp]);
                break;
            case sloop:
                if (brainF_data[dp] != 0){
                    brainF_stack.push(ip);
                }
                else
                {
                    int c = 1;
                    while(c && ip < brainF_code.end()){
                        ip++;
                        if(*ip == sloop){
                            c++;
                        }
                        else if (*ip == eloop) {
                            c--;
                        }
                    }
                }
                brainF_stack.push(ip);
                break;
            case eloop:
                if (brainF_data[dp]){
                    ip = brainF_stack.top();
                }
                else{
                    brainF_stack.pop();
                }
                break;
            default:
                break;
        }
    }

public:
    brainF_vm(){
        this->default_sTable();
        this->init_segments();
    }

    void init_all(){
        this->sTable.clear();
        brainF_vm();
    }

    void ineractive()
    {
        char c;
        while((c=getchar())!='\n'){
            if(sTable.count(c)) {
                this->brainF_code.push_back(sTable[c]);
            }
        }
        brainF_code.push_back(end);
//        for (BFCode::iterator i = brainF_code.begin();i!=brainF_code.end();++i)
//        {std::cout<<*i;}
//        std::cout<<std::endl;

//        int count = 0;
        for (ip = brainF_code.begin();ip != brainF_code.end(); ++ip){

//            std::cout<<*ip<<std::flush;
            if (*ip == end){return ;}
            aStep(*ip);
        }
    }

//    void srcFile(){
//
//    }

};

int main(int argc,  char *argv[])
{
    brainF_vm example;
    example.ineractive();
}