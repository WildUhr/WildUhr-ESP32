#ifndef JSON_OBJECT_H
#define JSON_OBJECT_H

#include <string>

class JsonObject
{
public:
    virtual std::string ToJson() const = 0;
};

#endif