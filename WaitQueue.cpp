#include "WaitQueue.hpp"

WaitQueue::WaitQueue() {
    
    head = NULL;
}


bool WaitQueue::isEmpty() {
    
    if(head == NULL)
    { return true; }
    
    return false;
}


void WaitQueue::enqueue(int pid) {
    
    Queue *queue = new Queue;
    
    queue->pid = pid;
    queue->next = NULL;
    
    if(isEmpty())
    { head = queue; }
    
    else
    {
        Queue *temp = head;
        
        while(temp->next != NULL)
        { temp = temp->next; }
        
        temp->next = queue;
    }
    
}


void WaitQueue::dequeue(int pid) {
    
    Queue *temp = head;
    
    while(temp != NULL && temp->pid != pid)
    { temp = temp->next; }
    
    if(temp == head)
    {
        if(temp->next == NULL)
        { head = NULL; }
        
        else
        { head = head->next; }
    }
    
    else
    {
        Queue *prev = head;
        
        while(prev->next != temp)
        { prev = prev->next; }
       
        prev->next = temp->next;
    }
    
    delete temp;
}




void WaitQueue::showBlocks() {
    
    std::cout << "WaitQueue(PID):" << "\t\t";
    
    if(isEmpty())
    { std::cout << "EMPTY" << std::endl;
      return; }
    
    Queue *queue = head;
    
    while(queue != NULL)
    {
        std::cout << queue->pid << "\t";
        queue = queue->next;
    }
    
    std::cout << std::endl;
}



int WaitQueue::top() {
    
    return head->pid - 1;
}




WaitQueue::~WaitQueue() {
    
    delete head;
}
