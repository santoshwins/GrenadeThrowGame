#include <ngl/Random.h>
#include <ngl/VAOPrimitives.h>
#include "GameLevel.h"
#include "GameModelFactory.h"


GameLevel::GameLevel()
{
    _drawEngine = 0;
    _physicsEngine = 0;
    _screenHeight = 0;
    _screenWidth = 0;
    _player = 0;
    m_difficulty = BEGINNER;
    accumulatedDirection = 0;
    accumulatedAcceleration = 0;
    arrowDraw = 1;
    m_freeCamFlag = 0;
    ngl::VAOPrimitives *prim = ngl::VAOPrimitives::instance();
    prim->createLineGrid("plane",140,140,80);
}

GameLevel::GameLevel(GameDrawEngine *_inputDrawEngine, GamePhysicsEngine *_inputPhysicsEngine, int _inputScreenHeight, int _inputScreenWidth)
{
    _drawEngine = _inputDrawEngine;
    _physicsEngine = _inputPhysicsEngine;
    _screenHeight = _inputScreenHeight;
    _screenWidth = _inputScreenWidth;
    _player = 0;
    m_difficulty = BEGINNER;
    accumulatedDirection = 0;
    accumulatedAcceleration = 0;
    arrowDraw = 1;
    m_freeCamFlag = 0;
}


void GameLevel::drawLevel()
{

    ngl::Transformation trans;

    unsigned int bodies=_physicsEngine->getNumCollisionObjects();

    ngl::Mat4 MVP;
    ngl::ShaderLib *shader=ngl::ShaderLib::instance();
    (*shader)["Texture"]->use();

    for(int i=0; (i<bodies) && (i<= _numberOfObstacles); ++i)
    {

        trans.setMatrix(_physicsEngine->getTransformMatrix(i));
        m_transformStack.setCurrent(trans);
        if ((i==0) && (m_freeCamFlag == 0))
        {
           ngl::Vec3 grenadeCurrentPos = _physicsEngine->getPosition(i);

           if((_drawEngine->getActiveCam() == _drawEngine->getSpecificCam(CAM_DEFAULT)))
            {
                _drawEngine->getActiveCam()->setEye(ngl::Vec4((grenadeCurrentPos.m_x),30,(grenadeCurrentPos.m_z - 40)));
            }
            else if((_drawEngine->getActiveCam() == _drawEngine->getSpecificCam(CAM_TOP)))
            {
                 _drawEngine->getActiveCam()->setEye(ngl::Vec4(grenadeCurrentPos.m_x,200,grenadeCurrentPos.m_z,1));
            }
            else if((_drawEngine->getActiveCam() == _drawEngine->getSpecificCam(CAM_LEFT)))
            {
                _drawEngine->getActiveCam()->setEye(ngl::Vec4((grenadeCurrentPos.m_x + 80),30,grenadeCurrentPos.m_z,1));

            }
            else if((_drawEngine->getActiveCam() == _drawEngine->getSpecificCam(CAM_RIGHT)))
            {
                _drawEngine->getActiveCam()->setEye(ngl::Vec4((grenadeCurrentPos.m_x - 80),30,grenadeCurrentPos.m_z,1));

            }
     }




     MVP= m_transformStack.getCurrentTransform().getMatrix() * _drawEngine->getActiveCam()->getVPMatrix(); //m_modelRepos[_modelIndex]->getCam()->getVPMatrix() ;
     shader->setShaderParamFromMat4("qt_ModelViewProjectionMatrix",MVP);

     switch(_physicsEngine->getCollisionShape(i))
     {
       case BOX_SHAPE_PROXYTYPE :
        {
            if ((m_modelRepos[i]->getModelName() == MODEL_CRATEBOX) && (_physicsEngine->getGameWonFlag() == false))
            {
                _drawEngine->drawModels(m_modelRepos[i]);
            }
            break;
        }
       case CONVEX_HULL_SHAPE_PROXYTYPE:
        {
            _drawEngine->drawModels(m_modelRepos[i]);
            break;
        }

        case STATIC_PLANE_PROXYTYPE:
        {
            ngl::TransformStack tempTransformation = m_modelRepos[i]->getTransformStack();

            tempTransformation.pushTransform();
            {
                tempTransformation.getCurrentTransform().setPosition(m_modelRepos[i]->getPosition());
                tempTransformation.getCurrentTransform().setRotation(m_modelRepos[i]->getRotation());
                tempTransformation.getCurrentTransform().setScale(m_modelRepos[i]->getScale());
                MVP= tempTransformation.getCurrentTransform().getMatrix() * _drawEngine->getActiveCam()->getVPMatrix(); //m_modelRepos[_modelIndex]->getCam()->getVPMatrix() ;
                shader->setShaderParamFromMat4("qt_ModelViewProjectionMatrix",MVP);
                _drawEngine->drawModels(m_modelRepos[i]);
            }
            tempTransformation.popTransform();
            break;
        }

      }
    }


    drawExtras();

}

void GameLevel::addPlayerToLevel()
{
    m_modelRepos[MODEL_PLAYER] = GameModelFactory::makeGameModel(MODEL_PLAYER);
    m_modelRepos[MODEL_PLAYER]->setDrawArea(_drawEngine);
    _player = m_modelRepos[MODEL_PLAYER];
}

GameModel* GameLevel::getPlayer()
{
    return _player;
}

void GameLevel::addObstaclesToLevel()
{
    ngl::Random *randObj = ngl::Random::instance();

    if (m_difficulty == BEGINNER)
    {
        _numberOfObstacles = randObj->randomPositiveNumber(20);
    }
    else
    {
        _numberOfObstacles = randObj->randomPositiveNumber(25);
    }


   for(int i=1;i<=_numberOfObstacles;i++) // 0th pos is reserved for PLAYER,so obstacle shud not be inserted
                                          //hence loop from i=1
   {
        std::cout<<"coming in addobstacles";
        if ( i != 1)
        {
            m_modelRepos[i] = m_modelRepos[i - 1]->cloneItself();
        }
        else
        {
            m_modelRepos[i] = GameModelFactory::makeGameModel(MODEL_OBSTACLE);
            m_modelRepos[i]->setDrawArea(_drawEngine);
        }
    }

    _numberOfObstacles++;

}

void GameLevel::addCrateBoxToLevel()
{

    m_modelRepos[_numberOfObstacles] = GameModelFactory::makeGameModel(MODEL_CRATEBOX);
    m_modelRepos[_numberOfObstacles]->setDrawArea(_drawEngine);
    m_modelRepos[_numberOfObstacles]->setPosition(ngl::Vec3(-80,0,m_modelRepos[_numberOfObstacles - 1]->getPosition().m_z + 380));
     _numberOfObstacles++;
}


void GameLevel::addBGToLevel()
{
    m_modelRepos[_numberOfObstacles] = new GameModel(_drawEngine,ngl::Vec3(0,0,0),ngl::Vec4(0,0,0,1),ngl::Vec4(100,0,300,1),"models/ground_plane.obj","textures/grass.jpg",MODEL_BGS);
    _numberOfObstacles++;
    m_modelRepos[_numberOfObstacles] = new GameModel(_drawEngine,ngl::Vec3(0,-50,0),ngl::Vec4(0,0,0,1),ngl::Vec4(50,50,50,1),"models/ammunition.obj","textures/SkyBox_Clouds.png",MODEL_SKY);

}

void GameLevel::setLevelDifficulty(DIFFICULTY_LEVELS _difficulty)
{
    m_difficulty = _difficulty;
}

void GameLevel::setLevelNumber(GAME_LEVELS _inputLevelNumber)
{
    m_levelNumber = _inputLevelNumber;
}

GAME_LEVELS GameLevel::getLevelNumber()
{
    return m_levelNumber;
}

void GameLevel::simulateAI()
{


    static int x = 0; // temp variable to delay change in direction of tank movements in level3
    static float velocity = 0.1;
    if (  x > 100)
    {
        x = 0;
        velocity = (-velocity);
    }


    for(int i=0;i<=_numberOfObstacles;i++)

    {
         if((i+1) <= _numberOfObstacles)
         {
            switch(m_modelRepos[i + 1]->getModelName())
            {
                case MODEL_OBSTACLE:
                {
                     _physicsEngine->moveBox((i + 1),velocity); // according to bullet, our obstacles start from 2, so i + 1
                      break;
                }
                default:
                    break;
            }
        }
    }

    x++;
}


void GameLevel::addCollisionBoxes()
{

    ngl::Vec3 pos = 0;
    ngl::Vec3 rot = 0;
    ngl::Vec4 scale = 0;


    for(int i=0;i<m_modelRepos.size();i++)
    {
         pos = m_modelRepos[i]->getPosition();
         scale = m_modelRepos[i]->getScale();
         rot = m_modelRepos[i]->getRotation();
         std::cout<<"scale is :"<<scale;
         switch(m_modelRepos[i]->getModelName())
         {
            case MODEL_PLAYER:
            {
                _physicsEngine->addConvexGrenade(pos);
                break;
            }
            case MODEL_OBSTACLE:
            {
                _physicsEngine->addConvexStones(pos,rot);
                break;
            }
            case MODEL_CRATEBOX:
            {
                _physicsEngine->addBox(pos,(scale * 10),CRATE_AMMUNITION);  // this 10 has to be given appropriate value if ammunition box is scaled
                break;
            }

            case MODEL_BGS:
            {
                _physicsEngine->addGround(pos);
                break;
            }
         }
     }

        // add all 3 walls - left, right and far beyond the last goal - crate box
        _physicsEngine->addBox(ngl::Vec3(250,0,-100),ngl::Vec4(10,1000,1000,1),INVISIBLE_WALL); // change the 1000 and 2 to constant values
        _physicsEngine->addBox(ngl::Vec3(-250,0,-100),ngl::Vec4(10,1000,1000,1),INVISIBLE_WALL);
        _physicsEngine->addBox(ngl::Vec3(0,0,m_graphicRepos[BOUNDARY_WALL_BACK]->getPosition().m_z),ngl::Vec4(1000,1000,10,1),INVISIBLE_WALL);
}


void GameLevel::moveGrenade()
{
    // if its level3 call this func with 3 args..so third arg as to be got via a random number generator

    _physicsEngine->moveGrenade((accumulatedDirection),accumulatedAcceleration);
}

void GameLevel::moveGrenade(int _windValue)
{
    // if its level3 call this func with 3 args..so third arg as to be got via a random number generator

    _physicsEngine->moveGrenade((accumulatedDirection),accumulatedAcceleration,_windValue);
}


void GameLevel::setArrowDrawFlag(int _inputKey)
{
    arrowDraw = _inputKey;
   // std::cout<<"arrow key state is \n"<<arrowDraw;
}

void GameLevel::accumulateDirection(int _inputdir)
{
    ngl::Transformation trans;
    trans.setMatrix(_physicsEngine->getTransformMatrix(0));

    if(_inputdir != 0)
    {
        accumulatedDirection = accumulatedDirection + _inputdir;


        if(accumulatedDirection > 90)
        {
            accumulatedDirection = 90;
        }
        else if (accumulatedDirection < -90)
        {
            accumulatedDirection = -90;
        }

    m_graphicRepos[DIRECTION_ARROW]->setRotation(ngl::Vec4(0,accumulatedDirection,0,1));

    }
    else if (_inputdir == 0)
    {

        m_graphicRepos[DIRECTION_ARROW]->setRotation(ngl::Vec4(0,0,0,1));

    }
}


void GameLevel::accumulateAcceleration(int _inputAcceleration)
{


    if(_inputAcceleration != 0)
    {
        accumulatedAcceleration = accumulatedAcceleration + _inputAcceleration;


        if(accumulatedAcceleration >= 40)
        {
            accumulatedAcceleration = 40;
        }

        else if (accumulatedAcceleration <= 0)
        {
            accumulatedAcceleration = 0;
        }

    }

    else if (_inputAcceleration == 0) // for down arrow to reduce power
    {
        accumulatedAcceleration = 0;
    }

}


void GameLevel::addGraphicImages()
{
    m_graphicRepos[DIRECTION_ARROW] = new GameModel(_drawEngine,ngl::Vec3(0,0,0),/*_physicsEngine->getPosition(MODEL_PLAYER),*/ngl::Vec4(0,0,0,1),ngl::Vec4(3,1,3,1),"models/arrow.obj","textures/Stone.png",MODEL_GRAPHICS);

    m_graphicRepos[BOUNDARY_WALL_LEFT] = new GameModel(_drawEngine,ngl::Vec3(250,0,-100),ngl::Vec4(0,90,0,1),ngl::Vec4(10,10,10,1),"models/sandbags_02.obj","textures/sandbags.jpg",MODEL_GRAPHICS);

    m_graphicRepos[BOUNDARY_WALL_RIGHT] = new GameModel(_drawEngine,ngl::Vec3(-250,0,-100),ngl::Vec4(0,-90,0,1),ngl::Vec4(10,10,10,1),"models/sandbags_02.obj","textures/sandbags.jpg",MODEL_GRAPHICS);


    // the back wall of the world is always placed beyond the last obstacle..so this is not hardcoded..each time the final crate box' position changes, our world's back wall will change accordingly
    m_graphicRepos[BOUNDARY_WALL_BACK] = new GameModel(_drawEngine,ngl::Vec3(0,0,m_modelRepos[_numberOfObstacles - 2]->getPosition().m_z + 3),ngl::Vec4(0,0,0,1),ngl::Vec4(10,10,10,1),"models/sandbags_02.obj","textures/sandbags.jpg",MODEL_GRAPHICS);

}


// draws other extra meshes such as skybox, boundary walls(predifned locations) and graphical direction arrows(at the position of the player)
void GameLevel::drawExtras()
{

     ngl::TransformStack tempTransformStack;

    for(int i=0;i<m_graphicRepos.size();i++)
    {

        switch (i)
        {
        case DIRECTION_ARROW:

            if((arrowDraw == 1) && (_physicsEngine->getSimulationFlag() == 0)) // just to make sure the grenade is not in air while direction arrow is drawn
            {
            tempTransformStack = m_graphicRepos[DIRECTION_ARROW]->getTransformStack();
            loadModelsWithoutBulletTransformations(tempTransformStack,_physicsEngine->getPosition(0),m_graphicRepos[DIRECTION_ARROW]->getRotation(),m_graphicRepos[DIRECTION_ARROW]->getScale());

            _drawEngine->drawModels(m_graphicRepos[DIRECTION_ARROW]);
            }
            break;

        case BOUNDARY_WALL_LEFT:
            tempTransformStack = m_graphicRepos[BOUNDARY_WALL_LEFT]->getTransformStack();

            for (float i = -250; i <= 250;i=i+55)
            {
                loadModelsWithoutBulletTransformations(tempTransformStack,ngl::Vec3(m_graphicRepos[BOUNDARY_WALL_LEFT]->getPosition().m_x,0,i),m_graphicRepos[BOUNDARY_WALL_LEFT]->getRotation(),m_graphicRepos[BOUNDARY_WALL_LEFT]->getScale());

                _drawEngine->drawModels(m_graphicRepos[BOUNDARY_WALL_LEFT]);
            }

            break;
        case BOUNDARY_WALL_RIGHT:
            tempTransformStack = m_graphicRepos[BOUNDARY_WALL_RIGHT]->getTransformStack();
            for (float i = -250; i <= 250;i=i+55)
            {
                loadModelsWithoutBulletTransformations(tempTransformStack,ngl::Vec3(m_graphicRepos[BOUNDARY_WALL_RIGHT]->getPosition().m_x,0,i),m_graphicRepos[BOUNDARY_WALL_RIGHT]->getRotation(),m_graphicRepos[BOUNDARY_WALL_RIGHT]->getScale());

                _drawEngine->drawModels(m_graphicRepos[BOUNDARY_WALL_RIGHT]);
            }
            break;
        case BOUNDARY_WALL_BACK:
            tempTransformStack = m_graphicRepos[BOUNDARY_WALL_BACK]->getTransformStack();

            for (float i = -250; i <= 250;i=i+55)
            {
                loadModelsWithoutBulletTransformations(tempTransformStack,ngl::Vec3(i,0,m_graphicRepos[BOUNDARY_WALL_BACK]->getPosition().m_z),m_graphicRepos[BOUNDARY_WALL_BACK]->getRotation(),m_graphicRepos[BOUNDARY_WALL_BACK]->getScale());
                 _drawEngine->drawModels(m_graphicRepos[BOUNDARY_WALL_BACK]);
            }



            break;

        default:
            break;
        }


    }


    // drawing SKY BOX which is our last obstacle in the model repository
    loadModelsWithoutBulletTransformations(m_modelRepos[_numberOfObstacles]->getTransformStack(),m_modelRepos[_numberOfObstacles]->getPosition(),m_modelRepos[_numberOfObstacles]->getRotation(),m_modelRepos[_numberOfObstacles]->getScale());
    _drawEngine->drawModels(m_modelRepos[_numberOfObstacles]);



}


void GameLevel::loadModelsWithoutBulletTransformations(ngl::TransformStack _transformStack, const ngl::Vec3& _pos,const ngl::Vec4& _rot, const ngl::Vec4& _scale)
{

    ngl::Mat4 MVP;

    ngl::ShaderLib *shader=ngl::ShaderLib::instance();


    {
    _transformStack.getCurrentTransform().setPosition(_pos);
    _transformStack.getCurrentTransform().setRotation(_rot);
    _transformStack.getCurrentTransform().setScale(_scale);
    MVP= _transformStack.getCurrentTransform().getMatrix() * _drawEngine->getActiveCam()->getVPMatrix(); //m_modelRepos[_modelIndex]->getCam()->getVPMatrix() ;
    shader->setShaderParamFromMat4("qt_ModelViewProjectionMatrix",MVP);
    }

}

void GameLevel::setFreeCamFlag(int _inputFreeCamFlag )
{
    m_freeCamFlag = _inputFreeCamFlag;
}

GameLevel::~GameLevel()
{

}

