#include "pagetable.h"

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
    //printf("key size: %ld\n",keys.size());

    for (i = 0; i < keys.size(); i++)
    {
        // TODO: print all pages
        std::cout<<keys[i]<<" "<<_table[keys[i]]<<std::endl;
    }
}
int PageTable::getPageSize()
{
    return _page_size;
}