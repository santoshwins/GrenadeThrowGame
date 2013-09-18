#include "GameModel.h"
#include <ngl/ShaderLib.h>


GameModel::GameModel()
{
    drawArea = 0;
}

GameModel::GameModel(
                     GameDrawEngine *_drawEngine,
                     ngl::Vec3 _pos,
                     ngl::Vec4 _rot,
                     ngl::Vec4 _scale,
                     std::string _fname,
                     std::string _tname,
                     MODEL_NAMES _modelName
                     )
 {
   drawArea = _drawEngine;
   modelName = _modelName;
   m_pos=_pos;
   m_mesh = new ngl::Obj(_fname,_tname);
   m_mesh->createVAO();
   m_rotation = _rot;
   m_scale = _scale;


 }



ngl::TransformStack GameModel::getTransformStack()
{
    return m_transform;
}

ngl::Obj* GameModel::getMesh()
{
    return m_mesh;
}

ngl::Vec3 GameModel::getPosition()
{
    return m_pos;
}

ngl::Vec4 GameModel::getRotation()
{
    return m_rotation;
}

void GameModel::setPosition(ngl::Vec3 _pos)
{
    m_pos = _pos;
}

void GameModel::setRotation(ngl::Vec4 _rot)
{
   m_rotation = _rot;
}

void GameModel::setScale(ngl::Vec4 _scale)
{
   m_scale = _scale;
}

ngl::Vec4 GameModel::getScale()
{
   return m_scale;
}


MODEL_NAMES GameModel::getModelName()
{
    return modelName;
}



void GameModel::setDrawArea(GameDrawEngine* _inputDrawEngine)
{
    drawArea = _inputDrawEngine;
}

GameModel* GameModel::cloneItself()
{

}
