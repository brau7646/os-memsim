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
        if (command.compare("")==0){

        }
        //create
        else if (command_args[0].compare("create")==0){
            if (command_args.size()<3){
                printf("error: not enough parameters\n");
            }
            else {
            createProcess(std::stoi(command_args[1]),std::stoi(command_args[2]),mmu,page_table);
            }
        }
        //allocate
        else if (command_args[0].compare("allocate")==0){
            if (command_args.size()<5){
                printf("error: not enough parameters\n");
            }
            else {
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
            
                allocateVariable(std::stoul(command_args[1]), command_args[2],type,std::stoul(command_args[4]), mmu, page_table);
                printf("%d\n",mmu->fetchVirtualAddress(std::stoul(command_args[1]),command_args[2]));
            }
        }
        //set
        else if (command_args[0].compare("set")==0){
            if (command_args.size()<5){
                printf("error: not enough parameters\n");
            }
            else {
            //std::cout<<(command_args[4].c_str())<<std::endl;
            DataType type = mmu->getType(std::stoul(command_args[1]),command_args[2]);
            int offset = std::stoul(command_args[3]);
            for (int i = 4; i < command_args.size(); i++){
                if(type == DataType::Char){
                    const char * valueArr= command_args[i].c_str();
                    char value = valueArr[0];
                    setVariable(std::stoul(command_args[1]), command_args[2], offset, &value, mmu, page_table, memory);
                    offset++;

                }
                else if (type == DataType::Short){
                    short value = std::stoi(command_args[i].c_str());
                    setVariable(std::stoul(command_args[1]), command_args[2], offset, &value, mmu, page_table, memory);
                    offset++;
                }
                else if (type == DataType::Int){
                    int value = std::stoi(command_args[i].c_str());
                    setVariable(std::stoul(command_args[1]), command_args[2], offset, &value, mmu, page_table, memory);
                    offset++;
                }
                else if (type == DataType::Float){
                    float value = std::stof(command_args[i].c_str());
                    setVariable(std::stoul(command_args[1]), command_args[2], offset, &value, mmu, page_table, memory);
                    offset++;
                }
                else if (type == DataType::Long){
                    long value = std::stol(command_args[i].c_str());
                    setVariable(std::stoul(command_args[1]), command_args[2], offset, &value, mmu, page_table, memory);
                    offset++;
                } 
                else{
                    double value = std::stod(command_args[i].c_str());
                    setVariable(std::stoul(command_args[1]), command_args[2], offset, &value, mmu, page_table, memory);
                    offset++;
                }

            }
            

            //Char, Short, Int, Float, Long, Double, FreeSpace
            }
            
        }
        //free
        else if (command_args[0].compare("free")==0){
            if (command_args.size()<3){
                printf("error: not enough parameters\n");
            }
            else {
            freeVariable(std::stoul(command_args[1]), command_args[2], mmu, page_table);
            }
        }
        //terminate
        else if (command_args[0].compare("terminate")==0){
            if (command_args.size()<2){
                printf("error: not enough parameters\n");
            }
            else {
            terminateProcess(std::stoul(command_args[1]), mmu, page_table);
            }
        }
        //print
        else if (command_args[0].compare("print")==0){
            if (command_args.size()<2){
                printf("error: not enough parameters\n");
            }
            else {
            if(command_args[1].compare("mmu")==0){
                mmu->print();
            }
            else if(command_args[1].compare("page")==0){

                page_table->print();
            }
            else if(command_args[1].compare("processes")==0){
                mmu->printProcesses();
            }
            else{
                
                std::vector<std::string> pid_args;
                splitString(command_args[1],':', pid_args);
                //int Npid = std::stoui(command_args);
                uint32_t var_virtual_address = mmu->fetchVirtualAddress(std::stoul(pid_args[0]), pid_args[1]);
                uint32_t var_physical_address = page_table->getPhysicalAddress(std::stoul(pid_args[0]),var_virtual_address);
                //std::string var_value;
                //memory[var_physical_address];
                //std::cout<<memory[var_physical_address].c_str()<<std::endl;
                //printf("%p\n",memory[var_physical_address]);
                void* addr = (uint8_t*)memory + var_physical_address;
                void* printVar;
                uint8_t copySize = mmu->getVarDataType(std::stoi(pid_args[0]),pid_args[1]);
                
                //memcpy(&printVar,addr,copySize);
                DataType type = mmu->getType(std::stoi(pid_args[0]),pid_args[1]);
                int variableSize = mmu->getVarSize(std::stoi(pid_args[0]),pid_args[1]);
                int numElements = variableSize/copySize;
                for (int i=0; i < numElements; i++)
                {
                    if(type == DataType::Char){
                        char printVar;
                        memcpy(&printVar,(uint8_t*)addr+(i*copySize),copySize*(i+1));
                        std::cout<<printVar;
                    }
                    else if (type == DataType::Short){
                        short printVar;
                        memcpy(&printVar,(uint8_t*)addr+(i*copySize),copySize*(i+1));
                        std::cout<<printVar;
                    }
                    else if (type == DataType::Int){
                        int printVar;
                        memcpy(&printVar,(uint8_t*)addr+(i*copySize),copySize*(i+1));
                        std::cout<<printVar;
                    }
                    else if (type == DataType::Float){
                        float printVar;
                        memcpy(&printVar,(uint8_t*)addr+(i*copySize),copySize*(i+1));
                        std::cout<<printVar;
                    }
                    else if (type == DataType::Long){
                        long printVar;
                        memcpy(&printVar,(uint8_t*)addr+(i*copySize),copySize*(i+1));
                        std::cout<<printVar;
                    } 
                    else{
                        double printVar;
                        memcpy(&printVar,(uint8_t*)addr+(i*copySize),copySize*(i+1));
                        std::cout<<printVar;
                        
                    }
                    if (i != numElements-1){
                        printf(", ");
                    }
                    else {
                        printf("\n");
                    }
                    
                }
                

                //printf("%d\n",printVar);

            }
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

    // Clean up
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
    int pid = mmu->createProcess();
    //   - allocate new variables for the <TEXT>, <GLOBALS>, and <STACK>
    //load that file into memory (<TEXT>)
    allocateVariable(pid,"<TEXT>", DataType::Char,text_size,mmu,page_table);
    //if program has any global variables they also need to be loaded into memory <GLOBALS>
    allocateVariable(pid,"<GLOBAL>", DataType::Char,data_size,mmu,page_table);
    //set amount of memory where local variables are saved <STACK>
    allocateVariable(pid,"<STACK>", DataType::Char,65536,mmu,page_table);
    //
    //   - print pid
    printf("%d\n",pid);
}

void allocateVariable(uint32_t pid, std::string var_name, DataType type, uint32_t num_elements, Mmu *mmu, PageTable *page_table)
{
    if (mmu->fetchVirtualAddress(pid,var_name)!=-1){
        printf("error: variable already exists\n");
        return;
    }
    if (!mmu->doesProcessExist(pid)){
        printf("error: process not found\n");
        return;
    }
    int spaceNeeded;
    if(type == DataType::Char){
        spaceNeeded = 1 * num_elements;
    }
    else if(type== DataType::Int || type== DataType::Float){
        spaceNeeded = 4 * num_elements;
    }
    else if(type== DataType::Double || type == DataType::Long){
        spaceNeeded = 8 * num_elements;
    }
    else if(type == DataType::Short){
        spaceNeeded = 2 * num_elements;
    }
    int virtualAddress = mmu->findNextAddress(pid,spaceNeeded);
    if (virtualAddress + spaceNeeded > 67108864){
        printf("error: allocation would exceed system memory");
        return;
    }
    mmu->addVariableToProcess(pid,var_name,type,spaceNeeded,virtualAddress);
    
    int i = 0;
    while (i <= (virtualAddress+spaceNeeded)/page_table->getPageSize()){
        page_table->addEntry(pid,i);
        i++;
    }
    
    
    
}

void setVariable(uint32_t pid, std::string var_name, uint32_t offset, void *value, Mmu *mmu, PageTable *page_table, void *memory)
{
    
    //   - look up physical address for variable based on its virtual address / offset
    
    uint32_t vA = mmu->fetchVirtualAddress(pid, var_name);
    if(vA == -1){
        puts("error: variable not found");
        return;
    }
    uint8_t dataSize = mmu->getVarDataType(pid, var_name);
    uint32_t addr = vA + (offset * dataSize);
    int physicalAddress = page_table->getPhysicalAddress(pid,addr);
    //printf("%d\n",physicalAddress);
    
    void* add = (uint8_t*)memory + physicalAddress;
    //std::cout<< "physical: " << physicalAddress<< " add:" << add << std::endl;
    //std::cout<< "uint: " << (uint8_t*)memory<< " &:" << &memory << std::endl;
    //void* writeLoc = add + addr;
    //   - insert `value` into `memory` at physical address
    
    //printf("%ld\n", sizeof(value));
    //std::cout<<value<<std::endl;
    
    memcpy(add,value,dataSize);
    
    void* printVar;
    memcpy(&printVar,add,dataSize);
    
    //copy from mem into some var for printing
    /*
    void* printVar;
    memcpy(&printVar,add,dataSize);
    //memcpy(printVar,(&memory+addr),sizeof(&memory+addr));
    int* test = (int*)printVar;
    printf("%d\n",*test);
    //   * note: this function only handles a single element (i.e. you'll need to call this within a loop when setting
    //           multiple elements of an array) 
    */
}

void freeVariable(uint32_t pid, std::string var_name, Mmu *mmu, PageTable *page_table)
{
    // TODO: implement this!
    //   - remove entry from MMU
    uint32_t vA = mmu->fetchVirtualAddress(pid,var_name);
    std::cout << "vSize is " << mmu->getVarSize(pid,var_name) << "\n";
    if (!mmu->doesProcessExist(pid)){
        printf("error: process not found\n");
        return;
    }
    if(vA == -1){//the entry does not exist
        puts("Error: the entry does not exist");
        return;
    }
    else{ // remove it
        int varStart = vA/page_table->getPageSize();
       
        int varEnd = (vA+mmu->getVarSize(pid, var_name))/page_table->getPageSize();
        printf("vs is %d\n",mmu->getVarSize(pid,var_name));
        
        
        std::cout << "end is " << varEnd << "\n";
        std::cout << "start is " << varStart << "\n";

        if(varEnd-varStart >= 1){ //it spans more than one page
            /*
            int start = varStart;

            int sMod = vA% page_table->getPageSize();
            int eMod = (vA + mmu->getVarSize(pid, var_name)) % page_table->getPageSize();

            std::cout << "smod is " << sMod << "\n";
            std::cout << "emod is " << eMod << "\n";


            if(sMod > eMod){ //this means that the variable crosses over into at least one new page
                puts("deleted");
                
                page_table->removeEntry(pid,var_name,varStart+1);
            }
            */
            int currPage = varStart;
            int currAddr = ((vA / page_table->getPageSize()) +1)* (page_table->getPageSize());
            std::cout << "currAddr is " << currAddr << "\n";
            int endAddr = (vA + mmu->getVarSize(pid,var_name));
            std::cout << "endAddr is " << endAddr << "\n";
            while(currAddr < endAddr){
                page_table->removeEntry(pid,var_name,currPage+1);
                currAddr = currAddr + page_table->getPageSize();
                std::cout << "currP is " << currPage << "\n";
                currPage++;

            }
            //page_table->removeEntry(pid,var_name,varStart+1);
            /*
            while(start < varEnd){
                if(vA%page_table->getPageSize() == 0){//remove the page
                    puts("in 1");
                    page_table->removeEntry(pid,var_name,varStart);
                }
                else if((vA + mmu->getVarSize(pid,var_name)) % page_table->getPageSize() == 1){
                    puts("in 2");
                    int begin = varStart;
                    while(begin < varEnd){

                        page_table->removeEntry(pid,var_name,begin);
                        std::cout << "delete\n";
                        begin++;
                    }
                
                 
                }

                start++;
            }
            */


            
        }
        

    }
    mmu->setFree(pid,var_name);
    //   - free page if this variable was the only one on a given page
}

void terminateProcess(uint32_t pid, Mmu *mmu, PageTable *page_table)
{
    if (!mmu->doesProcessExist(pid)){
        printf("error: process not found\n");
        return;
    }
    // TODO: implement this!
    //   - remove process from MMU
    mmu->terminateProcess(pid);
    //   - free all pages associated with given process
    page_table->removeProcess(pid);

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