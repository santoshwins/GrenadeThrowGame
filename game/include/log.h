#ifndef LOG_H
#define LOG_H

#include<fstream>
#define logenable 1


bool logerr( std::string inputFileName,std::string a = NULL,std::string b = NULL)
{
    std::ofstream errfile;
    errfile.open("errfile.txt");
    errfile<<"filename is:"<<inputFileName<<"\n";
    errfile<<a<<"\n"<<b;
    errfile.close();
    return true;
}

#endif // LOG_H
