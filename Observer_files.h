#ifndef OBSERVER_FILES_H
#define OBSERVER_FILES_H

#include <iostream>
#include <vector>
#include <algorithm>
#include <QFile>
#include <QString>
#include <QTextStream>
QTextStream cout(stdout);
QTextStream cin(stdin);

// Структура State_of_file для представления состояния файла.
// Метод operator!=(const State_of_file& state_other_file) перегружает
// оператор сравнения !=; сравнивает текущую структуру State_of_file
// с другой структурой State_of_file (передается аргументом).
struct State_of_file {
    bool file_exists;
    int file_size;

    bool operator!=(const State_of_file& state_other_file) {
        if (file_exists != state_other_file.file_exists || file_size != state_other_file.file_size) {
            return true;
        }
        return false;
    }
};

// Базовый класс IObserver представляет интерфейс,
// который определяет метод Update(const State_of_file& state_of_file), который
// должен вызываться при каждом изменении объекта слежения.
// Метод Update(const State_of_file& state_of_file) - чистая
// виртуальная функция, эту функцию реализуют все конкретные
// классы наблюдателей.
class IObserver
{
public:
    virtual void Update(const State_of_file& state_of_file) = 0;
};

// Конкретный класс Observer_of_files для слежения и реагирования
// на изменения, произошедших в файле.
// Реализует метод Update(const State_of_file& state_of_file),
// который выводит на консоль уведомление о произошедших изменениях
// в файле.
class Observer_of_files : public IObserver {
public:
    void Update(const State_of_file& state_of_file);
};

void Observer_of_files::Update(const State_of_file& state_of_file) {
    std::cout << "State of file is changed: " << std::endl;
    if (state_of_file.file_exists) {
        std::cout << "  File exists" << std::endl;
        std::cout << "  File size: " << state_of_file.file_size << " byte" << std::endl;
    }
    else
        std::cout << "File does not exist" << std::endl;
    std::cout << std::endl;
}

// Класс ASubject отслеживает всех наблюдателей и предоставляет
// возможность добавлять или удалять наблюдателей. Также отвечает
// за обновление наблюдателей при любых изменениях.
// Методы класса:
// Attach(IObserver* observer_of_files) - добавляет наблюдателя
// (передается аргументом) в список.
// Detach(IObserver* observer_of_files) - удаляет наблюдателя
// (передается аргументом) из списка.
// Notify(const State_of_file& state_of_file) - проходится по списку
// подписчиков при наступлении изменения (передается аргументом) и
// оповещает их об этом, вызывая метод Update(const State_of_file& state_of_file).
class ASubject
{
    std::vector<IObserver*> list;

public:
    void Attach(IObserver* observer_of_files);
    void Detach(IObserver* observer_of_files);
    void Notify(const State_of_file& state_of_file);
};

void ASubject::Attach(IObserver* observer_of_files)
{
    list.push_back(observer_of_files);
}

void ASubject::Detach(IObserver* observer_of_files)
{
    list.erase(std::remove(list.begin(), list.end(), observer_of_files), list.end());
}

void ASubject::Notify(const State_of_file& state_of_file)
{
    for (std::vector<IObserver*>::const_iterator iter = list.begin(); iter != list.end(); ++iter) {
        if (*iter != 0) {
            (*iter)->Update(state_of_file);
        }
    }
}

// Конкретный класс FileMonitor, который реализует ASubject.
// Изменение объекта этого класса повлияет на наблюдателей.
// Фиксирует изменения состояния наблюдаемого файла.
// Методы класса:
// Конструктор с аргументом FileMonitor(const char* Name_of_file),
// принимает в качестве аргумента имя файла, состояние которого
// должен отслеживать объект данного класса.
// getFileState() получает сведения
// о состоянии отслеживаемого файла - существует файл или нет,
// размер файла. Возвращает структуру State_of_file - состояние файла.
// checkFile() проверяет изменилось состояние файла или нет.
// Если изменилось, то вызывает метод Update(const State_of_file& state_of_file)
// для оповещения наблюдателей.
class FileMonitor : public ASubject
{
private:
    QFile The_File;
    State_of_file state_prev_file;
public:
    FileMonitor(const QString& Name_of_file);
    State_of_file getFileState();
    void checkFile();
};

FileMonitor::FileMonitor(const QString& Name_of_file) :
    The_File(Name_of_file), state_prev_file(getFileState()) {}

State_of_file FileMonitor::getFileState() {
    State_of_file state_of_file;
    state_of_file.file_exists = The_File.exists();
    state_of_file.file_size = The_File.size();
    return state_of_file;
}

void FileMonitor::checkFile() {
    State_of_file state_of_file = getFileState();
    if (state_of_file != state_prev_file) {
        Notify(state_of_file);
        state_prev_file = state_of_file;
    }
}

#endif // OBSERVER_FILES_H
