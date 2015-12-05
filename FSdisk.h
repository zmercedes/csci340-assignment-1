/*
 * Zoilo Mercedes
 * FSdisk.h
 * implementation of FSCAN scheduling for disks
 */
#ifndef FSDISK_H
#define FSDISK_H
#include "Queue.h"
#include <vector>

class FSdisk{
public:
	FSdisk() : cylinderTotal{0}, flag{true}, first{true} { }

	int getSize(){ // returns size of locked queue, might need removing
		if(!flag)
			return one.size();
		else
			return two.size();
	}

	int getCylinder() const { // returns number of cylinders
		return cylinderTotal;
	}

	bool enq(PCB* inc){ // adds PCB to request queue, returns true if success, false on nullptr
		if(inc == nullptr)
			return false;

		if(flag){
			one.push_back(inc);
			sort();
			setFirst();
			return true;
		} else {
			two.push_back(inc);
			sort();
			return true;
		}
	}

	bool empty() const{ // returns true on both queues being empty
		return one.empty() && two.empty();
	}

	bool dq(){  // removes PCB from head of locked queue, returns true if success, false otherwise
		if(flag){
			two.erase(two.begin());
			setFlag();
			setFirst();
			return true;
		} else {
			one.erase(one.begin());
			setFlag();
			setFirst();
			return true;
		}

		return false;
	}

	PCB* peek(){ // returns the PCB at the front of locked queue, nullptr if empty
		if(flag && !two.empty())
			return two[0];
		else if(!flag && !one.empty())
			return one[0];
		
		return nullptr;
	}

	void setCylinder(){ // sets number of cylinders for this disk device
		cylinderTotal = inputInt(500000,"  ## How many cylinders? ");
	}

	void print(std::ostream& out = std::cout){ // prints queues
		float average;
   		if(empty()){
      		std::cout << "-"<< std::setw(12) <<"-"<< std::setw(12) <<"-"
                << std::setw(12) <<"-"<< std::setw(12) <<"-"<< std::setw(12) 
                <<"-"<<std::setw(12) <<"-\n";
   		} else {
   			if(flag){
   				if(!one.empty()){
   					out << " ## Requests\n";
   					for(auto& o: one){
   						average = o->cpuTime/o->totalCpu;
   						out << o->pid <<std::setw(12) << o->mm << std::setw(12) << o->state
   							<< std::setw(12) << o->ofl << std::setw(12) << o->unique << std::setw(12)
            				<< o->cpuTime << std::setw(12) << (o->totalCpu==0 ? 0 : average) << std::endl;
   					}
   				}
   				if(!two.empty()){  					
   					out << " ## Being Served\n";
   					for(auto& t: two){
   						average = t->cpuTime/t->totalCpu;
   						out << t->pid <<std::setw(12) << t->mm << std::setw(12) << t->state
   							<< std::setw(12) << t->ofl << std::setw(12) << t->unique << std::setw(12)
            				<< t->cpuTime << std::setw(12) << (t->totalCpu==0 ? 0 : average) << std::endl;
   					}
   				}
   			} else {
   				if(!two.empty()){  					
   					out << " ## Requests\n";
   					for(auto& t: two){
   						average = t->cpuTime/t->totalCpu;
   						out << t->pid <<std::setw(12) << t->mm << std::setw(12) << t->state
   							<< std::setw(12) << t->ofl << std::setw(12) << t->unique << std::setw(12)
            				<< t->cpuTime << std::setw(12) << (t->totalCpu==0 ? 0 : average) << std::endl;
   					}
   				}
   				if(!one.empty()){
   					out << " ## Being Served\n";
   					for(auto& o: one){
   						average = o->cpuTime/o->totalCpu;
   						out << o->pid <<std::setw(12) << o->mm << std::setw(12) << o->state
   							<< std::setw(12) << o->ofl << std::setw(12) << o->unique << std::setw(12)
            				<< o->cpuTime << std::setw(12) << (o->totalCpu==0 ? 0 : average) << std::endl;
   					}
   				}
   			}
   		}
    }

private:
	std::vector<PCB*> one;
	std::vector<PCB*> two; 
	int cylinderTotal;
	bool flag;
	bool first;

	void setFlag(){ // sets flag to opposite value to activate/lock alternate queue
		if(flag && two.empty() || !flag && one.empty())
			flag = !flag;
	}

	void setFirst(){ // on receiving a request for the first time after being empty, immediately lock one to service the request, and allow two to collect requests.
		if(first){
			setFlag();
			first = false;
		}
		if(!first && empty()){
			flag = true;
			first = true;
		}
	}

	void sort(){ // sorts items in queue
		int i,j;
		if(empty())
			return;
		if(one.size() > 1){
			for(i=1;i<one.size();i++){
				j=i;
				while(j>0 && one[j]->cylinder <= one[j-1]->cylinder){
					if(one[j]->cylinder == one[j-1]->cylinder && one[j]->pid < one[j-1]->pid){ // tiebreaker
						std::swap(one[j],one[j-1]);
						j--;
						continue;
					}
					std::swap(one[j],one[j-1]);
					j--;
				}
			}
		}
		if(two.size() > 1){
			for(i=1;i<two.size();i++){
				j=i;
				while(j>0 && two[j]->cylinder <= two[j-1]->cylinder){
					if(two[j]->cylinder == two[j-1]->cylinder && two[j]->pid < two[j-1]->pid){ // tiebreaker
						std::swap(two[j],two[j-1]);
						j--;
						continue;
					}
					std::swap(two[j],two[j-1]);
					j--;
				}
			}
		}
	}
};
#endif