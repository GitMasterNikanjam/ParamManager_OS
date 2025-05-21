// ###########################################################################################
// Include Libraries:

#include "ParamManager.h"

// ##########################################################################################

ParamHandler::ParamHandler(void)
{   
    _changeParamFlag = false;
    changedParamFlagID.clear();
}

bool ParamHandler::initMap(void)
{
    _changeParamFlag = true;

    if(parDataUnitVector != nullptr)
    {
        // Populate the maps using the parameter list.
        for (auto& param : *parDataUnitVector) 
        {
            if(checkValuetype(param.value, param.type) == false)
            {
                errorMessage = "The value type does not match the value data for parameter: " + param.name;
                return false;
            }
            paramsById[param.id] = &param;
            paramsByName[param.name] = &param;

            changedParamFlagID.push_back(param.id);
        }
    }

    return true;
}

ParamInfo* ParamHandler::getParamById(const uint32_t &id) 
{
    auto it = paramsById.find(id);
    if (it != paramsById.end()) 
    {
        return it->second;
    }
    return nullptr;  // Return nullptr if not found.
}

bool ParamHandler::setParamValueById(const uint32_t &id, std::string value)
{
    auto it = paramsById.find(id);
    if (it == paramsById.end()) 
    {
        errorMessage = "Can not set parameter. ID:" + to_string(id) + " is not existed.";
        return false;  // Return false if not found.
    }
    
    value = trimString(value);

    if(!checkValuetype(value, it->second->type))
    {
        errorMessage = "Value format for parameter ID:" + to_string(id) + " is not correct.";
        return false;
    }

    it->second->value = value;

    // change flag for parameters change.
    _changeParamFlag = true;

    changedParamFlagID.push_back(it->second->id);

    return true;
}

ParamInfo* ParamHandler::getParamByName(const std::string& name) 
{
    auto it = paramsByName.find(name);
    if (it != paramsByName.end()) 
    {
        return it->second;
    }
    return nullptr;  // Return nullptr if not found.
}

bool ParamHandler::setParamValueByName(const std::string& name, std::string value)
{
    auto it = paramsByName.find(name);
    if (it == paramsByName.end()) 
    {
        errorMessage = "The parameter can not set. Parameter name: " + name + " is not existed.";
        return false;  // Return false if not found.
    }
    
    value = trimString(value);

    // Check value format
    if(!checkValuetype(value, it->second->type))
    {
        errorMessage = "The value for parameter name: " + name + " type not correct.";
        return false;
    }

    it->second->value = value;

    // change flag for parameters change.
    _changeParamFlag = true;

    changedParamFlagID.push_back(it->second->id);
    
    return true;
}

bool ParamHandler::getParamValueById(const uint32_t &id, std::string *value, bool fullMode)
{
    // Access parameter by ID.
    ParamInfo* p = getParamById(id);

    if(p == nullptr)
    {
        errorMessage = "Can not get ID:" + to_string(id) + " parameter. It is not existed.";
        return false;
    }

    if(fullMode == true)
    {
        *value = to_string(p->id) + "," + p->name + "," + p->value + "," + p->defaultValue + "," + p->range + "," + p->type + "," + p->dimension + "," + p->description;
        return true;
    }

    *value = getParamById(id)->value;

    return true;
}

// bool ParData::getParamValueById(const uint32_t &id, std::any *value)
// {
//     // Access parameter by ID.
//     HmiComNamespace::ParDataUnit* p = getParamById(id);

//     if(p == nullptr)
//     {
//         errorMessage = "Error ParData: can not get ID:" + to_string(id) + " parameter. It is not existed.";
//         return false;
//     }

//     std::string valueType = getValueType(*value);

//     if(valueType != p->type)
//     {
//         errorMessage = "Error ParData: The value type of parameter not matched.";
//         return false;
//     }
   
//     if(isNumber(p->value))
//     {
//         *value = std::stoi(p->value);
//     }
//     else if(valueType == "float")
//     {
//         *value = std::stof(p->value);
//     }
//     else if(valueType == "double")
//     {
//         *value = std::stod(p->value);
//     }
//     else if(valueType == "string")
//     {
//         *value = p->value;
//     }

//     return true;
// }

bool ParamHandler::getParamValueStrByName(const std::string& name, std::string *value, bool fullMode)
{
    // Access parameter by name.
    ParamInfo* p = getParamByName(name);

    if(p == nullptr)
    {
        errorMessage = "Can not get " + name + " parameter. It is not existed.";
        return false;
    }

    if(fullMode == true)
    {
        *value = to_string(p->id) + "," + p->name + "," + p->value + "," + p->defaultValue + "," + p->range + "," + p->type + "," + p->dimension + "," + p->description;
        return true;
    }

    *value = p->value;

    return true;
}

// template <typename T>
// bool ParData::getParamValueByName(const std::string& name, T &value)
// {
//     // Access parameter by name.
//     HmiComNamespace::ParDataUnit* p = getParamByName(name);

//     if(p == nullptr)
//     {
//         errorMessage = "Error ParData: can not get " + name + " parameter. It is not exist.ed";
//         return false;
//     }

//     std::string valueType = getValueType(value);

//     if(valueType != p->type)
//     {
//         errorMessage = "Error ParData: The value type of parameter not matched.";
//         return false;
//     }
   
//     if(isNumber(p->value))
//     {
//         value = std::stoi(p->value);
//     }
//     else if(valueType == "float")
//     {
//         value = std::stof(p->value);
//     }
//     else if(valueType == "double")
//     {
//         value = std::stod(p->value);
//     }
//     else if(valueType == "string")
//     {
//         value = p->value;
//     }

//     return true;
// }

bool ParamHandler::setParamsAddress(std::vector<ParamInfo> *data)
{
    parDataUnitVector = data;
    if(initMap() == false)
    {
        return false;
    }

    return true;
}

bool ParamHandler::addParam(const ParamInfo data)
{
    if(isExistId(data.id) || isExistName(data.name))
    {
        return false;
    }

    parDataUnitVector->push_back(data);
    _changeParamFlag = true;
    changedParamFlagID.push_back(data.id);

    // Re initial map after add parameters.
    initMap();

    return true;
}

uint32_t ParamHandler::getParNum(void)
{
    if(parDataUnitVector == NULL)
    {
        return 0;
    }

    return parDataUnitVector->size();
}

bool ParamHandler::getChangeParamsFlag(void)
{
    return _changeParamFlag;
}

bool ParamHandler::isExistId(const uint32_t &id)
{
    auto it = paramsById.find(id);
    if (it != paramsById.end()) 
    {
        return true;
    }
    return false;  // Return false if not found.
}

bool ParamHandler::isExistName(const std::string &name)
{
    auto it = paramsByName.find(name);
    if (it != paramsByName.end()) 
    {
        return true;
    }
    return false;  // Return false if not found.
}

bool ParamHandler::resetParamsDefault(void)
{
    for (auto &p : *parDataUnitVector)
    {
        if(!setParamValueById(p.id, p.defaultValue))
        {
            errorMessage = "Can not reset parameters to defaults.";
            return false;
        }
    }

    return true;
}

bool ParamHandler::saveParamsFile(const std::string &filepath)
{
    // Create an ofstream object for writing to a file.
    // Remove all data in the file after open it.
    // If file not exist, create it. 
    ofstream file(filepath, std::ios::trunc);

    // Check file format for .param
    if (!endsWith(filepath, ".param"))
    {
        errorMessage = filepath + " format is not .param!";
        file.close();
        return false;
    }

    // Check if the file is successfully opened
    if (!file.is_open()) 
    {
        errorMessage = "Could not open the " + filepath;
        return false;
    }

    // String for each line of file.
    string line;

    for (const auto &p : *parDataUnitVector) 
    {
        line = p.name + "," + p.value;
        file << line << endl;
    }

    file.close();
    return true;
}

bool ParamHandler::loadParamsFromFile(const string &filepath)
{
    ifstream file(filepath);

    // Check file format for .param
    if (!endsWith(filepath, ".param"))
    {
        errorMessage = filepath + " format is not .param!";
        file.close();
        return false;
    }

    // Check if the file is successfully opened
    if (!file.is_open()) 
    {
        errorMessage = "Could not open the " + filepath;
        return false;
    }

    // String for each line of file.
    string line;

    // Define the expected number of columns based on the header (adjust as per your file)
    size_t expectedColumnsCount = 2;

    // Read the file line by line
    while (getline(file, line)) 
    {
        // Skip empty or whitespace-only lines
        if (line.empty() || isWhitespaceOnly(line)) 
        {
            continue;
        }

        // Split the line by ',' and store it as a row in the matrix
        vector<string> row = splitString(line, ',');
        
        // Check if the row is empty to prevent adding empty data
        if (row.empty()) 
        {
            continue;
        }

        // Validate the row format
        if (!validateRow(row, expectedColumnsCount)) 
        {
            errorMessage = "Failed to load parameters from file: " + filepath + " in line: " + line;
            file.close();
            return false; // Stop reading if a row has an invalid format
        }

        // Set parameter by name and value for a ParDataUnit object.
        if(!setParamValueByName(row[0], row[1]))
        {
            errorMessage = "Can not set parameter from file: " + filepath + ". at line: " + line + "\n" + errorMessage;
            file.close();
            return false;
        }
    }

    // Close the file after reading
    file.close();

    return true;
}

void ParamHandler::resetChangeParamFlag(void)
{
    changedParamFlagID.clear();
    _changeParamFlag = false;
}


