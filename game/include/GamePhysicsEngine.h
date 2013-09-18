#ifndef __GAMEPHYSICSENGINE_H_
#define __GAMEPHYSICSENGINE_H_

#include "bullet/LinearMath/btAlignedObjectArray.h"
#include <btBulletDynamicsCommon.h>

#include <ngl/Vec3.h>
#include <ngl/Camera.h>
#include <ngl/ShaderLib.h>
#include <ngl/TransformStack.h>
#include <ngl/Obj.h>
#include <vector>
#include <map>
#include "GameDrawEngine.h"


struct collisionMesh
{
  std::vector<unsigned int> m_index;
  std::vector<float> m_verts;
};

typedef enum
{
    SIMPLE_BOX,
    COMPLEX_CONVEX
}collisionShapes;


typedef enum
{
    CRATE_AMMUNITION,
    INVISIBLE_WALL,
    INVISIBLE_CAM_WALL
}boundingExtras;

/// @file GamePhysicsEngine.h
/// @author Santosh Krishnan
/// @version 1.0
/// @date 28/2/13
/// @class GamePhysicsEngine
/// @brief GamePhysicsEngine class is the bullet physics engine class handling the players and obstacle movements and detecting collisions and
/// applying impulse and gravity on the models.

class GamePhysicsEngine
{


    public:

        //----------------------------------------------------------------------------------------------------------------------
        /// @brief def ctor
        //----------------------------------------------------------------------------------------------------------------------
        GamePhysicsEngine();

        //----------------------------------------------------------------------------------------------------------------------
        /// @brief virtual dtor
        //----------------------------------------------------------------------------------------------------------------------
        virtual ~GamePhysicsEngine();

        //----------------------------------------------------------------------------------------------------------------------
        /// @brief gets triangulated mesh from obj
        //----------------------------------------------------------------------------------------------------------------------
        void getMeshFromObj(ngl::Obj *_m);

        //----------------------------------------------------------------------------------------------------------------------
        /// @brief step simulation module
        //----------------------------------------------------------------------------------------------------------------------
        void step();

         //----------------------------------------------------------------------------------------------------------------------
        /// @brief static ground plane
        //----------------------------------------------------------------------------------------------------------------------
        void addGround(const ngl::Vec3 &_pos);

        //----------------------------------------------------------------------------------------------------------------------
        /// @brief box shaped coll box for ammunition
        //----------------------------------------------------------------------------------------------------------------------
        void addBox(const ngl::Vec3 &_pos, ngl::Vec4 _scale, boundingExtras _inputRbodyType);

        //----------------------------------------------------------------------------------------------------------------------
        /// @brief add sky box
        //----------------------------------------------------------------------------------------------------------------------
        void addSkyBox(const ngl::Vec3 &_pos, ngl::Vec4 _bounds);

        //----------------------------------------------------------------------------------------------------------------------
        /// @brief used in level3 to move tanks
        //----------------------------------------------------------------------------------------------------------------------
        void moveBox(int _bodyreference, float _inputimpulse);


        //----------------------------------------------------------------------------------------------------------------------
        /// @brief add our convex player
        //----------------------------------------------------------------------------------------------------------------------
        void addConvexGrenade(const ngl::Vec3 &_pos);

        //----------------------------------------------------------------------------------------------------------------------
        /// @brief add our convex tank
        //----------------------------------------------------------------------------------------------------------------------
        void addConvexStones(const ngl::Vec3 &_pos,const ngl::Vec3 &_rot);

        //----------------------------------------------------------------------------------------------------------------------
        /// @brief add impulse to player
        //----------------------------------------------------------------------------------------------------------------------
        void moveGrenade(int, int _inputUpDownAccumulated, int _inputWindValue = 0);

        //----------------------------------------------------------------------------------------------------------------------
        /// @brief sense the collison using continuous collision detection
        //----------------------------------------------------------------------------------------------------------------------
        void senseCollision();


        //----------------------------------------------------------------------------------------------------------------------
        /// @brief gets a particular collision shape
        /// @param _index to get from
        //----------------------------------------------------------------------------------------------------------------------
        int getCollisionShape(unsigned int _index) const;

        //----------------------------------------------------------------------------------------------------------------------
        /// @brief gets a transformation matrix
        /// @param _index to get from
        //----------------------------------------------------------------------------------------------------------------------
        ngl::Mat4 getTransformMatrix(unsigned int _index);

        //----------------------------------------------------------------------------------------------------------------------
        /// @brief gets a particular position
        /// @param _index to get from
        //----------------------------------------------------------------------------------------------------------------------
        ngl::Vec3 getPosition(unsigned int _index);

        //----------------------------------------------------------------------------------------------------------------------
        /// @brief gets total number of collision objects
        //----------------------------------------------------------------------------------------------------------------------
        unsigned int getNumCollisionObjects()const {return m_dynamicsWorld->getNumCollisionObjects();}


        //----------------------------------------------------------------------------------------------------------------------
        /// @brief gets the simulation flag
        //----------------------------------------------------------------------------------------------------------------------
        bool getSimulationFlag();


        //----------------------------------------------------------------------------------------------------------------------
        /// @brief sets the grenade obstacle count value used for scoring
        //----------------------------------------------------------------------------------------------------------------------
        void setGrenadeObstacleCcount(int _noOfLives);


        //----------------------------------------------------------------------------------------------------------------------
        /// @brief gets the obstacle count value used for scoring
        //----------------------------------------------------------------------------------------------------------------------
        float getGrenadeObstacleCcount();

        //----------------------------------------------------------------------------------------------------------------------
        /// @brief get the game won flag if grenade hits the ammunition
        //----------------------------------------------------------------------------------------------------------------------
        bool getGameWonFlag();

    private:


        //----------------------------------------------------------------------------------------------------------------------
        /// @brief flag to indicate simulation
        //----------------------------------------------------------------------------------------------------------------------
        bool doingSimulationInAir;


        //----------------------------------------------------------------------------------------------------------------------
        /// @brief collision repository
        //----------------------------------------------------------------------------------------------------------------------
        std::map<std::string,collisionMesh *> m_collisionData;

        //----------------------------------------------------------------------------------------------------------------------
        /// @brief temp obj mesh
        //----------------------------------------------------------------------------------------------------------------------
        ngl::Obj* tempObj;

        //----------------------------------------------------------------------------------------------------------------------
        /// @brief pointer to determine the collison shape
        //----------------------------------------------------------------------------------------------------------------------
        void *boxptr;

        //----------------------------------------------------------------------------------------------------------------------
        /// @brief pointer to determine the collison shape
        //----------------------------------------------------------------------------------------------------------------------
        void *wallptr;

        //----------------------------------------------------------------------------------------------------------------------
        /// @brief pointer to determine the collison shape
        //----------------------------------------------------------------------------------------------------------------------
        void *grenadeptr;

        //----------------------------------------------------------------------------------------------------------------------
        /// @brief pointer to determine the collison shape
        //----------------------------------------------------------------------------------------------------------------------
        void *obstacleptr;

        //----------------------------------------------------------------------------------------------------------------------
        /// @brief pointer to determine the collison shape
        //----------------------------------------------------------------------------------------------------------------------
       int grenadeObstacleCcount;

       //----------------------------------------------------------------------------------------------------------------------
       /// @brief game won flag when grenade hits ammunition
       //----------------------------------------------------------------------------------------------------------------------
        bool gameWonFlag;

        //----------------------------------------------------------------------------------------------------------------------
        /// @brief standard bullet variables to store collison shapes, the broadphase filter, the dispatcher, the constraint
        /// solver, defaut collision config, the discrete world, triangulated mesh, static plane
        //----------------------------------------------------------------------------------------------------------------------
        btAlignedObjectArray<btCollisionShape*>	m_collisionShapes;

        btBroadphaseInterface*	m_broadphase;

        btCollisionDispatcher*	m_dispatcher;

        btConstraintSolver*	m_solver;

        btDefaultCollisionConfiguration* m_collisionConfiguration;

        btDiscreteDynamicsWorld* m_dynamicsWorld;

        btTriangleMesh *mesh;

        btBvhTriangleMeshShape *meshShape;

        btStaticPlaneShape *planeShape;


        //----------------------------------------------------------------------------------------------------------------------
        /// @brief sets the game won flag when grande hits amunition box
        //----------------------------------------------------------------------------------------------------------------------
        void setGameWonFlag();


};

#endif // __GAMEPHYSICSENGINE_H_
