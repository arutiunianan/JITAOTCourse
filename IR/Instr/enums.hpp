#ifndef IR_ENUMS_HPP
#define IR_ENUMS_HPP

#include <cstddef>

enum class OpType: size_t{
    #define OPR_DEF(name, dump_name) name,

    #include "oprdef.hpp"
    #undef OPR_DEF
};

enum class DataType {
    #define DATA_DEF(name, dump_name) name,

    #include "datadef.hpp"
    #undef DATA_DEF
};

#endif  // IR_ENUMS_HPP