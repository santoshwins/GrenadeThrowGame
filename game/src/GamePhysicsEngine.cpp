/*
Bullet Continuous Collision Detection and Physics Library
Copyright (c) 2003-2006 Erwin Coumans  http://continuousphysics.com/Bullet/

This software is provided 'as-is', without any express or implied warranty.
In no event will the authors be held liable for any damages arising from the use of this software.
Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it freely,
subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.
*/


#include "GamePhysicsEngine.h"
#include <bullet/BulletCollision/CollisionShapes/btShapeHull.h>
#include "iostream" //printf debugging
const static float friction=1000.0f;

GamePhysicsEngine::GamePhysicsEngine()
{

    std::cout<<"came in bullet engine......\n\n\n\n";

    doingSimulationInAir = 0;
    m_collisionConfiguration = new btDefaultCollisionConfiguration();
    m_dispatcher = new btCollisionDispatcher(m_collisionConfiguration);
    m_broadphase = new btDbvtBroadphase();
    ///the default constraint solver. For parallel processing you can use a different solver (see Extras/BulletMultiThreaded)
    btSequentialImpulseConstraintSolver* sol = new btSequentialImpulseConstraintSolver;
    m_solver = sol;
    m_dynamicsWorld = new btDiscreteDynamicsWorld(m_dispatcher,m_broadphase,m_solver,m_collisionConfiguration);
    m_dynamicsWorld->getSolverInfo().m_solverMode = SOLVER_USE_WARMSTARTING + SOLVER_SIMD;
    m_dynamicsWorld->setGravity(btVector3(0,-10,0));
    grenadeObstacleCcount = 0; // initial life of the grenade will be set from game logic
    gameWonFlag = false;


}

GamePhysicsEngine::~GamePhysicsEngine()
{

    //cleanup in the reverse order of creation/initialization

    //remove the rigidbodies from the dynamics world and delete them
    int i;
    for (i=m_dynamicsWorld->getNumCollisionObjects()-1; i>=0 ;i--)
    {
        btCollisionObject* obj = m_dynamicsWorld->getCollisionObjectArray()[i];
        btRigidBody* body = btRigidBody::upcast(obj);
        if (body && body->getMotionState())
        {
            delete body->getMotionState();
        }
        m_dynamicsWorld->removeCollisionObject( obj );
        delete obj;
    }

    //delete collision shapes
    for (int j=0;j<m_collisionShapes.size();j++)
    {
        btCollisionShape* shape = m_collisionShapes[j];
        delete shape;
    }
    m_collisionShapes.clear();

    delete m_dynamicsWorld;

    delete m_solver;

    delete m_broadphase;

    delete m_dispatcher;

    delete m_collisionConfiguration;


}


//taken frm ode example...
void GamePhysicsEngine::getMeshFromObj(ngl::Obj* _m)
{


    mesh = new btTriangleMesh(false);
    ngl::Vec3 vv,vv1,vv2;
    btVector3 bv1,bv2,bv3;

    std::vector<ngl::Face> faces=_m->getFaceList();
    unsigned int size1=faces.size();
    for(unsigned int i=0; i<size1; i++)
    {


        vv=_m->getVertexAtIndex(faces[i].m_vert[0]);
        vv1=_m->getVertexAtIndex(faces[i].m_vert[1]);
        vv2=_m->getVertexAtIndex(faces[i].m_vert[2]);
        bv1 = btVector3(btScalar(vv.m_x),btScalar(vv.m_y),btScalar(vv.m_z));
        bv2 = btVector3(btScalar(vv1.m_x),btScalar(vv1.m_y),btScalar(vv1.m_z));
        bv3 = btVector3(btScalar(vv2.m_x),btScalar(vv2.m_y),btScalar(vv2.m_z));
        mesh->addTriangle(bv1,bv2,bv3);
    }
    meshShape = new btBvhTriangleMeshShape(mesh,true,true);
}


void GamePhysicsEngine::step()
{
    m_dynamicsWorld->stepSimulation(1.0/60.0,10);
    senseCollision();

}



void GamePhysicsEngine::addGround(const ngl::Vec3 &_pos)
{
        //m_groundShape = new btBoxShape(btVector3(_size.m_x,_size.m_y, _size.m_z));



        planeShape = new btStaticPlaneShape(btVector3(0,1,0),_pos.m_y);

        m_collisionShapes.push_back(planeShape);

        btTransform groundTransform;
        groundTransform.setIdentity();
        {
             btScalar mass(0.);
             //rigidbody is dynamic if and only if mass is non zero, otherwise static
             bool isDynamic = (mass != 0.f);
             btVector3 localInertia(0,0,0);
             if (isDynamic)
                 planeShape->calculateLocalInertia(mass,localInertia);
             //using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
             btDefaultMotionState* myMotionState = new btDefaultMotionState(groundTransform);
             btRigidBody::btRigidBodyConstructionInfo rbInfo(mass,myMotionState,planeShape,localInertia);
             btRigidBody* body = new btRigidBody(rbInfo);
             body->setFriction(friction);
             //add the body to the dynamics world
             m_dynamicsWorld->addRigidBody(body);
        }

}


int GamePhysicsEngine::getCollisionShape(unsigned int _index) const
{
  btCollisionObject* obj = m_dynamicsWorld->getCollisionObjectArray()[_index];
  btCollisionShape *collisionShape = obj->getCollisionShape();

  return collisionShape->getShapeType();
}

ngl::Mat4 GamePhysicsEngine::getTransformMatrix(unsigned int _index)
{
        btCollisionObject* obj = m_dynamicsWorld->getCollisionObjectArray()[_index];
        btRigidBody* body = btRigidBody::upcast(obj);
        if (body && body->getMotionState())
        {
                btTransform trans;

                body->getMotionState()->getWorldTransform(trans);
                float matrix[16];
                trans.getOpenGLMatrix(matrix);
                return ngl::Mat4( matrix[0],matrix[1],matrix[2],matrix[3],
                                  matrix[4],matrix[5],matrix[6],matrix[7],
                                  matrix[8],matrix[9],matrix[10],matrix[11],
                                  matrix[12],matrix[13],matrix[14],matrix[15]
                                );



        }

}

ngl::Vec3 GamePhysicsEngine::getPosition(unsigned int _index)
{
        btCollisionObject* obj = m_dynamicsWorld->getCollisionObjectArray()[_index];
        btRigidBody* body = btRigidBody::upcast(obj);
        if (body && body->getMotionState())
        {
                btTransform trans;
                body->getMotionState()->getWorldTransform(trans);
                return ngl::Vec3(trans.getOrigin().getX(),
                                                                                 trans.getOrigin().getY(),
                                                                                 trans.getOrigin().getZ()
                                                                                );
        }
}



void GamePhysicsEngine::addBox(const ngl::Vec3 &_pos, ngl::Vec4 _scale, boundingExtras _inputRbodyType)
{
    //create a dynamic rigidbody

    btCollisionShape* colShape;


    // this condition might look redundant, but this is needed to differentiate bewtween similar collision shapes by setting our own pointer into the collision shape depending on the object
    if (_inputRbodyType == CRATE_AMMUNITION)
    {
       colShape  = new btBoxShape(btVector3(_scale.m_x,_scale.m_y,_scale.m_z));
        boxptr = this;
    }
    else if (_inputRbodyType == INVISIBLE_WALL)
    {
        colShape = new btBoxShape(btVector3(_scale.m_x,_scale.m_y,_scale.m_z));
         wallptr = 0;
    }
    else if (_inputRbodyType == INVISIBLE_CAM_WALL)
    {
        colShape = new btBoxShape(btVector3(_scale.m_x,_scale.m_y,_scale.m_z));
         wallptr = 0;
    }





    //btCollisionShape* colShape = new  btCapsuleShape(0.5,1.0);//  btBoxShape(btVector3(_rad/2.0,_rad/2.0,_rad/2.0));

    m_collisionShapes.push_back(colShape);

    /// Create Dynamic Objects
    btTransform startTransform;
    startTransform.setIdentity();
    btScalar	mass(10.f);
    btVector3 localInertia(0,0,0);
    colShape->calculateLocalInertia(mass,localInertia);
    startTransform.setOrigin(btVector3(_pos.m_x,_pos.m_y,_pos.m_z));
    //using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
    btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
    btRigidBody::btRigidBodyConstructionInfo rbInfo(mass,myMotionState,colShape,localInertia);
    rbInfo.m_restitution = 0.8f;
    rbInfo.m_friction = 100.5f;
    rbInfo.m_additionalAngularDampingFactor=4.0;
    rbInfo.m_additionalDamping=5.0;
    btRigidBody* body = new btRigidBody(rbInfo);
    body->setFriction(friction);
    //body->setActivationState(ISLAND_SLEEPING);
    body->setLinearFactor(btVector3(1,0,1));
    body->setAngularFactor(btVector3(0,1,0));


    if (_inputRbodyType == CRATE_AMMUNITION)
    {
        body->setUserPointer(boxptr);
    }
    else if (_inputRbodyType == INVISIBLE_WALL)
    {
        body->setUserPointer(wallptr);
    }

    body->setCollisionFlags(btCollisionObject::CF_STATIC_OBJECT);
    m_dynamicsWorld->addRigidBody(body);


}



void GamePhysicsEngine::addSkyBox(const ngl::Vec3 &_pos, ngl::Vec4 _bounds)
{
    //create a dynamic rigidbody



    btCollisionShape* colShape = new btBoxShape(btVector3(_bounds.m_x,_bounds.m_y,_bounds.m_z));

    m_collisionShapes.push_back(colShape);

    /// Create Dynamic Objects
    btTransform startTransform;
    startTransform.setIdentity();
    btScalar	mass(1.f);
    btVector3 localInertia(0,0,0);
    colShape->calculateLocalInertia(mass,localInertia);
    startTransform.setOrigin(btVector3(_pos.m_x,_pos.m_y,_pos.m_z));
    //using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
    btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
    btRigidBody::btRigidBodyConstructionInfo rbInfo(mass,myMotionState,colShape,localInertia);
    rbInfo.m_restitution = 0.8f;
    rbInfo.m_friction = 100.5f;
    rbInfo.m_additionalAngularDampingFactor=4.0;
    rbInfo.m_additionalDamping=5.0;
    btRigidBody* body = new btRigidBody(rbInfo);
    body->setFriction(friction);
    body->setActivationState(ISLAND_SLEEPING);
    body->setCollisionFlags(btCollisionObject::CF_STATIC_OBJECT);
    m_dynamicsWorld->addRigidBody(body);
}


void GamePhysicsEngine::moveBox(int _bodyreference, float _inputimpulse)
{
    btCollisionObject* obj = m_dynamicsWorld->getCollisionObjectArray()[_bodyreference];
    btRigidBody* body = btRigidBody::upcast(obj);

    body->setActivationState(ACTIVE_TAG);

    body->setGravity(btVector3(0,-10,0));

    if (body && body->getMotionState())
    {
            btTransform trans = body->getWorldTransform();
            trans.getOrigin() += (btVector3((_bodyreference * _inputimpulse),0, 0 ));
            trans.setRotation(trans.getRotation() + btQuaternion(0,1,0,(_bodyreference * _inputimpulse)));
            body->setWorldTransform(trans);
    }
}


void GamePhysicsEngine::addConvexGrenade(const ngl::Vec3 &_pos)
{

        tempObj = new ngl::Obj("models/Grenade.obj","textures/Grenade.tga");
        getMeshFromObj(tempObj);
        grenadeptr = (this + 1);

        btConvexShape* tmpConvexShape = new btConvexTriangleMeshShape(mesh);

        std::cout<<"reducing vertices by creating a convex hull\n";
        //create a hull approximation
        btShapeHull* hull = new btShapeHull(tmpConvexShape);
        btScalar margin = tmpConvexShape->getMargin();
        hull->buildHull(margin);
        btConvexHullShape* simplifiedConvexShape = new btConvexHullShape((btScalar*)hull->getVertexPointer(),hull->numVertices());
        std::cout<<"new numTriangles = "<< hull->numTriangles();
        std::cout<<"new numIndices = "<<hull->numIndices();
        std::cout<<"new numVertices = "<< hull->numVertices();
        m_collisionShapes.push_back(simplifiedConvexShape);
        ///Create Dynamic Objects
        btTransform startTransform;
        startTransform.setIdentity();
        btScalar	mass(3.f);
        btVector3 localInertia(0,0,0);
        simplifiedConvexShape->calculateLocalInertia(mass,localInertia);
        startTransform.setOrigin(btVector3(_pos.m_x,_pos.m_y,_pos.m_z));
        //using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
        btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
        btRigidBody::btRigidBodyConstructionInfo rbInfo(mass,myMotionState,simplifiedConvexShape,localInertia);
        rbInfo.m_restitution = 0.8f;
        rbInfo.m_friction = 100.5f;
        rbInfo.m_additionalAngularDampingFactor=4.0;
        rbInfo.m_additionalDamping=5.0;
        btRigidBody* body = new btRigidBody(rbInfo);
        body->setFriction(friction);
        body->setActivationState(DISABLE_DEACTIVATION);
        body->setUserPointer(grenadeptr);
        std::cout<<"ptr is "<<body->getUserPointer();
        m_dynamicsWorld->addRigidBody(body);

}



void GamePhysicsEngine::addConvexStones(const ngl::Vec3 &_pos, const ngl::Vec3 &_rot)
{

        tempObj = new ngl::Obj("models/truck.obj","textures/Grenade.tga");
        getMeshFromObj(tempObj);

        obstacleptr = (this + 2);

        btConvexShape* tmpConvexShape = new btConvexTriangleMeshShape(mesh);
        std::cout<<"reducing vertices by creating a convex hull\n";
        //create a hull approximation
        btShapeHull* hull = new btShapeHull(tmpConvexShape);
        btScalar margin = tmpConvexShape->getMargin();
        hull->buildHull(margin);
        btConvexHullShape* simplifiedConvexShape = new btConvexHullShape((btScalar*)hull->getVertexPointer(),hull->numVertices());
        std::cout<<"new numTriangles = "<< hull->numTriangles();
        std::cout<<"new numIndices = "<<hull->numIndices();
        std::cout<<"new numVertices = "<< hull->numVertices();
        m_collisionShapes.push_back(simplifiedConvexShape);

        btTransform startTransform;
        startTransform.setIdentity();

        btScalar	mass(10.f);


        btVector3 localInertia(0,0,0);
        simplifiedConvexShape->calculateLocalInertia(mass,localInertia);
        startTransform.setOrigin(btVector3(_pos.m_x,_pos.m_y,_pos.m_z));
        startTransform.setRotation(btQuaternion(0,1,0,_rot.m_y));

        //using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
        btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);

        btRigidBody::btRigidBodyConstructionInfo rbInfo(mass,myMotionState,simplifiedConvexShape,localInertia);
        rbInfo.m_restitution = 0.8f;
        rbInfo.m_friction = 100.5f;
        rbInfo.m_additionalAngularDampingFactor=4.0;
        rbInfo.m_additionalDamping=5.0;
        btRigidBody* body = new btRigidBody(rbInfo);
        body->setFriction(friction);

        body->setLinearFactor(btVector3(1,0,1));
        body->setAngularFactor(btVector3(0,1,0));

        body->setUserPointer(obstacleptr);

        std::cout<<"ptr is "<<body->getUserPointer();
        m_dynamicsWorld->addRigidBody(body);

}


void GamePhysicsEngine::moveGrenade(int _inputRightLeftAccumulated, int _inputUpDownAccumulated, int _inputWindValue )
{


    if (_inputUpDownAccumulated != 0)
    {
        btCollisionObject* obj = m_dynamicsWorld->getCollisionObjectArray()[0];
        btRigidBody* body = btRigidBody::upcast(obj);
        if (body && body->getMotionState())
        {
            btTransform trans = body->getWorldTransform();

            // if level3, get wind as input and set x of gravity to that input
            if (_inputWindValue != 0)
            {
            body->setGravity(btVector3((_inputWindValue),-10,0));
            }

              body->applyCentralImpulse(btVector3(_inputRightLeftAccumulated,_inputUpDownAccumulated,_inputUpDownAccumulated));



        }
    }


}


void GamePhysicsEngine::senseCollision()
{
    int numContacts = 0;

    int numManifolds = m_dispatcher->getNumManifolds();

    //std::cout<<"number of Manifolds is "<<numManifolds<<std::endl;

    for (int i=0;i<numManifolds;i++)
    {

        btPersistentManifold* contactManifold = m_dispatcher->getManifoldByIndexInternal(i);

        btCollisionObject* obA = static_cast<btCollisionObject*>(contactManifold->getBody0());

        btCollisionObject* obB = static_cast<btCollisionObject*>(contactManifold->getBody1());

        numContacts = contactManifold->getNumContacts();


        if ((numContacts == 1) && (doingSimulationInAir == 1))
        {
            if((obA->getCollisionShape()->getShapeType() == CONVEX_HULL_SHAPE_PROXYTYPE) && (obB->getCollisionShape()->getShapeType() == CONVEX_HULL_SHAPE_PROXYTYPE))
            {

                if((obA->getUserPointer() == grenadeptr) && (obB->getUserPointer() == obstacleptr))
                {
                    std::cout<<"yesssss i hit youuuuuu....";

                    obA->setCollisionFlags(btCollisionObject::CF_NO_CONTACT_RESPONSE|btCollisionObject::CF_DISABLE_SPU_COLLISION_PROCESSING);
                    obB->setCollisionFlags(btCollisionObject::CF_DISABLE_SPU_COLLISION_PROCESSING);

                    setGrenadeObstacleCcount(0);
                    break; // this is reqd to exit, else, numcontacts remains as one for quite a while which will lead to deduction of lives more than once
                }
        }
        if((obA->getCollisionShape()->getShapeType() == CONVEX_HULL_SHAPE_PROXYTYPE) && (obB->getCollisionShape()->getShapeType() == BOX_SHAPE_PROXYTYPE))
        {
            if((obA->getUserPointer() == grenadeptr) && (obB->getUserPointer() == boxptr))
            {
                std::cout<<"oh, yeah..you won...";
                setGameWonFlag();
                break;
            }
        }

    }

    if ((numContacts == 0) && (obA->getUserPointer() == grenadeptr) && (obA->getCollisionShape()->getShapeType() == CONVEX_HULL_SHAPE_PROXYTYPE) && (obB->getCollisionShape()->getShapeType() == STATIC_PLANE_PROXYTYPE))

    {

      obA->setCollisionFlags(btCollisionObject::CF_DISABLE_SPU_COLLISION_PROCESSING);
      doingSimulationInAir = 1;

      //std::cout<<"doing simulation:\n"<<doingSimulationInAir;

    }
    else if((obA->getUserPointer() == grenadeptr) && (obA->getCollisionShape()->getShapeType() == CONVEX_HULL_SHAPE_PROXYTYPE) && (obB->getCollisionShape()->getShapeType() == STATIC_PLANE_PROXYTYPE))

    {
        obA->setCollisionFlags(btCollisionObject::CF_DISABLE_SPU_COLLISION_PROCESSING);
        doingSimulationInAir = 0;
     }

  }

  //std::cout<<"number of contacts is "<< numContacts<<std::endl;
}




bool GamePhysicsEngine::getSimulationFlag()
{
  return doingSimulationInAir;
}

//grenade obstacle collision count
void GamePhysicsEngine::setGrenadeObstacleCcount(int _noOfLives)
{
    if(_noOfLives == 0)
    {
     grenadeObstacleCcount-=0.01;
    }
    else
    {
        grenadeObstacleCcount = _noOfLives;
    }

    std::cout<<"life of the grenade is "<<grenadeObstacleCcount;
}


float GamePhysicsEngine::getGrenadeObstacleCcount()
{
    return grenadeObstacleCcount;
}




void GamePhysicsEngine::setGameWonFlag()
{
    // a hit on the goal (crate box) has been detected, set the flag which will be queried by the game logic to let the user know that the game has been won
    gameWonFlag = true;
}

bool GamePhysicsEngine::getGameWonFlag()
{
    return gameWonFlag;
}
