#include <QMainWindow>
#include <QCheckBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QTextEdit>
#include <QLineEdit>
#include <QGridLayout>
#include <QComboBox>
#include <QSpinBox>
#include <QPushButton>
#include <QDragEnterEvent>
#include "cmdgenerator.h"

class MainWindow : public QMainWindow
{
Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override = default;

private slots:
    void updateCommand();

private:
    QLineEdit* m_fileEdit;
    QCheckBox* m_videoEnabled;
    QLabel* m_videoInfo;
    QComboBox* m_videoCodec;
    QComboBox* m_proresQuality;
    QComboBox* m_videoWidth;
    QComboBox* m_videoHeight;
    QCheckBox* m_keepAspectRatio;
    QSpinBox* m_videoCRF;
    QComboBox* m_encodingSpeed;
    QLineEdit* m_maxVideoBitRate;
    QCheckBox* m_firstFramePlay;
    QLineEdit* m_videoExtraParameters;

    QCheckBox* m_audioEnabled;
    QLabel* m_audioInfo;
    QComboBox* m_audioCodec;
    QLineEdit* m_audioBitRate;
    QLineEdit* m_audioExtraParameters;

    QComboBox* m_containerFormat;
    QCheckBox* m_overwriteExisting;

    QLabel* m_timeRemaining;

    QTextEdit* m_commandLine;
    QPushButton* m_goButton;

    std::string m_ffmpegPath;
    std::string m_ffprobePath;

    CmdGenerator* m_cmdGenerator;

    void dragEnterEvent(QDragEnterEvent *event) override;
    void dropEvent(QDropEvent *event) override;
};
