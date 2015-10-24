/*
 * Helper.h
 * Zoilo Mercedes	
 */
#ifndef HELPER_H
#define HELPER_H
#include <iostream>
#include <string>
#include <limits.h>

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

int inputInt(int max, std::string prompt){
	int outInt;
	std::string input; 

	while(true){
		std::cout<<prompt;
		std::cin>>input;

		if(isdigit(input[0]) && input.length() < 9)
			outInt = std::stoi(input);
		else{
			std::cout<<" ### Invalid input.\n";
			continue;
		}

		if(outInt < max)
			return outInt;
		else
			std::cout<< " ### Number too large. (Max is " << max << ")\n";
	}
}
#endif