#ifndef PRIMITIVE_JSON
#define PRIMITIVE_JSON 
#include "JsonObject.h"
#include "esp_err.h"
#include <typeinfo>

class PrimitiveJSON : public JsonObject
{
private:
    enum primitiveType
    {
        INT,
        FLOAT,
        DOUBLE,
        BOOL,
        STRING,
        CHAR,
        TIMEVAL
    };

    void* _value;
    PrimitiveJSON::primitiveType _type;

    inline std::string TimeToString(timeval* time) const
    {
        char buffer[30];
        strftime(buffer, 30, "%Y-%m-%d %H:%M:%S", localtime(&time->tv_sec));
        return std::string(buffer);
    }
    
public:
    inline PrimitiveJSON(int* value) : _value(value), _type(PrimitiveJSON::primitiveType::INT) {};
    inline PrimitiveJSON(float* value) : _value(value), _type(PrimitiveJSON::primitiveType::FLOAT) {};
    inline PrimitiveJSON(double* value) : _value(value), _type(PrimitiveJSON::primitiveType::DOUBLE) {};
    inline PrimitiveJSON(bool* value) : _value(value), _type(PrimitiveJSON::primitiveType::BOOL) {};
    inline PrimitiveJSON(std::string* value) : _value(value), _type(PrimitiveJSON::primitiveType::STRING) {};
    inline PrimitiveJSON(const char* value) : _value((void*)value), _type(PrimitiveJSON::primitiveType::CHAR) {};
    inline PrimitiveJSON(timeval* value) : _value((void*)value), _type(PrimitiveJSON::primitiveType::TIMEVAL) {};

    inline std::string ToJson() const override
    {
        std::string json = "{\"value\":";
        switch (_type)
        {
        case PrimitiveJSON::primitiveType::INT:
            json += std::to_string(*((int*)_value));
            break;
        case PrimitiveJSON::primitiveType::FLOAT:
            json += std::to_string(*((float*)_value));
            break;
        case PrimitiveJSON::primitiveType::DOUBLE:
            json += std::to_string(*((double*)_value));
            break;
        case PrimitiveJSON::primitiveType::BOOL:
            json += (*((bool*)_value)) ? "true" : "false";
            break;
        case PrimitiveJSON::primitiveType::STRING:
            json += "\"" + *((std::string*)_value) + "\"";
            break;
        case PrimitiveJSON::primitiveType::CHAR:
            json += "\"" + std::string((char*)_value) + "\"";
            break;
        case PrimitiveJSON::primitiveType::TIMEVAL:
            json += "\"" + TimeToString((timeval*)_value) + "\"";
            break;
        default:
            break;
        }
        json += "}";
        return json;
    }
};
#endif