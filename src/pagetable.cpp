#include "pagetable.h"
#include <string>

PageTable::PageTable(int page_size)
{
    _page_size = page_size;
}

PageTable::~PageTable()
{
}

std::vector<std::string> PageTable::sortedKeys()
{
    std::vector<std::string> keys;

    std::map<std::string, int>::iterator it;
    for (it = _table.begin(); it != _table.end(); it++)
    {
        keys.push_back(it->first);
    }

    std::sort(keys.begin(), keys.end(), PageTableKeyComparator());

    return keys;
}

void PageTable::addEntry(uint32_t pid, int page_number)
{
    // Combination of pid and page number act as the key to look up frame number
    std::string entry = std::to_string(pid) + "|" + std::to_string(page_number);
    if (_table.count(entry)==0){
        
        //check if page already exists
        int frame = 0; 
        // Find free frame
        // TODO: implement this!
        std::map<std::string, int>::iterator it;
        //bool frame_found = false;
        while(true){

            std::vector<std::string> keys = sortedKeys();
            bool frame_is_free = true;
            for (int i = 0; i < keys.size(); i++)
            {
                if (_table[keys[i]]==frame){
                    frame_is_free = false;
                    break;
                }
            }
            if (frame_is_free){
                break;
            } else {
                frame++;
            }
        
        }
        _table[entry] = frame;
    }
    

}

void PageTable::removeEntry(uint32_t pid, std::string var_name, int pagenumber){


    std::string entry = std::to_string(pid) + "|" + std::to_string(pagenumber);

     _table.erase(entry);
    
    


    /*
    std::vector<std::string> keys = sortedKeys();
    for (int i = 0; i < keys.size(); i++)
    {
        // TODO: print all pages
        //std::cout<<keys[i]<<" "<<_table[keys[i]]<<std::endl;
        std::string pid = keys[i].substr(0,keys[i].find('|'));
        std::string pageNum = keys[i].substr(keys[i].find('|')+1);

        int pN = stoi(pageNum,nullptr,0);
        if(pN == pagenumber){//we remove it

        }
        
    }
    */
}





int PageTable::getPhysicalAddress(uint32_t pid, uint32_t virtual_address)
{
    // Convert virtual address to page_number and page_offset
    // TODO: implement this!
    int page_number = virtual_address/_page_size;
    int page_offset = virtual_address%_page_size;
    

    // Combination of pid and page number act as the key to look up frame number
    std::string entry = std::to_string(pid) + "|" + std::to_string(page_number);
    
    // If entry exists, look up frame number and convert virtual to physical address
    int address = -1;
    if (_table.count(entry) > 0)
    {
        // TODO: implement this!
        address = (_table[entry] * _page_size) + page_offset;
        
    }

    return address;
}

void PageTable::print()
{
    int i;

    std::cout << " PID  | Page Number | Frame Number" << std::endl;
    std::cout << "------+-------------+--------------" << std::endl;

    std::vector<std::string> keys = sortedKeys();

    for (i = 0; i < keys.size(); i++)
    {
        // TODO: print all pages
        //std::cout<<keys[i]<<" "<<_table[keys[i]]<<std::endl;
        std::string pid = keys[i].substr(0,keys[i].find('|'));
        std::string pageNum = keys[i].substr(keys[i].find('|')+1);

        //std::cout<<pid<<" "<<pageNum<<std::endl;
        printf(" %4s | %11s | %12d\n",pid.c_str(), pageNum.c_str(), _table[keys[i]]);
        
    }
}
int PageTable::getPageSize()
{
    return _page_size;
}
void PageTable::removeProcess(uint32_t pid)
{
    int curPage = 0;
    std::string entry = std::to_string(pid) + "|" + std::to_string(curPage);
    while (_table.count(entry) > 0){
        _table.erase(entry);
        curPage++;
        entry = std::to_string(pid) + "|" + std::to_string(curPage);
    }
}