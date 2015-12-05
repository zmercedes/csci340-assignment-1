/*
 * Queue.h
 * Zoilo Mercedes	
 */
#ifndef QUEUE_H
#define QUEUE_H
#include <iomanip>
#include "Helper.h"

// template <typename obj>
class Queue {
public:
 Queue(): head {nullptr}, tail {nullptr}, size{0} {}

  ~Queue(){ clear(); }

  bool enq(PCB* top){ // returns true on top insert to queue, false on top being nullptr
    if(top == nullptr)
      return false;

    if(head==nullptr){
      tail = top;
      head = top;
      size++;
      return true;
    } else {
      PCB* tmp = head;
      top->next = tmp;
      tmp->prev = top;
      head = top;
      size++;
      return true;
    }
  }

  bool dq(){ // returns true on removal of pcb from queue, false on empty queue
    if(head==tail && head!=nullptr){
      tail = nullptr;
      head = nullptr;
      size--;
      return true;
    } else {
      PCB* tmp = tail->prev;
      tmp->next = nullptr;
      tail->prev = nullptr;
      tail = tmp;
      size--;
      return true;
    }

    return false; 
  }

  PCB* peek(){ // returns next pcb at tail of queue
    return tail;
  }

  int getSize(){ // returns size of queue
    return size;
  }

  int getCylinder() const { // returns 0 (function belongs to FSdisk.h)
    return 0;
  }

  bool empty(){ // returns true on queue being empty
    return head==nullptr;
  }

  void print(std::ostream& out = std::cout){ // prints all nodes in queue.
    float average;
    if(!empty()){
      PCB* tmp = head;
      while(tmp != nullptr){
        average = tmp->cpuTime/tmp->totalCpu;
        out << tmp->pid <<std::setw(12) << tmp->mm << std::setw(12) << tmp->state
            << std::setw(12) << tmp->ofl << std::setw(12) << tmp->unique << std::setw(12)
            << tmp->cpuTime << std::setw(12) << (tmp->totalCpu==0 ? 0 : average) << std::endl;
        tmp = tmp->next;
      }
    } else
      std::cout << "-"<< std::setw(12) <<"-"<< std::setw(12) <<"-"
                << std::setw(12) <<"-"<< std::setw(12) <<"-"<< std::setw(12) 
                <<"-"<<std::setw(12) <<"-\n";
  }

private:
  PCB* head;
  PCB* tail;
  int size;

  void clear(){ // clears queue, leaves empty queue.
    PCB* tmp;
    while(head != nullptr){
      tmp=head->next;
      delete head;
      head = tmp;
    }
    head = nullptr;
    tail = nullptr;
    size = 0;
  }
};
#endif