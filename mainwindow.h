#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<QtMultimedia/QMediaPlayer>
#include<QTimer>
#include <QListWidgetItem>
#include <QString>
#include <QFileDialog>
#include <lyrics.h>
#include <QMessageBox>
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE



class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QString settime(int time);

private slots:
    void on_btnPlay_clicked();

    void on_btnPause_clicked();

    void on_hSliderPlayProgress_sliderMoved(int position);

    void setSliderValue();

    void on_vSliderVolume_sliderMoved(int position);

    void on_pushButton_clicked();

    void on_lwMusicList_itemDoubleClicked(QListWidgetItem *item);

    void on_btnPreMusic_clicked();

    void on_btnNextMusic_clicked();

    void on_vSliderVolume_sliderReleased();

    void on_btnVolume_clicked();

    void getduration();

    void setPlayTime();

    void on_lwMusicList_customContextMenuRequested(const QPoint &pos);

    void on_menu_click();
private:
    Ui::MainWindow *ui;
    QMediaPlayer Player; // 实例化对象
    QTimer *timer;
    QString m_playPath;
    int m_PlayRow;
    Lyrics lyrics;
    int playtime;
    int lyricsID = 0;
};
#endif // MAINWINDOW_H
