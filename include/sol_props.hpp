#ifndef SOL_PROPS_HPP
#define SOL_PROPS_HPP
#include <string>

const char** read_props(const std::string& propsfile);

void dump_props(const char** props);

#endif
