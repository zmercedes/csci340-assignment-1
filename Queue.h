/*
 * Queue.h
 * Zoilo Mercedes	
 */
#ifndef QUEUE_H
#define QUEUE_H
#include <iostream>
#include <iomanip>
#include "Helper.h"

class Queue {
 public:
 Queue(): head {nullptr}, tail {nullptr}, size{0} {}

  ~Queue(){ empty(); }

  bool enq(PCB* top){

    if(head==nullptr){
      tail = top;
      head = top;
      size++;
      return true;
    }
    else{
      PCB* tmp = head;
      top->next = tmp;
      tmp->prev = top;
      head = top;
      size++;
      return true;
    }
    return false;
  }

  bool dq(){
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
    }

    return false; // empty queue
  }

  PCB* peek(){
    return tail;
  }

  void empty(){
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

  void print(std::ostream& out = std::cout) const {
    if(head!=nullptr){
      PCB* tmp = head;
      //   out << setw(10);
      while(tmp != nullptr){
        out << tmp->pid <<std::setw(12) << tmp->mm << std::setw(12) << tmp->state
            << std::setw(12) << tmp->ofl << std::setw(12) << tmp->unique << std::endl;
        tmp = tmp->next;
      }
    } else
      std::cout << "-"<< std::setw(12) <<"-"<< std::setw(12) <<"-"
                << std::setw(12) <<"-"<< std::setw(12) <<"-\n";
  }

  friend std::ostream& operator<<(std::ostream& out,const Queue& rhs){
    rhs.print(out);
    return out;
  }

 private:
  PCB* head;
  PCB* tail;
  int size;
};
#endif