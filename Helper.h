/*
 * Helper.h
 * Zoilo Mercedes	
 */
#ifndef HELPER_H
#define HELPER_H
#define <string>

const int MAX = 10;

struct PCB{
  int pid;
  std::string mm;
  int state;
  std::string ofl;
  std::string unique;
  PCB* next;
  PCB* prev;

  PCB(int p, std::string m, int s, std::string o, std::string u)
  : pid{p}, mm{m}, state{s}, ofl{o}, unique{u}, next{nullptr}, prev{nullptr} {}
};

#endif