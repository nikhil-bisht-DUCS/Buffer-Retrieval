#include "Free_List.hpp"

FreeList::FreeList(int buffers) {
    
    for(int i = 0; i < buffers; i++)
    { addEnd(new Buffer); }
    
}


int FreeList::getBlockNo() {
    
    return head->header->block_no;
    
}


bool FreeList::isEmpty() {
    
    if(head == NULL)
    { return true; }
    
    return false;
}


void FreeList::addHead(Buffer *buffer) {
    
    if(!isEmpty())
    { Buffer *temp = head;
      
      buffer->header->next_buffer_freeList = temp;
      buffer->header->prev_buffer_freeList = NULL;
      
      temp->header->prev_buffer_freeList = buffer;
    }
    
    head = buffer;
}



void FreeList::addEnd(Buffer *buffer) {
    
    if(isEmpty())
    { head = buffer; }
    
    else
    {
        Buffer *temp = head;
        
        while(temp->header->next_buffer_freeList != NULL)
        { temp = temp->header->next_buffer_freeList; }
        
        temp->header->next_buffer_freeList = buffer;
        buffer->header->prev_buffer_freeList = temp;
        buffer->header->next_buffer_freeList = NULL;
    }
    
}



Buffer* FreeList::getBuffer(int block_no) {
    
    Buffer *buffer = head;
    
    while(buffer != NULL && buffer->header->block_no != block_no)
    { buffer = buffer->header->next_buffer_freeList; }
        
    return buffer;
}




void FreeList::removeBuffer(Buffer *buffer) {

    
    if(buffer == head)
    {
        if(buffer->header->next_buffer_freeList != NULL)
        { head = buffer->header->next_buffer_freeList;
          head->header->prev_buffer_freeList = NULL; }
        
        else
        { head = NULL; }
    }
    
    
    else
    {
        Buffer *prev = head;
        Buffer *next = NULL;
        
        while(prev->header->next_buffer_freeList != buffer)
        { prev = prev->header->next_buffer_freeList; }
        
        if(buffer->header->next_buffer_freeList != NULL)
        {
            next = buffer->header->next_buffer_freeList;
            
            prev->header->next_buffer_freeList = next;
            next->header->prev_buffer_freeList = prev;
        }
        
        else
        { prev->header->next_buffer_freeList = NULL; }
        
    }
    
    buffer->resetPointers_freeList();
}




void FreeList::showBlocks() {
    
    std::cout << "FreeList:" << "\t\t\t";
    
    if(isEmpty())
    { std::cout << "EMPTY" << std::endl;
      return; }
    
    Buffer *buffer = head;
    
    while(buffer != NULL)
    {
        std::cout << buffer->header->block_no << "\t";
        buffer = buffer->header->next_buffer_freeList;
    }
    
    std::cout << std::endl;
}



bool FreeList::hasBuffer(int block_no) {
    
    Buffer *buffer = head;
    bool found = false;
    
    while(buffer != NULL)
    {
        if(buffer->header->block_no == block_no)
        { found = true;
          break;
        }
        
        buffer = buffer->header->next_buffer_freeList;
    }
    
    return found;
}


FreeList::~FreeList() {
    
    delete head;
}




