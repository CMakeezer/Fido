#include "Robby.h"

Robby::Robby(int width, int height):
sf::RectangleShape(sf::Vector2f(width,height)) {
    setFillColor(sf::Color(0, 0, 0));
    setPosition(500,400);
    setOrigin(width/2,height/2);
    setRotation(90);
    lastSpeed=0;
}

// creds to http://chess.eecs.berkeley.edu/eecs149/documentation/differentialDrive.pdf
double Robby::go(int motLeft,int motRight,double speed) {
    double mLeft = -(((double)motLeft)/200)*speed;
    double mRight = -(((double)motRight)/200)*speed;
    
    double xprime,yprime,rprime;
    double length = getSize().x;
    double time = moveClock.restart().asMilliseconds();
    
    double rX = getPosition().x;
    double rY = getPosition().y;
    double rTheta = getRotation()*0.0174532925;
    double omega = (mRight-mLeft)/length;

    if (mLeft == mRight) {
        xprime = rX + mLeft*cos(rTheta)*time;
        yprime = rY + mLeft*sin(rTheta)*time;
        rprime = rTheta;
    } else {
        double radius = (length/2)*(mLeft+mRight)/(mRight-mLeft);
        
        double iccX = rX - radius*sin(rTheta);
        double iccY = rY + radius*cos(rTheta);
        
        xprime = iccX + (cos(omega*time)*(rX-iccX)+
                         -sin(omega*time)*(rY-iccY));
        yprime = iccY + (sin(omega*time)*(rX-iccX)+
                         cos(omega*time)*(rY-iccY));
        rprime = rTheta + omega*time;
    }
    
    setPosition(xprime,yprime);
    setRotation(rprime*57.2957795);
    
    m1Last = motLeft;
    m2Last = motRight;
    
    return omega*10000;
}

double Robby::goAccel(int motLeft,int motRight) {
    double lastSpeed = speed;
    double aChange = abs(m1Last-motLeft)+abs(m2Last-motRight);
    if(!(m1Last==0 && m2Last==0)) lastSpeed -= .0015*aChange;
    if (motLeft==0 && motRight==0) lastSpeed=0;
    if (lastSpeed < 1) lastSpeed += .05;
    double gyro = go(motLeft,motRight,lastSpeed);
    m1Last = motLeft; m2Last = motRight;
    
    double delta = 100*(lastSpeed-speed);
    if (motLeft<0&&motRight<0) delta *= -1;
    //std::cout<<"delta: "<<delta<<"\n";
    speed = lastSpeed;
    
    return delta;
}