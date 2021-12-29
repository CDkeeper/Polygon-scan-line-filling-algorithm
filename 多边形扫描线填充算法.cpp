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
    cout << "�밴��x��y�������ʽ��������꣺" << endl;
    for (int i = 0; i < POINTNUM; i++) {
        cin >> polypoint[i].x >> polypoint[i].y;
    }
}

void saomiao() {
    glClear(GL_COLOR_BUFFER_BIT);//�Ѵ������Ϊ��ǰ��ɫ
    glBegin(GL_POINTS);
    glColor3f(0.3, 0.2, 0.2);//������ɫ�ĺ���
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
    //��ʼ��AET NET��
    AET* pAET = new AET;
    pAET->next = NULL;
    NET* pNET[1024];
    for (int i = MinY; i <= MaxY; i++) {
        pNET[i] = new NET;
        pNET[i]->next = NULL;
    }
    //��NET��
    for (int i = MinY; i <= MaxY; i++)
    {
        for (int j = 0; j < POINTNUM; j++)
        {
            if (polypoint[j].y == i)
            {
                if (polypoint[(j - 1 + POINTNUM) % POINTNUM].y > polypoint[j].y) //��ߵĵ�
                {
                    NET* p = new NET;
                    p->x = polypoint[j].x;
                    p->ymax = polypoint[(j - 1 + POINTNUM) % POINTNUM].y;
                    p->dx = (polypoint[(j - 1 + POINTNUM) % POINTNUM].x - polypoint[j].x) / (polypoint[(j - 1 + POINTNUM) % POINTNUM].y - polypoint[j].y);
                    p->next = pNET[i]->next;
                    pNET[i]->next = p;
                }

                if (polypoint[(j + 1 + POINTNUM) % POINTNUM].y > polypoint[j].y)  //�ұߵĵ�
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
    //��������AET
    for (int i = MinY; i <= MaxY; i++) {
        //�����µĽ���x������AET
        NET* p = pAET->next;
        while (p != NULL)
        {
            p->x = p->x + p->dx;
            p = p->next;
        }

        //���º���AET������
        //�ϱ�����,���ٿ��ٿռ�
        AET* tq = pAET;
        p = pAET->next;
        tq->next = NULL;
        while (p != NULL)  //��x����
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

        //(�Ľ��㷨)�ȴ�AET����ɾ��ymax==i�Ľ��
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

        //��NET�е��µ����AET,���ò��뷨��Xֵ��������
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
            p = p->next->next;//���Ƕ˵����
        }
    }
    NET* phead = NULL;
    NET* pnext = NULL;
    //�ͷŻ�Ծ�߱�
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
    glutInit(&argc, argv);//���ڵĳ�ʼ��
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);//����ıģʽ���趨
    glutInitWindowPosition(250, 250);//����λ�õ��趨
    glutInitWindowSize(500, 500);//���ڴ�С���趨
    glutCreateWindow("����ε�ɨ�����");
    glClearColor(1.0, 1.0, 1.0, 0.0);
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0.0, 500.0, 0.0, 500.0);
    glutDisplayFunc(saomiao);//���ú���
    glutMainLoop();
    return 0;
}


