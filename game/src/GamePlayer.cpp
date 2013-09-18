#include "GamePlayer.h"

GamePlayer::GamePlayer():GameModel()
{

}

GamePlayer::GamePlayer(
                     GameDrawEngine *_drawEngine,
                     ngl::Vec3 _pos,
                    ngl::Vec4 _rot,
                    ngl::Vec4 _scale,
                     std::string _fname,
                     std::string _tname,
                     MODEL_NAMES _modelName
        ):GameModel(_drawEngine,_pos,_rot,_scale,_fname,_tname,_modelName)

 {

 }


GamePlayer* GamePlayer::cloneItself()
{

    std::cout<<"in cloneitself";

    return new GamePlayer(this->drawArea,ngl::Vec3(-3,0,-50),ngl::Vec4(0,0,0,1),ngl::Vec4(1,1,1,1),"models/Grenade.obj","textures/Grenade.tga",MODEL_PLAYER);
}

