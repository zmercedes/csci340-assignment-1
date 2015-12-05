Zoilo Mercedes
CSCI340 Assignment 1 README

g++ -std=c++11 CPUdriver.cpp -o run.me is all you need to run this program. 

Running the program will make it enter sysgen section, where it will ask for the number of disks, CD/RWs, and printers you'd like to add; the maximum per device is 10. After this, you will get a hash prompt " # " in which you can input the commands 

A: generates a new process and moves it into the ready queue.
t: terminates process in the cpu.
d#, c#, p#: # being some number 1-10, lower case letters represent the process generating a process in a device (disk, printer, CD/RW) which was specified during the sysgen.
D#, C#, P#: upper case letters represent an interrupt from the device which has a process in it. This process is moved to the ready queue.
S: uppercase S requests a snapshot of a device which you specify (prints all the processes the device has in its queue.)
T: uppercase T sends the process currently in the CPU to the back of the ready queue. 

When a process reaches the ready queue, if the CPU is not currently running a process, the cpu will accept the next incoming process from the ready queue. Device queues are FIFO, ready queue is FIFO with round robin scheduling, and disk queues are implemented with FSCAN. CPU accounting information has also been added to processes and CPU.