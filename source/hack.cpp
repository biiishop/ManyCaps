#include "hack.hpp"

void Hack::tpHack(sead::Vector3f* p1Pos) {
  mActor->makeActorAlive();
  al::setTrans(mActor, *p1Pos);
}

