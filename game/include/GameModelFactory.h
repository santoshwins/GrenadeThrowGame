#ifndef __GAMEMODELFACTORY_H_
#define __GAMEMODELFACTORY_H_
#include "GamePlayer.h"
#include "GameObstacles.h"
#include "GameCrateBox.h"


/// @file GameModelFactory.h
/// @author Santosh Krishnan
/// @version 1.0
/// @date 28/2/13
/// @class GameModelFactory
/// @brief GameModelFactory class is the factory class handling the creation of 3 types of major game models namely
/// GamePlayer,GameObstacle,GameCrateBox

//do harward referencing later---idea taken from http://sourcemaking.com/design_patterns/prototype/cpp/1

//dofactory.com/Patterns/PatternFactory.aspx#_self1

class GameModelFactory
{
public:

    //----------------------------------------------------------------------------------------------------------------------
    /// @brief game model production module
    //----------------------------------------------------------------------------------------------------------------------
    static GameModel* makeGameModel(MODEL_NAMES);
private:
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief enum model types
    //----------------------------------------------------------------------------------------------------------------------
    static GameModel* _modelProtoTypes[3];

    //----------------------------------------------------------------------------------------------------------------------
    /// @brief private ctor
    //----------------------------------------------------------------------------------------------------------------------
    GameModelFactory();

    //----------------------------------------------------------------------------------------------------------------------
    /// @brief private dtor
    //----------------------------------------------------------------------------------------------------------------------
    ~GameModelFactory();

    //----------------------------------------------------------------------------------------------------------------------
    /// @brief private copy const
    //----------------------------------------------------------------------------------------------------------------------
    GameModelFactory(const GameModelFactory&);

};

#endif // __GAMEMODELFACTORY_H_
