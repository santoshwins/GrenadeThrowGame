#include "GameControls.h"
#include "ui_GameControls.h"
#include <ngl/Random.h>




GameControls::GameControls(
                       QWidget *_parent
                      ):
                        QMainWindow(_parent),
                        m_ui(new Ui::GameControls)
{


    // setup the user interface
    m_ui->setupUi(this);



    // first we re-size the window to the size we need
    this->resize(QSize(1024,720));
    // now create our GL window and parent it to this
   // m_gl = new GameLogic(this);
    m_gl = 0;

    // this is the window title
    this->setWindowTitle(QString("PRESS 'START GAME' TO START THE AWARD WINNING GRENADE THROW GAME "));

    connect(m_ui->m_startGame,SIGNAL(clicked()),this,SLOT(startGame()));
    connect(m_ui->m_levelSetDial,SIGNAL(sliderMoved(int)),this,SLOT(setLevelDisplay(int)));
    connect(m_ui->restartButton,SIGNAL(clicked()),this,SLOT(resetGame()));
    connect(m_ui->leftCamera,SIGNAL(clicked()),this,SLOT(setCameraFromUI()));
    connect(m_ui->topCamera,SIGNAL(clicked()),this,SLOT(setCameraFromUI()));
    connect(m_ui->rightCamera,SIGNAL(clicked()),this,SLOT(setCameraFromUI()));
    connect(m_ui->defaultBackCamera,SIGNAL(clicked()),this,SLOT(setCameraFromUI()));
    connect(m_ui->freeCam,SIGNAL(stateChanged(int)),this,SLOT(setFreeCamFlagFromUI(int)));


    m_ui->m_progressBar->setVisible(false);
    m_ui->loading->setVisible(false);

    m_ui->cameraBox->setDisabled(true);

    ColorHigh = QString (
                    "QProgressBar::chunk {"
                    "background-color: #11E615;"
                     "}");
    ColorHigh.append("QProgressBar {"
                  "border: 1px solid grey;"
                  "border-radius: 5px;"
                  "text-align: center;"
                  "background: #eeeeee;"
                  "}");

    ColorIntermediate = QString (
                    "QProgressBar::chunk {"
                    "background-color: #F3FF02;"
                     "}");

    ColorIntermediate.append("QProgressBar {"
                  "border: 1px solid grey;"
                  "border-radius: 5px;"
                  "text-align: center;"
                  "background: #eeeeee;"
                  "}");

    ColorLow = QString (
                    "QProgressBar::chunk {"
                    "background-color: #FF0217;"
                     "}");

    ColorLow.append("QProgressBar {"
                  "border: 1px solid grey;"
                  "border-radius: 5px;"
                  "text-align: center;"
                  "background: #eeeeee;"
                  "}");

    QIcon left_cam = QIcon(QString("textures/camera_lt.jpg"));
    QIcon right_cam = QIcon(QString("textures/camera_rt.jpg"));
    QIcon top_cam = QIcon(QString("textures/camera_top.jpg"));
    QIcon back_cam = QIcon(QString("textures/camera_back.jpg"));


    m_ui->leftCamera->setIcon(left_cam);
    m_ui->leftCamera->setIconSize(QSize(40,40));
    m_ui->rightCamera->setIcon(right_cam);
    m_ui->rightCamera->setIconSize(QSize(40,40));
    m_ui->topCamera->setIcon(top_cam);
    m_ui->topCamera->setIconSize(QSize(40,40));
    m_ui->defaultBackCamera->setIcon(back_cam);
    m_ui->defaultBackCamera->setIconSize(QSize(40,40));
    m_ui->grenadePower->setStyleSheet(ColorHigh);
    m_ui->powerBar->setStyleSheet(ColorLow);
    m_ui->inGameBarsDisplay->setVisible(false);
    m_settingWind = 0;
    m_updateScore = 0;

    movieLeft = new QMovie("textures/glowingarrowleft.gif");
    movieRight = new QMovie("textures/glowingarrowright.gif");
    m_ui->windDirRtArrow->setMovie(movieRight);
    m_ui->windDirLtArrow->setMovie(movieLeft);

    QLCDNumber *lcd = m_ui->windNumber;
    lcd->setSegmentStyle( QLCDNumber::Filled );
    QPalette palette = lcd->palette();
    palette.setColor(QPalette::Normal, QPalette::Background,  Qt::black);
    palette.setColor(QPalette::Normal, QPalette::Light, Qt::red);
    lcd->setPalette(palette);

    m_ui->inGameBarsDisplay->setVisible(false);
}

//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
GameControls::~GameControls()
{
    // no need to do anything at present
    delete m_ui;
}


//----------------------------------------------------------------------------------------------------------------------
void GameControls::keyReleaseEvent(
                                                                    QKeyEvent *_event
                                                                 )
{
    // once we have processed any key here pass the event
    // onto the GLWindow class to do more processing
    if((m_gl != NULL) && (m_gl->getGameWonFlag() == false) && (m_gl->getGameLostFlag() == false))
    {
        m_gl->processKeyUp(_event);
    }

}
//----------------------------------------------------------------------------------------------------------------------
void GameControls::keyPressEvent(
                               QKeyEvent *_event
                              )
{
  // this method is called every time the main window recives a key event.
  // we then switch on the key value and set the camera in the GLWindow


    if ((m_gl != NULL) && (m_gl->getGameWonFlag() == false) && (m_gl->getGameLostFlag() == false))
    {
        if(m_gl->getSimulationFlag() == 0)
        {
            switch (_event->key())
            {
                case Qt::Key_Escape : QApplication::exit(EXIT_SUCCESS); break;
                case Qt::Key_F : showFullScreen(); break;
                case Qt::Key_N : showNormal(); break;
                case Qt::Key_Up : setPowerBar(10); break;
                case Qt::Key_Down : setPowerBar(-10); break;
                case Qt::Key_Space : setPowerBar(0);  break;
                default : break;
        }


        m_gl->processKeyDown(_event);

        }
    }// block all keys if we are in air

}




void GameControls::startGame()
{


    m_ui->m_progressBar->setVisible(true);
    m_ui->loading->setVisible(true);
    m_gameRunning = startTimer(1);
    setProgressBar();
    constructTheGame();
    if ( m_ui->m_beginner->isChecked())
    {
        m_gl->setDifficulty(BEGINNER);

    }
    else if(m_ui->m_advanced->isChecked())
    {
        m_gl->setDifficulty(ADVANCED);
    }
    m_gl->setLevel(m_ui->m_levelSetDial->value());
    m_ui->s_GameWindowLayout->addWidget(m_gl,0,0,1,8);
    m_ui->m_startGame->setDisabled(true);
    m_ui->levelSettingsBox->setDisabled(true);
    m_ui->textBrowser->clearFocus();
    m_ui->inGameBarsDisplay->setVisible(true);
    m_ui->cameraBox->setDisabled(false);
    m_ui->windDirRtArrow->setDisabled(true);
    m_ui->windDirLtArrow->setDisabled(true);
    m_ui->windNumber->setDisabled(true);
    m_ui->windText->setDisabled(true);

    if(m_ui->m_levelSetDial->value() >= 2)
    {
        m_settingWind = startTimer(2);
        m_ui->windDirRtArrow->setDisabled(false);
        m_ui->windDirLtArrow->setDisabled(false);
        m_ui->windNumber->setDisabled(false);
        m_ui->windText->setDisabled(false);
        movieLeft->start();
        movieRight->start();
        movieLeft->setPaused(true);
        movieRight->setPaused(true);
    }

    m_updateScore = startTimer(5);
}

void GameControls::setBeginnerDifficulty()
{
    // can do some processing if we want here later,else connect signal directly using m_gl,setDifficulty at the top in this code
    m_gl->setDifficulty(BEGINNER);
}

void GameControls::setAdvancedDifficulty()
{
    m_gl->setDifficulty(ADVANCED);
}

void GameControls::setLevelDisplay(int _levelDisplay)
{
    QString level = "LEVEL->>>>";
    m_ui->m_levelLabel->setText(level + QString::number(_levelDisplay));
}

void GameControls::setPowerBar(float _inputPower)
{
    static float _accumulatedPower = 0; // staitic as one time initialization fo power enough, its not aproperty of controls, so cant be its member
    if(_inputPower != 0)
    {
        _accumulatedPower = _accumulatedPower + _inputPower;


        if(_accumulatedPower >= 40)
        {
            _accumulatedPower = 40;
        }

        else if (_accumulatedPower <= 0)
        {
            _accumulatedPower = 0;
        }
     }

    else if (_inputPower == 0) // for down arrow to reduce power
    {
        _accumulatedPower = 0;
    }

    if (_accumulatedPower <= 20)
    {
       m_ui->powerBar->setStyleSheet(ColorHigh);
    }
    else if (_accumulatedPower > 20 && _accumulatedPower <= 30)
    {
       m_ui->powerBar->setStyleSheet(ColorIntermediate);
    }
    else if (_accumulatedPower > 30)
    {
        m_ui->powerBar->setStyleSheet(ColorLow);
    }

    m_ui->powerBar->setValue(_accumulatedPower);
}


void GameControls::setProgressBar()
{

    if ( m_gl && m_gl->getGameStartedFlag() == 1)
    {
        m_ui->m_progressBar->setValue(m_ui->m_progressBar->maximum());
        m_ui->m_progressBar->setVisible(false);
        m_ui->loading->setVisible(false);
        killTimer(m_gameRunning);

    }
    else
    {
        m_ui->m_progressBar->setValue(10);
    }

}

void GameControls::timerEvent(QTimerEvent *_event)
{
    if (_event->timerId() == m_gameRunning)
    {
        setProgressBar();
    }

    if (_event->timerId() == m_settingWind)
    {
        setRandomValueForWindLevel3();
    }

    if (_event->timerId() == m_updateScore)
    {
        // only if the game has not been won, should we update the score..so this might save us some function calls after the player has collided with the goal
        if (m_gl->getGameWonFlag() == true)
        {
            // display Text on the window
            m_ui->inGameBarsDisplay->setVisible(false);
            killTimer(m_updateScore);
        }

        else
        {
            float tempScore = getCurrentScore();

            if (tempScore <= 50 && tempScore > 25)
            {
                m_ui->grenadePower->setStyleSheet(ColorIntermediate);
            }
            else if (tempScore <= 25)
            {
                m_ui->grenadePower->setStyleSheet(ColorLow);
            }
            m_ui->grenadePower->setValue(tempScore);

       // also check if its 0, if so, the player has lost and this has to be informed and the timer killed
           if ( tempScore == 0)
           {
                m_gl->setGameLostFlag();
                m_ui->inGameBarsDisplay->setVisible(false);
                killTimer(m_updateScore);
           }
       }
   }
}

void GameControls::resetGame()
{
    if(m_gl)
    {
        m_ui->s_GameWindowLayout->removeWidget(m_gl);
        m_gl->~GameLogic();
        killTimer(m_gameRunning);
        killTimer(m_settingWind);
        killTimer(m_updateScore);
        m_gl = 0;
    }


    m_ui->m_startGame->setDisabled(false);
    m_ui->levelSettingsBox->setDisabled(false);
    m_ui->textBrowser->clearFocus();
    m_ui->inGameBarsDisplay->setVisible(false);
    m_ui->cameraBox->setDisabled(true);

}


////this func has to be called 2 secs once and entry check has to be in place to check if the grenade has touched down,
//if so, provide a random number and set
//// dirarrow in that direction in ui..this random value shud be passed in key_space input to game level
//and it will set its wind factor value attribute which wil be got
//// by gamelevel to pass it to movegrenade
void GameControls::setRandomValueForWindLevel3()
{
    static int seedValue = 10; // again common value, cannot be a member of this class
    static int valueGenerated = 0;
    int windValue = 0;

    if(m_gl)
    {

        if((m_gl->getSimulationFlag() == 0) && (valueGenerated == 0))
        {

            ngl::Random *randObj = ngl::Random::instance();

            windValue = (int)randObj->randomNumber(seedValue);
            m_gl->setWindValue(windValue);

            if (windValue > 0)
            {
                // set the label value to display wind
                m_ui->windNumber->display(windValue);
                movieRight->setPaused(true);
                movieLeft->setPaused(false);
            }
            else
            {
                m_ui->windNumber->display(windValue);
                movieLeft->setPaused(true);
                movieRight->setPaused(false);

            }

            valueGenerated = 1;


         }
        else if (m_gl->getSimulationFlag() == 1)
        {
            valueGenerated = 0;

        }
   }
}



float GameControls::getCurrentScore()
{
    if (m_gl)
    {
        return m_gl->getCurrentGrenadeLife();
    }

}

void GameControls::constructTheGame()
{
    if(!m_gl)
    {
        m_gl = new GameLogic(this);
    }
    else
    {
        std::cout<<"means game world already created";
    }


}


void GameControls::setCameraFromUI()
{
    if (QObject::sender() == m_ui->leftCamera)
    {

        m_gl->changeCamera(CAM_LEFT);
    }
    else if (QObject::sender() == m_ui->topCamera)
    {
         m_gl->changeCamera(CAM_TOP);

    }
    else if (QObject::sender() == m_ui->rightCamera)
    {
         m_gl->changeCamera(CAM_RIGHT);

    }
    else if (QObject::sender() == m_ui->defaultBackCamera)
    {
         m_gl->changeCamera(CAM_DEFAULT);

    }
}


 void GameControls::setFreeCamFlagFromUI(int _freeCamFlag)
 {
     if(m_gl)
     {
         m_gl->setFreeCamFlag(_freeCamFlag);
     }
 }

