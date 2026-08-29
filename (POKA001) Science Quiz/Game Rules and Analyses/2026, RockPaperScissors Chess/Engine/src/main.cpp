#include <iostream>
#include "orientation.h"
#include "protocol.h"
int main() {
    (void)rpsc::OrientationTable::instance();
    rpsc::Protocol protocol(std::cin, std::cout);
    protocol.loop();
    return 0;
}
