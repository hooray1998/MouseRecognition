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

class Line{
public:
    int length;
    QPoint pnt[10000];
    double sumValue;
    float averageValue;
    QFile *file;
    QTextStream *in;

    void init(){
            length =0;
            sumValue = 0;
            averageValue =0;
            file = new QFile("line.origin.db");
    }
    void save(){
            bool ok = file->open(QIODevice::WriteOnly|QIODevice::Text|QIODevice::Append);
            if(ok)
            {
                QTextStream out(file);
                for(int i=0;i<length;i++)
                    out<<pnt[i].x()<<" "<<pnt[i].y()<<" ";
                out<<endl;
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
    }
};
class myLabel : public QLabel
{
    Q_OBJECT
public:
    explicit myLabel(QWidget*parent = 0);

    void judgeInside();
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
