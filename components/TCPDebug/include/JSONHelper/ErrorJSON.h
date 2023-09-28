
#ifndef ERROR_JSON
#define ERROR_JSON 
#include "JsonObject.h"
#include "esp_err.h"

class ErrorJSON : public JsonObject
{
private:
    esp_err_t _error;
public:
    inline ErrorJSON(esp_err_t error) : _error(error) {};
    inline std::string ToJson() const override
    {
        return "{\"error\":" + std::to_string(_error) + "}";
    }
};
#endif