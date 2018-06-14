#ifndef CHESSBOARD_H
#define CHESSBOARD_H
#include<QWidget>
#include<QPainter>
#include<QPushButton>
#include<QDebug>
#include<QDialog>
#include<QThread>
#include<QStack>
#include<QLabel>
#include<QTime>
#include<QVector>
#include<QMouseEvent>
#include<QObject>
#include<QMediaPlayer>
#include<QLCDNumber>


class index;
class chessboard:public QWidget
{
    Q_OBJECT
public:
    friend class index;
    chessboard(QWidget *parent=0);
    bool win(int row,int col);
    void setwidget(QWidget*);
    void Delay(unsigned int msec);
    void calculateScore();
    //void ByAi(int &x,int &y);
    void ShowWin();
    ~chessboard();
    void setindex(index * index);
    QPushButton *dicing;
    QTimer *timer;
    QLabel *stopwatch;
    QLabel *winlabel;
    static bool AI;

private slots:
    void Turnback();
    void Dicing();
    void Restart();
    void giveup();
    void Retry();
    void Timeout();
    void ByAi(int &x,int &y);

private:
    int chessFlag[15][15];  //坐标数组
    int row,col,black,white;
    int second=30;
    bool flag,gamestate,AiState;
    QVector<QVector<int>> score;
    QStack<QPoint> chessroad;
    index *ind;
    QMovie *movie1,*movie2;
    QLabel *imagelabel;

protected:
    void paintEvent(QPaintEvent*);
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
};

#endif // CHESSBOARD_H
