#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QStandardItemModel>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:
    void on_open_clicked();

    void on_play_clicked();

    void on_pause_clicked();

    void on_stop_clicked();

    void on_previous_clicked();

    void on_next_clicked();

    void on_mute_unmute_clicked();

    void on_volume_control_valueChanged(int value);

    void durationChanged(qint64 duration);

    void positionChanged(qint64 progress);

    void show_time(quint64 currentInfo);

    void on_song_sliderMoved(int position);

private:
    Ui::Widget *ui;
    QMediaPlayer *Mmusicplayer;
    QMediaPlaylist *playlist;
    QStandardItemModel *playlistModel;
    quint64 duration;

};
#endif // WIDGET_H
