#ifndef RPSC_ORIENTATION_H_INCLUDED
#define RPSC_ORIENTATION_H_INCLUDED
#include <array>
#include "types.h"
namespace rpsc {struct GestureState{Gesture ud,ns,ew;};enum class WristDirection:std::uint8_t{North,South,East,West};class OrientationTable{public:static const OrientationTable&instance();Orientation roll(Orientation,Direction)const;Orientation rotate_left(Orientation)const;Orientation rotate_right(Orientation)const;Gesture top_gesture(Orientation)const;WristDirection wrist_direction(Orientation)const;GestureState gesture_state(Orientation)const;std::uint8_t gesture_state_id(Orientation)const;Orientation canonical(Gesture,WristDirection)const;std::size_t size()const{return 24;}private:OrientationTable();std::array<std::array<Orientation,4>,24> roll_{};std::array<Orientation,24> rotate_left_{},rotate_right_{};std::array<Gesture,24> top_{};std::array<WristDirection,24>wrist_{};std::array<GestureState,24>state_{};std::array<std::uint8_t,24>state_id_{};std::array<std::array<Orientation,4>,3>canonical_{};};}
#endif
