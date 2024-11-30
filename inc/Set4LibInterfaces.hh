#ifndef SET4LIBINTERFACES_HH
#define SET4LIBINTERFACES_HH

#include <map>
#include <memory>

#include "LibInterface.hh"

class Set4LibInterfaces : public std::map<std::string,std::shared_ptr<LibInterface>>
{
private:

public:
    Set4LibInterfaces(/* args */);
    ~Set4LibInterfaces();

    bool print_command(std::string cmd_name, std::istream& process_instruction);
    bool load_libraries();
    bool load_library(std::string lib_name);
    bool check_library(std::string cmd_name);
    std::shared_ptr<AbstractInterp4Command> make_cmd(std::string cmd_name, std::istream& process_instruction);
};

#endif