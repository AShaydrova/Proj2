#include "Observer_files.h"

int main()
{
    std::cout << "Path to the input file:" << std::endl;
    QString path;
    path = cin.readLine();

    // Создание объекта Источника для отслеживания конкретного файла
    FileMonitor filemonitor(path);
    // Определение объекта Наблюдателя
    Observer_of_files observer_of_files;
    // Связывание Наблюдателя с Источником
    filemonitor.Attach(&observer_of_files);
    std::cout << "Program is working" << std::endl;
    // Бесконечный цикл, в котором обновляется состояние
    // объекта Источника каждые 100 миллисекунд
    while (true) {
        filemonitor.checkFile();
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    return 0;
}
