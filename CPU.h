/*
 * CPU.h
 * Keeps track of ready queue and device queues in a single object. 
 * Zoilo Mercedes	
 */
#ifndef CPU_H
#define CPU_H
#include "Qtype.h"

class CPU {
 public:
 CPU(): PID{0}, current{nullptr} {
    int n;
    std::cout << "#### Welcome to zOS. #### \n"
        << " ### SYSGEN ###\n";
    n = inputInt(MAX,"  ## How many disks? ");
    disks.init('d',n);

    n = inputInt(MAX,"  ## How many CD/RW? ");
    cdrw.init('c',n);

    n = inputInt(MAX,"  ## How many printers? ");
    printers.init('p',n);

    std::cout << " ### SYSGEN successful.\n";
  }

/*  ~CPU(){
    // stuff goes here?
  }*/

  void systemCall(){ // disgustingly long
    int n;
    char l;
    std::string d;
    std::string in;
    std::cout << " # ";
    std::cin >> in;
    l = in[0];
    if(in.length() == 2 && isdigit(in[1])){
      d = in[1];
      n = std::stoi(d);
    } else if(in.length() > 2){
      std::cout<< " ### Invalid input.\n";
      return cpuStatus();
    }
    switch(l){
    case 'A':
      {
        PCB* tmp = new PCB{PID,"root",0,"rw","-"};
        RQ.enq(tmp);
        tmp = nullptr;
        std::cout<< " ### Process ready.\n";
        PID++;
      }
      break;
    case 't':
      if(current==nullptr){
        std::cout<< " ### No process to terminate.\n";
        break;
      }
      terminate();
      break;
    case 'd':
      if(current==nullptr){
        std::cout<< " ### No process to call disks.\n";
        break;
      } else if(n > disks.devices()){
      	std::cout<< " ### This device doesn't exist.\n";
      	break;
      }
      if(disks.insert(n,PID,current))
     	std::cout<< " ### Device updated.\n";
      else
     	std::cout<< " ### Error updating device.\n";
      current = nullptr;
      break;
    case 'c':
      if(current==nullptr){
        std::cout<< " ### No process to call CD/RW.\n";
        break;
      } else if(n > cdrw.devices()){
      	std::cout<< " ### This device doesn't exist.\n";
      	break;
      }
      if(cdrw.insert(n,PID,current))
     	std::cout<< " ### Device updated.\n";
      else
     	std::cout<< " ### Error updating device.\n";
      current = nullptr;
      break;
    case 'p':
      if(current==nullptr){
        std::cout<< " ### No process to call printers.\n";
        break;
      } else if(n > printers.devices()){
      	std::cout<< " ### This device doesn't exist.\n";
      	break;
      }
      if(printers.insert(n,PID,current))
     	std::cout<< " ### Device updated.\n";
      else
     	std::cout<< " ### Error updating device.\n";
      current = nullptr;
      break;
    case 'D':
      if(disks.qPeek(n)==nullptr){ // checks if device queue is empty
        std::cout<< " ### This device is empty.\n";
        break;
      } else if(n > disks.devices()){
      	std::cout<< " ### This device doesn't exist.\n";
      	break;
      }
      RQ.enq(disks.moveToReady(n));
      std::cout<< " ### Process in " << in << " is ready.\n";
      break;
    case 'C':
      if(cdrw.qPeek(n)==nullptr){ // checks if device queue is empty
        std::cout<< " ### This device is empty.\n";
        break;
      } else if(n > cdrw.devices()){
      	std::cout<< " ### This device doesn't exist.\n";
      	break;
      }
      RQ.enq(cdrw.moveToReady(n));
      std::cout<< " ### Process in " << in << " is ready.\n";
      break;
    case 'P':
      if(printers.qPeek(n)==nullptr){ // checks if device queue is empty
        std::cout<< " ### This device is empty.\n";
        break;
      } else if(n > printers.devices()){
      	std::cout<< " ### This device doesn't exist.\n";
      	break;
      }
      RQ.enq(printers.moveToReady(n));
      std::cout<< " ### Process in " << in << " is ready.\n";
      break;
    case 'S':
      snapShot();
      break;
    default:
      std::cout<< " ### Invalid input.\n";
    }
    return cpuStatus();
  }

  void terminate(){
    delete current;
    current = nullptr;
    std::cout<< " ### Process in CPU has been terminated.\n";
  }

  void cpuStatus(){
    if(current==nullptr && RQ.peek()!=nullptr){
      current = RQ.peek();
      RQ.dq();
      std::cout<< " ### Process is now in CPU.\n";
    }

    return systemCall();
  }

  void snapShot(){
    char s;
    std::cout<< "  ## Please select r,d,c,p: ";
    std::cin>> s;
    std::cout << "PID" << std::setw(12) <<"Filename"<<std::setw(12)<< "Memstart"
              <<std::setw(11)<< "R/W" <<std::setw(14)<< "File Length\n";
    if(s=='r')
      std::cout<< RQ;
    else if(s=='d')
      disks.print();
    else if(s=='c')
      cdrw.print();
    else if(s=='p')
      printers.print();
    else
      std::cout<< " ### Invalid input. \n";

    return systemCall();
  }

 private:
  Qtype disks;
  Qtype cdrw;
  Qtype printers;
  Queue RQ;
  PCB* current;
  int PID;
};
#endif