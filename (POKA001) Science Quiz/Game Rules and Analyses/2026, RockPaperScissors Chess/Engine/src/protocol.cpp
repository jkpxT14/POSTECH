#include "protocol.h"
#include "engine.h"
#include "movegen.h"
#include "notation.h"
#include "perft.h"
#include <iostream>
#include <sstream>
namespace rpsc {
int run_protocol(){Position p;Engine e;std::string line;while(std::getline(std::cin,line)){if(line=="quit")break;if(line=="rpsc"||line=="uci"){std::cout<<"id name RPSC Engine 0.24.0\nid author Jungwoo Kim / POKA001\nrpscok\n"<<std::flush;continue;}if(line=="isready"){std::cout<<"readyok\n"<<std::flush;continue;}if(line=="position startpos"){p=Position{};std::cout<<"ok\n"<<std::flush;continue;}if(line.rfind("perft ",0)==0){int d=std::stoi(line.substr(6));std::cout<<"nodes "<<perft(p,d)<<"\n"<<std::flush;continue;}if(line.rfind("go",0)==0){int ms=1000,mpv=3;std::istringstream in(line);std::string t;while(in>>t){if(t=="movetime")in>>ms;else if(t=="multipv")in>>mpv;}SearchLimits lim;lim.movetime_ms=ms;lim.multipv=mpv;auto r=e.analyze(p,lim);for(size_t i=0;i<r.candidates.size();i++)std::cout<<"info multipv "<<i+1<<" depth "<<r.candidates[i].depth<<" score cp "<<r.candidates[i].value<<" pv "<<move_notation(p,r.candidates[i].move)<<"\n";if(r.has_best)std::cout<<"bestmove "<<move_notation(p,r.best)<<"\n";else std::cout<<"bestmove (none)\n";std::cout<<std::flush;continue;}std::cout<<"unknown\n"<<std::flush;}return 0;}
} // namespace rpsc
