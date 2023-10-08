#ifndef JSON_OBJECT_H
#define JSON_OBJECT_H

#include <string>
#include "esp_core_dump.h"

class JsonObject
{
public:
    virtual std::string ToJson() const = 0;
};

#endif