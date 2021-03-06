#include "SceneManager.h"
#include "MenuScreen.h"
#include "GameScreen.h"
#include "AudioTest.h"
#include "MutinyScreen.h"
#include "GameOverScreen.h"
#include "Audio.h"

using namespace mutiny::engine;

void SceneManager::onAwake()
{
  dontDestroyOnLoad(getGameObject());
  loadLevel();
}

void SceneManager::onLevelWasLoaded()
{
  loadLevel();
}

void SceneManager::loadLevel()
{
  Debug::log("Level loaded: " + Application::getLoadedLevelName());

  Audio::initialize();

  if(Application::getLoadedLevelName() == "mutiny")
  {
    MutinyScreen::create();
  }
  else if(Application::getLoadedLevelName() == "introduction")
  {
    MenuScreen::create();
    GameScreen::create();
  }
  else if(Application::getLoadedLevelName() == "audiotest")
  {
    AudioTest::create();
  }
  else if(Application::getLoadedLevelName() == "game")
  {
    GameScreen::create();
  }
  else if(Application::getLoadedLevelName() == "gameover")
  {
    GameOverScreen::create();
  }
}

