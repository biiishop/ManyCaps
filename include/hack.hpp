#include "game/Player/PlayerActorHakoniwa.h"
#include "game/HakoniwaSequence/HakoniwaSequence.h"
#include "game/StageScene/StageScene.h"
#include "al/factory/ActorFactoryEntries100.h"

class Hack {
public:
  Hack(const char* name, const unsigned char arr[]) {
    mName = name;
    iter = al::ByamlIter(arr);
  };
  const char* mName;
  
  template <class T>
  void* spawnHack(al::ActorInitInfo const &initInfo/* , int* i */);
  void tpHack(sead::Vector3f* p1Pos);

  al::LiveActor* mActor;

  al::ByamlIter iter;
};

template <class T>
void* Hack::spawnHack(al::ActorInitInfo const &initInfo/* , int* i */) {
  al::ActorInitInfo const &cpy = initInfo;
  cpy.mPlacementInfo._0 = iter;

  mActor = al::createActorFunction<T>(mName);
  
  al::initCreateActorWithPlacementInfo(mActor, cpy);
  mActor->makeActorDead();
  // (*i)++;
}

