#include<gl/GLUT.H>
#include<math.h>
#include<string>
#include <iostream>
using namespace std;

const int POINTNUM = 5;
typedef struct XET {
    float x;
    float dx, ymax;
    struct XET* next;
}AET, NET;

struct point {
    float x;
    float y;
    point() {}
    point(float x, float y) {
        this->x = x;
        this->y = y;
    }
}polypoint[POINTNUM] = {};

void input() {
    cout << "请按照x，y坐标的形式输入点坐标：" << endl;
    for (int i = 0; i < POINTNUM; i++) {
        cin >> polypoint[i].x >> polypoint[i].y;
    }
}

void saomiao() {
    glClear(GL_COLOR_BUFFER_BIT);//把窗口清除为当前颜色
    glBegin(GL_POINTS);
    glColor3f(0.3, 0.2, 0.2);//设置颜色的函数
    int MaxY = 0;
    int MinY = 1000;
    for (int i = 0; i < POINTNUM; i++) {
        if (polypoint[i].y > MaxY) {
            MaxY = polypoint[i].y;
        }
        if (polypoint[i].y < MinY) {
            MinY = polypoint[i].y;
        }
    }
    //初始化AET NET表
    AET* pAET = new AET;
    pAET->next = NULL;
    NET* pNET[1024];
    for (int i = MinY; i <= MaxY; i++) {
        pNET[i] = new NET;
        pNET[i]->next = NULL;
    }
    //填NET表
    for (int i = MinY; i <= MaxY; i++)
    {
        for (int j = 0; j < POINTNUM; j++)
        {
            if (polypoint[j].y == i)
            {
                if (polypoint[(j - 1 + POINTNUM) % POINTNUM].y > polypoint[j].y) //左边的点
                {
                    NET* p = new NET;
                    p->x = polypoint[j].x;
                    p->ymax = polypoint[(j - 1 + POINTNUM) % POINTNUM].y;
                    p->dx = (polypoint[(j - 1 + POINTNUM) % POINTNUM].x - polypoint[j].x) / (polypoint[(j - 1 + POINTNUM) % POINTNUM].y - polypoint[j].y);
                    p->next = pNET[i]->next;
                    pNET[i]->next = p;
                }

                if (polypoint[(j + 1 + POINTNUM) % POINTNUM].y > polypoint[j].y)  //右边的点
                {
                    NET* p = new NET;
                    p->x = polypoint[j].x;
                    p->ymax = polypoint[(j + 1 + POINTNUM) % POINTNUM].y;
                    p->dx = (polypoint[(j + 1 + POINTNUM) % POINTNUM].x - polypoint[j].x) / (polypoint[(j + 1 + POINTNUM) % POINTNUM].y - polypoint[j].y);
                    p->next = pNET[i]->next;
                    pNET[i]->next = p;
                }
            }
        }
    }
    //建立更新AET
    for (int i = MinY; i <= MaxY; i++) {
        //计算新的交点x，更新AET
        NET* p = pAET->next;
        while (p != NULL)
        {
            p->x = p->x + p->dx;
            p = p->next;
        }

        //更新后新AET先排序
        //断表排序,不再开辟空间
        AET* tq = pAET;
        p = pAET->next;
        tq->next = NULL;
        while (p != NULL)  //按x排序
        {
            while (tq->next != NULL && p->x >= tq->next->x)
            {
                tq = tq->next;
            }

            NET* s = p->next;
            p->next = tq->next;
            tq->next = p;
            p = s;
            tq = pAET;
        }

        //(改进算法)先从AET表中删除ymax==i的结点
        AET* q = pAET;
        p = q->next;
        while (p != NULL)
        {
            if (p->ymax == i)
            {
                q->next = p->next;
                delete p;
                p = q->next;
            }
            else
            {
                q = q->next;
                p = q->next;
            }
        }

        //将NET中的新点加入AET,并用插入法按X值递增排序
        p = pNET[i]->next;
        q = pAET;
        while (p != NULL)
        {
            while (q->next != NULL && p->x >= q->next->x)
            {
                q = q->next;
            }

            NET* s = p->next;
            p->next = q->next;
            q->next = p;
            p = s;
            q = pAET;
        }
        p = pAET->next;
        while (p && p->next)
        {
            for (float j = p->x; j <= p->next->x; j++)
                glVertex2f(j, i);
            //pDC->SetPixel(static_cast<int>(j), i, RGB(255, 0, 0));
            p = p->next->next;//考虑端点情况
        }
    }
    NET* phead = NULL;
    NET* pnext = NULL;
    //释放活跃边表
    phead = pAET;
    while (phead != NULL)
    {
        pnext = phead->next;
        delete phead;
        phead = pnext;
    }
    glEnd();
    glFlush();
}

int main(int argc, char** argv)
{
    input();
    glutInit(&argc, argv);//窗口的初始化
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);//窗口谋模式的设定
    glutInitWindowPosition(250, 250);//窗口位置的设定
    glutInitWindowSize(500, 500);//窗口大小的设定
    glutCreateWindow("多边形的扫描填充");
    glClearColor(1.0, 1.0, 1.0, 0.0);
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0.0, 500.0, 0.0, 500.0);
    glutDisplayFunc(saomiao);//调用函数
    glutMainLoop();
    return 0;
}


