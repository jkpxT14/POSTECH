#include "types.h"
#include <stdexcept>

namespace rpsc {
std::string side_name(Side s){ return s==Side::White?"W":"B"; }
std::string gesture_name(Gesture g){ return g==Gesture::Scissors?"S":g==Gesture::Rock?"R":"P"; }
std::string dir_name(Dir d){ switch(d){case Dir::N:return"N";case Dir::S:return"S";case Dir::E:return"E";case Dir::W:return"W";} return"?"; }
std::string item_name(Item i){ switch(i){case Item::None:return"";case Item::Push:return"Pu";case Item::RoN:return"RoN";case Item::RoS:return"RoS";case Item::RoE:return"RoE";case Item::RoW:return"RoW";case Item::RoL:return"RoL";case Item::RoR:return"RoR";case Item::StepShort:return"StS";case Item::StepLong:return"StL";} return"?"; }
Dir opposite(Dir d){ switch(d){case Dir::N:return Dir::S;case Dir::S:return Dir::N;case Dir::E:return Dir::W;case Dir::W:return Dir::E;} throw std::logic_error("bad dir"); }
int base_distance(Gesture g){ return g==Gesture::Scissors?3:g==Gesture::Rock?4:5; }
int fight(Gesture a,Gesture b){ if(a==b)return 0; if((a==Gesture::Scissors&&b==Gesture::Paper)||(a==Gesture::Rock&&b==Gesture::Scissors)||(a==Gesture::Paper&&b==Gesture::Rock))return 1; return -1; }
} // namespace rpsc
