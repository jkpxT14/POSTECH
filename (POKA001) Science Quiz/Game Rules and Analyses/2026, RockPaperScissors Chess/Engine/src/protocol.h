#ifndef RPSC_PROTOCOL_H_INCLUDED
#define RPSC_PROTOCOL_H_INCLUDED

#include <iosfwd>
#include <string>

#include "engine.h"

namespace rpsc {

class Protocol {
   public:
    Protocol(std::istream& in, std::ostream& out);
    void loop();

   private:
    void command(const std::string& line);
    void print_search(const SearchResult& result);
    void bench();

    Engine engine_;
    std::istream& in_;
    std::ostream& out_;
    bool running_ = true;
};

}  // namespace rpsc

#endif
