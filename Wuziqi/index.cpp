#include "index.h"
#include "chessboard.h"

index::index(QWidget *parent):QWidget(parent)
{

    setWindowTitle("五子棋"); //设置窗口标题
    //setWindowFlags(Qt::FramelessWindowHint);  //无边框
    QPixmap picture(":/images/resource/indx.jpg");
    QPalette palette;
    palette.setBrush(backgroundRole(), QBrush(picture));
    setPalette(palette);
    setFixedSize(800,600);

   playlist.addMedia(QMediaContent(QUrl("qrc:/resource/canon.mp3")));
   playlist.setPlaybackMode(QMediaPlaylist::Random);
   player.setPlaylist(&playlist);
   player.play();

   QFont font;
   font.setFamily("HYXueJunJ");
   font.setPixelSize(25);    //设置文字大小
  // font.setBold(true);       //设置文字粗体
   //font.setItalic(true);     //设置文字斜体

   //创建3个Button
   actByPerson = new QPushButton(QString("双人对战"),this);
   actByAi = new QPushButton(QString("人机对战"),this);
   Settings = new QPushButton(QString("游戏设置"),this);

   QString buttonstyle="QPushButton{border-radius:10px; border:2px groove rgb(205,170,125);}"
   "QPushButton:hover{background-color:rgb(245, 245, 240);}"
   "QPushButton:pressed{background-color:rgb(238, 233, 233);}";

   actByPerson->setFont(font); //设置按钮字体
   actByPerson->setFlat(true); //设置按钮透明
   actByPerson->setStyleSheet(buttonstyle); //设置按钮样式
   actByAi->setFont(font);
   actByAi->setFlat(true);
   actByAi->setStyleSheet(buttonstyle);
   Settings->setFont(font);
   Settings->setFlat(true);
   Settings->setStyleSheet(buttonstyle);

   actByPerson->setGeometry(100,180,150,50); //设置按钮坐标及大小
   actByAi->setGeometry(100,280,150,50);
   Settings->setGeometry(100,380,150,50);

   actByPerson->setAttribute(Qt::WA_DeleteOnClose);
   actByAi->setAttribute(Qt::WA_DeleteOnClose);
   Settings->setAttribute(Qt::WA_DeleteOnClose);

   settinglabel=new QLabel(this);
   backgroundmusic = new QCheckBox(settinglabel);
   chessmusic = new QCheckBox(settinglabel);
   backgroundmusic->setChecked(true);
   chessmusic->setChecked(true);
   bslider=new QSlider(Qt::Horizontal,settinglabel);
   bslider->setRange(0,100);
   cslider=new QSlider(Qt::Horizontal,settinglabel);
   cslider->setRange(0,100);

   connect(actByPerson,SIGNAL(clicked()),this,SLOT(hide()));   //信号槽连接
   connect(actByPerson,SIGNAL(clicked()),this,SLOT(startgame()));

   connect(actByAi,SIGNAL(clicked()),this,SLOT(hide()));
   connect(actByAi,SIGNAL(clicked()),this,SLOT(startAI()));

   connect(Settings,SIGNAL(clicked()),this,SLOT(settingslabel()));

}

bool index::Chess_Music=true;
QMediaPlayer* index::chessplayer=new QMediaPlayer;

void index::startgame(){
    chessboard *chessb=new chessboard();
    chessb->setindex(this);

    chessb->show();
    chessb->setAttribute(Qt::WA_DeleteOnClose);
}

void index::startAI(){
    chessboard *chessb=new chessboard();
    chessboard::AI=true;
    chessb->setindex(this);

    chessb->show();
    chessb->setAttribute(Qt::WA_DeleteOnClose);
}

void index::settingslabel(){

    actByPerson->setEnabled(false);
    actByAi->setEnabled(false);
    Settings->setEnabled(false);
    QGridLayout *gridlayout = new QGridLayout;
    QPixmap image;
    image=QPixmap(":/images/resource/setting.png");
    settinglabel->setPixmap(image);
    settinglabel->setStyleSheet("background:transparent;border-radius:10px;border:4px grey");
    settinglabel->setScaledContents(true);
    // solid grey
    settinglabel->setObjectName("游戏设置");
    gridlayout->addWidget(settinglabel, 1, 1, Qt::AlignCenter);
    settinglabel->setFixedSize(400,300);
    setLayout(gridlayout);
    settinglabel->show();
    gridlayout->deleteLater();

    backgroundmusic->setText(tr("背景音乐"));
    chessmusic->setText(tr("落子音效"));

    chessmusic->setStyleSheet("QCheckBox::indicator::checked:disabled,QRadioButton::indicator::checked:disabled,\
                               QTreeView::indicator::checked:disabled {background-color: qradialgradient(cx:0.5, cy:0.5, fx:0.25, fy:0.15, radius:0.3, stop:0 #BBBBBB, stop:1 #444444);}");
    backgroundmusic->setStyleSheet("QCheckBox::indicator::checked:disabled, QRadioButton::indicator::checked:disabled, \
                               QTreeView::indicator::checked:disabled {background-color: qradialgradient(cx:0.5, cy:0.5, fx:0.25, fy:0.15, radius:0.3, stop:0 #BBBBBB, stop:1 #444444);}");
    backgroundmusic->setVisible(true);
    chessmusic->setVisible(true);
    backgroundmusic->setGeometry(95,60,100,30);
    bslider->setValue(player.volume());
    bslider->setGeometry(195,60,100,30);
    bslider->setStyleSheet("QSlider::add-page:Horizontal{background-color: rgb(87, 97, 106);height:4px;}\
                           QSlider::sub-page:Horizontal {background-color:qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0 rgba(231,80,229, 255), stop:1 rgba(7,208,255, 255));\
                           height:4px;}\
                           QSlider::groove:Horizontal {background:transparent;height:6px;}\
                           QSlider::handle:Horizontal {height: 30px;width:8px;border-image:url(qrc:/images/resource/slider.png);margin: -8 0px;}");
    chessmusic->setGeometry(95,120,100,30);
    cslider->setStyleSheet("QSlider::add-page:Horizontal{background-color: rgb(87, 97, 106);height:4px;}\
                           QSlider::sub-page:Horizontal {background-color:qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0 rgba(231,80,229, 255), stop:1 rgba(7,208,255, 255));\
                           height:4px;}\
                           QSlider::groove:Horizontal {background:transparent;height:6px;}\
                           QSlider::handle:Horizontal {height: 30px;width:8px;border-image:url(qrc:/images/resource/slider.png);margin: -8 0px;}");
    cslider->setGeometry(195,120,100,30);
    cslider->setValue(chessplayer->volume());

    QPushButton *exit = new QPushButton(QString("退出"),settinglabel);
    QFont font;
    font.setFamily("宋体");
    font.setPixelSize(15);
    font.setBold(true);
    font.setItalic(true);
    QString buttonstyle="QPushButton{border-radius:10px; border:2px groove rgb(205,170,125);}"
    "QPushButton:hover{background-color:rgb(245, 245, 240);}"
    "QPushButton:pressed{background-color:rgb(238, 233, 233);}";
    exit->setFont(font);
    exit->setFlat(true);
    exit->setStyleSheet(buttonstyle);
    exit->setGeometry(70,200,100,40);
    exit->setVisible(true);
    exit->setAttribute(Qt::WA_DeleteOnClose);

    QPushButton *turnback = new QPushButton(QString("返回"),settinglabel);
    QFont font1;
    font1.setFamily("宋体");
    font1.setPixelSize(15);
    font1.setBold(true);
    font1.setItalic(true);
    turnback->setFont(font1);
    turnback->setFlat(true);
    turnback->setStyleSheet(buttonstyle);
    turnback->setGeometry(220,200,100,40);
    turnback->setVisible(true);
    turnback->setAttribute(Qt::WA_DeleteOnClose);

    // 连接信号槽
    connect(backgroundmusic, SIGNAL(stateChanged(int)), this, SLOT(Backgroundmusic(int)));
    connect(bslider,SIGNAL(valueChanged(int)),this,SLOT(changebmusic(int)));
    connect(chessmusic,SIGNAL(stateChanged(int)), this, SLOT(Chessmusic(int)));
    connect(cslider,SIGNAL(valueChanged(int)),this,SLOT(changecmusic(int)));
    connect(exit,SIGNAL(clicked()),this,SLOT(close()));
    connect(turnback,SIGNAL(clicked()),this,SLOT(Turnback()));
}

void index::Turnback(){

    actByPerson->setEnabled(true);
    actByAi->setEnabled(true);
    Settings->setEnabled(true);
    settinglabel->hide();
}

void index::Backgroundmusic(int state)
{
    if (state != Qt::Checked) player.pause();        // "不选中"
    else player.play();
}

void index::changebmusic(int volume){          //改变背景音乐音量
    volume = bslider->value();
    player.setVolume(volume);
}

void index::Chessmusic(int state)
{
    if (state != Qt::Checked) Chess_Music=false;        // "不选中"
    else Chess_Music=true;
}

void index::changecmusic(int volume){
    volume = cslider->value();
    chessplayer->setVolume(volume);
}

index::~index()
{

}
