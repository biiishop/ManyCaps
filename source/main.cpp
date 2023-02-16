#include "main.hpp"
#include "al/area/AreaObj.h"
#include "al/area/AreaObjGroup.h"
#include "al/byaml/ByamlData.h"
#include "al/camera/CameraDirector.h"
#include "al/camera/CameraPoser.h"
#include "al/util.hpp"
#include "al/util/LiveActorUtil.h"
#include "al/util/StringUtil.h"
#include "cameras/CameraPoserCustom.h"
#include "debugMenu.hpp"
#include "keeper.hpp"
#include "al/byaml/writer/ByamlWriter.h"

static bool showMenu = false;

static bool isInGame = false;

DebugWarpPoint warpPoints[40];

int listCount = 0;

int curWarpPoint = 0;

void drawBackground(agl::DrawContext *context)
{

    sead::Vector3<float> p1; // top left
    p1.x = -1.0;
    p1.y = 0.3;
    p1.z = 0.0;
    sead::Vector3<float> p2; // top right
    p2.x = -0.2;
    p2.y = 0.3;
    p2.z = 0.0;
    sead::Vector3<float> p3; // bottom left
    p3.x = -1.0;
    p3.y = -1.0;
    p3.z = 0.0;
    sead::Vector3<float> p4; // bottom right
    p4.x = -0.2;
    p4.y = -1.0;
    p4.z = 0.0;

    sead::Color4f c;
    c.r = 0.1;
    c.g = 0.1;
    c.b = 0.1;
    c.a = 0.9;

    agl::utl::DevTools::beginDrawImm(context, sead::Matrix34<float>::ident, sead::Matrix44<float>::ident);
    agl::utl::DevTools::drawTriangleImm(context, p1, p2, p3, c);
    agl::utl::DevTools::drawTriangleImm(context, p3, p4, p2, c);
}

// ------------- Hooks -------------

al::StageInfo *initDebugListHook(const al::Scene *curScene)
{

    // hook that gets all objects put in DebugList and adds their coordinates to a warp point array

    al::StageInfo *info = al::getStageInfoMap(curScene, 0);

    al::PlacementInfo rootInfo = al::PlacementInfo();

    al::tryGetPlacementInfoAndCount(&rootInfo, &listCount, info, "DebugList");

    if (listCount > 0)
    {
        for (size_t i = 0; i < listCount; i++)
        {
            al::PlacementInfo objInfo = al::PlacementInfo();

            al::getPlacementInfoByIndex(&objInfo, rootInfo, i);

            const char *displayName = "";
            al::tryGetDisplayName(&displayName, objInfo);

            strcpy(warpPoints[i].pointName, displayName);

            al::tryGetTrans(&warpPoints[i].warpPos, objInfo);
        }
    }

    return info;
}

void drawMainHook(HakoniwaSequence *curSequence, sead::Viewport *viewport, sead::DrawContext *drawContext)
{

    if (!showMenu)
    {
        al::executeDraw(curSequence->mLytKit, "２Ｄバック（メイン画面）");
        return;
    }

    int dispWidth = al::getLayoutDisplayWidth();
    int dispHeight = al::getLayoutDisplayHeight();

    gTextWriter->mViewport = viewport;

    gTextWriter->mColor = sead::Color4f(
        1.f,
        1.f,
        1.f,
        0.8f);

    al::Scene *curScene = curSequence->curScene;

    if (curScene && isInGame)
    {

        drawBackground((agl::DrawContext *)drawContext);

        gTextWriter->beginDraw();

        gTextWriter->setCursorFromTopLeft(sead::Vector2f(10.f, (dispHeight / 3) + 30.f));

        gTextWriter->setScaleFromFontHeight(20.f);

        al::CameraPoser *curPoser;

        al::CameraDirector *director = curScene->getCameraDirector();
        if (director) {
            al::CameraPoseUpdater *updater = director->getPoseUpdater(0);
            if (updater && updater->mTicket) {
                curPoser = updater->mTicket->mPoser;
            }
        }


    gTextWriter->printf("Current Capture: %d", getKeeper().hackArr[getKeeper().curHackNum]);


		if (curPoser)
		{

			// we can print anything from our current camera poser here

			// gTextWriter->printf("IsOnGroundFace: %s\n");

			if (al::isEqualString(curPoser->getName(), "CameraPoserCustom"))
			{
				cc::CameraPoserCustom *poserCustom = (cc::CameraPoserCustom *)curPoser; // casts current poser to custom poser class so we can access class-specific members
				gTextWriter->printf("V Angle: %f\n", poserCustom->mAngle);
			}
		}

		isInGame = false;
	}

	gTextWriter->endDraw();

    al::executeDraw(curSequence->mLytKit, "２Ｄバック（メイン画面）");
}

void stageInitHook(StageScene *initStageScene, al::SceneInitInfo *sceneInitInfo)
{
    __asm("MOV X19, X0");
    __asm("LDR X24, [X1, #0x18]");

    // place any code that needs to be ran during init here (creating actors for example)
	

	__asm("MOV X1, X24");
}
// const unsigned char frog[] = {
//   0x59, 0x42, 0x03, 0x00, 0x10, 0x00, 0x00, 0x00, 0xe4, 0x00, 0x00, 0x00,
//   0x1c, 0x01, 0x00, 0x00, 0xc2, 0x0f, 0x00, 0x00, 0x44, 0x00, 0x00, 0x00,
//   0x47, 0x00, 0x00, 0x00, 0x5b, 0x00, 0x00, 0x00, 0x66, 0x00, 0x00, 0x00,
//   0x6d, 0x00, 0x00, 0x00, 0x7a, 0x00, 0x00, 0x00, 0x8a, 0x00, 0x00, 0x00,
//   0x9c, 0x00, 0x00, 0x00, 0xa3, 0x00, 0x00, 0x00, 0xa9, 0x00, 0x00, 0x00,
//   0xb3, 0x00, 0x00, 0x00, 0xc2, 0x00, 0x00, 0x00, 0xcb, 0x00, 0x00, 0x00,
//   0xcd, 0x00, 0x00, 0x00, 0xcf, 0x00, 0x00, 0x00, 0xd1, 0x00, 0x00, 0x00,
//   0x49, 0x64, 0x00, 0x49, 0x73, 0x46, 0x6f, 0x72, 0x63, 0x65, 0x52, 0x65,
//   0x76, 0x69, 0x76, 0x65, 0x4f, 0x6e, 0x44, 0x65, 0x61, 0x64, 0x00, 0x49,
//   0x73, 0x4c, 0x69, 0x6e, 0x6b, 0x44, 0x65, 0x73, 0x74, 0x00, 0x49, 0x73,
//   0x4d, 0x6f, 0x6f, 0x6e, 0x00, 0x49, 0x73, 0x57, 0x65, 0x61, 0x72, 0x69,
//   0x6e, 0x67, 0x43, 0x61, 0x70, 0x00, 0x4c, 0x61, 0x79, 0x65, 0x72, 0x43,
//   0x6f, 0x6e, 0x66, 0x69, 0x67, 0x4e, 0x61, 0x6d, 0x65, 0x00, 0x50, 0x6c,
//   0x61, 0x63, 0x65, 0x6d, 0x65, 0x6e, 0x74, 0x46, 0x69, 0x6c, 0x65, 0x4e,
//   0x61, 0x6d, 0x65, 0x00, 0x52, 0x6f, 0x74, 0x61, 0x74, 0x65, 0x00, 0x53,
//   0x63, 0x61, 0x6c, 0x65, 0x00, 0x54, 0x72, 0x61, 0x6e, 0x73, 0x6c, 0x61,
//   0x74, 0x65, 0x00, 0x55, 0x6e, 0x69, 0x74, 0x43, 0x6f, 0x6e, 0x66, 0x69,
//   0x67, 0x4e, 0x61, 0x6d, 0x65, 0x00, 0x57, 0x61, 0x69, 0x74, 0x54, 0x79,
//   0x70, 0x65, 0x00, 0x58, 0x00, 0x59, 0x00, 0x5a, 0x00, 0x00, 0x00, 0x00,
//   0xc2, 0x04, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0x1f, 0x00, 0x00, 0x00,
//   0x24, 0x00, 0x00, 0x00, 0x2e, 0x00, 0x00, 0x00, 0x38, 0x00, 0x00, 0x00,
//   0x43, 0x6f, 0x6d, 0x6d, 0x6f, 0x6e, 0x00, 0x46, 0x72, 0x6f, 0x67, 0x00,
//   0x55, 0x6e, 0x64, 0x65, 0x66, 0x69, 0x6e, 0x65, 0x64, 0x00, 0x6f, 0x6a,
//   0x62, 0x61, 0x62, 0x65, 0x6c, 0x6c, 0x73, 0x00, 0xc1, 0x0c, 0x00, 0x00,
//   0x00, 0x00, 0x00, 0xa0, 0x03, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0xd0,
//   0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0xd0, 0x00, 0x00, 0x00, 0x00,
//   0x03, 0x00, 0x00, 0xd0, 0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0xd0,
//   0x00, 0x00, 0x00, 0x00, 0x05, 0x00, 0x00, 0xa0, 0x00, 0x00, 0x00, 0x00,
//   0x06, 0x00, 0x00, 0xa0, 0x02, 0x00, 0x00, 0x00, 0x07, 0x00, 0x00, 0xc1,
//   0x80, 0x01, 0x00, 0x00, 0x08, 0x00, 0x00, 0xc1, 0xb8, 0x01, 0x00, 0x00,
//   0x09, 0x00, 0x00, 0xc1, 0x9c, 0x01, 0x00, 0x00, 0x0a, 0x00, 0x00, 0xa0,
//   0x01, 0x00, 0x00, 0x00, 0x0b, 0x00, 0x00, 0xd2, 0x00, 0x00, 0x00, 0x00,
//   0xc1, 0x03, 0x00, 0x00, 0x0c, 0x00, 0x00, 0xd2, 0x00, 0x00, 0x00, 0x00,
//   0x0d, 0x00, 0x00, 0xd2, 0x00, 0x00, 0x00, 0x00, 0x0e, 0x00, 0x00, 0xd2,
//   0x00, 0x00, 0x00, 0x00, 0xc1, 0x03, 0x00, 0x00, 0x0c, 0x00, 0x00, 0xd2,
//   0x1f, 0x85, 0xe3, 0xc0, 0x0d, 0x00, 0x00, 0xd2, 0xf6, 0x28, 0x9a, 0xc2,
//   0x0e, 0x00, 0x00, 0xd2, 0xa4, 0x70, 0x77, 0x42, 0xc1, 0x03, 0x00, 0x00,
//   0x0c, 0x00, 0x00, 0xd2, 0x00, 0x00, 0x80, 0x3f, 0x0d, 0x00, 0x00, 0xd2,
//   0x00, 0x00, 0x80, 0x3f, 0x0e, 0x00, 0x00, 0xd2, 0x00, 0x00, 0x80, 0x3f
// };
// unsigned int InitDebug_byml_len = 468;

void initActorWithArchiveNameHook(al::LiveActor *actor, al::ActorInitInfo const &initInfo, sead::SafeStringBase<char> const &string, char const *anotherString)
{
	// al::LiveActor *testActor;
 //  // al::LiveActor *koopaActor;
 //  //
 //  al::ByamlIter balls = {InitDebug_byml};
 //  al::ActorInitInfo const &cpy = initInfo;
 //  cpy.mPlacementInfo._0 = balls;

  
  getKeeper().setList();

  // getKeeper().hackArr[1]->spawnHack<Frog>(initInfo);
  // getKeeper().spawnAllHacks<Koopa, Frog, Yoshi>(initInfo);

  

  // testActor = al::createActorFunction<Frog>("FrogMoon");
  // koopaActor = al::createActorFunction<Koopa>("KoopaSpawn");

  // al::initCreateActorWithPlacementInfo(testActor, cpy);
  // al::initCreateActorNoPlacementInfo(koopaActor, initInfo);

  // getKeeper().actor = testActor;
  // getKeeper().koopa = koopaActor;

	// call the original function
	al::initActorWithArchiveName(actor, initInfo, string, anotherString);
}

// ulong threadInit()
// { // hook for initializing any threads we need
//     __asm("STR X21, [X19,#0x208]");

//     return 0x20;
// }

void stageSceneHook()
{

    __asm("MOV X19, X0");

    StageScene *stageScene;
    __asm("MOV %[result], X0"
          : [result] "=r"(stageScene));

    al::PlayerHolder *pHolder = al::getScenePlayerHolder(stageScene);
    PlayerActorHakoniwa *p1 = (PlayerActorHakoniwa*)al::tryGetPlayerActor(pHolder, 0);

    isInGame = true;

	// if (al::isPadTriggerUp(-1)) // enables/disables debug menu
	// {
	//     showMenu = !showMenu;
	// }

	__asm("MOV X0, %[input]"
		  : [input] "=r"(stageScene));
}

void seadPrintHook(const char *fmt, ...) // hook for replacing sead::system::print with our custom logger
{
    va_list args;
    va_start(args, fmt);

    gLogger->LOG(fmt, args);

    va_end(args);
}

bool hakoniwaSequenceHook(HakoniwaSequence *sequence)
{
	StageScene *stageScene = (StageScene *)sequence->curScene;

	bool isFirstStep = al::isFirstStep(sequence);

	sead::Vector3f lowestPos;

	al::PlayerHolder *pHolder = al::getScenePlayerHolder(stageScene);
	PlayerActorHakoniwa *p1 = (PlayerActorHakoniwa *)al::tryGetPlayerActor(pHolder, 0);
	sead::Vector3f p1Pos = al::getTrans(p1);

  if (al::isPadTriggerLeft(-1) && getKeeper().curHackNum > 0) {
    getKeeper().curHackNum--;
  }
  if (al::isPadTriggerRight(-1) && getKeeper().curHackNum < 2) {
    getKeeper().curHackNum++;
  }
  if (al::isPadTriggerDown(-1)) {
    getKeeper().hackArr[getKeeper().curHackNum]->tpHack(&p1Pos);
  }

	return isFirstStep;
}

