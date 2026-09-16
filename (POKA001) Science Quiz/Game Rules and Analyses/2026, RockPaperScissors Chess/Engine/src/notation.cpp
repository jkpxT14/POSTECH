#include "notation.h"
#include "move.h"
#include <sstream>
namespace rpsc {
std::string square_name(int x,int y){std::string s;s.push_back(char('a'+x));s+=std::to_string(y+1);return s;}
std::string move_notation(const Position& before,const Move&m){std::ostringstream o;o<<(m.piece<4?'W':'B')<<(m.piece%4+1);auto it=item_name(m.item);if(!it.empty())o<<'['<<it<<']';o<<": "<<square_name(before.pieces[m.piece].x,before.pieces[m.piece].y);if(m.item==Item::Push)o<<'>'<<square_name(m.push_x,m.push_y);for(size_t i=1;i<m.path.size();i++)o<<'-'<<square_name(m.path[i].first,m.path[i].second);Position n=before;int cb=n.captures[0]+n.captures[1];bool r=false;n.apply(m,&r);int ca=n.captures[0]+n.captures[1];if(ca>cb){for(int i=0;i<8;i++)if(before.pieces[i].alive&&!n.pieces[i].alive)o<<" x"<<(i<4?'W':'B')<<(i%4+1);}if(r)o<<" Reset";return o.str();}
} // namespace rpsc
