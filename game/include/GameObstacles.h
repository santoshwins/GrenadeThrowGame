#ifndef __GAMEOBSTACLE_H_
#define __GAMEOBSTACLE_H_

#include "GameModel.h"

/// @file GameObstacles.h
/// @author Santosh Krishnan
/// @version 1.0
/// @date 28/2/13
/// @class GameObstacles
/// @brief GameObstacles class is the class handling the creation of the obstacle(tanks) at random positions in the world
/// instatiated initially through the gamemodel factory inherting GameModel

class GameObstacles:public GameModel
{
public:

    //----------------------------------------------------------------------------------------------------------------------
    /// @brief def ctor
    //----------------------------------------------------------------------------------------------------------------------
    GameObstacles();

    //----------------------------------------------------------------------------------------------------------------------
    /// @brief parameterized ctor
    //----------------------------------------------------------------------------------------------------------------------
    GameObstacles(
                         GameDrawEngine *_drawEngine,
                         ngl::Vec3 _pos,
                         ngl::Vec4 _rot,
                         ngl::Vec4 _scale,
                         std::string _fname, std::string _tname, MODEL_NAMES _modelName
                 );

    //----------------------------------------------------------------------------------------------------------------------
    /// @brief clones itself to produce game obstacle everytime randomly
    //----------------------------------------------------------------------------------------------------------------------
    GameObstacles* cloneItself();

};

#endif // __GAMEOBSTACLE_H_
