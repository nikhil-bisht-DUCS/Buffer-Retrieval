#include "Hash_Queue.hpp"
#include "Free_List.hpp"
#include "Process.hpp"
#include "WaitQueue.hpp"
#include<thread>


// Buffer Cache program by Nikhil Bisht.
// The process flow in the program is simulated through the use of Threads.


using namespace std;


#define BUFFER 10           // No. of Buffers
#define QUEUE 4             // No. of Queues
#define PROCESS 30          // No. of participating Processes



HashQueue *hashQueue = new HashQueue[QUEUE];    //No. of HashQueues specified by the value of QUEUE
FreeList *freeList = new FreeList(BUFFER);      //Freelist is loaded with Buffers, specified by the value of BUFFER
Process *process = new Process[PROCESS];        //No. of participating Processes, specified by the value of PROESSS
WaitQueue *waitQueue = new WaitQueue;           //Wait-Queue will hold Sleeping Processes


int block_counter = 0;
int current_pid = 0;

mutex mu_lock;
condition_variable condition;


void sys_status() {
    
    cout << endl << endl;
    
    for(int i = 0; i < 150; i++)
    { cout << '-'; }
    
    cout << endl << endl << endl;
    
    for(int i = 0; i < QUEUE; i++)
    { hashQueue[i].showBlocks(); }
    
    freeList->showBlocks();
    
    cout << endl;
    waitQueue->showBlocks();
    
    cout << endl << endl;
}


void sleep_procs() {
    
    for(int i = 0; i < PROCESS; i++)
    {
        if(process[i].in_queue)
        { process[i].sleep(); }
    }
    

}


void wakeup_procs() {
    
    for(int i = 0; i < PROCESS; i++)
    {
        if(process[i].in_queue)
        { process[i].wakeup(); }
    }
    
    condition.notify_all();     // All Threads wake-up
    
}




void brelse(Buffer *locked_buffer) {
    
    wakeup_procs();
    
    
    locked_buffer->setFree();
    freeList->addEnd(locked_buffer);
    cout << "Buffer @ Block #" << locked_buffer->header->block_no << " Released..." << endl << endl;
}




void bwrite(Buffer *buffer, int pid) {
    
    if(buffer->header->status == DELAYED_WRITE)
    {
        this_thread::sleep_for(1s);
        buffer->setFree();
        freeList->addHead(buffer);
        cout << "Delayed-Write Buffer @ Block #" << buffer->header->block_no << " Written-off & Appended to the Head of the Freelist" << endl;
        
    }
    
    
    else
    {
        cout << "Process #" << pid + 1 << " releasing Buffer @ Block #" << buffer->header->block_no << "...." << endl;
        this_thread::sleep_for(1s);
        brelse(buffer);
    }
    
}




Buffer *getblk(int block_no, int pid) {
    
    int queue_no = block_no % QUEUE;
    bool buffer_found = false;
    
    Buffer *buffer = NULL;
    
    while(!buffer_found)
    {
        if(hashQueue[queue_no].hasBuffer(block_no))
        {
            Buffer *this_buffer = hashQueue[queue_no].getBuffer(block_no);
            
            if(this_buffer->header->status == BUSY)
            {
                cout << "Requested Buffer @ Block #" << block_no << " is Busy...." << endl;
                cout << "Process #" << pid + 1 << " going to Sleep...." << endl;
                
                process[pid].sleep();
                process[pid].in_queue = true;
                
                waitQueue->enqueue(process[pid].pid);
                break;
            }
            
            this_buffer->setBusy();
            freeList->removeBuffer(this_buffer);
            buffer = this_buffer;
        }
        
        
        else
        {
            cout << "Block #" << block_no << " Not in HashQueue #" << queue_no << "...." << endl;
            
            if(freeList->isEmpty())
            {
                cout << "Freelist is Empty...." << endl;
                cout << "Process is going to Sleep...." << endl;
                
                process[pid].sleep();
                process[pid].in_queue = true;
                
                waitQueue->enqueue(process[pid].pid);
                break;
            }
            
            Buffer *this_buffer = freeList->getBuffer(block_no);
            freeList->removeBuffer(this_buffer);
            
            if(this_buffer->header->status == DELAYED_WRITE)
            {
                cout << "Requested Buffer is marked for Delayed-Write...." << endl;
                cout << "Retrieving next Buffer from the Freelist...." << endl << endl;
                
                return this_buffer;
            }
            
            this_buffer->setBusy();
            hashQueue[queue_no].addBuffer(this_buffer);
            buffer = this_buffer;
        }
        
        buffer_found = true;
    }
    
    return buffer;
}




void buffer_process(int pid) {
    
    
    sys_status();
    
    if(!freeList->isEmpty())
    { block_counter = freeList->getBlockNo() ; }
    
    else
    { cout << "Freelist is Empty...." << endl;
      block_counter = (block_counter % BUFFER) + 1; }
    
    
    int request_block = block_counter;
    
    
    while(true)
    {
        
        if(process[pid].state == SLEEP)
        { unique_lock<mutex> locker(mu_lock);
            
          current_pid = pid;
            
          condition.wait(locker, [] (){ return !process[current_pid].isAsleep(); });  // Thread goes to Sleep
          cout << endl << "Process #" << process[pid].pid << " is Awake...." << endl;
          continue;
        }
        
        
        
        
        // If the given Process is in Wait-Queue and Awake, and the Freelist is Not Empty
        else if(process[pid].in_queue && process[pid].state == AWAKE && !freeList->isEmpty() )
        {
            mu_lock.lock();             // Mutex Lock------------Lock      // To avoid Race Condition
            
            if(!freeList->isEmpty())
            {
                request_block = freeList->getBlockNo();
                
                sys_status();
                
                process[pid].memory_buffer = getblk(request_block, pid);
                
                cout << "Process #" << pid + 1 << ": ";
                cout << "Retrieved Buffer from Block #" << request_block << endl;
                
                waitQueue->dequeue(process[pid].pid);
                process[pid].in_queue = false;
                
                if(!waitQueue->isEmpty())
                { sleep_procs(); }
            }
            
            mu_lock.unlock();       // Mutex Lock------------Unlock
            
            if(process[pid].memory_buffer == NULL)
            { continue; }
            
            this_thread::sleep_for(30s);
            bwrite(process[pid].memory_buffer, pid);
            break;
        }
        
        
        
        //If the Process is able to retrieve a Buffer
        else if(!process[pid].in_queue && (process[pid].memory_buffer = getblk(request_block, pid)) != NULL)
        {
            if(process[pid].memory_buffer->header->status == DELAYED_WRITE)
            {
                Buffer *delay_buffer = process[pid].memory_buffer;
                
                if(!freeList->isEmpty())
                { block_counter = freeList->getBlockNo() ; }
                
                else
                { block_counter = (block_counter % BUFFER) + 1; }
                
                request_block = block_counter;
                
                
                if((process[pid].memory_buffer = getblk(request_block, pid)) != NULL)
                {
                    cout << "Process #" << pid + 1 << ": ";
                    cout << "Retrieved Buffer from Block #" << request_block << endl;
                    
                    bwrite(delay_buffer, pid);   // Writing-off Delayed Buffer
                    
                    this_thread::sleep_for(30s);             // Each process will take 30 secs. to write-off its buffer contents
                    bwrite(process[pid].memory_buffer, pid);
                    break;
                }
                
                
                else
                {
                    process[pid].in_queue = true;
                    bwrite(delay_buffer, pid);           // Writing-off Delayed Buffer
                }
                
            }
            
            
            else
            {
                cout << "Process #" << pid + 1 << ": ";
                cout << "Retrieved Buffer from Block #" << request_block << endl;
                this_thread::sleep_for(30s);
                bwrite(process[pid].memory_buffer, pid);
                break;
            }
            
        }
        
        
    }
    
    
}


int main() {
    
    thread *threads = new thread[PROCESS];
    
    for(int i = 0; i < PROCESS; i++)
    {  threads[i] = thread(buffer_process, i);
       this_thread::sleep_for(2s);
    }
    
    sys_status();
    
    for(int i = 0; i < PROCESS; i++)
    {  threads[i].join(); }
    
    sys_status();
    
    
    cout << endl << endl;
    
    return 0;
}
