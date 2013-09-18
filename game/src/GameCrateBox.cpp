#include "GameCrateBox.h"
#include <ngl/Random.h>

GameCrateBox::GameCrateBox():GameObstacles()
{

}

GameCrateBox::GameCrateBox(
                     GameDrawEngine *_drawEngine,
                     ngl::Vec3 _pos,
                     ngl::Vec4 _rot,
                    ngl::Vec4 _scale,
                     std::string _fname,
                     std::string _tname,
                     MODEL_NAMES _modelName
        ):GameObstacles(_drawEngine,_pos,_rot,_scale,_fname,_tname,_modelName)

 {

 }


GameCrateBox* GameCrateBox::cloneItself()
{

    return new GameCrateBox(this->drawArea,ngl::Vec3(10,0,0),ngl::Vec4(0,0,0,1),ngl::Vec4(1,1,1,1),"models/ammunition.obj","textures/ammos_box.jpg",MODEL_CRATEBOX);

}


