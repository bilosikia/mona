#include <string>
#include <cstring>

namespace mona {
std::string errnoToStr(int err, char *description = "") {
    std::string errStr = strerror(err);
    errStr.append(": ");
    errStr.append(description);
    errStr.append("\n");
    return errStr;
}
}

