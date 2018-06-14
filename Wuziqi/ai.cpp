#include "ai.h"

void Ai::change(int x, int y)
{
    //playerFlag=1为黑棋
    if (playerFlag)
        map[x][y] = 1;
    else
        map[x][y] = 2;

    // 换手
    playerFlag = !playerFlag;
}

QPoint Ai::remove(int x, int y)
{
    if (!s.empty())
    {
        map[x][y] = 0;
        s.pop();
        // 换手
        playerFlag = !playerFlag;
        //返回上一个点
        return s.top();
    }
    else return QPoint(-1, -1);
}

void Ai::calculateScore()
{
    int personNum = 0;         // 玩家连成子的个数
    int computerNum = 0;       // AI连成子的个数
    int emptyNum = 0;          // 各方向空白位的个数

    score.clear();
    for (int i = 0; i < mapSize; i++)
    {
        vector<int> l;
        for (int j = 0; j < mapSize; j++)
            l.push_back(0);
        score.push_back(l);
    }

    for (int x = 0; x < mapSize; x++)
        for (int y = 0; y < mapSize; y++)
        {
            if (map[x][y] == 0)
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
                                if (x + k * i >= 0 && x + k * i < mapSize &&
                                    y + k * j >= 0 && y + k * j < mapSize &&
                                    map[x + k * i][y + k * j] == 2)
                                {
                                    personNum++;
                                }
                                //空白
                                else if (x + k * i >= 0 && x + k * i < mapSize&&
                                    y + k * j >= 0 && y + k * j < mapSize&&
                                    map[x + k * i][y + k * j] == 0)
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
                                if (x - k * i >= 0 && x - k * i < mapSize &&
                                    y - k * j >= 0 && y - k * j < mapSize &&
                                    map[x - k * i][y - k * j] == 2)
                                {
                                    personNum++;
                                }
                                //空白
                                else if (x - k * i >= 0 && x - k * i < mapSize&&
                                    y - k * j >= 0 && y - k * j < mapSize&&
                                    map[x - k * i][y - k * j] == 0)
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
                                if (x + k * i > 0 && x + k * i < mapSize &&
                                    y + k * j > 0 && y + k * j < mapSize &&
                                    map[x + k * i][y + k * j] == 1)
                                {
                                    computerNum++;
                                }
                                //空白
                                else if (x + k * i > 0 && x + k * i < mapSize&&
                                    y + k * j>0 && y + k * j < mapSize&&
                                    map[x + k * i][y + k * j] == 0)
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
                                if (x - k * i > 0 && x - k * i < mapSize &&
                                    y - k * j > 0 && y - k * j < mapSize &&
                                    map[x - k * i][y - k * j] == 1)
                                {
                                    computerNum++;
                                }
                                //空白
                                else if (x - k * i > 0 && x - k * i < mapSize&&
                                    y - k * j>0 && y - k * j < mapSize&&
                                    map[x - k * i][y - k * j] == 0)
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

void Ai::actionByAI(int &clickX, int &clickY)
{
    if (playerFlag != 1)return;
    else if (s.empty())
    {
        chessone->play();
        clickX = clickY = 7;
        s.push(QPoint(clickX, clickY));
        change(7, 7);
        return;
    }
    chessone->play();

    calculateScore();

    int maxScore = 0;
    vector<QPoint> maxPoints;

    for (int x = 0; x < mapSize; x++)
        for (int y = 0; y < mapSize; y++)
        {
            if (map[x][y] == 0)
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
    clickX = point.rx();
    clickY = point.ry();
    s.push(point);
    change(clickX, clickY);
}

Ai::Ai()
{

}
