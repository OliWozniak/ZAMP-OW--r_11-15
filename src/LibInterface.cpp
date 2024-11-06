#include "../inc/LibInterface.hh"


LibInterface::LibInterface(){}

LibInterface::~LibInterface(){
  dlclose(_LibHandler);
}

int LibInterface::load_lib(std::string cmd_name){
    std::string lib_name = "libInterp4" + cmd_name + ".so";
    const char *c= lib_name.c_str();
    std::cout << c << std::endl;
    _LibHandler = dlopen(c,RTLD_LAZY);

    if (!_LibHandler) {
    std::cerr << "!!! Brak biblioteki:" << c << std::endl;
    return 1;
    }

    void *pFun = dlsym(_LibHandler,"CreateCmd");
    if (!pFun) {
    std::cerr << "!!! Nie znaleziono funkcji CreateCmd" << std::endl;
    return 1;
    }
    _pCreateCmd = reinterpret_cast<AbstractInterp4Command* (*)(void)>(pFun);

    return 0;
}

AbstractInterp4Command * LibInterface::get_cmd(){
    return _pCreateCmd();
}