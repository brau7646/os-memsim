#ifndef __MMU_H_
#define __MMU_H_

#include <iostream>
#include <string>
#include <vector>

enum DataType : uint8_t {Char, Short, Int, Float, Long, Double, FreeSpace};

typedef struct Variable {
    std::string name;
    DataType type;
    uint32_t virtual_address;
    uint32_t size;
} Variable;

typedef struct Process {
    uint32_t pid;
    std::vector<Variable*> variables;
} Process;

class Mmu {
private:
    uint32_t _next_pid;
    uint32_t _max_size;
    std::vector<Process*> _processes;

public:
    Mmu(int memory_size);
    ~Mmu();

    uint32_t createProcess();
    void addVariableToProcess(uint32_t pid, std::string var_name, DataType type, uint32_t size, uint32_t address);
    void print();
    void terminateProcess(uint32_t pid);
    uint32_t fetchVirtualAddress(uint32_t pid, std::string var_name);
    uint32_t findNextAddress(uint32_t pid, uint32_t size);
    uint8_t getVarDataType(uint32_t pid, std::string var_name);

    bool doesProcessExist(uint32_t pid);
    DataType getType(uint32_t pid, std::string var_name);
    void printProcesses();
    int getVarSize(uint32_t pid, std::string var_name);
    void setFree(uint32_t pid, std::string var_name);
    int getProcessesSize();
    std::vector<Process*> getProcesses();
};

#endif // __MMU_H_
