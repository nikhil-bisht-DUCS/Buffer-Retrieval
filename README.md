Advanced Operating Systems 
 		 	Buffer Cache Project


Project By:-

Nikhil Bisht
MCA Batch 2017 – 2020





Problem Statement:-

Implementation and simulation of the getblk() algorithm to demonstrate how a process retrieves a buffer under various scenarios.


Implementation:-

•	The program has been created in C++.

•	Threads have been implemented to simulate how multiple processes; working concurrently, attempt to retrieve Buffer through the getblk() method.

•	Doubly Linked-List has been used predominantly as the data structure for Hash-Queues and Freelist.

•	A Wait-Queue has also been implemented as a         Singly Linked-List, to hold and prioritize Processes which go into Sleep due to non-retrieval of their requested Buffer.


Entities involved in the Program:-


•	Buffers
•	Processes
•	Hash-Queues
•	Freelist
•	Wait-Queue



Program Files:-


•	main.cpp:-

The main program which will run the whole process.


Auxiliary Files:-

•	Buffer.cpp & Buffer.hpp

•	Hash_Queue.cpp & Hash_Queue.hpp

•	Free_List.cpp & Free_List.hpp

•	Process.cpp & Process.hpp

•	WaitQueue.cpp & WaitQueue.hpp



Each respective header file “.hpp” contains a class which encapsulates the required variables and function declarations.

The function definitions and variable initializations of the said classes are defined in their respective “.cpp” file.
getblk() Cases Handled:-


•	Block found in Hash-Queue and its Buffer is free.

•	Block not found in Hash-Queue, so a Buffer from the Freelist is fetched.

•	Block not found in Hash-Queue, and the requested Buffer from the Freelist is marked for Delayed-Write.

•	Block not found in Hash-Queue, and the Freelist is empty.

•	Block found in Hash-Queue, but its Buffer is currently busy.





Process Flow:-


•	Number of Buffers, Hash-Queues, and participating processes are specified here:-


#define BUFFER 10           // No. of Buffers
#define QUEUE 4             // No. of Queues
#define PROCESS 30          // No. of participating Processes





•	Hash-Queues, Freelist, Processes, and Wait-Queue are initialized:-

 
•	Hash-Queue, Process, Freelist, and Wait-Queue class objects will be involving the transferring and processing of the Buffer class objects primarily.


•	All the processes are passed onto a function which will run the actual process of managing various processes and buffer retrievals. This function is executed through the use of Threads.



thread *threads = new thread[PROCESS];
    
    for(int i = 0; i < PROCESS; i++)
    {  threads[i] = thread(buffer_process, i);
       this_thread::sleep_for(2s);
    }


Threads are created based on the number of participating processes. 
Each process executes in their own respective thread.
These threads run in-parallel to one another. 

The start time of each consecutive thread has been set to 2 sec. after the previous one, to prevent overlapping or abrupt display of output messages, and to properly show the process flow.


The program terminates after all the Processes have released their Buffer, after writing it off.


for(int i = 0; i < PROCESS; i++)
    {  threads[i].join(); }


Basically, at the end of the program, the Wait-Queue should be Empty and the Freelist should have all the Buffers back.






Program sequence:-


•	For the Buffer Retrieval Process, the Block is obtained from the Head of the Freelist.

•	During the search for the requested block:-

o	The relevant Hash Queue is checked first. 
If the Buffer is found in the Hash Queue, and it’s status
is Free, then it is allocated to the Process.

o	If the Hash-Queue Buffer is Busy, then the current Process goes to sleep until the Process holding that particular Buffer releases it.
 
If the Block is not in that particular Hash Queue altogether, 
Then further down the line, the Freelist is checked.


o	If the Freelist is holding the Buffer, then it is checked whether
Buffer’s status is marked as Free or Delayed-Write.

o	If the Buffer’s status is Free, then it is allocated to the Process.

o	If it is marked for Delayed-Write, then an attempt to retrieve the next Buffer from the Freelist is made.

o	If the Freelist is found to be Empty, then the Process goes to sleep until some other process releases its Buffer.


•	If a Block exists in both Hash-Queue and the Freelist, it merely means that the Block is not Busy and is available for use.

•	Released Buffers are appended to the End of the Freelist.

•	If a Delayed-Write Buffer gets Written-off,
then the specific Buffer is appended to the Head of the Freelist.

