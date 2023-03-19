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
#include <QProcess>
#include <QProgressBar>
#include "cmdgenerator.h"
#include "cmdexecutor.h"

class MainWindow : public QMainWindow
{
Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override = default;

    void updateEnabledWidgets();

private slots:
    void selectFile();
    void updateCommand();
    void runCommand();
    void onFailed(int exitCode);
    void onSucceeded(const QTime& time);
    void onProgress(double percentage, const QTime& time);

private:
    QLineEdit* m_fileEdit;
    QPushButton* m_fileSelect;
    QCheckBox* m_videoEnabled;
    QComboBox* m_videoCodec;
    QComboBox* m_proresQuality;
    QComboBox* m_videoWidth;
    QComboBox* m_videoHeight;
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
    QProgressBar* m_progressBar;

    QTextEdit* m_commandLine;
    QPushButton* m_goButton;

    std::string m_ffprobePath;

    CmdGenerator* m_cmdGenerator;

    CommandLineExecutor* m_executor;

    double m_totalSeconds{};

    void dragEnterEvent(QDragEnterEvent *event) override;
    void dropEvent(QDropEvent *event) override;

    void resetProgressUI();

    void processOneLine(const QString& line);
};
