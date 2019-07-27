#include "Hash_Queue.hpp"

int HashQueue::count = 0;


HashQueue::HashQueue() {
    
    sno = count++;
    buffers_present = 0;
    head = NULL;
}



void HashQueue::addBuffer(Buffer *buffer) {
    
    if(isEmpty())
    { head = buffer; }
        
    else
    {
        Buffer *temp = head;
        
        while(temp->header->next_buffer_hashQ != NULL)
        { temp = temp->header->next_buffer_hashQ; }
        
        temp->header->next_buffer_hashQ = buffer;
        buffer->header->prev_buffer_hashQ = temp;
        buffer->header->next_buffer_hashQ = NULL;
    }
    
    buffers_present++;
}



bool HashQueue::isEmpty() {
    
    if(head == NULL)
    { return true; }
    
    return false;
}




Buffer* HashQueue::getBuffer(int block_no) {
    
    Buffer *buffer = head;
    
    while(buffer != NULL && buffer->header->block_no != block_no)
    { buffer = buffer->header->next_buffer_hashQ; }
    
    return buffer;
}



bool HashQueue::hasBuffer(int block_no) {
    
    Buffer *buffer = head;
    bool found = false;
    
    while(buffer != NULL)
    {
        if(buffer->header->block_no == block_no)
        { found = true;
          break;
        }
        
        buffer = buffer->header->next_buffer_hashQ;
    }
    
    return found;
}



void HashQueue::showBlocks() {
    
    std::cout << "HashQueue #" << sno << ":\t\t";
    
    if(isEmpty())
    { std::cout << "EMPTY" << std::endl;
      return; }
    
    Buffer *buffer = head;
    
    while(buffer != NULL)
    {
        std::cout << buffer->header->block_no << "\t";
        buffer = buffer->header->next_buffer_hashQ;
    }
    
    std::cout << std::endl;
}



int HashQueue::getRandomBlock() {
    
    srand(static_cast<unsigned int>(time(NULL)));
    
    Buffer *buffer = head;
    
    int buffer_no = 1 + rand() % buffers_present;
    int count = 1;
    
    while(count != buffer_no)
    { buffer = buffer->header->next_buffer_hashQ;
      count++;
    }
    
    return buffer->header->block_no;
}



HashQueue::~HashQueue() {
    
    delete head;
}
