#include "techlogger.h"

void TechLogger::dropData(std::condition_variable *cond){
    while (true) {
        std::mutex mut;
        std::unique_lock<std::mutex> lck_cond(mut);
        cond->wait_for(lck_cond, std::chrono::milliseconds(500));

        std::unique_lock<std::mutex> lck(mutLocal);
        std::vector<std::string> dataTemp = std::move(data);
        lck.unlock();

        if (fileName.isEmpty() && !isRunning)
            break;
        if (fileName.isEmpty())
            continue;

        std::string absFileName = strDirPath.toStdString() + fileName.toStdString();
        absFileName += ".log";

        if(!strDirPath.isEmpty() && dataTemp.size() != 0){
            std::ofstream fstr(absFileName, std::ios::app);
            if(fstr.is_open()){
                for(const auto &ln : dataTemp)
                    fstr << ln;
                fstr.close();
            }
        }

        if(!isRunning)
            break;
    }
}

void TechLogger::updFileName(const QString &name){
    fileName = name;

    QDateTime qdt = QDateTime::currentDateTime();
    QString _date = qdt.date().toString("yyyy-MM-dd");
    QString _time = qdt.time().toString("hh-mm-ss");

    fileName += _date;
    fileName += "_";
    fileName += _time;
}

void TechLogger::addSymb(const QString &symb){
    fileName += "_" + symb;
}
