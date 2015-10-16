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

  const int devices(){
  	return total;
  }

  bool insert(int q, int p, PCB* cpu){ // move pcb in cpu to device queue
    std::cout << " What is the file name? ";
    std::cin >> cpu->mm;
    std::cout << " What memory location? ";
    std::cin >> cpu->state;
    if(prefix == 'p')
      cpu->ofl = "w";
    else {
      std::cout << " Read/write? ";
      std::cin >> cpu->ofl;
    }
    if(cpu->ofl=="w" || cpu->ofl=="rw"){
      std::cout<< " How long is the file? ";
      std::cin >> cpu->unique;
    }

    return theQs[q-1].enq(cpu);
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