#include "game/Player/PlayerActorHakoniwa.h"
#include "game/HakoniwaSequence/HakoniwaSequence.h"
#include "game/StageScene/StageScene.h"
#include "al/factory/ActorFactoryEntries100.h"
#include "hack.hpp"

#define MAX_CAPS_PER_STAGE 8
#define NUM_CAPTURES 10
#define PAGES ceil(NUM_CAPTURES/MAX_CAPS_PER_STAGE)


class Keeper
{
public:
	HakoniwaSequence *hakoniwaSequenceRef;
	StageScene *stageSceneRef; // Current stage scene
	PlayerActorHakoniwa* p1Ref; // Player
	al::LiveActor *actor;

  int curHackNum = 0;

  void setList();

  static Hack *hackArr[3];

  // template <class...T>
  // void* spawnAllHacks(al::ActorInitInfo const &initInfo);
};

// template <class...T>
// void* Keeper::spawnAllHacks(al::ActorInitInfo const &initInfo) {
//   int i = 0;
//   // (Keeper::hackArr[i]->spawnHack<T>(initInfo, &i), ...);
// }

Keeper& getKeeper();
