#include "chessboard.h"
#include"index.h"

chessboard::chessboard(QWidget *parent):QWidget(parent)
{
    //index::chessplayer=new QMediaPlayer;
    setWindowTitle(tr("五子棋"));
    setWindowFlags(Qt::FramelessWindowHint); //无边框
    QPixmap picture(":/images/resource/chess.jpg");   //设置背景图片
    QPalette palette;
    palette.setBrush(backgroundRole(), QBrush(picture));
    setPalette(palette);
    setFixedSize(800,600);
    setMouseTracking(true);   //开启鼠标跟踪
    for(int i=0;i<15;i++)   //初始化落子情况
        for(int j=0;j<15;j++)
            chessFlag[i][j]=0;

    flag=true; gamestate=false; AiState=false;
    timer=new QTimer(this);
    stopwatch=new QLabel(this);
    stopwatch->setAttribute(Qt::WA_DeleteOnClose);

    imagelabel=new QLabel(this);
    imagelabel->setGeometry(647,40,100,100);
    imagelabel->setStyleSheet("background:transparent");
    imagelabel->setAttribute(Qt::WA_DeleteOnClose);

    QFont font;
    font.setFamily("HYXueJunJ");
    font.setPixelSize(22);   //设置文字大小

    dicing = new QPushButton(QString("定先"),this);
    QPushButton *retry = new QPushButton(QString("悔棋"),this);
    QPushButton *giveup = new QPushButton(QString("认输"),this);
    QPushButton *restart=new QPushButton(QString("重开"),this);
    QPushButton *turnback = new QPushButton(QString("返回"),this);

    QString buttonstyle="QPushButton{border-radius:10px; border:2px groove rgb(205,170,125);}"
    "QPushButton:hover{background-color:rgb(244,164,96);}"
    "QPushButton:pressed{background-color:rgb(238, 197, 145);}";

    dicing->setFont(font); //设置按钮字体
    dicing->setFlat(true); //设置按钮透明
    dicing->setStyleSheet(buttonstyle); //设置按钮样式
    retry->setFont(font);
    retry->setFlat(true);
    retry->setStyleSheet(buttonstyle);
    giveup->setFont(font);
    giveup->setFlat(true);
    giveup->setStyleSheet(buttonstyle);
    restart->setFont(font);
    restart->setFlat(true);
    restart->setStyleSheet(buttonstyle);
    turnback->setFont(font);
    turnback->setFlat(true);
    turnback->setStyleSheet(buttonstyle);

    dicing->setGeometry(630,260,100,30); //设置按钮坐标及大小
    retry->setGeometry(630,320,100,30);
    giveup->setGeometry(630,380,100,30);
    restart->setGeometry(630,440,100,30);
    turnback->setGeometry(630,500,100,30);

    dicing->setAttribute(Qt::WA_DeleteOnClose);  //delete
    retry->setAttribute(Qt::WA_DeleteOnClose);
    giveup->setAttribute(Qt::WA_DeleteOnClose);
    restart->setAttribute(Qt::WA_DeleteOnClose);
    turnback->setAttribute(Qt::WA_DeleteOnClose);

    connect(dicing,SIGNAL(clicked()),this,SLOT(Dicing()));
    connect(giveup,SIGNAL(clicked()),this,SLOT(giveup()));
    connect(retry,SIGNAL(clicked()),this,SLOT(Retry()));
    connect(restart,SIGNAL(clicked()),this,SLOT(Restart()));
    connect(turnback,SIGNAL(clicked()),this,SLOT(Turnback()));
    connect(turnback,SIGNAL(clicked()),this,SLOT(hide()));
    connect(timer, SIGNAL(timeout()), this, SLOT(Timeout()));

}

bool chessboard::AI=false;

void chessboard::Dicing(){      //扔骰子
    if(!gamestate){
        qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
        black=qrand()%6+1, white=qrand()%6+1;
        qDebug()<<black<<" "<<white<<endl;
        for(int i=1;i<=6;i++){            //选择相应骰子图片
            if(black==i) movie1=new QMovie(":/images/resource/dicing"+QString::number(i)+".gif");
            if(white==i) movie2=new QMovie(":/images/resource/dicing"+QString::number(i)+".gif");
        }

        QLabel *giflabel1=new QLabel(this);
        QLabel *giflabel2=new QLabel(this);
        giflabel1->setMovie(movie1);
        giflabel2->setMovie(movie2);
        movie1->setSpeed(300);
        movie2->setSpeed(300);
        giflabel1->setGeometry(525,140,150,120);
        giflabel2->setGeometry(620,140,150,120);
        giflabel1->setStyleSheet("background:transparent;border-width:0;border-style:outset");
        giflabel2->setStyleSheet("background:transparent;border-width:0;border-style:outset");
        movie1->start();
        movie2->start();

        giflabel1->setVisible(true);
        giflabel2->setVisible(true);

        giflabel1->setAttribute(Qt::WA_DeleteOnClose);
        giflabel2->setAttribute(Qt::WA_DeleteOnClose);

        if(black>white){
            timer->start(1000);  //计时开始
            flag=true;
            gamestate=true;
            Delay(1200);
            if(AI) {
                AiState=true;
                ByAi(row,col);
            }
            dicing->hide();
        }
        else if(black<white){
            timer->start(1000);  //计时开始
            flag=false;
            gamestate=true;
            dicing->hide();
        }
    }
}

void chessboard::Timeout(){
    QFont ft;
    ft.setFamily("HYXueJunJ");
    ft.setPixelSize(50);
     QPalette pale;

    if(second>=0&&second<10){
       pale.setColor(QPalette::WindowText,Qt::red);
       stopwatch->setText(QString::number(0)+QString::number(second));
    }
    else
        stopwatch->setText(QString::number(second));
    stopwatch->setPalette(pale);
    stopwatch->setFont(QFont(ft));
    stopwatch->setGeometry(657,100,100,100);
    stopwatch->show();
    stopwatch->setAttribute(Qt::WA_DeleteOnClose);

    if(second>0){
       second-=1;
    }
    else{
       second=30;

       flag=!flag;     //时间到，换手
       if(AI){
           ByAi(row,col);
           if(win(row,col)&&gamestate==true) ShowWin();
       }
    }
}

void chessboard::ShowWin(){        //显示输赢
    QGridLayout *gridlayout = new QGridLayout;
    winlabel=new QLabel(this);
    winlabel->setStyleSheet("background:transparent;border-radius:10px;border:4px grey");
    QPixmap image;

    if(flag==true) {
        image=QPixmap(":/images/resource/whitewin.png");
    }
    else{
        image=QPixmap(":/images/resource/blackwin.png");
    }

    winlabel->setPixmap(image);
    gridlayout->addWidget(winlabel, 1, 1, Qt::AlignCenter);
    winlabel->setFixedSize(400,600);
    setLayout(gridlayout);
    winlabel->setVisible(true);

    timer->stop();     //停止计时
    second=30;

    stopwatch->hide();
    gamestate=false;
    chessroad.clear();

    gridlayout->deleteLater();
    Delay(2000);
    winlabel->setVisible(false);
    winlabel->setAttribute(Qt::WA_DeleteOnClose);
    return;
}
void chessboard::Turnback(){
    QThread h;
    h.msleep(150);
    ind->show();
}

void chessboard::giveup(){         //认输
    if(gamestate){
        ShowWin();
        for(int i=0;i<15;i++)
            for(int j=0;j<15;j++)
                chessFlag[i][j]=0;
        gamestate=false;
        chessroad.clear();
        dicing->show();
    }
}

void chessboard::Retry(){        //悔棋
    if(gamestate&&!chessroad.isEmpty())
    {
        if(AI==true){
            int a=chessroad.top().x();
            int b=chessroad.top().y();
            qDebug()<<"a: "<<a<<" "<<"b: "<<b<<endl;
            chessFlag[a][b]=0;
            chessroad.pop();
            flag=!flag;
            Delay(500);
        }

        int x=chessroad.top().x();
        int y=chessroad.top().y();
        qDebug()<<x<<" "<<y<<endl;
        chessFlag[(x-20)/40][(y-20)/40]=0;

        chessroad.pop();
        flag=!flag;
    }
}

void chessboard::Restart(){      //重开
    for(int i=0;i<15;i++)
        for(int j=0;j<15;j++)
            chessFlag[i][j]=0;
    chessroad.clear();
    gamestate=false;
    AiState=false;
    dicing->show();
    timer->stop();  //停止计时
    second=30;
    stopwatch->hide();
}

void chessboard::setindex(index* index){
    ind=index;
}

void chessboard::paintEvent(QPaintEvent*){

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing,true);  //反走样
    painter.setPen("QColor:black"); // 设置画笔颜色
    painter.setBrush(QBrush(Qt::black,Qt::SolidPattern));//设置实心点

    QPixmap Qizib(":/images/resource/heiqi.png");      //黑棋点数提示
    QPixmap Qiziw(":/images/resource/baiqi.png");      //白棋点数提示
    painter.drawPixmap(590,190,28,28,Qizib);
    painter.drawPixmap(685,190,28,28,Qiziw);

    QLineF linef1[14],linef2[14];
    for (int i=0; i<14; ++i)   {
        linef1[i].setP1(QPointF(20, 20+i*40));
        linef1[i].setP2(QPointF(580, 20+i*40));
    }
    for (int j=0; j<14; ++j)   {
        linef2[j].setP1(QPointF(20+j*40, 20));
        linef2[j].setP2(QPointF(20+j*40, 580));
    }

    painter.drawLines(linef1,15);      //横向棋盘线
    painter.drawLines(linef2,15);      //竖向棋盘线

    painter.drawEllipse(QPoint(140,140),5,5);  //五个星点
    painter.drawEllipse(QPoint(140,460),5,5);
    painter.drawEllipse(QPoint(300,300),5,5);
    painter.drawEllipse(QPoint(460,140),5,5);
    painter.drawEllipse(QPoint(460,460),5,5);

    QPixmap image;
    if(gamestate==true){
        if(flag==true) image=QPixmap(":/images/resource/black.png");
        else image=QPixmap(":/images/resource/white.png");
    }
    imagelabel->setPixmap(image);
    if(black!=white){
        imagelabel->show();
    }

    if(row!=15&&col!=15){
        if(flag==true)    //黑棋提示
        {
            painter.setPen("QColor:white");
            painter.setBrush(QBrush(Qt::black,Qt::SolidPattern));
        }
        else    //白棋提示
        {
            painter.setPen("QColor:black");
            painter.setBrush(QBrush(Qt::white,Qt::SolidPattern));
        }
    painter.drawEllipse(QPoint(row*40+20,col*40+20),5,5);  //绘制鼠标跟随提示
    }

    for(int i=0;i<15;i++)
        for(int j=0;j<15;j++)
        {
            if(chessFlag[i][j]==1)  //下黑棋
            {
                QPixmap *qizi=new QPixmap(":/images/resource/heiqi.png");
                painter.drawPixmap(i*40+6,j*40+6,28,28,*qizi);
                delete qizi;
            }
            else if(chessFlag[i][j]==-1)   //下白棋
            {
                QPixmap *qizi=new QPixmap(":/images/resource/baiqi.png");
                painter.drawPixmap(i*40+6,j*40+6,28,28,*qizi);
                delete qizi;
            }
        }

    painter.end();
    update();
}

void chessboard::mouseMoveEvent(QMouseEvent *event)    //鼠标移动事件
{
    if(event->x()<=590&&event->x()>=10&&event->y()<=590&&event->y()>=10&&   //鼠标跟随限制范围
        abs((event->x()-20)%20)<15&&abs((event->y()-20)%20<15)){
        row=abs((event->x())/40);
        col=abs((event->y())/40);
    }
    else if(abs((event->x()-20)%20)<15&&abs((event->y()-20)%20<15))   //鼠标出棋盘时标记消失
        row=col=15;
    //qDebug()<<event->pos()<<endl;
    update();
}

void chessboard::mousePressEvent(QMouseEvent *event)  //鼠标点击事件
{
    if(event->buttons()==Qt::LeftButton&&gamestate&&!AiState)
    {

        int x=event->x(); int y=event->y();
        if(x>=10&&x<=580&&y>=10&&y<=580)    //四种落子情况
        {

            row=(x-20)/40; col=(y-20)/40;
            if(flag){
                if((x-20)%40<20&&(y-20)%40>20)   col++; //左下角落子
                else if((x-20)%40>20&&(y-20)%40<20)   row++; //右上角落子
                else if((x-20)%40>20&&(y-20)%40>20) {     //右下角角落子
                    row++;col++;
                }
            }
            else{
                if((x-20)%40<20&&(y-20)%40>20)   col++;
                else if((x-20)%40>20&&(y-20)%40<20)   row++;
                else if((x-20)%40>20&&(y-20)%40>20)  {
                    row++;col++;
                }
            }
            if(!chessFlag[row][col]){      //检查是否重复落子

                if(flag){
                    chessFlag[row][col]=1;
                }
                else{
                    chessFlag[row][col]=-1;
                }
                chessroad.push(QPoint(row*40+20,col*40+20));    //当前棋子入栈
                flag=!flag;          //非重复落子时，flag变值

                if(index::Chess_Music){
                    index::chessplayer->setMedia(QMediaContent(QUrl("qrc:/resource/chess.mp3"))); //落子音效
                    index::chessplayer->play();
                    second=30;            //计时器重置
                    Timeout();
                    timer->stop();
                    timer->start(1000);
                }

                if(AI){
                    AiState=true;
                    if(win(row,col)) ShowWin();
                    else {
                        Delay(500);       //延时落子
                        ByAi(row,col);
                    }
                    qDebug()<<row<<"  "<<col<<endl;
                }


            }
            else if(index::Chess_Music){
                index::chessplayer->setMedia(QMediaContent(QUrl("qrc:/resource/ohno.mp3"))); //重复落子音效
                index::chessplayer->play();
            }
        }
    }
    if(win(row,col)&&gamestate&&!AiState)
        ShowWin();
}


void chessboard::Delay(unsigned int msec)
{
    QTime _Timer = QTime::currentTime().addMSecs(msec);
    while( QTime::currentTime() < _Timer )
    QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}

void chessboard::calculateScore()
{

    int personNum = 0;         // 玩家连成子的个数
    int computerNum = 0;       // AI连成子的个数
    int emptyNum = 0;          // 各方向空白位的个数

    score.clear();
    for (int i = 0; i < 15; i++)
    {
       QVector<int> l;
        for (int j = 0; j < 15; j++)
            l.push_back(0);
        score.push_back(l);
    }

    for (int x = 0; x < 15; x++)
        for (int y = 0; y < 15; y++)
        {
            if (chessFlag[x][y] == 0)
            {
                //周围八个方向
                for (int i = -1; i <= 1; i++)
                    for (int j = -1; j <= 1; j++)
                    {
                        personNum = 0;
                        computerNum = 0;
                        emptyNum = 0;

                        if (!(i == 0 && j == 0))           //不考虑原位置
                        {
                            //两个方向，每个方向延伸四个

                            //对玩家白字评分
                            for (int k = 1; k <= 4; k++)
                            {
                                //玩家棋子
                                if (x + k * i >= 0 && x + k * i < 15 &&
                                    y + k * j >= 0 && y + k * j < 15 &&
                                    chessFlag[x + k * i][y + k * j] == -1)
                                {
                                    personNum++;
                                }
                                //空白
                                else if (x + k * i >= 0 && x + k * i < 15&&
                                    y + k * j >= 0 && y + k * j < 15&&
                                    chessFlag[x + k * i][y + k * j] == 0)
                                {
                                    emptyNum++;
                                    break;
                                }
                                else
                                    break;
                            }

                            for (int k = 1; k <= 4; k++)
                            {
                                //玩家棋子
                                if (x - k * i >= 0 && x - k * i < 15 &&
                                    y - k * j >= 0 && y - k * j < 15 &&
                                    chessFlag[x - k * i][y - k * j] == -1)
                                {
                                    personNum++;
                                }
                                //空白
                                else if (x - k * i >= 0 && x - k * i < 15&&
                                    y - k * j >= 0 && y - k * j < 15&&
                                    chessFlag[x - k * i][y - k * j] == 0)
                                {
                                    emptyNum++;
                                    break;
                                }
                                else
                                    break;
                            }

                            if (personNum == 1)             //杀二
                                score[x][y] += 100;
                            else if (personNum == 2)        //杀三
                            {
                                if (emptyNum == 1)
                                    score[x][y] += 100;
                                else if (emptyNum == 2)
                                    score[x][y] += 1000;
                            }
                            else if (personNum == 3)         //杀四
                            {
                                if (emptyNum == 1)
                                    score[x][y] += 1000;
                                else if (emptyNum == 2)
                                    score[x][y] += 10000;
                            }
                            else if (personNum == 4)         //杀五
                                score[x][y] += 100000;

                            emptyNum = 0;

                            //AI黑棋
                            for (int k = 1; k <= 4; k++)
                            {
                                //AI棋子
                                if (x + k * i > 0 && x + k * i < 15 &&
                                    y + k * j > 0 && y + k * j < 15 &&
                                    chessFlag[x + k * i][y + k * j] == 1)
                                {
                                    computerNum++;
                                }
                                //空白
                                else if (x + k * i > 0 && x + k * i < 15&&
                                    y + k * j>0 && y + k * j < 15&&
                                    chessFlag[x + k * i][y + k * j] == 0)
                                {
                                    emptyNum++;
                                    break;
                                }
                                else
                                    break;
                            }

                            for (int k = 1; k <= 4; k++)
                            {
                                //AI棋子
                                if (x - k * i > 0 && x - k * i < 15 &&
                                    y - k * j > 0 && y - k * j < 15 &&
                                    chessFlag[x - k * i][y - k * j] == 1)
                                {
                                    computerNum++;
                                }
                                //空白
                                else if (x - k * i > 0 && x - k * i < 15&&
                                    y - k * j>0 && y - k * j < 15&&
                                    chessFlag[x - k * i][y - k * j] == 0)
                                {
                                    emptyNum++;
                                    break;
                                }
                                else
                                    break;
                            }

                            if (computerNum == 0)            //普通下子
                                score[x][y] += 10;
                            else if (computerNum == 1)       //活二
                                score[x][y] += 200;
                            else if (computerNum == 2)
                            {
                                if (emptyNum == 1)           //死三
                                    score[x][y] += 100;
                                else if (emptyNum == 2)
                                    score[x][y] += 2000;       //活三
                            }
                            else if (computerNum == 3)
                            {
                                if (emptyNum == 1)           //死四
                                    score[x][y] += 1000;
                                else if (emptyNum == 2)
                                    score[x][y] += 20000;      //活四
                            }
                            else if (computerNum == 4)         //活五
                                score[x][y] += 200000;
                        }
                    }
            }
        }

}

void chessboard::ByAi(int &x,int &y)
{
    //if (flag != 1) return;

    if (chessroad.empty())
    {
        x = y = 7;
        chessroad.push(QPoint(x, y));
        if(flag) chessFlag[x][y]=1;
        else chessFlag[x][y]=-1;
        if(index::Chess_Music)
            index::chessplayer->play();
        AiState=false;
        flag=!flag;
        return;
    }

    calculateScore();

    int maxScore = 0;
    QVector<QPoint> maxPoints;

    for (int x = 0; x < 15; x++)
        for (int y = 0; y < 15; y++)
        {
            if (chessFlag[x][y] == 0)
            {
                if (score[x][y] > maxScore)
                {
                    maxPoints.clear();
                    maxScore = score[x][y];
                    maxPoints.push_back(QPoint(x, y));
                }
                else if (score[x][y] == maxScore)
                    maxPoints.push_back(QPoint(x, y));
            }
        }

    srand((unsigned)time(0));
    int index = rand() % maxPoints.size();

    QPoint point = maxPoints[index];
    x = point.rx();
    y = point.ry();
    chessroad.push(point);
    if(flag) chessFlag[x][y]=1;
    else chessFlag[x][y]=-1;
     if(index::Chess_Music)
         index::chessplayer->play();
    flag=!flag;
    AiState=false;
}

bool chessboard::win(int row,int col)
{
    // 横竖斜四种大情况，每种情况都根据当前落子往后遍历5个棋子，有一种符合就算赢

    for (int i = 0; i < 5; i++)      // 水平方向
    {
        // 往左5个，往右匹配4个子，20种情况
        if (col - i >0 && col - i + 4 <15 &&
            chessFlag[row][col - i] == chessFlag[row][col - i + 1] &&
            chessFlag[row][col - i] == chessFlag[row][col - i + 2] &&
            chessFlag[row][col - i] == chessFlag[row][col - i + 3] &&
            chessFlag[row][col - i] == chessFlag[row][col - i + 4])
            return true;
    }

    for (int i = 0; i < 5; i++)      // 竖直方向(上下延伸4个)
    {
        if (row - i >0 && row - i + 4 <15 &&
            chessFlag[row - i][col] == chessFlag[row - i + 1][col] &&
            chessFlag[row - i][col] == chessFlag[row - i + 2][col] &&
            chessFlag[row - i][col] == chessFlag[row - i + 3][col] &&
            chessFlag[row - i][col] == chessFlag[row - i + 4][col])
            return true;
    }

    for (int i = 0; i < 5; i++)      // 左斜方向
    {
        if (row + i <15 && row + i - 4 >0 && col - i >0 &&  col - i + 4 <15 &&
            chessFlag[row + i][col - i] == chessFlag[row + i - 1][col - i + 1] &&
            chessFlag[row + i][col - i] == chessFlag[row + i - 2][col - i + 2] &&
            chessFlag[row + i][col - i] == chessFlag[row + i - 3][col - i + 3] &&
            chessFlag[row + i][col - i] == chessFlag[row + i - 4][col - i + 4])
            return true;
    }

    for (int i = 0; i < 5; i++)     // 右斜方向
    {
        if (row - i >0 && row - i + 4 <15 &&  col - i >0 && col - i + 4 <15 &&
            chessFlag[row - i][col - i] == chessFlag[row - i + 1][col - i + 1] &&
            chessFlag[row - i][col - i] == chessFlag[row - i + 2][col - i + 2] &&
            chessFlag[row - i][col - i] == chessFlag[row - i + 3][col - i + 3] &&
            chessFlag[row - i][col - i] == chessFlag[row - i + 4][col - i + 4])
            return true;
    }
   return false;
}

chessboard::~chessboard(){

}
