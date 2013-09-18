#include "GameControls.h"

int main(int argc, char *argv[])
{
   //std::cout << "Hello World!" << endl;
    // make an instance of the QApplication
   QApplication a(argc, argv);
     // Create a new GameWindow
   GameControls w;
     // show it
   w.show();
     // hand control over to Qt framework
   return a.exec();
//return 0;
}

