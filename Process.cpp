#include "Process.hpp"
#include<thread>

using namespace std;


int Process::process_count = 1;


Process::Process() {
    
    pid = process_count++;
    memory_buffer = NULL;
    state = AWAKE;
    
    in_queue = false;
}


bool Process::hasBuffer() {
    
    if(memory_buffer != NULL)
    { return true; }
    
    return false;
}


const char* Process::getProcessData() {
    
    return memory_buffer->memory;
}


void Process::storeData(const char *data) {
    
    memory_buffer->memory = data;
}


int Process::getBlockNo() {
    
    return memory_buffer->header->block_no;
}




void Process::sleep() {
    
    state = SLEEP;
    
}


void Process::wakeup() {
    
    state = AWAKE;
    
}

bool Process::isAsleep() {
    
    if(state == SLEEP)
    { return true; }
    
    return false;
}


Process::~Process() {
    
    delete memory_buffer;
}


