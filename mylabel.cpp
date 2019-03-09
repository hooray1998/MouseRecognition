#include "mylabel.h"

#define StartNumber 20
#define UsePointNumber 300
#define PI 3.1415

myLabel::myLabel(QWidget *parent) :
    QLabel(parent)
{
    check = false;
    RecordingLine = false;
    lineNumber = 0;
    setMouseTracking(true);
    Press = false;
    this->setCursor(Qt::BlankCursor);

    CursorRadius = 50;

    curCheck = 0;
    curx = -100;
    cury = -100;

    line.init();


}
void myLabel::paintEvent(QPaintEvent *)
{
        if(!RecordingLine)
            judgeInside();
        painter.begin(this);          //进行重绘;
        int s[5] = {5,8,12,17,20};
        painter.setPen(QPen(Qt::white, s[1], Qt::SolidLine, Qt::RoundCap));    //设置画笔;

        //绘制所有的点
        for(int i=0;i<UsePointNumber;i++)
        {
            painter.setPen(QPen(Qt::white, s[1], Qt::SolidLine, Qt::RoundCap));    //设置画笔;
            if(allPoint[i].shape=="dot")
                painter.drawPoint(allPoint[i].point);
            else
            {
                painter.setPen(QPen(Qt::yellow, s[1], Qt::SolidLine, Qt::RoundCap));    //设置画笔;
                painter.setFont(QFont(NULL,25));
                painter.drawText(QRect(allPoint[i].point.x()-15,allPoint[i].point.y()-15,30,30), Qt::AlignCenter, QString("%1").arg(allPoint[i].shape));
            }
            if(Press&&allPoint[i].inside&&allPoint[i].label&&!RecordingLine)
            {
                painter.setPen(QPen(Qt::blue, 4, Qt::SolidLine, Qt::RoundCap));    //设置画笔;
                painter.setFont(QFont(NULL,15));
                painter.drawText(QRect(allPoint[i].point.x(),allPoint[i].point.y(),20,20), Qt::AlignCenter, QString("%1").arg(allPoint[i].label));
            }
        }

        //手势识别  拐点,轨迹及预测结果显示
        int len = line.length;
        //painter.setFont(QFont(NULL,30));
        //painter.drawText(QRect(700,10,500,60), Qt::AlignCenter, QString("%1").arg(line.dirString));
        //painter.setFont(QFont(NULL,90));
        //painter.drawText(QRect(400,10,300,120), Qt::AlignCenter, QString("%1").arg(line.curKey));

        if(RecordingLine)
        {
            painter.drawEllipse(allPoint[curCheck].point,20,20);
            painter.setPen(QPen(Qt::red, s[1], Qt::SolidLine, Qt::RoundCap));    //设置画笔;
            painter.drawPoint(allPoint[curCheck].point);

            for(int i=1;i<len;i++)//绘制鼠标轨迹
            {
                painter.setPen(QPen(Qt::white, s[1], Qt::SolidLine, Qt::RoundCap));    //设置画笔;
                painter.drawLine(line.pnt[i-1],line.pnt[i]);
            }
            //corner Point
            /*
            for(int i=0;i<line.cornerLen;i++)
            {
                painter.setPen(QPen(Qt::red, s[4], Qt::SolidLine, Qt::RoundCap));    //设置画笔;
                painter.drawPoint(line.cornerArr[i]);
            }
            */
            painter.setPen(QPen(Qt::white, s[1], Qt::SolidLine, Qt::RoundCap));    //设置画笔;
            painter.drawEllipse(QPoint(curx,cury),10,10);
        }
        else
        {
            if(len>StartNumber&&insideNumber)//滑动一定距离才出现
            {
                check = true;
                int x = curx-line.pnt[0].x();
                int y = cury-line.pnt[0].y();
                int moveDistance = sqrt(x*x+y*y);
                if(moveDistance>CursorRadius&&moveDistance<300)
                {
                    CursorRadius = moveDistance;
                    if(CursorRadius>80)
                        CursorRadius = 80;
                }
                painter.save();
                painter.translate(line.pnt[0].x(),line.pnt[0].y());


                int ret;
                if(x==0)
                    ret = (y>0)?90:270;
                else
                {
                    ret = 180.0*qAtan(1.0*y/x)/PI;
                    if(x<0)
                        ret += 180;
                    if(ret>360)
                        ret -= 360;
                    if(ret<0)
                        ret += 360;
                }
                painter.rotate(ret);

                //NOTE: 改成弧线，细一点
                int startAngle = -30 * 16;
                int spanAngle = 60 * 16;

                    int r,i;
                    curCheck = 0;
                    for(r=2*CursorRadius,i=0;i<insideNumber-1;i++,r+=50)
                    {
                        if(!curCheck&&moveDistance<r)
                        {
                            curCheck = i+1;
                        }
                        QRectF rectangle(-r, -r, r<<1, r<<1);
                        painter.drawArc(rectangle, startAngle, spanAngle);
                    }
                    if(!curCheck)//否则选中最远的
                        curCheck = i+1;
                    curCheck = insideList[curCheck-1];
                    painter.setFont(QFont(NULL,20));
                    for(r=2*CursorRadius,i=0;i<insideNumber-1;i++,r+=50)
                        painter.drawText(QRect(r-50,0,30,32), Qt::AlignCenter, QString("%1").arg(i+1));
                    painter.drawText(QRect(r-50,0,30,32), Qt::AlignCenter, QString("%1").arg(insideNumber));
                    painter.restore();
                    painter.drawEllipse(allPoint[curCheck].point,20,20);
            }
            else if(Press&&insideNumber)
            {
                check = true;
                curCheck = insideList[0];
                painter.drawEllipse(allPoint[curCheck].point,20,20);
                painter.setPen(QPen(Qt::red, s[1], Qt::SolidLine, Qt::RoundCap));    //设置画笔;
                if(allPoint[curCheck].shape=="dot")
                    painter.drawPoint(allPoint[curCheck].point);
                else
                {
                    painter.setPen(QPen(Qt::yellow, s[1], Qt::SolidLine, Qt::RoundCap));    //设置画笔;
                    painter.setFont(QFont(NULL,25));
                    painter.drawText(QRect(allPoint[curCheck].point.x()-15,allPoint[curCheck].point.y()-15,30,30), Qt::AlignCenter, QString("%1").arg(allPoint[curCheck].shape));
                }
            }
            else if(!Press&&insideNumber)
            {
                if(check)
                {
                    painter.drawEllipse(allPoint[curCheck].point,20,20);
                    painter.setPen(QPen(Qt::red, s[1], Qt::SolidLine, Qt::RoundCap));    //设置画笔;
                    if(allPoint[curCheck].shape=="dot")
                        painter.drawPoint(allPoint[curCheck].point);
                    else
                    {
                        painter.setPen(QPen(Qt::yellow, s[1], Qt::SolidLine, Qt::RoundCap));    //设置画笔;
                        painter.setFont(QFont(NULL,30));
                        painter.drawText(QRect(allPoint[curCheck].point.x()-15,allPoint[curCheck].point.y()-15,30,30), Qt::AlignCenter, QString("%1").arg(allPoint[curCheck].shape));
                    }
                }
            }
            else if(!Press&&!insideNumber)
            {
                check = false;
            }

            if(Press)
            {
                painter.setPen(QPen(Qt::white, s[1], Qt::SolidLine, Qt::RoundCap));    //设置画笔;
                painter.drawEllipse(line.pnt[0],CursorRadius,CursorRadius);
                painter.drawEllipse(QPoint(curx,cury),CursorRadius/15,CursorRadius/15);
            }
            else
            {
                painter.setPen(QPen(Qt::white, s[1], Qt::SolidLine, Qt::RoundCap));    //设置画笔;
                painter.drawEllipse(QPoint(curx,cury),CursorRadius,CursorRadius);
            }
        }
        painter.end();  //重绘结束;

}

void myLabel::mousePressEvent(QMouseEvent *event)
{
    if(event->button()==Qt::LeftButton)
    {
        if(!RecordingLine)
        {
            CursorRadius = 50;
            judgeInside();
        }
        Press = true;
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
            CursorRadius = 50;
            for(int i=0;i<insideNumber;i++)
            {
                allPoint[insideList[i]].inside = false;
                allPoint[insideList[i]].label = 0;
            }
            line.predect();
            line.length = 0;
            if(RecordingLine)
                allPoint[curCheck].shape = line.curKey;
            if(check)
                RecordingLine = !RecordingLine;
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
    for(int i=0;i<UsePointNumber;i++)
    {
        distanceSqrt = (line.pnt[0].x()-allPoint[i].point.x())*(line.pnt[0].x()-allPoint[i].point.x()) + (line.pnt[0].y()-allPoint[i].point.y())*(line.pnt[0].y()-allPoint[i].point.y());
        if(distanceSqrt<(CursorRadius*CursorRadius))
        {
            insideList[insideNumber] = i;
            insideNumber++;
            allPoint[i].inside = true;
        }
        else
            allPoint[i].inside = false;
    }
    int temp;
    for(int i=0;i<insideNumber;i++)
    {
        for(int j=i+1;j<insideNumber;j++)
        {
            if(computeDistance(insideList[j])<computeDistance(insideList[i]))
            {
                temp = insideList[i];
                insideList[i] = insideList[j];
                insideList[j] = temp;
            }
        }
    }
    for(int i=0;i<insideNumber;i++)
        allPoint[insideList[i]].label = i+1;

}


int myLabel::computeDistance(int index)
{
    int distance = 0;
    distance += (line.pnt[0].x()-allPoint[index].point.x())*(line.pnt[0].x()-allPoint[index].point.x());
    distance += (line.pnt[0].y()-allPoint[index].point.y())*(line.pnt[0].y()-allPoint[index].point.y());
    return distance;
}
