/*
 * CPU.h
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
        << " ### SYSGEN ###\n"
              << "  ## How many disks? ";
    std::cin >> n;
    while(n>MAX){
      std::cout << "  ## The max number of any device is 10.\n"
          << "  ## Please disconnect some devices.\n"
                << "  ## How many disks? ";
      std::cin >> n;
    }
    disks.init('d',n);

    std::cout << "  ## How many CD/RW? ";
    std::cin >> n;
    while(n>MAX){
      std::cout << "  ## The max number of any device is 10.\n"
          << "  ## Please disconnect some devices.\n"
                << "  ## How many CD/RW? ";
      std::cin >> n;
    }
    cdrw.init('c',n);

    std::cout << "  ## How many printers? ";
    std::cin >> n;
    while(n>MAX){
      std::cout << "  ## The max number of any device is 10.\n"
          << "  ## Please disconnect some devices.\n"
                << "  ## How many disks? ";
      std::cin >> n;
    }
    printers.init('p',n);

    std::cout << " ### SYSGEN successful.\n";
  }

  ~CPU(){
    // stuff goes here?
  }

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
      } else if(in.length()<2 && isdigit(in[1])){
        std::cout<< " ### Need a valid input.\n";
        break;
      }
      disks.insert(n,PID);
      PID++;
      break;
    case 'c':
      if(current==nullptr){
        std::cout<< " ### No process to call CD/RW.\n";
        break;
      }
      cdrw.insert(n,PID);
      PID++;
      break;
    case 'p':
      if(current==nullptr){
        std::cout<< " ### No process to call printers.\n";
        break;
      }
      printers.insert(n,PID);
      PID++;
      break;
    case 'D':
      if(current==nullptr){ // checks for process in cpu
        std::cout<< " ### No process to call disks.\n";
        break;
      } else if(disks.qPeek(n)==nullptr){ // checks if device queue is empty
        std::cout<< " ### This device is empty.\n";
        break;
      }
      RQ.enq(disks.moveToReady(n));
      std::cout<< " ### Process in " << in << " is ready.\n";
      break;
    case 'C':
      if(current==nullptr){ // checks for process in cpu
        std::cout<< " ### No process to call CD/RW.\n";
        break;
      } else if(disks.qPeek(n)==nullptr){ // checks if device queue is empty
        std::cout<< " ### This device is empty.\n";
        break;
      }
      RQ.enq(cdrw.moveToReady(n));
      std::cout<< " ### Process in " << in << " is ready.\n";
      break;
    case 'P':
      if(current==nullptr){ // checks for process in cpu
        std::cout<< " ### No process to call printers.\n";
        break;
      } else if(disks.qPeek(n)==nullptr){ // checks if device queue is empty
        std::cout<< " ### This device is empty.\n";
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
              <<std::setw(12)<< "R/W" <<std::setw(12)<< "File Length\n";
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