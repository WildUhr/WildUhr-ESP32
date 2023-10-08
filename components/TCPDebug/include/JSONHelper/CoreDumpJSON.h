#ifndef CORE_DUMP_JSON
#define CORE_DUMP_JSON 

#include "JsonObject.h"

class CoreDumpJSON : public JsonObject
{
private:
    esp_core_dump_summary_t _dump;

//      typedef struct {
//          uint32_t exc_tcb;                           /*!< TCB pointer to the task causing exception */
//          char exc_task[16];                          /*!< Name of the task that caused exception */
//          uint32_t exc_pc;                            /*!< Program counter for exception */
//          esp_core_dump_bt_info_t exc_bt_info;        /*!< Backtrace information for task causing exception */
//          uint32_t core_dump_version;                 /*!< Core dump version */
//          uint8_t app_elf_sha256[APP_ELF_SHA256_SZ];  /*!< Crashing application's SHA256 sum as a string */
//          esp_core_dump_summary_extra_info_t ex_info; /*!< Architecture specific extra data */
//      } esp_core_dump_summary_t;
//typedef struct {
//    uint8_t stackdump[CONFIG_ESP_COREDUMP_SUMMARY_STACKDUMP_SIZE];    /*!< Stack dump of the crashing task. */
//    uint32_t dump_size;                                               /*!< Size (in bytes) of the stack dump */
//} esp_core_dump_bt_info_t;
//typedef struct {
//    uint32_t mstatus;     /* Machine Status */
//    uint32_t mtvec;       /* Machine Trap-Vector Base Address */
//    uint32_t mcause;      /* Machine Trap Cause */
//    uint32_t mtval;       /* Machine Trap Value */
//    uint32_t ra;          /* Return Address */
//    uint32_t sp;          /* Stack pointer */
//    uint32_t exc_a[8];    /* A0-A7 registers when the exception caused */
//} esp_core_dump_summary_extra_info_t;

public:
    inline CoreDumpJSON(esp_core_dump_summary_t dump) : _dump(dump) {};
    inline std::string ToJson() const override
    {
        std::string json = "{";
        json += "\"exc_tcb\": " + std::to_string(_dump.exc_tcb) + ",";
        json += "\"exc_task\": \"" + std::string(_dump.exc_task) + "\",";
        json += "\"exc_pc\": " + std::to_string(_dump.exc_pc) + ",";
        json += "\"exc_bt_info\": {";
        json += "\"stackdump\": \"" + std::string((char*)_dump.exc_bt_info.stackdump) + "\",";
        json += "\"dump_size\": " + std::to_string(_dump.exc_bt_info.dump_size);
        json += "},";
        json += "\"core_dump_version\": " + std::to_string(_dump.core_dump_version) + ",";
        json += "\"app_elf_sha256\": \"" + std::string((char*)_dump.app_elf_sha256) + "\",";
        json += "\"ex_info\": {";
        json += "\"mstatus\": " + std::to_string(_dump.ex_info.mstatus) + ",";
        json += "\"mtvec\": " + std::to_string(_dump.ex_info.mtvec) + ",";
        json += "\"mcause\": " + std::to_string(_dump.ex_info.mcause) + ",";
        json += "\"mtval\": " + std::to_string(_dump.ex_info.mtval) + ",";
        json += "\"ra\": " + std::to_string(_dump.ex_info.ra) + ",";
        json += "\"sp\": " + std::to_string(_dump.ex_info.sp) + ",";
        json += "\"exc_a\": [";
        for (int i = 0; i < 8; i++)
        {
            json += std::to_string(_dump.ex_info.exc_a[i]);
            if (i < 7)
            {
                json += ",";
            }
        }
        json += "]";
        json += "}";
        json += "}";
        return json;
    }
};
#endif