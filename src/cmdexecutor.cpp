#include "cmdexecutor.h"
#include <QRegularExpression>

CommandLineExecutor::CommandLineExecutor(const QString& commandLine):
    m_commandLine(commandLine)
{

}


void CommandLineExecutor::Start()
{
    m_process = new QProcess(this);
    QTime startTime = QTime::currentTime();
    connect(m_process, &QProcess::finished, [&, startTime](int exitCode, QProcess::ExitStatus exitStatus){
        if ((exitCode == 0) && (exitStatus == QProcess::NormalExit))
        {
            QTime endTime = QTime::currentTime();
            int seconds = startTime.secsTo(endTime);
            emit Succeeded(QTime().addSecs(seconds));
        }
        else
        {
            emit Failed(exitCode);
        }
    });
    connect(m_process, &QProcess::readyReadStandardOutput, this, &CommandLineExecutor::updateProgress);
    connect(m_process, &QProcess::readyReadStandardError, this, &CommandLineExecutor::updateProgress);
    m_process->startCommand(m_commandLine);
}


void CommandLineExecutor::updateProgress()
{
    auto data = QString(m_process->readAllStandardError());
    for (auto line: data.split('\n'))
        ProcessOneLine(line);
}


void CommandLineExecutor::ProcessOneLine(const QString &line)
{
//    printf("%s\n", line.toStdString().c_str());
//    printf("============\n");

    QString data = line.trimmed();
    if (data.startsWith("Duration: "))
    {
        auto match = QRegularExpression(R"(.*Duration: (\d+):(\d+):(\d+)\.(\d+))").match(data);
        if (match.hasMatch())
        {
            int hour = match.captured(1).toInt();
            int minute = match.captured(2).toInt();
            int second = match.captured(3).toInt();
            int mseconds = match.captured(3).toInt();
            m_totalSeconds = 3600 * hour + 60 * minute + second + (double)mseconds / 100;
        }
    }
    else if (data.startsWith("frame"))
    {
        auto match = QRegularExpression(R"(.*time=(\d+):(\d+):(\d+)\.(\d+))").match(data);
        double secondsElapsed = 0.0;
        bool timeValid = false;
        double speed = 0.0;
        bool speedValid = false;

        if (match.hasMatch())
        {
            int hour = match.captured(1).toInt();
            int minute = match.captured(2).toInt();
            int second = match.captured(3).toInt();
            int mseconds = match.captured(3).toInt();
            secondsElapsed = 3600 * hour + 60 * minute + second + (double)mseconds / 100;
            timeValid = true;
        }

        match = QRegularExpression(R"(.*speed=(.*)x)").match(data);
        if (match.hasMatch())
        {
            speed = match.captured(1).toDouble();
            speedValid = true;
        }

        if (timeValid && speedValid && (speed != 0))
        {
            auto rate = secondsElapsed / m_totalSeconds * 100;
            int secondsRemain = (m_totalSeconds - secondsElapsed) / speed;
            QTime time = QTime(0, 0, 0).addSecs(secondsRemain);
            emit Progress(rate, time);
        }
    }
}


void CommandLineExecutor::Kill()
{
    m_process->kill();
}
