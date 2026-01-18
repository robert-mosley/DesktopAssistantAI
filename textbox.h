#pragma once
#ifndef TEXTBOX_H
#define TEXTBOX_H
#include <QWidget>
#include <QLineEdit>
#include <QAudioSource>
#include <QBuffer>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QPushButton>
#include <QTextEdit>
#include <windows.h>
#include <sapi.h>
#include <QTextToSpeech>
#include <QFile>

struct VoskModel;
struct VoskRecognizer;

class TextBox : public QWidget
{
    Q_OBJECT
public:
    explicit TextBox(QWidget *parent = nullptr);
    void StartRecording();
    void StopRecording();
    void HandleReply(QNetworkReply *reply);
    void speak(const QString &text);
    QString openfile(const QString& filepath);
    void onPressed();
    void convertToWav(const QString& rawFile, const QString& wavFile);
    bool whisperAvailable();
private:
    bool on = false;
    QNetworkAccessManager *manager;
    QLineEdit *input;
    QAudioSource *audio;
    QBuffer *buffer;
    QPushButton *start;
    QPushButton *stop;
    QString interface_word;
    QTextToSpeech *tts;
    QFile *recordingFile = nullptr;

    VoskModel* voskModel = nullptr;
    VoskRecognizer* recognizer = nullptr;
};

#endif // TEXTBOX_H
