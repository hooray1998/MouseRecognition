#include "mylabel.h"

#define StartNumber 20
#define PointNumber 100
#define CursorRadius 100
#define PI 3.1415

myLabel::myLabel(QWidget *parent) :
    QLabel(parent)
{
    RecordingLine = true;
    lineNumber = 0;
    setMouseTracking(true);
    Press = false;
    this->setCursor(Qt::BlankCursor);



    line.init();

    for(int i=0;i<PointNumber;i++)
    {
        allPoint[i] = QPoint(rand()%1900,rand()%1000);
    }

}



void myLabel::paintEvent(QPaintEvent *)
{
        painter.begin(this);          //进行重绘;
        int s[5] = {8,17,20,23,24};
        painter.setPen(QPen(Qt::white, s[0], Qt::SolidLine, Qt::RoundCap));    //设置画笔;

        for(int i=0;i<PointNumber;i++)
        {
            painter.setPen(QPen(Qt::white, s[0], Qt::SolidLine, Qt::RoundCap));    //设置画笔;
            if(Press&&inside[i])
            {
                painter.setPen(QPen(Qt::green, s[0], Qt::SolidLine, Qt::RoundCap));    //设置画笔;
            }
            painter.drawPoint(allPoint[i]);
        }


        int len = line.length;
        if(RecordingLine)
        {
            for(int i=1;i<len;i++)
            {
                //painter.drawLine(line.pnt[i-1],line.pnt[i]);
                painter.setPen(QPen(Qt::white, s[0], Qt::SolidLine, Qt::RoundCap));    //设置画笔;
                painter.drawPoint(line.pnt[i]);
            }
            for(int i=0;i<line.cornerLen;i++)
            {
                //painter.setPen(QPen(Qt::red, s[4], Qt::SolidLine, Qt::RoundCap));    //设置画笔;
                painter.drawPoint(line.cornerArr[i]);
            }
        }
        else
        {

            if(len>StartNumber)
            {
                //painter.drawLine(line.pnt[0],QPoint(curx,cury));
                painter.save();
                painter.translate(line.pnt[0].x(),line.pnt[0].y());

                int h = cury-line.pnt[0].y();
                int w = curx-line.pnt[0].x();

                painter.rotate(180.0*qAtan(1.0*h/w)/PI);

                painter.drawLine(-30,-10,-30,100);
                //NOTE: 改成弧线，细一点
                for(int i=0;i<insideNumber;i++)
                    painter.drawLine(CursorRadius*2+i*50,-300,CursorRadius*2+i*50,300);

                painter.restore();

            }

            if(Press)
            {
                painter.setPen(QPen(Qt::red, s[0], Qt::SolidLine, Qt::RoundCap));    //设置画笔;
                //painter.drawLine(,QPoint(curx,cury));
                painter.drawEllipse(line.pnt[0],CursorRadius,CursorRadius);
                painter.drawEllipse(QPoint(curx,cury),CursorRadius/5,CursorRadius/5);
            }
            else
            {
                painter.drawEllipse(QPoint(curx,cury),CursorRadius,CursorRadius);
            }
        }
            for(int i=0;i<line.cornerLen;i++)
            {
                painter.setPen(QPen(Qt::red, s[4], Qt::SolidLine, Qt::RoundCap));    //设置画笔;
                painter.drawPoint(line.cornerArr[i]);
            }
        painter.end();  //重绘结束;

}

void myLabel::mousePressEvent(QMouseEvent *event)
{

    if(event->button()==Qt::LeftButton)
    {
        Press = true;
        judgeInside();
        line.addPoint(event->pos());
        update();
    }
}
void myLabel::mouseMoveEvent(QMouseEvent *event)
{
    curx=event->pos().x();
    cury=event->pos().y();
    if(Press)
    {
        line.addPoint(event->pos());
    }
    update();
}
void myLabel::mouseReleaseEvent(QMouseEvent *event)
{

    Press = false;
    if(event->button()==Qt::LeftButton)
    {
            line.addPoint(event->pos());
            line.AddEnd();
            line.save();
            line.length = 0;
    }
    else if(event->button()==Qt::RightButton)
    {
        if(lineNumber>-1)
        {
            line.length = 0;
        }
    }
        update();
}

void myLabel::judgeInside()
{
    int distanceSqrt;
    insideNumber = 0;
    for(int i=0;i<PointNumber;i++)
    {
        distanceSqrt = (curx-allPoint[i].x())*(curx-allPoint[i].x()) + (cury-allPoint[i].y())*(cury-allPoint[i].y());
        if(distanceSqrt<(CursorRadius*CursorRadius))
        {
            insideNumber++;
            inside[i] = true;
        }
        else
            inside[i] = false;
    }
}
