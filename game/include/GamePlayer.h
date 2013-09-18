#ifndef __GAMEPLAYER_H_
#define __GAMEPLAYER_H_

#include "GameModel.h"

/// @file GamePlayer.h
/// @author Santosh Krishnan
/// @version 1.0
/// @date 28/2/13
/// @class GamePlayer
/// @brief GamePlayer class is the main hero class handling the creationg of grenade instantiated through gamemodelfactoy and inherits
/// GameModel

class GamePlayer:public GameModel
{
    public:

        //----------------------------------------------------------------------------------------------------------------------
        /// @brief def ctor
        //----------------------------------------------------------------------------------------------------------------------
        GamePlayer();

        //----------------------------------------------------------------------------------------------------------------------
        /// @brief param ctor
        //----------------------------------------------------------------------------------------------------------------------
        GamePlayer(
                   GameDrawEngine *_drawEngine,
                   ngl::Vec3 _pos,
                   ngl::Vec4 _rot,
                   ngl::Vec4 _scale,
                   std::string _fname,
                   std::string _tname,
                   MODEL_NAMES _modelName
                 );

        //----------------------------------------------------------------------------------------------------------------------
        /// @brief clones itself to produce game obstacle everytime randomly
        //----------------------------------------------------------------------------------------------------------------------
        GamePlayer* cloneItself();

};

#endif // __GAMEPLAYER_H_

