#include "GameLogic.h"
#include "GamePhysicsEngine.h"


const QString winningText("GREAT!!!! YOU WON BY DESTROYING THEIR AMMUNITION");
const QString winningText2("NOW,EXPLORE TOUGHER LEVELS BY RESETTING THE GAME!!!");
const QString lostText("OH!! YOU GOT ATTACKED!!! TRY AGAIN BY RESETTING THE GAME!!!");


GameLogic::GameLogic(
                            QWidget *_parent
                        ): QGLWidget( new CreateCoreGLContext(QGLFormat::defaultFormat()), _parent )
{
    //def constructor

    // set this widget to have the initial keyboard focus
    setFocus();
    // re-size the widget to that of the parent (in this case the GLFrame passed in on construction)
    this->resize(_parent->size());

    difficulty = BEGINNER;
    m_levelNumber = LEVEL1;
    m_keysPressed.clear();
    physicsWorld = new GamePhysicsEngine();
    gameStarted = 0;
    windValue = 0;
    physicsWorld->setGrenadeObstacleCcount(100);
    lostFlag = false;
}



//----------------------------------------------------------------------------------------------------------------------
GameLogic::~GameLogic()
{
    // call the static NGLQuit method to close down
    // NGL and tidy up any shaders textures etc
    // first get an instance of the NGLInit singleton
    killTimer(m_redrawTimer);
    killTimer(progressTimer);



    ngl::NGLInit *ngl = ngl::NGLInit::instance();
    // then call the quit method
    ngl->NGLQuit();


}


//----------------------------------------------------------------------------------------------------------------------
// This virtual function is called once before the first call to paintGL() or resizeGL(),
//and then once whenever the widget has been assigned a new QGLContext.
// This function should set up any required OpenGL context rendering flags, defining display lists, etc.
//----------------------------------------------------------------------------------------------------------------------
void GameLogic::initializeGL()
{



  glClearColor(0.4f, 0.4f, 0.4f, 1.0f);	// Black Background
  // enable depth testing for drawing
  glEnable(GL_DEPTH_TEST);
  // we need to initialise the NGL lib, under windows and linux we also need to
  // initialise GLEW, under windows this needs to be done in the app as well
  // as the lib hence the WIN32 define
  ngl::NGLInit *Init = ngl::NGLInit::instance();
  Init->initGlew();    m_keysPressed.clear();
  #ifdef WIN32
    glewInit(); // need a local glew init as well as lib one for windows
  #endif
//  // now to load the shader and set the values
//  // grab an instance of shader manager

  ngl::ShaderLib *shader=ngl::ShaderLib::instance();
  // we are creating a shader called Texture
  shader->createShaderProgram("Texture");
  // now we are going to create empty shaders for Frag and Vert
  shader->attachShader("TextureVertex",ngl::VERTEX);
  shader->attachShader("TextureFragment",ngl::FRAGMENT);
  // attach the source
  shader->loadShaderSource("TextureVertex","shaders/TextureVertex.glsl");
  shader->loadShaderSource("TextureFragment","shaders/TextureFragment.glsl");
  // compile the shaders
  shader->compileShader("TextureVertex");
  shader->compileShader("TextureFragment");
  // add them to the program
  shader->attachShaderToProgram("Texture","TextureVertex");
  shader->attachShaderToProgram("Texture","TextureFragment");

  // now we have associated this data we can link the shader
  shader->linkProgramObject("Texture");
  // and make it active ready to load values
  (*shader)["Texture"]->use();


  // Now we will create a basic Camera from the graphics library
  // This is a static camera so it only needs to be set once
  // First create Values for the camera position
  ngl::Vec3 from_cam1(0,20,-80);//20,-80
  ngl::Vec3 to_cam1(0,0,0);
  ngl::Vec3 up_cam1(0,1,0);
  // now load to our new camera
  m_cam= new ngl::Camera(from_cam1,to_cam1,up_cam1,ngl::PERSPECTIVE);
  // set the shape using FOV 45 Aspect Ratio based on Width and Height
  // The final two are near and far clipping planes of 0.5 and 10
  m_cam->setShape(45,(float)1024.0/720.0,0.05,350,ngl::PERSPECTIVE);

  ngl::Vec3 from_cam2(30,30,-50);
  ngl::Vec3 to_cam2(0,0,0);
  ngl::Vec3 up_cam2(0,1,0);
  m_cam1 = new ngl::Camera(from_cam2,to_cam2,up_cam2,ngl::PERSPECTIVE);
  m_cam1->setShape(90,(float)1024.0/720.0,0.05,350,ngl::PERSPECTIVE);
  m_cam1->yaw(60);
  m_cam1->roll(25);

  ngl::Vec3 from_cam3(0,200,-10);
  ngl::Vec3 to_cam3(0,0,0);
  ngl::Vec3 up_cam3(0,1,0);



  m_cam2 = new ngl::Camera(from_cam3,to_cam3,up_cam3,ngl::PERSPECTIVE);
  m_cam2->setShape(45,(float)1024.0/720.0,0.05,350,ngl::PERSPECTIVE); // change camera value to accomodate mor viewing angle from top, may be set projection


  ngl::Vec3 from_cam4(-30,30,-50);
  ngl::Vec3 to_cam4(0,0,0);
  ngl::Vec3 up_cam4(0,1,0);
  m_cam3 = new ngl::Camera(from_cam4,to_cam4,up_cam4,ngl::PERSPECTIVE);
  m_cam3->setShape(90,(float)1024.0/720.0,0.05,350,ngl::PERSPECTIVE);
  m_cam3->yaw(-60);
  m_cam3->roll(-25);






  shader->setShaderParam3f("viewerPos",m_cam->getEye().m_x,m_cam->getEye().m_y,m_cam->getEye().m_z);

  drawEngine = new GameDrawEngine(1024,780);
  level = new GameLevel(drawEngine,physicsWorld,1024,780);
  level->setLevelDifficulty(difficulty);
  level->setLevelNumber(m_levelNumber);
  level->addPlayerToLevel();
  m_grenade = level->getPlayer();
  level->addObstaclesToLevel(); // change this based on difficulty
  level->addCrateBoxToLevel();
  level->addBGToLevel();
  drawEngine->addCamToRepos(CAM_DEFAULT,m_cam);
  drawEngine->addCamToRepos(CAM_LEFT,m_cam1);
  drawEngine->addCamToRepos(CAM_TOP,m_cam2);
  drawEngine->addCamToRepos(CAM_RIGHT,m_cam3);
  changeCamera(CAM_DEFAULT);
  level->addGraphicImages();
  level->addCollisionBoxes();
  gameStarted = 1;


m_redrawTimer = startTimer(10);

}

//----------------------------------------------------------------------------------------------------------------------
//This virtual function is called whenever the widget has been resized.
// The new size is passed in width and height.
//----------------------------------------------------------------------------------------------------------------------
void GameLogic::resizeGL(
                        int _w,
                        int _h
                       )
{
  glViewport(0,0,_w,_h);
  //m_cam->setShape(45,(float)_w/_h,0.5,150,ngl::PERSPECTIVE);

  m_cam->setShape(45,(float)720.0/576.0,0.05,350,ngl::PERSPECTIVE);
  m_text->setScreenSize(_w,_h);
}


//----------------------------------------------------------------------------------------------------------------------
//This virtual function is called whenever the widget needs to be painted.
// this is our main drawing routine
//----------------------------------------------------------------------------------------------------------------------
void GameLogic::paintGL()
{
  // clear the screen and depth buffer
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  level->drawLevel();
  if(level->getLevelNumber() == LEVEL3) // jus for prototypes..change it to more appropriate switch case when more levels are developed
  {
      level->simulateAI();
  }

  if(getGameWonFlag() == true)
  {
      static int counter = 1;

      static int color = 1;

      m_text = new ngl::Text(QFont("PakTypeNaqsh",30));
      m_text->setColour(color,color,color);
      if ( counter == 50)
      {
          counter = 1;
          color = -color;
      }
      m_text->renderText(20,20,winningText);
      m_text->renderText(20,50,winningText2);
      counter++;
  }

  if (lostFlag == true)
  {
      static int lostColorCounter = 1;

      static int lostColor = 1;

      m_text = new ngl::Text(QFont("PakTypeNaqsh",30));
      m_text->setColour(lostColor,lostColor,lostColor);
      if ( lostColorCounter == 50)
      {

       lostColorCounter = 1;
       lostColor = -lostColor;
      }

     m_text->renderText(50,50,lostText);

     lostColorCounter++;
  }

}


//----------------------------------------------------------------------------------------------------------------------
// this overloaded method is called every time the timers trigger
//----------------------------------------------------------------------------------------------------------------------
void GameLogic::timerEvent(
                          QTimerEvent *_event
                         )
{

    if (_event->timerId() == m_redrawTimer)
    {
    physicsWorld->step();
    updateGL();
    }


}



//----------------------------------------------------------------------------------------------------------------------
// in this case this method is called from the MainWindow class and passed
// the key event from that, if we didn't need to process the events in main
// window we could just overload this as the keyPressEvent and only process here.
//----------------------------------------------------------------------------------------------------------------------
void GameLogic::processKeyDown(
                                                             QKeyEvent *_event
                                                            )
{
    switch(_event->key())
    {
        case Qt::Key_1 : changeCamera(CAM_DEFAULT);break;
        case Qt::Key_2 : changeCamera(CAM_LEFT);break;
        case Qt::Key_3 : changeCamera(CAM_TOP);break;
        case Qt::Key_4 : changeCamera(CAM_RIGHT);break;
    case Qt::Key_Left:
    {
        level->setArrowDrawFlag(1);
        level->accumulateDirection(10);
        break;
    }
    case Qt::Key_Right:
    {
        level->setArrowDrawFlag(1);
        level->accumulateDirection(-10);
        break;
    }

    case Qt::Key_Up:
    {


        level->accumulateAcceleration(10);
        //moveGrenade();
        break;
    }

    case Qt::Key_Down:
    {


        level->accumulateAcceleration(-10);
        //moveGrenade();
        break;
    }

    case Qt::Key_Space:
    {
        level->setArrowDrawFlag(0);
        level->accumulateDirection(0);
        if (_event->isAutoRepeat() == 1)
        {
            break;
        }
        if (level->getLevelNumber() >= 2)
        {
        level->moveGrenade(windValue);
        }
        else
        {
           level->moveGrenade();
        }
        level->accumulateAcceleration(0);

        break;
    }
        default:break;
    }

    if ((_event->key() != Qt::Key_1) || (_event->key() != Qt::Key_2) )
    {
        // add to our keypress set the values of any keys pressed
        m_keysPressed += (Qt::Key)_event->key();
    }

}


//----------------------------------------------------------------------------------------------------------------------
// in this case this method is called from the MainWindow class and passed
// the key event from that, if we didn't need to process the events in main
// window we could just overload this as the keyPressEvent and only process here.
//----------------------------------------------------------------------------------------------------------------------
void GameLogic::processKeyUp(
                                                             QKeyEvent *_event
                                                            )
{
    // remove from our key set any keys that have been released
    m_keysPressed -= (Qt::Key)_event->key();
}


void GameLogic::setDifficulty(DIFFICULTY_LEVELS _difficulty)
{
    difficulty = _difficulty;
}

void GameLogic::setLevel(int _inputLevel)
{
    m_levelNumber = (GAME_LEVELS)_inputLevel;
}


void GameLogic::changeCamera(CAMERA_NAMES _camName)
{
    drawEngine->setActiveCam(_camName);
}



int GameLogic::getGameStartedFlag()
{
    return gameStarted;
}

bool GameLogic::getSimulationFlag()
{
    return (physicsWorld->getSimulationFlag());
}


void GameLogic::setWindValue(int _inputWindValue)
{
    windValue = _inputWindValue;
}


float GameLogic::getCurrentGrenadeLife()
{
    return physicsWorld->getGrenadeObstacleCcount();
}

bool GameLogic::getGameWonFlag()
{
    return physicsWorld->getGameWonFlag();
}


void GameLogic::setFreeCamFlag(int _inputFreeCamFlag )
{
    level->setFreeCamFlag(_inputFreeCamFlag);
}

void GameLogic::setGameLostFlag()
{
    lostFlag = true;
}



bool GameLogic::getGameLostFlag()
{
    return lostFlag;
}

