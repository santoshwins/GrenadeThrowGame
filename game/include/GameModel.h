
#ifndef __GAMEMODEL_H_
#define __GAMEMODEL_H_

#include "GameDrawEngine.h"
#include <ngl/Camera.h>
#include <ngl/Vec3.h>
#include <ngl/Obj.h>
#include <ngl/TransformStack.h>


typedef enum
{

    MODEL_PLAYER,
    MODEL_OBSTACLE,
    MODEL_CRATEBOX,
    MODEL_BGS,
    MODEL_SKY,
    MODEL_GRAPHICS

}MODEL_NAMES;



/// @file GameModel.h
/// @author Santosh Krishnan
/// @version 1.0
/// @date 28/2/13
/// @class GameModel
/// @brief GameModel class handles the features of the game models passed into it as objs,textures,positions,rotations and scales.

class GameModel
{




  public :

    //----------------------------------------------------------------------------------------------------------------------
    /// @brief def ctor
    //----------------------------------------------------------------------------------------------------------------------
    GameModel();

    //----------------------------------------------------------------------------------------------------------------------
    /// @brief dtor
    //----------------------------------------------------------------------------------------------------------------------
    ~GameModel();

    //----------------------------------------------------------------------------------------------------------------------
    /// @brief parameterised ctor
    //----------------------------------------------------------------------------------------------------------------------
    GameModel(
               GameDrawEngine *_drawEngine,
               ngl::Vec3 _pos,
               ngl::Vec4 _rot,
               ngl::Vec4 _scale, std::string _fname, std::string _tname, MODEL_NAMES _modelName
              );




    //----------------------------------------------------------------------------------------------------------------------
    /// @brief gets the transform stack
    //----------------------------------------------------------------------------------------------------------------------
     ngl::TransformStack getTransformStack();

     //----------------------------------------------------------------------------------------------------------------------
     /// @brief gets the position
     //----------------------------------------------------------------------------------------------------------------------
     ngl::Vec3 getPosition();

     //----------------------------------------------------------------------------------------------------------------------
     /// @brief gets the models mesh
     //----------------------------------------------------------------------------------------------------------------------
     ngl::Obj* getMesh();

     //----------------------------------------------------------------------------------------------------------------------
     /// @brief gets the rotation
     //----------------------------------------------------------------------------------------------------------------------
     ngl::Vec4 getRotation();


     //----------------------------------------------------------------------------------------------------------------------
     /// @brief sets the rotation
     /// @param _rot roation value
     //----------------------------------------------------------------------------------------------------------------------
     void setRotation(ngl::Vec4 _rot);

     //----------------------------------------------------------------------------------------------------------------------
     /// @brief parameterised ctor
     /// @param _pos position value
     //----------------------------------------------------------------------------------------------------------------------
     void setPosition(ngl::Vec3 _pos);

     //----------------------------------------------------------------------------------------------------------------------
     /// @brief gets the model name
     //----------------------------------------------------------------------------------------------------------------------
     MODEL_NAMES getModelName();

     //----------------------------------------------------------------------------------------------------------------------
     /// @brief gets the cam pointer
     //----------------------------------------------------------------------------------------------------------------------
     ngl::Camera* getCam();

     //----------------------------------------------------------------------------------------------------------------------
     /// @brief sets the drawengine
     /// @param _pos position value
     //----------------------------------------------------------------------------------------------------------------------
     void setDrawArea(GameDrawEngine*);

     //----------------------------------------------------------------------------------------------------------------------
     /// @brief gets draw area
     //----------------------------------------------------------------------------------------------------------------------
     GameDrawEngine* getDrawArea();

     //----------------------------------------------------------------------------------------------------------------------
     /// @brief sets the scale
     //----------------------------------------------------------------------------------------------------------------------
     void setScale(ngl::Vec4 _scale);

     //----------------------------------------------------------------------------------------------------------------------
     /// @brief gets the scale
     //----------------------------------------------------------------------------------------------------------------------
     ngl::Vec4 getScale();



     //----------------------------------------------------------------------------------------------------------------------
     /// @brief parameterised ctor
     //----------------------------------------------------------------------------------------------------------------------
     void setDirection(ngl::Vec3 _inputDirection);


     //----------------------------------------------------------------------------------------------------------------------
     /// @brief clones itself to produce new model
     //----------------------------------------------------------------------------------------------------------------------
     virtual GameModel* cloneItself(); // make this virtual when you derive from this to create separate player and obstacle classes
                               // and implement this in each class
                               // for player cloneitself will have static pos and filename obj etc
                               // and for obstacle we have impleneted here in gamemodel..so take this and
                               // paste in obstacle class so that this cloneitself wil become empty and virtual


  protected :

    //----------------------------------------------------------------------------------------------------------------------
    /// @brief the rotation of the player
    //----------------------------------------------------------------------------------------------------------------------
    ngl::Vec4 m_rotation;

    //----------------------------------------------------------------------------------------------------------------------
    /// @brief our drawengine
    //----------------------------------------------------------------------------------------------------------------------
    GameDrawEngine *drawArea;

    //----------------------------------------------------------------------------------------------------------------------
    /// @brief enum model names
    //----------------------------------------------------------------------------------------------------------------------
    MODEL_NAMES modelName;

    //----------------------------------------------------------------------------------------------------------------------
    /// @brief the position of the player
    //----------------------------------------------------------------------------------------------------------------------
    ngl::Vec3 m_pos;

    //----------------------------------------------------------------------------------------------------------------------
    /// @brief the scale of the player
    //----------------------------------------------------------------------------------------------------------------------
    ngl::Vec4 m_scale;

    /// @brief the mesh
    ngl::Obj *m_mesh;

    /// @brief a transform stack
    ngl::TransformStack m_transform;

};


#endif // __GAMEMODEL_H_
