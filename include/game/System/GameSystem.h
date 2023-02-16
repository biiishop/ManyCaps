#pragma once

#include "GameSystemInfo.h"
#include "al/nerve/NerveExecutor.h"
#include "al/sequence/Sequence.h"

class GameSystem : public al::NerveExecutor {
    public:
        void init();
        al::Sequence* mSequence;          // 0x10
        al::GameSystemInfo* mSystemInfo;  // 0x18
        // 0x78 GameConfigData
};