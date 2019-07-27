#ifndef Free_List_hpp
#define Free_List_hpp

#include "Buffer.hpp"

class FreeList {
    
    Buffer *head;
    
public:
    
    FreeList(int buffers);
    int getBlockNo();                       // returns the block number of the Buffer present at the head of the Freelist
    bool isEmpty();                         // checks whether Freelist is empty or not
    void addHead(Buffer *buffer);           // appends Buffer at the head of the Freelist
    void addEnd(Buffer *buffer);            // appends Buffer at the end of the Freelist
    Buffer *getBuffer(int block_no);        // returns Buffer with the specified block number
    void removeBuffer(Buffer *buffer);      // removes the specified Buffer from the Freelist
    void showBlocks();                      // prints all the blocks currently present in the Freelist
    bool hasBuffer(int block_no);
    ~FreeList();
    
};


#endif 
