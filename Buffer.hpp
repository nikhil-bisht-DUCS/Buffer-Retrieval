#ifndef Buffer_hpp
#define Buffer_hpp

#include<iostream>
#define SIZE 512

enum Status {FREE, BUSY, DELAYED_WRITE};


class Buffer {

    struct BufferHeader {
        
        int block_no;
        Status status;
        
        Buffer *next_buffer_hashQ;
        Buffer *prev_buffer_hashQ;
        
        Buffer *next_buffer_freeList;
        Buffer *prev_buffer_freeList;
        
    };
    
public:
    
    static int block_count;
    
    const char *memory;
    BufferHeader *header;
    
    Buffer();
    void setFree();
    void setBusy();
    void resetPointers_freeList();
    ~Buffer();
};




#endif
