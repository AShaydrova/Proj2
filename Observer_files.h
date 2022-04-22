#ifndef OBSERVER_FILES_H
#define OBSERVER_FILES_H

#include <QFile>
#include <iostream>
#include <vector>
#include <algorithm>
using std::cout; using std::cin; using std::endl;
using std::string; using std::vector;

class IObserver
{
};

class Observer_of_files : IObserver
{
};

class ASubject
{
vector<Observer_of_files*> list;

public:
void Attach(Observer_of_files *product);
void Detach(Observer_of_files *product);
void Notify();
};


#endif // OBSERVER_FILES_H
