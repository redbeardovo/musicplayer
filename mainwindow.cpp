#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
        timer = new QTimer();
        timer->setInterval(2000);//初始化定时器 2s发一次信号
        timer->start();
        ui->pushButton->setIcon(QIcon(":/add.png"));
        ui->btnPlay->setIcon(QIcon(":/bofang.png"));
        ui->vSliderVolume->setVisible(false);
        QString fileName = "D:/QTfile/musicplayer";
        QDir dir(fileName);
        QStringList nameFilters;
        nameFilters << "*.mp3";
        QStringList files = dir.entryList(nameFilters, QDir::Files|QDir::Readable, QDir::Name);
        ui->lwMusicList->addItems(files);
        m_playPath = fileName;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btnPlay_clicked()
{
    if(QMediaPlayer:: PlayingState == Player.state()){

            ui->btnPlay->setIcon(QIcon(":/bofang.png"));
            //ui->btnPlay->setText("Play");
            Player.pause();
        }
        else{
            ui->btnPlay->setIcon(QIcon(":/bf1.png"));
            //ui->btnPlay->setText("Pause");
            Player.play();
        }
}

void MainWindow::on_btnPause_clicked()
{
    if(QMediaPlayer:: PlayingState == Player.state()){
            Player.pause();
        }
}

void MainWindow::on_hSliderPlayProgress_sliderMoved(int position)
{
    Player.setPosition(Player.duration()*position/100);
}
/*
 * m_mediaPlayer.duration()：音频文件的总帧数（）
position：是当前滑动条的位置，默认滑动条的总长度是0到99。
m_mediaPlayer.duration()*position/100：音频文件的第多少帧。
m_mediaPlayer.setPosition():设置从哪一帧开始播放*/
void MainWindow::setSliderValue()
{
    ui->hSliderPlayProgress->setValue(Player.position()*100/Player.duration());
}

void MainWindow::on_vSliderVolume_sliderMoved(int position)
{
    Player.setVolume(position);
}

void MainWindow::on_pushButton_clicked()
{
   QString fileName = QFileDialog::getExistingDirectory(NULL,"Select Music Dir",".",NULL);
        QDir dir(fileName);
        QStringList nameFilters;
        nameFilters << "*.mp3";
        nameFilters << "*.ncm";
        QStringList files = dir.entryList(nameFilters, QDir::Files|QDir::Readable, QDir::Name);
        ui->lwMusicList->addItems(files);
        m_playPath = fileName;
}

void MainWindow::on_lwMusicList_itemDoubleClicked(QListWidgetItem *item)
{
    m_PlayRow = ui->lwMusicList->row(item);
        Player.stop();
        Player.setMedia(QUrl::fromLocalFile(m_playPath+"/"+item->text()));
        QString name = item->text().left(item->text().lastIndexOf("."));
        if(!lyrics.readLyricsFile(m_playPath+"/"+name+".lrc")){
            ui->lblLyrics1->setText("未检测到歌词文件");
            ui->lblLyrics2->setText("请检查歌词文件是否存在");
        }
        lyricsID = 0;
        Player.play();
        ui->hSliderPlayProgress->setValue(Player.position());
        ui->btnPlay->setIcon(QIcon(":/bf1.png"));
        //ui->btnPlay->setText("Pause");
        connect(timer, SIGNAL(timeout()), this, SLOT(setSliderValue()));//将信号与槽连接，其实就相当于每两秒钟执行一次槽函数
        connect(&Player,SIGNAL(durationChanged(qint64)),this,SLOT(getduration()));
        connect(&Player,SIGNAL(positionChanged(qint64)),this,SLOT(setPlayTime()));
}

void MainWindow::on_btnPreMusic_clicked()
{
    if(m_PlayRow == 0){
            m_PlayRow = ui->lwMusicList->count() - 1;
        }
        else{
            m_PlayRow--;
        }
        QListWidgetItem *item = ui->lwMusicList->item(m_PlayRow);
        item->setSelected(true);
        Player.stop();
        QString name = item->text().left(item->text().lastIndexOf("."));
        if(!lyrics.readLyricsFile(m_playPath+"/"+name+".lrc")){
            ui->lblLyrics1->setText("未检测到歌词文件");
            ui->lblLyrics2->setText("请检查歌词文件是否存在");
        }
        lyricsID = 0;
        Player.setMedia(QUrl::fromLocalFile(m_playPath+"/"+item->text()));
        Player.play();
}

void MainWindow::on_btnNextMusic_clicked()
{
    if(m_PlayRow + 1 == ui->lwMusicList->count()){
            m_PlayRow = 0;
        }
        else{
            m_PlayRow++;
        }
        QListWidgetItem *item = ui->lwMusicList->item(m_PlayRow);
        item->setSelected(true);
       Player.stop();
       QString name = item->text().left(item->text().lastIndexOf("."));
       if(!lyrics.readLyricsFile(m_playPath+"/"+name+".lrc")){
           ui->lblLyrics1->setText("未检测到歌词文件");
           ui->lblLyrics2->setText("请检查歌词文件是否存在");
       }
       lyricsID = 0;
        Player.setMedia(QUrl::fromLocalFile(m_playPath+"/"+item->text()));
       Player.play();
}


void MainWindow::on_vSliderVolume_sliderReleased()
{
    ui->vSliderVolume->setVisible(false);
}

void MainWindow::on_btnVolume_clicked()
{
    if(ui->vSliderVolume->isVisible()){
        ui->vSliderVolume->setVisible(false);
    }else{
        ui->vSliderVolume->setVisible(true);
    }
}

QString MainWindow::settime(int time)
{
    int h,m,s;
    time /= 1000;  //获得的时间是以毫秒为单位的
    h = time/3600;
    m = (time-h*3600)/60;
    s = time-h*3600-m*60;
    return QString("%1:%2:%3").arg(h).arg(m).arg(s);
}

void MainWindow::getduration()
{
    playtime = Player.duration();
    ui->lblMusicTime->setText(settime(playtime));
}
void MainWindow::setPlayTime()
{
    if(!lyrics.getListLyricsTime().empty()&&Player.position()>=lyrics.getListLyricsTime().at(lyricsID)&&lyricsID<lyrics.getListLyricsTime().size()-1){
            ui->lblLyrics1->setText(lyrics.getListLyricsText().at(lyricsID));
            ui->lblLyrics2->setText(lyrics.getListLyricsText().at(lyricsID+1));
            lyricsID++;
        }
        ui->lblPlayTime->setText(settime(Player.position()));
}


void MainWindow::on_lwMusicList_customContextMenuRequested(const QPoint &/*pos*/)
{
    QMenu *cmenu = new QMenu(ui->pushButton);

        QAction *action1 = new QAction(tr("删除"), this);
        action1->setData(1);
        cmenu->addAction(action1);
        connect(action1, SIGNAL(triggered(bool)), this, SLOT(on_menu_click()));
        cmenu->exec(QCursor::pos());
}

void MainWindow::on_menu_click()
{
    /*获取当前选中的Item*/
        QList<QListWidgetItem*> items = ui->lwMusicList->selectedItems();

        if (items.count() > 0)
        {
            /*弹出询问对话框*/
            if (QMessageBox::Yes == QMessageBox::question(this, QStringLiteral("Remove Item"),
                QStringLiteral("Remove %1 item").arg(QString::number(items.count())), QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes))
            {
                foreach(QListWidgetItem* var, items) {
                    ui->lwMusicList->removeItemWidget(var);
                    items.removeOne(var);
                    delete var;
                }
            }
        }
}
