/*
 * Qtype.h
 * Zoilo Mercedes	
 */
#ifndef QTYPE_H
#define QTYPE_H
#include "Queue.h"
#include <vector>
#include <string>

class Qtype{
public:
  Qtype() : prefix{'-'}, total{0} { };

  ~Qtype(){ theQs.clear(); }

  void init(char p, int t){
    prefix = p;
    total = t;
    theQs.resize(t);
  }

  bool insert(int q, int p){
    std::string fn,rw,length = "-";
    int mem;
    std::cout << " What is the file name? ";
    std::cin >> fn;
    std::cout << " What memory location? ";
    std::cin >> mem;
    if(prefix == 'p')
      rw = "w";
    else {
      std::cout << " Read/write? ";
      std::cin >> rw;
    }
    if(rw=="w"){
      std::cout<< " How long is the file? ";
      std::cin >> length;
    }

    PCB* tmp = new PCB{p,fn,mem,rw,length};
    return theQs[q-1].enq(tmp);
  }

  PCB* qPeek(int q){
    return theQs[q-1].peek();
  }

  PCB* moveToReady(int q){
    PCB* tmp = theQs[q-1].peek();
    theQs[q-1].dq();
    return tmp;
  }

  void print(){
    for(int i = 0;i<total;i++){
      std::cout<< "###" << prefix << i+1 << std::endl;
      std::cout<< theQs[i];
    }
  }

private:
  char prefix;
  std::vector<Queue> theQs;
  int total;
};
#endif