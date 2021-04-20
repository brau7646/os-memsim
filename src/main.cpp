#include <iostream>
#include <string>
#include <cstring>
#include "mmu.h"
#include "pagetable.h"

void printStartMessage(int page_size);
void createProcess(int text_size, int data_size, Mmu *mmu, PageTable *page_table);
void allocateVariable(uint32_t pid, std::string var_name, DataType type, uint32_t num_elements, Mmu *mmu, PageTable *page_table);
void setVariable(uint32_t pid, std::string var_name, uint32_t offset, void *value, Mmu *mmu, PageTable *page_table, void *memory);
void freeVariable(uint32_t pid, std::string var_name, Mmu *mmu, PageTable *page_table);
void terminateProcess(uint32_t pid, Mmu *mmu, PageTable *page_table);
void splitString(std::string text, char d, std::vector<std::string>& result);

int main(int argc, char **argv)
{
    // Ensure user specified page size as a command line parameter
    if (argc < 2)
    {
        fprintf(stderr, "Error: you must specify the page size\n");
        return 1;
    }

    // Print opening instuction message
    int page_size = std::stoi(argv[1]);
    printStartMessage(page_size);

    // Create physical 'memory'
    uint32_t mem_size = 67108864;
    void *memory = malloc(mem_size); // 64 MB (64 * 1024 * 1024)

    // Create MMU and Page Table
    Mmu *mmu = new Mmu(mem_size);
    PageTable *page_table = new PageTable(page_size);

    // Prompt loop
    std::string command;
    std::cout << "> ";
    std::getline (std::cin, command);
    std::vector<std::string> command_args;
    while (command != "exit") {
        // Handle command
        // TODO: implement this!
        splitString(command, ' ', command_args);

        //Handling each individual command and its arguments
        //create
        if (command_args[0].compare("create")==0){
            createProcess(std::stoi(command_args[1]),std::stoi(command_args[2]),mmu,page_table);
        }
        //allocate
        else if (command_args[0].compare("allocate")==0){
            //Char, Short, Int, Float, Long, Double
            DataType type;
            if (command_args[03].compare("char")==0){
                type = DataType::Char;
            }
            else if (command_args[03].compare("short")==0){
                type = DataType::Short;
            }
            else if (command_args[03].compare("int")==0){
                type = DataType::Int;
            }
            else if (command_args[03].compare("float")==0){
                type = DataType::Float;
            }
            else if (command_args[03].compare("long")==0){
                type = DataType::Long;
            }
            else {
                type = DataType::Double;
            }
        
            allocateVariable(std::stoul(command_args[1]), command_args[2],DataType::Int,std::stoul(command_args[4]), mmu, page_table);
        }
        //set
        else if (command_args[0].compare("set")==0){
            setVariable(std::stoul(command_args[1]), command_args[2], std::stoul(command_args[3]), (void*)(command_args[4].c_str()), mmu, page_table, memory);
        }
        //free
        else if (command_args[0].compare("free")==0){
            freeVariable(std::stoul(command_args[1]), command_args[2], mmu, page_table);
        }
        //terminate
        else if (command_args[0].compare("terminate")==0){
            terminateProcess(std::stoul(command_args[1]), mmu, page_table);
        }
        //print
        else if (command_args[0].compare("print")==0){
            if (command_args[1].compare("mmu")==0){
                mmu->print();
            }
            else if (command_args[1].compare("page")==0){
                page_table->print();
            }
        }
        //invalid command
        else {
            printf("error: command not recognized\n");
        }

        // Get next command
        std::cout << "> ";
        std::getline (std::cin, command);
        
    }

    // Cean up
    free(memory);
    delete mmu;
    delete page_table;

    return 0;
}

void printStartMessage(int page_size)
{
    std::cout << "Welcome to the Memory Allocation Simulator! Using a page size of " << page_size << " bytes." << std:: endl;
    std::cout << "Commands:" << std:: endl;
    std::cout << "  * create <text_size> <data_size> (initializes a new process)" << std:: endl;
    std::cout << "  * allocate <PID> <var_name> <data_type> <number_of_elements> (allocated memory on the heap)" << std:: endl;
    std::cout << "  * set <PID> <var_name> <offset> <value_0> <value_1> <value_2> ... <value_N> (set the value for a variable)" << std:: endl;
    std::cout << "  * free <PID> <var_name> (deallocate memory on the heap that is associated with <var_name>)" << std:: endl;
    std::cout << "  * terminate <PID> (kill the specified process)" << std:: endl;
    std::cout << "  * print <object> (prints data)" << std:: endl;
    std::cout << "    * If <object> is \"mmu\", print the MMU memory table" << std:: endl;
    std::cout << "    * if <object> is \"page\", print the page table" << std:: endl;
    std::cout << "    * if <object> is \"processes\", print a list of PIDs for processes that are still running" << std:: endl;
    std::cout << "    * if <object> is a \"<PID>:<var_name>\", print the value of the variable for that process" << std:: endl;
    std::cout << std::endl;
}

void createProcess(int text_size, int data_size, Mmu *mmu, PageTable *page_table)
{
    // TODO: implement this!
    //   - create new process in the MMU
    uint32_t pid = mmu->createProcess();
    //   - allocate new variables for the <TEXT>, <GLOBALS>, and <STACK>
    //load that file into memory (<TEXT>)
    allocateVariable(pid,"<TEXT>", DataType::Char,text_size,mmu,page_table);
    //if program has any global variables they also need to be loaded into memory <GLOBALS>
    allocateVariable(pid,"<GLOBAL>", DataType::Char,data_size,mmu,page_table);
    //set amount of memory where local variables are saved <STACK>
    allocateVariable(pid,"<STACK>", DataType::Char,65536,mmu,page_table);
    //
    //   - print pid
    std::cout<<pid<<std::endl;
}

void allocateVariable(uint32_t pid, std::string var_name, DataType type, uint32_t num_elements, Mmu *mmu, PageTable *page_table)
{
    //std::cout<<type<<std::endl;
    // TODO: implement this!
    //   - find first free space within a page already allocated to this process that is large enough to fit the new variable
    
    //   - if no hole is large enough, allocate new page(s)

    //   - insert variable into MMU
    mmu->addVariableToProcess(pid, var_name, type, num_elements, 0);

    //   - print virtual memory address 
}

void setVariable(uint32_t pid, std::string var_name, uint32_t offset, void *value, Mmu *mmu, PageTable *page_table, void *memory)
{
    // TODO: implement this!
    //   - look up physical address for variable based on its virtual address / offset
    //   - insert `value` into `memory` at physical address
    //   * note: this function only handles a single element (i.e. you'll need to call this within a loop when setting
    //           multiple elements of an array) 
}

void freeVariable(uint32_t pid, std::string var_name, Mmu *mmu, PageTable *page_table)
{
    // TODO: implement this!
    //   - remove entry from MMU
    //   - free page if this variable was the only one on a given page
}

void terminateProcess(uint32_t pid, Mmu *mmu, PageTable *page_table)
{
    // TODO: implement this!
    //   - remove process from MMU
    //   - free all pages associated with given process
}
void splitString(std::string text, char d, std::vector<std::string>& result)
{
    enum states { NONE, IN_WORD, IN_STRING } state = NONE;

    int i;
    std::string token;
    result.clear();
    for (i = 0; i < text.length(); i++)
    {
        char c = text[i];
        switch (state) {
            case NONE:
                if (c != d)
                {
                    if (c == '\"')
                    {
                        state = IN_STRING;
                        token = "";
                    }
                    else
                    {
                        state = IN_WORD;
                        token = c;
                    }
                }
                break;
            case IN_WORD:
                if (c == d)
                {
                    result.push_back(token);
                    state = NONE;
                }
                else
                {
                    token += c;
                }
                break;
            case IN_STRING:
                if (c == '\"')
                {
                    result.push_back(token);
                    state = NONE;
                }
                else
                {
                    token += c;
                }
                break;
        }
    }
    if (state != NONE)
    {
        result.push_back(token);
    }
}