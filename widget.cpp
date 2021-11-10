#include "widget.h"
#include "ui_widget.h"
#include <QMediaPlayer>
#include <QFileDialog>
#include <QTime>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    ,ui(new Ui::Widget)
{
    ui->setupUi(this);
    Mmusicplayer = new QMediaPlayer(this);

    playlistModel = new QStandardItemModel(this);
    ui->songlist->setModel(playlistModel);
    duration = 0; // to display time

    playlistModel->setHorizontalHeaderLabels(QStringList()  << tr("Song Playlist")
                                                            << tr("File Path"));
    ui->songlist->hideColumn(1);    // Hide the column that stores the file path
    ui->songlist->verticalHeader()->setVisible(false);                  // Hide line numbering
    ui->songlist->setSelectionBehavior(QAbstractItemView::SelectRows);  // Turn on line selection
    ui->songlist->setSelectionMode(QAbstractItemView::SingleSelection); // Allow only one line to be selected
    ui->songlist->setEditTriggers(QAbstractItemView::NoEditTriggers);  // Disable editing

    ui->songlist->horizontalHeader()->setStretchLastSection(true);  // Turn on the resizing of the last visible column to the width of the TableView

    Mmusicplayer = new QMediaPlayer(this);         // Initialize the player
    playlist = new QMediaPlaylist(Mmusicplayer);  // Initialize the playlist
    Mmusicplayer->setPlaylist(playlist);         // Set the playlist to the player
                // Set the playback volume of the tracks
    playlist->setPlaybackMode(QMediaPlaylist::Loop);  // Set the cyclic playlist playback mode

    // When double-clicking on yhe  track in the playlist it will play
     connect(ui->songlist, &QTableView::doubleClicked, [this](const QModelIndex &index){
     playlist->setCurrentIndex(index.row());
     });

     // when changing the curent playing song it will change and display in the label(songtitle)
      connect(playlist, &QMediaPlaylist::currentIndexChanged, [this](int index){
          ui->songtitle->setText(playlistModel->data(playlistModel->index(index, 0)).toString());
      });
      connect(Mmusicplayer, SIGNAL(durationChanged(qint64)), this, SLOT(durationChanged(qint64)));
      connect(Mmusicplayer, SIGNAL(positionChanged(qint64)), this,SLOT(positionChanged(qint64)));

}

Widget::~Widget()
{
    delete ui;
    delete Mmusicplayer;
    delete playlist;
    delete playlistModel;
}


void Widget::on_open_clicked()
{
    //can select multiple mp3 files, using the file selection dialog
     QStringList files = QFileDialog::getOpenFileNames(this,
                                                       tr("Open files"),
                                                       QString(),
                                                       tr("Audio files (*.mp3)"));

     // Next, set data by name and path to files
    // to the playlist and table displaying the playlist
     foreach (QString filePath, files) {
         QList<QStandardItem *> items;
         items.append(new QStandardItem(QDir(filePath).dirName()));
         items.append(new QStandardItem(filePath));
         playlistModel->appendRow(items);
         playlist->addMedia(QUrl(filePath));

     }
    Mmusicplayer->setVolume(ui->volume_control->value());
    on_play_clicked();

}

void Widget::on_play_clicked()
{
    Mmusicplayer->play(); //play button

}

void Widget::on_pause_clicked()
{
    Mmusicplayer->pause(); //pause button

}

void Widget::on_stop_clicked()
{
    Mmusicplayer->stop(); //stop button

}

void Widget::on_previous_clicked()
{
    playlist->previous(); //previous button

}

void Widget::on_next_clicked()
{
    playlist->next(); //next button
}

void Widget::on_mute_unmute_clicked()
{   // set functions of the mute and unmute buttons
    if(ui->mute_unmute->text() == "mute"){
     Mmusicplayer->setMuted(true);
     ui->mute_unmute->setIcon(QIcon(":/Icons/icons/unmute .png"));
     ui->mute_unmute->setText("unmute");

  }
  else
   {
   Mmusicplayer->setMuted(false);
   ui->mute_unmute->setIcon(QIcon(":/Icons/icons/mute .png"));
   ui->mute_unmute->setText("mute");

    }
}

void Widget::on_volume_control_valueChanged(int value)
{
    Mmusicplayer->setVolume(value); //can controll volume level of the player
}

void Widget::positionChanged(qint64 progress)
{
  // if (!ui->songBar->isSliderDown()) {
        ui->song->setValue(progress / 1000);
        show_time(progress/1000);
   // }
}


void Widget::durationChanged(qint64 duration)
{
    this->duration = duration/1000;
    ui->song->setMaximum(duration / 1000);
}

void Widget::show_time(quint64 currentInfo){
       //set songduration to display total time and current playing time of track
        QString tStr;

            QTime currentTime(0,(currentInfo/60)%60, currentInfo%60);
            QTime totalTime(0,(duration/60)%60, duration%60);
            QString format = "mm:ss";
            tStr = currentTime.toString(format) + " / " + totalTime.toString(format);

        ui->songduration->setText(tStr);

}


void Widget::on_song_sliderMoved(int position)
{
    Mmusicplayer->setPosition(position*1000);
}

