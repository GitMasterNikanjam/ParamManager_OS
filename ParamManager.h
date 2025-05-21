#pragma once

// ############################################################################
// Include libraries:

#include <iostream>                 // Input and output stream library
#include <deque>                    // Double-ended queue container
#include <string>                   // String class and related functions
#include <vector>                   // Dynamic array container
#include <unordered_map>            // Hash table-based associative container
#include <fstream>                  // File stream classes for file handling
#include "../Stream_OS/Stream.h"

using namespace std;

// #############################################################################

/**
 * @ingroup public_general_structures
 * @struct ParamInfo 
 * @brief Structure for unit parameter information.
 *  */ 
struct ParamInfo 
{
    /// @brief ID number. each parameter must have defferent id.
    uint32_t id = 1;     

    /// @brief Name of parameter.               
    std::string name = ""; 

    /// @brief Value of parameter in string              
    std::string value = "";   

    /// @brief Default value of parameter in string.           
    std::string defaultValue = ""; 

    /// @brief Range indicator of parameter in string.      
    std::string range = ""; 

    /// @brief type: int8, int16, int32, int64, uint8, uint16, uint32, uint64, float, double, string, bool             
    std::string type = "";    

    /// @brief eg: [%], [m], [deg], ...           
    std::string dimension = "";      

    /// @brief Description of parameter.    
    std::string description = "";        
};

// ##########################################################################################################

/**
 * @ingroup public_utility_classes
 * @class ParamHandler
 * @brief ParamHandler class for access and management parameter units. 
 *  */ 
class ParamHandler 
{
    public:

        /// @brief Last error for ParData object.
        std::string errorMessage;

        /// @brief A uint32_t vector to store any parameter IDs that activate _changeParamFlag to true.
        std::vector<uint32_t> changedParamFlagID;

        /// @brief ParDataUnit vector pointer for store parameters data address.
        std::vector<ParamInfo> *parDataUnitVector = nullptr;

        /// @brief unordered_map for accessing parameters by ID. value is ParDataUnit pointer.
        std::unordered_map<uint32_t, ParamInfo *> paramsById;

        /// @brief unordered_map for accessing parameters by name. value is ParDataUnit pointer.
        std::unordered_map<std::string, ParamInfo *> paramsByName;

        /// @brief Default constructor. Init some variables. 
        ParamHandler();

        /**
         * @brief Initial ParData. init map for paramsById and paramsByName.
         * @return true if succeeded.
         *  */ 
        bool initMap(void);

        /**
         * @brief Function to get parameter pointer by ID.
         * @return ParDataUnit pointer.    
         * @note Return nullptr if not found.
         *  */ 
        ParamInfo* getParamById(const uint32_t &id);

        /**
         * @brief Function to get parameter pointer by name.
         * @return ParDataUnit pointer
         * @note Return nullptr if not found.
         *  */ 
        ParamInfo* getParamByName(const std::string& name);

        /**
         * Function to set parameter value by ID.
         * @return true if succeeded.
         */
        bool setParamValueById(const uint32_t &id, std::string value);

        /**
         * Function to set parameter value by name.
         * @return true if succeeded.
         */
        bool setParamValueByName(const std::string& name, std::string value);

        /**
         * @brief Function to get parameter value by ID.
         * @param id: ID number of parameter.
         * @param value: string pointer for store parameter value.
         * @param fullMode: If this true then return value by all data of certain parameter, such as name, value, description, ... .
         * @return true if succeeded.
         */
        bool getParamValueById(const uint32_t &id, std::string *value, bool fullMode = false);

        /**
         * @brief Function to get parameter value by ID.
         * @param id: ID number of parameter.
         * @param value: pointer for store parameter value.
         * @return true if succeeded. 
         * @note The value type is variable and depends on parameters type value.
         */
        // bool getParamValueById(const uint32_t &id, std::any *value);

        /**
         * @brief Function to get parameter value by name.
         * @param name: name of parameter.
         * @param value: string pointer for store parameter value.
         * @param fullMode: If this true then return value by all data of certain parameter, such as name, value, description, ... .
         * @return true if succeeded.
         */
        bool getParamValueStrByName(const std::string& name, std::string *value, bool fullMode = false);

        /**
         * @brief Function to get parameter value by name.
         * @param name: name of parameter.
         * @param value: pointer for store parameter value.
         * @return true if succeeded.
         * @note The value type is variable and depends on parameters type value.
         */
        // template <typename T>
        // bool getParamValueByName(const std::string& name, T &value);

        /**
         * @brief Set pointer address for ParDataUnit vector.
         * @return true if succeeded.
         *  */ 
        bool setParamsAddress(std::vector<ParamInfo> *data);

        /**
         * @brief Add ParDataUnit object to the parameters vector.
         * @return true if succeeded.
         */ 
        bool addParam(const ParamInfo data);

        /// @brief Get number of parameters.
        uint32_t getParNum(void);

        /**
         * @brief Get flag for any change at paramData.
         * @return true if any changes exist.
         *  */ 
        bool getChangeParamsFlag(void);

        /// @brief Reset flag for change parameters. usually when read parameters, then it can reset flag for param changes.
        void resetChangeParamFlag(void);

        /**
         * @brief Check existance of certain ID between parameters id.
         * @return true if ID number exist.
         */
        bool isExistId(const uint32_t &id);

        /**
         * @brief Check existance of certain name between parameters name.
         * @return true if parameter name exist.
         */
        bool isExistName(const std::string &name);

        /**
         * @brief Load parameters value from a file and update relative ParData parameters.
         * It is not neccessarily all parameters exist in file. Just which parameters update that existed in file.
         * file format parameters: Param_Name, value
         * This file contain parameters name and value.
         * @param filepath: path address of .param file relative to program.
         * @return true if succeeded.
         *  */ 
        bool loadParamsFromFile(const std::string &filepath);

        /**
         * @brief  Save all parameters by name and value to a file path.
         * @return true if succeeded.
         */
        bool saveParamsFile(const std::string &filepath);

        /**
         * @brief Reset all parameters value by its default values.
         * @return true if succeeded.
         *  */ 
        bool resetParamsDefault(void);

    private:

        /// @brief Flag for any change in ParamData. 
        bool _changeParamFlag;

        /// @brief parameters file path for store parameters.
        std::string _paramFilePath;
};