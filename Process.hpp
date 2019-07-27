#ifndef Process_hpp
#define Process_hpp

#include "Buffer.hpp"


enum State {SLEEP, AWAKE};


class Process {
    
public:
    
    static int process_count;

    int pid;
    Buffer *memory_buffer;
    State state;
    
    bool in_queue;
    
    Process();
    bool hasBuffer();                       // checks whether the process contains a Buffer
    const char *getProcessData();           // returns the information stored in the memory_buffer
    void storeData(const char *data);       // stores information in the memory_buffer
    int getBlockNo();                       // returns the block number of the Buffer held by the Process
    void sleep();                           // sets the process state to SLEEP
    void wakeup();                          // sets the process state to AWAKE
    bool isAsleep();                    
    ~Process();
    
};

#endif
