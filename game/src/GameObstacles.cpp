#include "GameObstacles.h"
#include <ngl/Random.h>

GameObstacles::GameObstacles():GameModel()
{
    //drawArea = 0;
}

GameObstacles::GameObstacles(
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


GameObstacles* GameObstacles::cloneItself()
{

    std::cout<<"in cloneitself";
    ngl::Random *randObj = ngl::Random::instance();
    static int i = 25; // a hack kind of logic to make sure that the objects are not too close to each other
    i = -i;
    ngl::Vec3 pos = ngl::Vec3(randObj->getRandomPoint(this->m_pos.m_x + (randObj->randomPositiveNumber(2) * 30),0,this->m_pos.m_z + 100));
    pos.m_z += i;
    if (abs(this->m_pos.m_z - pos.m_z) < 5)
    {
        pos.m_z += (i * 2);
    }

    return new GameObstacles(this->drawArea,pos,ngl::Vec4(0,0,0,1),ngl::Vec4(1,1,1,1),"models/truck.obj","textures/Grenade.tga",MODEL_OBSTACLE);


}

