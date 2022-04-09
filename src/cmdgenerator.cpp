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
    AddCommandLinePart("\"%1\"");
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
            AddCommandLinePart("-crf");
            AddCommandLinePart(QString("%1").arg(videoQuality));
            AddCommandLinePart("-preset");
            AddCommandLinePart(videoEncodingSpeed);

            if (!maxVideoBitRate.isEmpty())
            {
                double bitRate = maxVideoBitRate.toDouble();
                if (bitRate > 0.0)
                {
                    AddCommandLinePart("-maxrate");
                    AddCommandLinePart(QString("%1M").arg(bitRate, 0, 'f', 2));
                    AddCommandLinePart("-bufsize");
                    AddCommandLinePart(QString("%1M").arg(2 * bitRate, 0, 'f', 2));
                }
            }

            AddCommandLinePart("-pix");
            AddCommandLinePart("yuv420p");
        }
        else if (videoCodec == "prores")
        {
            AddCommandLinePart("-profile:v");
            AddCommandLinePart(proresQuality);
        }
        else if (videoCodec == "prores_ks")
        {
            AddCommandLinePart("-profile:v 4444");
        }
        else if (videoCodec == "copy")
        {
            if ((videoHeight != 0) && (videoHeight != 0))
            {
                AddCommandLinePart("-s");
                AddCommandLinePart(QString("%1*%2").arg(videoWidth).arg(videoHeight));
            }
        }

        if (!videoExtraParameter.isEmpty())
            AddCommandLinePart(videoExtraParameter);
    }
    else
    {
        AddCommandLinePart("-an");
    }

    if (firstFramePlay)
    {
        AddCommandLinePart("-movflags");
        AddCommandLinePart("+faststart");
    }

    if (audioEnabled)
    {
        if (audioCodec != "自动")
        {
            AddCommandLinePart("-acodec");
            AddCommandLinePart(audioCodec);
        }

        if (!audioBitRate.isEmpty())
        {
            double audioBitrate = audioBitRate.toDouble();
            if (audioBitrate > 0.0)
            {
                AddCommandLinePart("-b:a");
                AddCommandLinePart(QString("%1M").arg(audioBitrate, 0, 'f', 2));
            }
        }

        if (!audioExtraParameter.isEmpty())
        {
            AddCommandLinePart(audioExtraParameter);
        }
    }
    else
    {
        AddCommandLinePart("-an");
    }

    AddCommandLinePart("\"%2\"");
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

    QString parameterizedCommand = command.arg(sourcePath).arg(GetNextUsablePath());
    return parameterizedCommand;
}


QString CmdGenerator::GetNextUsablePath()
{
    QString fullPath = QFileInfo(sourcePath).absoluteFilePath();
    if (overwriteExisting)
        return fullPath;

    QFileInfo path(fullPath);
    QDir dir = path.dir();
    QString basename = path.baseName();
    QString suffix = path.suffix();
    QString newPath = dir.absoluteFilePath(basename);

    constexpr size_t MAX_TRY = 1000;
    for (size_t i = 0; i < MAX_TRY; i++)
    {
        QString candidatePath = QString("%1-converted-%2.%3").arg(newPath).arg(i).arg(containerFormat);
        if (!QFileInfo::exists(candidatePath))
            return candidatePath;
    }
    return "";
}
