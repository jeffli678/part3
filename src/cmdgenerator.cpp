#include "cmdgenerator.h"
#include <QCoreApplication>
#include <QDir>

CmdGenerator::CmdGenerator()
{
    QDir dir(QCoreApplication::applicationDirPath());
    dir.cd("ffmpeg");
    ffmpegPath = dir.filePath("ffmpeg");
    if (!QFile::exists(ffmpegPath))
        printf("Cannot find ffmpeg executable at %s\n", ffmpegPath.toStdString().c_str());
}


void CmdGenerator::PrepareCommand()
{
    m_cmdParts.clear();
    AddCommandLinePart(QString('"') + ffmpegPath + '"');
    AddCommandLinePart("-hide_banner");
    AddCommandLinePart("-i");
    AddCommandLinePart("\"%s\"");
    if (overwriteExisting)
        AddCommandLinePart("-y");

    if (videoEnabled)
    {
        if (videoCodec != "自动")
        {
            AddCommandLinePart("-vcodec");
            AddCommandLinePart(videoCodec);
        }
        if ((videoCodec == "h264") || (videoCodec == "libx265"))
        {

        }
    }
    else
    {
        AddCommandLinePart("-an");
    }
}


void CmdGenerator::AddCommandLinePart(const QString &part)
{
    m_cmdParts.push_back(part);
}


QString CmdGenerator::GetCommand()
{
    PrepareCommand();
    QString command;
    for (const QString& part: m_cmdParts)
    {
        command += (part + ' ');
    }
    return command;
}