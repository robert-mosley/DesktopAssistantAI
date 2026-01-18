#include "textbox.h"
#include "miniwindow.h"
#include <QLineEdit>
#include <QHBoxLayout>
#include <QDebug>
#include <QPushButton>
#include <QAudioFormat>
#include <QMediaDevices>
#include <QJsonObject>
#include <QJsonArray>
#include "vosk_api.h"
#include <QUrlQuery>
#include <QProcess>
#include "largewindow.h"
#include <QFile>
#include <QDir>
#include <QCoreApplication>
#include <cstdlib>
#include <QTimer>

TextBox::TextBox(QWidget *parent)
    : QWidget(parent)
{
    setWindowFlags(
        Qt::FramelessWindowHint |
        Qt::WindowStaysOnTopHint |
        Qt::Tool
        );

    setAttribute(Qt::WA_TranslucentBackground);
    QString tmic = "🔊";
    interface_word = "J.A.R.V.I.S.";
    input = new QLineEdit(this);
    tts = new QTextToSpeech();
    for (const QVoice &voice : tts->availableVoices()) {
        if (voice.name().contains("Guy Neural", Qt::CaseInsensitive)) {
            tts->setVoice(voice);
            break;
        }
    }

    tts->setRate(-0.05);
    tts->setPitch(-0.10);
    tts->setVolume(1.0);

    buffer = new QBuffer(this);
    QFile *recordingFile = nullptr;


    manager = new QNetworkAccessManager(this);
    input->setPlaceholderText(interface_word);
    start = new QPushButton(tmic, this);

    start->setStyleSheet(R"(
        QPushButton {
            background-color: rgba(0, 0, 0, 60);
            border-radius: 18px;
            font-size: 16px;
        }
        QPushButton:hover {
            background-color: rgba(0, 0, 0, 70);
        }
        QPushButton:pressed {
            background-color: rgba(0, 0, 0, 90);
        }
    )");

    start->setFixedSize(36, 36);
    connect(start, &QPushButton::clicked, this, &TextBox::StartRecording);
    connect(input, &QLineEdit::returnPressed, this, &TextBox::onPressed);
    connect(manager, &QNetworkAccessManager::finished, this, &TextBox::HandleReply);

    input->setStyleSheet(
        "color: white;"
        "font-size: 18px;"
        "background-color: rgba(20, 20, 20, 180);"
        "padding: 12px;"
        "border-radius: 10px;"
        );

    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->addWidget(input);
    layout->addWidget(start);
    setLayout(layout);

    resize(300, 60);
}

void TextBox::onPressed() {
    QString text = input->text();
    QString urlfile = "http://localhost:8000/ask";
    QUrl url(urlfile);
    QUrlQuery query;
    query.addQueryItem("text", text);
    url.setQuery(query);
    QNetworkRequest request(url);

    QJsonObject payload;
    payload["text"] = text;
    QJsonDocument pdoc(payload);

    manager->post(request, pdoc.toJson());
    qDebug() << "sent";
}

void TextBox::StartRecording() {
    if (on == true) {
        audio->stop();

        if (recordingFile) {
            recordingFile->close();
            delete recordingFile;
            recordingFile = nullptr;
        }

        on = false;QFile rawCheck("input.raw");
        if (rawCheck.exists()) {
            qDebug() << "Raw file size:" << rawCheck.size() << "bytes";
        } else {
            qDebug() << "Raw file doesn't exist!";
            return;
        }


        convertToWav("input.raw", "input.wav");

        QProcess process;

        QStringList args = {
            "-m", CoreApplication::applicationDirPath() + "/bin/ggml-base.en.bin",
            "-f", "input.wav",
            "-l", "en",
            "--no-timestamps"
        };
        pQString whisperPath = QCoreApplication::applicationDirPath() + "/bin/whisper-cli.exe";
        process.start(whisperPath, args);


        QString output = process.readAll();
        QString text;
        QStringList lines = output.split('\n');

        for (const QString& line : lines) {
            QString trimmed = line.trimmed();

            if (!trimmed.isEmpty() &&
                !trimmed.startsWith("whisper_") &&
                !trimmed.contains("error:") &&
                !trimmed.startsWith("[") &&
                !trimmed.contains("load time") &&
                !trimmed.contains("total time")) {
                text = trimmed;
                break;
            }
        }

        qDebug() << "Whisper stdout:" << text;
        qDebug() << "Exit code:" << process.exitCode();

        process.waitForFinished();


        input->setText(interface_word);
        QString urlfile = "http://localhost:8000/ask";
        QUrl url(urlfile);
        QUrlQuery query;
        query.addQueryItem("text", text);
        url.setQuery(query);
        QNetworkRequest request(url);

        QJsonObject payload;
        payload["text"] = text;
        QJsonDocument pdoc(payload);

        manager->post(request, pdoc.toJson());

        qDebug() << text;
        vosk_recognizer_free(recognizer);
        recognizer = nullptr;

    } else {
        qDebug() << whisperAvailable();
        if (whisperAvailable()) {
            QAudioDevice device = QMediaDevices::defaultAudioInput();
            QAudioFormat format;
            format.setSampleRate(16000);
            format.setChannelCount(1);
            format.setSampleFormat(QAudioFormat::Int16);

            audio = new QAudioSource(format, this);
            recordingFile = new QFile("input.raw");

            if (!recordingFile->open(QIODevice::WriteOnly)) {
                qDebug() << "Failed to open file";
                delete recordingFile;
                recordingFile = nullptr;
                return;
            }

            audio->start(recordingFile);
            input->setText("Recording... ");
            on = true;
        } else {
            qDebug() << "Whisper needs to be installed!!";
        }
    }
}

void TextBox::HandleReply(QNetworkReply *reply) {
    if (reply->error()) {
        input->setText(input->text() + reply->errorString());
        reply->deleteLater();
        return;
    }

    QByteArray response = reply->readAll();

    QString message = QString(response);
    for (LargeWindow *w : findChildren<LargeWindow*>()) {
        w->close();
    }
    LargeWindow *windo = new LargeWindow(this);
    windo->createLargeWindow(response);
    windo->move(770, 60);
    windo->show();

    tts->say(message);;
    qDebug() << "spoken";

    qDebug() << response;
    qDebug() << "done";

    reply->deleteLater();
}

void TextBox::speak(const QString &text) {
    ISpVoice *pVoice = nullptr;
    CoInitialize(nullptr);

    HRESULT hr = CoCreateInstance(CLSID_SpVoice, nullptr, CLSCTX_ALL,
                                  IID_ISpVoice, (void**)&pVoice);

    if (SUCCEEDED(hr)) {
        std::wstring wtext = text.toStdWString();
        pVoice->Speak(wtext.c_str(), 0, nullptr);
        pVoice->Release();
    }

    CoUninitialize();
}

QString TextBox::openfile(const QString& filePath)
{
    QFile file(filePath);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Could not open file";
        return "no file";
    }

    QTextStream in(&file);
    QString contents = in.readAll();
    file.close();

    return contents;
}

void TextBox::convertToWav(const QString& rawFile, const QString& wavFile) {
    QFile raw(rawFile);
    QFile wav(wavFile);

    if (!raw.open(QIODevice::ReadOnly)) {
        qDebug() << "Failed to open raw file for reading";
        return;
    }

    if (!wav.open(QIODevice::WriteOnly)) {
        qDebug() << "Failed to open WAV file for writing";
        raw.close();
        return;
    }

    QByteArray audioData = raw.readAll();
    qDebug() << "Read" << audioData.size() << "bytes from raw file";

    int dataSize = audioData.size();
    int sampleRate = 16000;
    short numChannels = 1;
    short bitsPerSample = 16;

    int chunkSize = dataSize + 36;
    int fmtSize = 16;
    short audioFormat = 1; // PCM
    int byteRate = sampleRate * numChannels * bitsPerSample / 8;
    short blockAlign = numChannels * bitsPerSample / 8;

    qDebug() << "Writing WAV header...";

    wav.write("RIFF", 4);
    wav.write(reinterpret_cast<const char*>(&chunkSize), 4);
    wav.write("WAVE", 4);
    wav.write("fmt ", 4);
    wav.write(reinterpret_cast<const char*>(&fmtSize), 4);
    wav.write(reinterpret_cast<const char*>(&audioFormat), 2);
    wav.write(reinterpret_cast<const char*>(&numChannels), 2);
    wav.write(reinterpret_cast<const char*>(&sampleRate), 4);
    wav.write(reinterpret_cast<const char*>(&byteRate), 4);
    wav.write(reinterpret_cast<const char*>(&blockAlign), 2);
    wav.write(reinterpret_cast<const char*>(&bitsPerSample), 2);
    wav.write("data", 4);
    wav.write(reinterpret_cast<const char*>(&dataSize), 4);
    wav.write(audioData);

    wav.flush();
    raw.close();
    wav.close();

    QFile check(wavFile);
    if (check.exists()) {
        qDebug() << "File created";
    } else {
        qDebug() << "File not created";
    }
}

bool TextBox::whisperAvailable()
{
    QString whisperPath = QCoreApplication::applicationDirPath() + "/bin/whisper";
    QString modelPath   = QCoreApplication::applicationDirPath() + "/models/ggml-base.en.bin";

    return QFile::exists(whisperPath) && QFile::exists(modelPath);
}
