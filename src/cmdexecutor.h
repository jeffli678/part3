#pragma once
#include <QObject>
#include <QString>
#include <QTime>
#include <QProcess>

class CommandLineExecutor: public QObject
{
    Q_OBJECT

private:
    QString m_commandLine;
    QProcess* m_process;

    double m_totalSeconds{};

    void ProcessOneLine(const QString &line);

public:
    explicit CommandLineExecutor(const QString& commandLine);
    void Start();
    void Kill();

signals:
    void Succeeded(const QTime& time);
    void Failed(int exitCode);
    void Progress(double percentDone, const QTime& timeRemaining);

private slots:
    void updateProgress();
};
