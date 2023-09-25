
#ifndef TIME_OBJECT_H
#define TIME_OBJECT_H 
#include "JsonObject.h"
#include "DisplayTime.h"

class TimeJSON : public JsonObject
{
private:
    DisplayTime _time;
public:
    inline TimeJSON(DisplayTime time) : _time(time) {};
    inline std::string ToJson() const override
    {
        return "{\"hour\":" + std::to_string(_time.hour) + ",\"minute\":" + std::to_string(_time.minute) + "}";
    }
};
#endif