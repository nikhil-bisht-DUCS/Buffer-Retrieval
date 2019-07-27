#ifndef WaitQueue_hpp
#define WaitQueue_hpp

#include "Process.hpp"

class WaitQueue {
    
    struct Queue {
        
        int pid;            // PID of the process
        Queue *next;        // Pointer to the next Process in the Wait-Queue
    };
    
    
public:
    
    Queue *head;            
    
    WaitQueue();
    bool isEmpty();
    void enqueue(int pid);
    void dequeue(int pid);
    void showBlocks();
    int top();
    ~WaitQueue();
    
};

#endif
