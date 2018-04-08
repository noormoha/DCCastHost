//
// Created by tang on 4/3/18.
//

#include "NormDefinition.h"

namespace DCCast {

DCException::DCException(std::string _error): runtime_error(_error) { }

const char *DCException::what() const noexcept {
    return runtime_error::what();
}

}
