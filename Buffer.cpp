#include "Buffer.hpp"

int Buffer::block_count = 1;

Buffer::Buffer() {
    
    memory = new char[SIZE];
    header = new BufferHeader;
    
    header->block_no = block_count++;
    
    if(header->block_no == 3 || header->block_no == 6 || header->block_no == 8)
    { header->status = DELAYED_WRITE; }
   
    else
    { header->status = FREE; }
    
    header->next_buffer_hashQ = NULL;
    header->prev_buffer_hashQ = NULL;
    
    header->next_buffer_freeList = NULL;
    header->prev_buffer_freeList = NULL;
}


void Buffer::setBusy() {
    
    header->status = BUSY;
}


void Buffer::setFree() {
    
    header->status = FREE;
}


void Buffer::resetPointers_freeList() {
    
    header->next_buffer_freeList = NULL;
    header->prev_buffer_freeList = NULL;
    
}


Buffer::~Buffer() {
    
    delete[] memory;
    delete header;
}
