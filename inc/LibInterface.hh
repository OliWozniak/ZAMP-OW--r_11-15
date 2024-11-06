#ifndef LIBINTERFACE_HH
#define LIBINTERFACE_HH

#include "AbstractInterp4Command.hh"
#include <dlfcn.h>
#include <iostream>



class LibInterface
{
private:
    void *_LibHandler;
    AbstractInterp4Command *(*_pCreateCmd)(void);
    std::string _CmdName;
public:
    LibInterface();
    ~LibInterface();

    int load_lib(std::string cmd_name);
    AbstractInterp4Command *get_cmd();
};



#endif