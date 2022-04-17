#include "mainwindow.h"
#include <QMimeData>
#include <QFileInfo>
#include <QMessageBox>
#include <QTime>
#include <QFileDialog>
#include <QStandardPaths>

using namespace std;

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent)
{
    setWindowTitle("Part3");
    resize(600, 700);
    setAcceptDrops(true);

    auto widget = new QWidget(this);

    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setAlignment(Qt::AlignTop);

    auto fileListLabel = new QLabel("文件：");
    mainLayout->addWidget(fileListLabel);

    auto fileLayout = new QHBoxLayout;
    fileLayout->setAlignment(Qt::AlignLeft);

    m_fileEdit = new QLineEdit;
    m_fileSelect = new QPushButton("选择文件...");
    fileLayout->addWidget(m_fileEdit);
    fileLayout->addWidget(m_fileSelect);

    mainLayout->addLayout(fileLayout);

    // Video
    auto videoLayout = new QHBoxLayout;
    videoLayout->addWidget(new QLabel("视频："));

    m_videoEnabled = new QCheckBox("启用");
    m_videoEnabled->setChecked(true);
    videoLayout->addWidget(m_videoEnabled);

    m_videoInfo = new QLabel("信息：");
    videoLayout->addWidget(m_videoInfo);

    videoLayout->setAlignment(Qt::AlignLeft);

    mainLayout->addLayout(videoLayout);

    auto videoConfigs = new QGridLayout(this);

    videoConfigs->addWidget(new QLabel("输出编码："), 0, 0);

    m_videoCodec = new QComboBox;
    QStringList videoCodecs = {"copy", "h264", "libx265", "prores", "prores_ks", "自动"};
    m_videoCodec->addItems(videoCodecs);
    videoConfigs->addWidget(m_videoCodec, 0, 1);

    videoConfigs->addWidget(new QLabel("Prores 质量："), 0, 2);
    m_proresQuality = new QComboBox;
    QStringList videoQualities = {"0: Proxy", "1: LT", "2: Normal", "3: HQ"};
    m_proresQuality->addItems(videoQualities);
    videoConfigs->addWidget(m_proresQuality, 0, 3);

    videoConfigs->addWidget(new QLabel("宽度："), 1, 0);

    m_videoWidth = new QComboBox;
    QStringList videoWidths = {"自动", "2048", "1920", "1280", "720", "480", "320"};
    m_videoWidth->addItems(videoWidths);
    videoConfigs->addWidget(m_videoWidth, 1, 1);

    videoConfigs->addWidget(new QLabel("高度："), 1, 2);

    m_videoHeight = new QComboBox;
    QStringList videoHeights = {"自动", "1080", "858", "720", "450", "360", "270"};
    m_videoHeight->addItems(videoHeights);
    videoConfigs->addWidget(m_videoHeight, 1, 3);

    m_keepAspectRatio = new QCheckBox("保持宽高比");
    m_keepAspectRatio->setChecked(true);
    videoConfigs->addWidget(m_keepAspectRatio, 1, 4);

    m_firstFramePlay = new QCheckBox("首帧播放");
    m_firstFramePlay->setChecked(false);
    videoConfigs->addWidget(m_firstFramePlay, 1, 5);

    videoConfigs->addWidget(new QLabel("质量："), 2, 0);

    m_videoCRF = new QSpinBox;
    m_videoCRF->setMinimum(0);
    m_videoCRF->setMaximum(51);
    m_videoCRF->setValue(18);
    m_videoCRF->setToolTip("建议取值18-28之间，数字越大，质量越差");
    videoConfigs->addWidget(m_videoCRF, 2, 1);

    videoConfigs->addWidget(new QLabel("编码速度："), 2, 2);

    m_encodingSpeed = new QComboBox;
    m_encodingSpeed->addItems({"veryfast", "faster", "fast", "medium"});
    m_encodingSpeed->setCurrentText("faster");
    videoConfigs->addWidget(m_encodingSpeed, 2, 3);

    videoConfigs->addWidget(new QLabel("最大码率："), 2, 4);

    m_maxVideoBitRate = new QLineEdit("0");
    videoConfigs->addWidget(m_maxVideoBitRate, 2, 5);

    videoConfigs->addWidget(new QLabel("Mb/s"), 2, 6);
    mainLayout->addLayout(videoConfigs);

    mainLayout->addWidget(new QLabel("额外参数："));

    m_videoExtraParameters = new QLineEdit;
    m_videoExtraParameters->setToolTip("-r rate 帧率\n-vframes number 要转换的帧数量\n-movflags +faststart 首帧播放");
    mainLayout->addWidget(m_videoExtraParameters);


    // Audio
    auto audioLayout = new QHBoxLayout;
    audioLayout->setAlignment(Qt::AlignLeft);
    audioLayout->addWidget(new QLabel("音频："));

    m_audioEnabled = new QCheckBox("启用");
    m_audioEnabled->setChecked(true);
    audioLayout->addWidget(m_audioEnabled);

    m_audioInfo = new QLabel("信息：");
    audioLayout->addWidget(m_audioInfo);

    mainLayout->addLayout(audioLayout);

    auto audioConfigs = new QHBoxLayout;
    audioConfigs->setAlignment(Qt::AlignLeft);
    audioConfigs->addWidget(new QLabel("输出编码："));

    m_audioCodec = new QComboBox;
    auto audioCodecs = QStringList({"copy", "aac", "pcm_s16le", "flac", "自动"});
    m_audioCodec->addItems(audioCodecs);
    audioConfigs->addWidget(m_audioCodec);

    audioConfigs->addWidget(new QLabel("码率："));

    m_audioBitRate = new QLineEdit("0");
    audioConfigs->addWidget(m_audioBitRate);

    audioConfigs->addWidget(new QLabel("Mb/s"));

    mainLayout->addLayout(audioConfigs);

    mainLayout->addWidget(new QLabel("额外参数："));

    m_audioExtraParameters = new QLineEdit;
    mainLayout->addWidget(m_audioExtraParameters);

    auto containerLayout = new QHBoxLayout;
    containerLayout->setAlignment(Qt::AlignLeft);
    containerLayout->addWidget(new QLabel("封装格式："));

    m_containerFormat = new QComboBox;
    auto containerFormats = QStringList({"MP4",  "MOV", "AAC", "WAV", "FLAC", "MP3"});
    m_containerFormat->addItems(containerFormats);
    containerLayout->addWidget(m_containerFormat);

    m_overwriteExisting = new QCheckBox("覆盖重名文件");
    containerLayout->addWidget(m_overwriteExisting);

    mainLayout->addLayout(containerLayout);

    auto commandLineLayout = new QHBoxLayout;
    commandLineLayout->setAlignment(Qt::AlignLeft);
    commandLineLayout->addWidget(new QLabel("命令行："));

    m_progressBar = new QProgressBar;
    m_progressBar->setMinimum(0);
    m_progressBar->setMaximum(100);
    commandLineLayout->addWidget(m_progressBar);
    m_progressBar->setVisible(false);

    m_timeRemaining = new QLabel("剩余时间:");
    commandLineLayout->addWidget(m_timeRemaining);
    m_timeRemaining->setVisible(false);

    mainLayout->addLayout(commandLineLayout);

    m_commandLine = new QTextEdit;
    mainLayout->addWidget(m_commandLine);

    m_goButton = new QPushButton("Go!");
    mainLayout->addWidget(m_goButton);

    widget->setLayout(mainLayout);
    setCentralWidget(widget);

    connect(m_fileEdit, &QLineEdit::textChanged, [&](){ updateCommand(); });
    connect(m_fileSelect, &QPushButton::clicked, this, &MainWindow::selectFile);

    connect(m_videoEnabled, &QCheckBox::stateChanged, [&](){ updateCommand(); });
    connect(m_videoCodec, &QComboBox::currentTextChanged, [&](){ updateCommand(); });
    connect(m_proresQuality, &QComboBox::currentTextChanged, [&](){ updateCommand(); });
    connect(m_videoWidth, &QComboBox::currentTextChanged, [&](){ updateCommand(); });
    connect(m_videoWidth, &QComboBox::currentTextChanged, [&](){ updateCommand(); });
    connect(m_videoWidth, &QComboBox::currentTextChanged, [&](){ updateCommand(); });
    connect(m_keepAspectRatio, &QCheckBox::stateChanged, [&](){ updateCommand(); });
    connect(m_firstFramePlay, &QCheckBox::stateChanged, [&](){ updateCommand(); });
    connect(m_videoCRF, &QSpinBox::valueChanged, [&](){ updateCommand(); });
    connect(m_encodingSpeed, &QComboBox::currentTextChanged, [&](){ updateCommand(); });
    connect(m_maxVideoBitRate, &QLineEdit::textChanged, [&](){ updateCommand(); });
    connect(m_videoExtraParameters, &QLineEdit::textChanged, [&](){ updateCommand(); });

    connect(m_audioEnabled, &QCheckBox::stateChanged, [&](){ updateCommand(); });
    connect(m_audioCodec, &QComboBox::currentTextChanged, [&](){ updateCommand(); });
    connect(m_audioBitRate, &QLineEdit::textChanged, [&](){ updateCommand(); });
    connect(m_audioExtraParameters, &QLineEdit::textChanged, [&](){ updateCommand(); });

    connect(m_containerFormat, &QComboBox::currentTextChanged, [&](){ updateCommand(); });
    connect(m_overwriteExisting, &QCheckBox::stateChanged, [&](){ updateCommand(); });

    connect(m_goButton, &QPushButton::clicked, this, &MainWindow::runCommand);

    m_cmdGenerator = new CmdGenerator;
    updateCommand();
}


void MainWindow::updateCommand()
{
    m_cmdGenerator->sourcePath = m_fileEdit->text();
    m_cmdGenerator->videoEnabled = m_videoEnabled->isChecked();
    m_cmdGenerator->videoCodec = m_videoCodec->currentText();
    m_cmdGenerator->proresQuality = m_proresQuality->currentText();
    // string value "auto" will automatically become 0
    m_cmdGenerator->videoWidth = m_videoWidth->currentText().toInt();
    m_cmdGenerator->videoHeight = m_videoHeight->currentText().toInt();
    m_cmdGenerator->keepAspectRatio = m_keepAspectRatio->isChecked();
    m_cmdGenerator->firstFramePlay = m_firstFramePlay->isChecked();
    m_cmdGenerator->videoQuality = m_videoCRF->text().toInt();
    m_cmdGenerator->videoEncodingSpeed = m_encodingSpeed->currentText();
    m_cmdGenerator->maxVideoBitRate = m_maxVideoBitRate->text();
    m_cmdGenerator->videoExtraParameter = m_videoExtraParameters->text();

    m_cmdGenerator->audioEnabled = m_audioEnabled->isChecked();
    m_cmdGenerator->audioCodec = m_audioCodec->currentText();
    m_cmdGenerator->audioBitRate = m_audioBitRate->text();
    m_cmdGenerator->audioExtraParameter = m_audioExtraParameters->text();

    m_cmdGenerator->containerFormat = m_containerFormat->currentText();
    m_cmdGenerator->overwriteExisting = m_overwriteExisting->isChecked();

    m_commandLine->setText(m_cmdGenerator->GetCommand());
}


void MainWindow::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasUrls())
        event->acceptProposedAction();
}


void MainWindow::dropEvent(QDropEvent *event)
{
    auto mimeData = event->mimeData();
    if ((!mimeData) || (!mimeData->hasUrls()))
        return;

    auto urls = event->mimeData()->urls();
    if (urls.empty())
        return;

    auto url = urls[0];
    auto path = url.toLocalFile();
    if (!QFileInfo::exists(path))
        return;

    m_fileEdit->setText(path);
    event->acceptProposedAction();
}


void MainWindow::runCommand()
{
    if (m_goButton->text() == "Go!")
    {
        m_goButton->setText("Stop!");
        m_progressBar->setVisible(true);
        m_timeRemaining->setVisible(true);
        m_executor = new CommandLineExecutor(m_commandLine->toPlainText());
        connect(m_executor, &CommandLineExecutor::Failed, this, &MainWindow::onFailed);
        connect(m_executor, &CommandLineExecutor::Succeeded, this, &MainWindow::onSucceeded);
        connect(m_executor, &CommandLineExecutor::Progress, this, &MainWindow::onProgress);
        m_executor->Start();
    }
    else
    {
        m_executor->Kill();
        delete m_executor;
        m_executor = nullptr;

        resetProgressUI();
    }
}


void MainWindow::resetProgressUI()
{
    m_goButton->setText("Go!");
    m_progressBar->setVisible(false);
    m_progressBar->setValue(0);
    m_timeRemaining->setVisible(false);
    m_timeRemaining->setText("剩余时间:");
    updateCommand();
}


void MainWindow::onFailed(int exitCode)
{
    QMessageBox(QMessageBox::Critical, "转换失败", QString("错误代码: %1").arg(exitCode),
                            QMessageBox::Ok).exec();
    resetProgressUI();
}


void MainWindow::onSucceeded(const QTime &time)
{
    QMessageBox(QMessageBox::Information, "Part3", QString("转换成功！用时%1s").arg(time.toString("hh:mm:ss")),
                        QMessageBox::Ok).exec();
    resetProgressUI();
}


void MainWindow::onProgress(double percentage, const QTime &time)
{
    m_progressBar->setValue(percentage);
    QString text = QString("%1\% 剩余时间： %2").arg(percentage, 0, 'g', 1).arg(time.toString("hh:mm:ss"));
    m_timeRemaining->setText(text);
}


void MainWindow::selectFile()
{
    QString fileName = QFileDialog::getOpenFileName(this, "选择文件",
                                                    QDir::homePath());
    m_fileEdit->setText(fileName);
}
