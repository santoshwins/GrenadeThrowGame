#ifndef __GAMECRATEBOX_H_
#define __GAMECRATEBOX_H_
#include "GameObstacles.h"

/// @file GameCrateBox.h
/// @author Santosh Krishnan
/// @version 1.0
/// @date 28/2/13
/// @class GameCrateBox
/// @brief GameCrateBox class handles the creation (cloning) of the ammunition at a position determined by the previous obstacle placed in the world.
/// This is instantiated through GameModelFactory implementation.It is a kind of obstacle, so inherits obstacle and shares the properties.

class GameCrateBox:public GameObstacles
{
 public:
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief def ctor
    //----------------------------------------------------------------------------------------------------------------------
    GameCrateBox();
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief ctor to create the ammunition box to be placed at the end of the level
    //----------------------------------------------------------------------------------------------------------------------
    GameCrateBox(
                         GameDrawEngine *_drawEngine,
                         ngl::Vec3 _pos,
                         ngl::Vec4 _rot,
                         ngl::Vec4 _scale,
                         std::string _fname,
                         std::string _tname, MODEL_NAMES _modelName
                 );
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief cloning module to create itself
    //----------------------------------------------------------------------------------------------------------------------
    GameCrateBox* cloneItself();
};

#endif // __GAMECRATEBOX_H_
