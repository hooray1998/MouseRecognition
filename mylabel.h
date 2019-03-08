#ifndef MYLABEL_H
#define MYLABEL_H

#include <QTimer>
#include <QLabel>
#include <QPainter>
#include <QPoint>
#include <QtDebug>
#include <QMouseEvent>
#include <QDebug>
#include <QFile>
#include <QTextStream>
#include <QPainter>
#include <QtMath>
#include <QFile>
#include <QTextStream>

#define QDBG qDebug()
#define MAXCornerLen 100
#define NoiseSize 10    //十分之一的长或宽

class Line{
public:
    QPoint pnt[10000];
    int length;
    char curKey;

    QPoint cornerArr[MAXCornerLen];
    int cornerLen;
    QPoint preCornerPoint;
    QPoint preDir;

    QFile *file;
    QTextStream *in;

    void init(){
            length =0;
            cornerLen = 0;
            for(int i=0;i<MAXCornerLen;i++)
            {
                cornerArr[i].setX(0);
                cornerArr[i].setY(0);
            }
            file = new QFile("line.origin.db");
    }
    void save(){
            bool ok = file->open(QIODevice::WriteOnly|QIODevice::Text|QIODevice::Append);
            if(ok)
            {
                if(cornerLen>10)
                    return;
                QTextStream out(file);
                for(int i=0;i<cornerLen;i++)
                    out<<cornerArr[i].x()-cornerArr[0].x()<<"\t"<<cornerArr[i].y()-cornerArr[0].y()<<"\t";
                for(int i=0;i<(10-cornerLen);i++)
                    out<<"0\t0\t";
                out<<curKey<<endl;
                file->close();
            }
            else
            {
                QDBG<<"write file  error.";
            }

    }
    void addPoint(QPoint p){
        if(length>=9995) return ;
        pnt[length]=p;
        length++;
        if(length>2)
        {
            QPoint curDir = p - preCornerPoint;
            //if((curDir.x()*preDir.x())<0||(curDir.y()*preDir.y())<0)

            bool corner = false;
            if(curDir.x()>=0&&preDir.x()<0)
            {
                corner = true;
            }
            else if(curDir.x()<0&&preDir.x()>=0)
            {
                corner = true;
            }
            else if(curDir.y()>=0&&preDir.y()<0)
            {
                corner = true;
            }
            else if(curDir.y()<0&&preDir.y()>=0)
            {
                corner = true;
            }
            if(corner)
            {
                cornerArr[cornerLen++] = pnt[length-2];
            }
            preCornerPoint = p;
            preDir = curDir;
        }
        else if(length==1)
        {
            preCornerPoint = p;
            cornerLen = 0;
        }
        else if(length==2)
        {
            preDir = p - preCornerPoint;
            cornerArr[cornerLen++] = pnt[length-2];
        }
    }
    void AddEnd(){

        QDBG<<"start"<<cornerLen<<endl;
        int y_max=-1,y_min=88888;
        int x_max=-1,x_min=88888;

        preCornerPoint = pnt[0];
        bool valid[MAXCornerLen];
        for(int i=0;i<cornerLen;i++)
        {
            if(cornerArr[i].x()>x_max)
                x_max = cornerArr[i].x();
            else if(cornerArr[i].x()<x_min)
                x_min = cornerArr[i].x();
            if(cornerArr[i].y()>y_max)
                y_max = cornerArr[i].y();
            else if(cornerArr[i].y()<y_min)
                y_min = cornerArr[i].y();
        }
        //int unitx = (x_max-x_min)/NoiseSize;
        //int unity = (y_max-y_min)/NoiseSize;
        int unitx = 30;
        int unity = 30;
        cornerArr[cornerLen++] = pnt[length-1];
        //去噪,记录噪点
        QPoint temp;
        for(int i=1;i<cornerLen;i++)
        {
            temp = QPoint(cornerArr[i]-preCornerPoint);
            if(abs(temp.x())<unitx&&abs(temp.y())<unity)
                valid[i] = false;
            else
                valid[i] = true;
            preCornerPoint = cornerArr[i];
        }
        //删除噪点
        int oldLen = cornerLen;
        cornerLen = 1;
        for(int i=1;i<oldLen;i++)
        {
            if(valid[i])
            {
                cornerArr[cornerLen++] = cornerArr[i];
            }
            else
            {
                cornerArr[i].setX(0);
                cornerArr[i].setY(0);
            }
        }
        QDBG<<"end"<<cornerLen<<endl;
    }
};
class myLabel : public QLabel
{
    Q_OBJECT
public:
    explicit myLabel(QWidget*parent = 0);

    void judgeInside();
    void setCurClass(char c){
        line.curKey = c;
    }
private:
    QPainter painter;
    bool Press;
    Line line;
    int lineNumber;
    int curx,cury;
    QPoint allPoint[1000];
    bool inside[1000];
    int insideNumber;

    bool RecordingLine;
protected:

public slots:
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent*event);
};

#endif // MYLABEL_H
