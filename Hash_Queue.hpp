#ifndef Hash_Queue_hpp
#define Hash_Queue_hpp

#include "Buffer.hpp"


class HashQueue {
    
    Buffer *head;
    
public:
    
    static int count;
    int sno;
    int buffers_present;                // total count of Buffers present in the HashQueue at a particular moment
    
    HashQueue();
    void addBuffer(Buffer *buffer);     // appends Buffer at the end of the HashQueue
    Buffer *getBuffer(int block_no);    // returns Buffer with the specified block number from the HashQueue
    bool isEmpty();                     // checks whether the HashQueue is Empty or not
    bool hasBuffer(int block_no);       // checks whether the HashQueue contains the Buffer with the specified block number
    void showBlocks();                  // prints all the blocks currently present in the HashQueue
    int getRandomBlock();              // returns a random Buffer's block number from the HashQueue (for Simulation purpose)
    ~HashQueue();
    
};


#endif
