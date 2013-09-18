#include "GameModelFactory.h"


GameModel* GameModelFactory::_modelProtoTypes[] = { new GamePlayer, new GameObstacles, new GameCrateBox};

GameModel* GameModelFactory::makeGameModel(MODEL_NAMES _inputModelName)
{
    return _modelProtoTypes[_inputModelName]->cloneItself();
}

