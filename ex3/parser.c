//#include "EscapeTechnion.h"
#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mtm_ex3.h"
#include "Utils.h"

static MtmErrorCode CallSysFunction(EscapeTechnion escape_system, 
                    char* function, char* parameters, FILE* output_channel) {
    MtmErrorCode sys_err;
    switch(function) {
        case "companyadd" :
            sys_err = CallAddCompany(escape_system, parameters);
            break;
        case "companyremove" :
            sys_err = CallAddCompany(escape_system, parameters);
            break;
        case "roomadd" :
            sys_err = CallAddCompany(escape_system, parameters);
            break;
        case "roomremove" :
            sys_err = CallAddCompany(escape_system, parameters);
            break;
        case "escaperadd" :
            sys_err = CallAddCompany(escape_system, parameters);
            break;
        case "escaperremove" :
            sys_err = CallAddCompany(escape_system, parameters);
            break;
        case "escaperorder" :
            sys_err = CallAddCompany(escape_system, parameters);
            break;
        case "escaperrecommend" :
            sys_err = CallAddCompany(escape_system, parameters);
            break;
        case "reportday" :
            sys_err = SystemReportDay(escape_system, output_channel);
            break;
        case "reportbest" :
            sys_err = SystemReportBest(escapse_system, output_channel);
    }
    return sys_err;
}

/* Helper function that gets char* buffer after 2 strtoks (after getting the 
    command name) and return pointer to the rest of the command (parameters). */
static char* GetParameters(char* buffer) {
    if(buffer == NULL) {
        return NULL;
    }
    char* ptr = buffer;
    int null_count = 0;
    while(*ptr != '\n' && null_count < 2) {
        if(*ptr == '\0') {
            null_count++;
        }
        ptr++;
    }
    return ptr;
}

static void GetPartsOfCommand(char** token, char** func, char* buffer) {
    assert(token != NULL && func != NULL && buffer != NULL);
    (*token) = strtok(buffer, " "); //Get the first word
    (*func) = StrDuplicate(token);
    if((*func) == NULL) {
        mtmPrintErrorMessage(err_channel, MTM_OUT_OF_MEMORY);
        return;
    }
    (*token) = strtok(NULL, " ");
    strcat(*func, *token); //Now func containts the first 2 words
    (*token) = GetParameters(buffer);
}

int main (int argc, char** argv) {
    FILE* output_channel, input_channel, err_channel = stderr;
    if(argc != 1 && argc != 3 && argc != 5) {
        mtmPrintErrorMessage(err_channel, MTM_INVALID_COMMAND_LINE_PARAMETERS);
        return 0;
    }
    char buffer[MAX_LEN] = "";
    char *token, *func, *output_str = NULL, *input_str = NULL, *token, *func;
    for(int i = 1; i < argc; ++i) {
        if(argv[i] == "-i" && input_str == NULL) {
            input_str = argv[i+1];
        } 
        if(argv[i] == "-o" && output_str == NULL) {
            output_str = argv[i+1];
        }
    }
    if(output_str == NULL) {
        output_str = "stdout";
    }
    if(input_str == NULL) {
        input_str = "stdin";
    }
    output_channel = fopen(output_str, "w");
    input_channel = fopen(input_str, "r");
    if(output_channel == NULL || input_channel == NULL) {
        mtmPrintErrorMessage(err_channel, MTM_CANNOT_OPEN_FILE);
        return 0;
    }
    EscapeTechnion escape_system = SystemCreate();
    if(escape_system == NULL) {
        mtmPrintErrorMessage(err_channel, MTM_OUT_OF_MEMORY);
        SystemDestroy(escape_system);
        return 0;
    }
    while(fgets(buffer, MAX_LEN, input_channel) != NULL) {
        if(strcmp(buffer, "\n") == 0 || buffer[0] == '#') {
            continue;
        }
        GetPartsOfCommand(&token, &func, buffer);
        if(func == NULL) {
            SystemDestroy(escape_system);
            return 0;
        }
        MtmErrorCode sys_err = CallSysFunction(func, token);
        if(sys_err != MTM_SUCCESS) {
            if(sys_err == MTM_SUCCESS) {
                mtmPrintErrorMessage(err_channel, sys_err);
                SystemDestroy(escape_system);
                return 0;
            }
            mtmPrintErrorMessage(err_channel, sys_err);
        }
    }
    SystemDestroy(escape_system);
    return 0;
}