/*
q * DevQ.h
 * Zoilo Mercedes	
 */
#ifndef DEVQ_H
#define DEVQ_H
#include "FSdisk.h"

template <typename qobj>
class DevQ{
public:
  DevQ() : prefix{'-'}, total{0} { };

  ~DevQ(){ theQs.clear(); }

  void init(char p, int t){ // initializes number of queues to be stored
    prefix = p;
    total = t;
    theQs.resize(t);
  }

  const int devices(){ // returns total number of devices 'connected'
  	return total;
  }

  int size(int n){ // returns size of device n's queue. might need removal
    return theQs[n].getSize();
  }

  bool insert(int q, int p, PCB* cpu){ // returns true if device enters process, false on getting nullptr
    if(cpu == nullptr)
      return false;

    std::cout << "  ## What is the file name? ";
    std::cin >> cpu->mm;
    cpu->state = inputInt(999999,"  ## What memory location? ");
    if(prefix == 'p')
      cpu->ofl = "w";
    else {
      std::cout << "  ## Read/write? (r, w, rw) ";
      std::cin >> cpu->ofl;
    }
    if(cpu->ofl=="w" || cpu->ofl=="rw"){
      std::cout<< "  ## How long is the file? ";
      std::cin >> cpu->unique;
    }
    if(prefix=='d'){
      cpu->cylinder = inputInt(theQs[q-1].getCylinder(),"  ## What cylinder? ");
    }

    return theQs[q-1].enq(cpu);
  }

  PCB* moveToReady(int q){ // returns PCB from device q's queue and removes it from the device
    PCB* tmp = theQs[q-1].peek();
    theQs[q-1].dq();
    return tmp;
  }

  void setCylinders(){ // only called by disk devices, sets cylinders for each
    for(int i=0;i<theQs.size();i++){
      std::cout<<" ### d" << i+1 << std::endl;
      theQs[i].setCylinder();
    }
  }

  bool empty(int q){ // checks if device q's queue is empty
    return theQs[q-1].empty();
  }

  void print(){ // prints all device queues
    for(int i = 0; i<total ; i++){
      std::cout<< "###" << prefix << i+1 << std::endl;
      theQs[i].print();
    }
  }

private:
  char prefix;
  std::vector<qobj> theQs;
  int total;
};
#endif