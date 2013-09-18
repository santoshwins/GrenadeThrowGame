#include "GameDrawEngine.h"
#include "GameModel.h"
#include "log.h"
#include "boost/current_function.hpp"



GameDrawEngine::GameDrawEngine()
{

}

GameDrawEngine::GameDrawEngine(int _screenHeight, int _screenWidth)
{
    screenHeight = _screenHeight;
    screenWidth = _screenWidth;
    _activeCam = CAM_DEFAULT;
}




void GameDrawEngine::drawModels(GameModel* _inputModel)
{
     _inputModel->getMesh()->draw();
}


void GameDrawEngine::addCamToRepos(CAMERA_NAMES _name,ngl::Camera * _cam)
{
    m_camRepos[_name] = _cam;
}

void GameDrawEngine::setActiveCam(CAMERA_NAMES _camName)
{
    _activeCam = _camName;
}

ngl::Camera* GameDrawEngine::getActiveCam()
{
    return m_camRepos[_activeCam];
}


void GameDrawEngine::setScreenHeight(float _inputScreenHeight)
{
    screenHeight = _inputScreenHeight;
}

float GameDrawEngine::getScreenHeight()
{
    return screenHeight;
}

void GameDrawEngine::setScreenWidth(float _inputScreenWidth)
{
  screenWidth = _inputScreenWidth;
}

float GameDrawEngine::getScreenWidth()
{
    return screenWidth;
}


ngl::Camera* GameDrawEngine::getSpecificCam(CAMERA_NAMES _inputCameraName)
{
    return m_camRepos[_inputCameraName];
}

GameDrawEngine::~GameDrawEngine()
{

}
