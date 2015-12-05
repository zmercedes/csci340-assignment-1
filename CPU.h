/*
 * CPU.h
 * Keeps track of ready queue and device queues in a single object. 
 * Zoilo Mercedes	
 * TODO:
 */
#ifndef CPU_H
#define CPU_H
#include "Devq.h"

class CPU {
public:
 CPU(): PID{0}, current{nullptr}, systemTime{0}, systemTotal{0} {
    int n;
    std::cout << "#### Welcome to zOS. #### \n"
              << " ### SYSGEN ###\n";
    n = inputInt(MAX,"  ## How many disks? ");
    disks.init('d',n);

    disks.setCylinders();

    n = inputInt(MAX,"  ## How many CD/RW? ");
    cdrw.init('c',n);

    n = inputInt(MAX,"  ## How many printers? ");
    printers.init('p',n);

    slice = inputInt(100000,"  ## How long is the time slice? ");
    std::cout << " ### SYSGEN successful.\n";
  }

/*  ~CPU(){
    // stuff goes here?
  }*/

  void systemCall(){ // asks for input and initiates appropriate system call/interrupt routine.
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
        std::cout<< " ### Process arrived.\n";
        PID++;
      }
      break;
    case 't':
      if(current==nullptr){
        std::cout<< " ### No process to terminate.\n";
        break;
      }
      timer('t');
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
      timer('d');
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
      timer('c');
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
      timer('p');
      if(printers.insert(n,PID,current))
     	  std::cout<< " ### Device updated.\n";
      else
     	  std::cout<< " ### Error updating device.\n";
      current = nullptr;
      break;
    case 'D':                                             
      if(disks.empty(n)){ // checks if device queue is empty
        std::cout<< " ### This device is empty.\n";
        break;
      } else if(n > disks.devices()){ // dne flag
      	std::cout<< " ### This device doesn't exist.\n";
      	break;
      }
      RQ.enq(disks.moveToReady(n));
      std::cout<< " ### Process in " << in << " is ready.\n";
      break;
    case 'C':
      if(cdrw.empty(n)){ // checks if device queue is empty
        std::cout<< " ### This device is empty.\n";
        break;
      } else if(n > cdrw.devices()){ // dne flag
      	std::cout<< " ### This device doesn't exist.\n";
      	break;
      }
      RQ.enq(cdrw.moveToReady(n));
      std::cout<< " ### Process in " << in << " is ready.\n";
      break;
    case 'P':
      if(printers.empty(n)){ // checks if device queue is empty
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
    case 'T':
      if(current == nullptr){
        std::cout << " ### No process to time out.\n";
        break;
      }
      timer('T');
      std::cout << " ### Process has been removed.\n"; // no
      RQ.enq(current);
      current = nullptr;
      break;
    default:
      std::cout<< " ### Invalid input.\n";
    }
    return cpuStatus();
  }

  void terminate(){
    float average = current->cpuTime / current->totalCpu;
    std::cout << "  ## PID: " << current->pid 
              << "\n  ## Average CPU burst: " << (current->totalCpu==0 ? 0 : average)
              << "ns\n  ## Total times in CPU: " << current->totalCpu << std::endl;
    delete current;
    current = nullptr;
    std::cout << " ### Process in CPU has been terminated.\n";
  }

  void cpuStatus(){ // checks what's going on in CPU, adds process to CPU when empty.
    if(current==nullptr && !RQ.empty()){
        current = RQ.peek();
        RQ.dq();
        std::cout<< " ### Process is now in CPU.\n";
    } else if(current == nullptr)
      std::cout << " ### CPU is idle.\n";

    return systemCall();
  }

  void snapShot(){ // calls print for specified devices
    char s;
    std::cout<< "  ## Please select r,d,c,p: ";
    std::cin>> s;
    std::cout << "PID" << std::setw(12) <<"Filename"<<std::setw(12)<< "Memstart"
              <<std::setw(11)<< "R/W" <<std::setw(14)<< "File Length" <<std::setw(12)
              << "CPU time" <<std::setw(12)<< "Avg. burst\n";
    if(s=='r')
      RQ.print();
    else if(s=='d')
      disks.print();
    else if(s=='c')
      cdrw.print();
    else if(s=='p')
      printers.print();
    else
      std::cout<< " ### Invalid input. \n";

    float average = systemTime/systemTotal;

    std::cout << "\n ### Average total CPU burst time: " << (systemTotal==0 ? 0 : average) << "ns\n";

    return systemCall();
  }

  void timer(char c){ // asks for time, updates process time and system wide time
    int n = inputInt(slice," ### How much time has passed since last slice? ");
    if(n==slice && (c == 'd'|| c == 'c' || c == 'p')){
      std::cout<<" ### The timeslice for this process is over.\n";
      current->cpuTime += n;
      current->totalCpu++;
      systemTime += n;
      systemTotal++;
      RQ.enq(current);
      current = nullptr;
      return cpuStatus();
    } else if(c == 'd' || c == 'c' || c == 'p' || c == 't') {
      current->cpuTime += n;
      current->totalCpu++;
      systemTime += n;
      systemTotal++;
    } else {
      current->cpuTime += n;
      systemTime += n;
    }
  }
  
 private:
  DevQ<FSdisk> disks;
  DevQ<Queue> cdrw;
  DevQ<Queue> printers;
  Queue RQ;
  PCB* current;
  int PID;
  int slice;
  float systemTime;
  int systemTotal;

};
#endif