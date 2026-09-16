#include "move.h"
#include <sstream>
namespace rpsc {
bool same_move(const Move&a,const Move&b){return a.piece==b.piece&&a.item==b.item&&a.push_x==b.push_x&&a.push_y==b.push_y&&a.dirs==b.dirs;}
bool is_rotation(Item i){return i>=Item::RoN&&i<=Item::RoR;}
int item_bucket(Item i){if(i==Item::Push)return 1;if(is_rotation(i))return 2;if(i==Item::StepShort||i==Item::StepLong)return 3;return 0;}
int destination_square(const Move&m){if(m.path.empty())return -1;auto[x,y]=m.path.back();return y*8+x;}
std::string move_signature(const Move&m){std::ostringstream o;o<<m.piece<<':'<<(int)m.item<<':'<<m.push_x<<','<<m.push_y<<':';for(auto d:m.dirs)o<<(int)d;return o.str();}
} // namespace rpsc
