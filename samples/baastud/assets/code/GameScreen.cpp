#include "GameScreen.h"
#include "GameCamera.h"
#include "Sheep.h"
#include "Fence.h"
#include "Player.h"
#include "Terrain.h"
#include "SkyBox.h"
#include "Audio.h"

#include <iostream>

using namespace mutiny::engine;

ref<GameObject> GameScreen::create()
{
  ref<GameObject> mainGo = GameObject::create("GameScreen");
  mainGo->addComponent<GameScreen>();

  return mainGo;
}

ref<GameObject> GameScreen::getFence()
{
  return fenceGo;
}

void GameScreen::onAwake()
{
  audio.reset(new Audio());

  if(Application::getLoadedLevelName() != "introduction")
  {
    Audio::playMusicA();
    Audio::playSound(16);
  }
}

ref<Audio> GameScreen::getAudio()
{
  return audio;
}

ref<GameCamera> GameScreen::getCamera()
{
  return cameraGo->getComponent<GameCamera>();
}

void GameScreen::onGui()
{
  //Gui::label(Rect(10, 10, 100, 100), "game running...");
}

void GameScreen::onStart()
{
  for(int i = 0; i < 20; i++)
    Sheep::create(this);
 
  fenceGo = Fence::create();
  ref<GameObject> playerGo = Player::create(this);
  SkyBox::create(playerGo);
  cameraGo = GameCamera::create(playerGo);
  Terrain::create();
}
