#ifndef WIDGHT_H
#define WIDGHT_H
#include<QWidget>
#include <QPushButton>
#include<QDialog>
#include<QtMultimedia>
#include<QCheckBox>
#include<QGridLayout>
#include<QBoxLayout>
#include<QLabel>
#include<QSlider>
//#include"globalv.h"    //引用全局变量
//static int Chess_Music=1;  //控制落子音效的播放

class chessborad;
class index: public QWidget
{
    Q_OBJECT
public:
    index(QWidget* parent=0);
    friend class chessboard;
    int SetIndex(QWidget*);
    QPushButton *actByPerson;
    QPushButton *actByAi;
    QPushButton *Settings;
    static bool Chess_Music;   //设置落子音效为全局变量来控制
    static QMediaPlayer *chessplayer;
    ~index();
public slots:
    void startgame();
    void startAI();
    void settingslabel();
    void Backgroundmusic(int state);
    void changebmusic(int volume);
    void changecmusic(int volume);
    void Chessmusic(int state);
    void Turnback();

private:
    QMediaPlayer player;
    QMediaPlaylist playlist;
    QLabel *settinglabel;
    QCheckBox *backgroundmusic;
    QCheckBox *chessmusic;
    QSlider *bslider,*cslider;
};

#endif // WIDGHT_H
