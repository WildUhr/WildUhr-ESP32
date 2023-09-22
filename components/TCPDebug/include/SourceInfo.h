#ifndef SOURCE_INFO_H
#define SOURCE_INFO_H

struct SourceInfo
{
    const char* file;
    int line;
    const char* func;

    SourceInfo(const char* file, int line, const char* func)
    {
        this->file = file;
        this->line = line;
        this->func = func;
    }
};

#endif
