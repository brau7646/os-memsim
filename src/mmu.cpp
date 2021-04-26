#include "mmu.h"
#include <string>
#include <algorithm>

Mmu::Mmu(int memory_size)
{
    _next_pid = 1024;
    _max_size = memory_size;
}

Mmu::~Mmu()
{
}

uint32_t Mmu::createProcess()
{
    Process *proc = new Process();
    proc->pid = _next_pid;

    Variable *var = new Variable();
    var->name = "<FREE_SPACE>";
    var->type = DataType::FreeSpace;
    var->virtual_address = 0;
    var->size = _max_size;
    proc->variables.push_back(var);

    _processes.push_back(proc);

    _next_pid++;
    return proc->pid;
}

void Mmu::addVariableToProcess(uint32_t pid, std::string var_name, DataType type, uint32_t size, uint32_t address)
{
    int i;
    Process *proc = NULL;
    for (i = 0; i < _processes.size(); i++)
    {
        if (_processes[i]->pid == pid)
        {
            proc = _processes[i];
        }
    }

    Variable *var = new Variable();
    var->name = var_name;
    var->type = type;
    var->virtual_address = address;
    var->size = size;
    if (proc != NULL)
    {
        proc->variables.push_back(var);
    }
}
void Mmu::print()
{
    int i, j;

    std::cout << " PID  | Variable Name | Virtual Addr | Size" << std::endl;
    std::cout << "------+---------------+--------------+------------" << std::endl;
    for (i = 0; i < _processes.size(); i++)
    {
        for (j = 0; j < _processes[i]->variables.size(); j++)
        {

            if(_processes[i]->variables[j]->name.compare("<FREE SPACE>") == 0) //it is free space
            {

            std::cout << _processes[i]->pid << " | " << _processes[i]->variables[j]->name << " | " << _processes[i]->variables[j]->virtual_address << " | " << _processes[i]->variables[j]->size;

            }
            // TODO: print all variables (excluding <FREE_SPACE> entries)
            if (_processes[i]->variables[j]->name.compare("<FREE_SPACE>") != 0)
            {
                printf(" %3u | %13s | %12u | %4u\n",
                _processes[i]->pid,
                _processes[i]->variables[j]->name.c_str(),
                _processes[i]->variables[j]->virtual_address,
                _processes[i]->variables[j]->size);
            }
            
            //_processes[i]->variables[j]->name;
        }
    }
}
void Mmu::terminateProcess(uint32_t pid)
{
    //std::vector<Process*>::iterator position;
    for (int i=0; i<_processes.size(); i++){
        if (_processes[i]->pid == pid){
            _processes.erase(std::find(_processes.begin(),_processes.end(),_processes[i]));
            //position = std::find(_processes.begin(),_processes.end(),_processes[i]);
        }
    }
}
uint32_t Mmu::fetchVirtualAddress(uint32_t pid, std::string var_name)
{
    int position;
    for (int i=0; i<_processes.size(); i++){
        if (_processes[i]->pid == pid){
            //_processes.erase(std::find(_processes.begin(),_processes.end(),_processes[i]));
            position = i;
        }
    }
    for (int i=0; i<_processes[position]->variables.size(); i++){
        if (_processes[position]->variables[i]->name.compare(var_name)==0){
            return _processes[position]->variables[i]->virtual_address;
        }
    }
    return -1;
}
uint32_t Mmu::findNextAddress(uint32_t pid, uint32_t size)
{
    uint32_t virtual_address;
    int position;
    std::vector<Variable*> non_free_variables;
    for (int i=0; i<_processes.size(); i++){
        if (_processes[i]->pid == pid){
            position = i;
        }
    }
    for (int i=0; i<_processes[position]->variables.size(); i++){
        if (_processes[position]->variables[i]->name.compare("<FREE_SPACE>") != 0){
            non_free_variables.push_back(_processes[position]->variables[i]);
        }
    }
    for (int i=0; i<non_free_variables.size(); i++){
        if (i==non_free_variables.size()-1){
            virtual_address = non_free_variables[i]->virtual_address+non_free_variables[i]->size;
            return virtual_address;
        }
        else {
            if (size <= non_free_variables[i+1]->virtual_address - (non_free_variables[i]->virtual_address + non_free_variables[i]->size)){
                virtual_address = non_free_variables[i]->virtual_address+non_free_variables[i]->size;
                return virtual_address;
            }
        }
    }
    return 0;
}
int Mmu::getLastVariable(uint32_t pid)
{
    int position;
    for (int i=0; i<_processes.size(); i++){
        if (_processes[i]->pid == pid){
            position = i;
        }
    }
    //std::cout<<position<<std::endl;
    int numVars = _processes[position]->variables.size();
    return _processes[position]->variables[numVars-1]->virtual_address+_processes[position]->variables[numVars-1]->size-1;
}