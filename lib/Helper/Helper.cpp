#include "Helper.h"

std::vector<std::string> *splitstr(std::string str, std::string deli)
{
    std::vector<std::string> *splitVec = new std::vector<std::string>();
    int start = 0;
    int end = str.find(deli);
    while (end != -1)
    {
        splitVec->push_back(str.substr(start, end - start));
        start = end + deli.size();
        end = str.find(deli, start);
    }
    splitVec->push_back(str.substr(start, end - start));

    return splitVec;
}
