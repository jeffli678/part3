#include "stdio.h"
#include <QString>
#include <vector>
using namespace std;

class CmdGenerator
{
public:
    QString ffmpegPath;
    QString sourcePath;

    bool videoEnabled{};
    QString videoCodec;
    QString proresQuality;
    uint32_t videoWidth{}, videoHeight{};
    bool keepAspectRatio{};
    uint32_t videoQuality{};
    QString videoEncodingSpeed;
    QString maxVideoBitRate;
    QString videoExtraParameter;

    bool audioEnabled{};
    QString audioCodec;
    QString audioBitRate;
    QString audioExtraParameter;

    QString containerFormat;
    bool overwriteExisting{};

    std::vector<QString> m_cmdParts;

    QString GetCommand();
    void AddCommandLinePart(const QString& part);

    CmdGenerator();

private:
    void PrepareCommand();
    void AddCommandPart(const QString& part);
};