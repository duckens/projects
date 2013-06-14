#ifndef GEOTODECART_H
#define GEOTODECART_H

#include "classes/position.h"



class coordinateCalculator{
    
    private:

        geoCoord basePosition;

    public:

        coordinateCalculator();
        coordinateCalculator(coordinateCalculator const &other);
        coordinateCalculator& operator=(coordinateCalculator const &other);
        ~coordinateCalculator();

        void setBasePosition(geoCoord newBasePosition);

        position geoToDecart(geoCoord geoPosition);
};


int calculateRouteDistanceFromReceiver(int begin_latitude[3], int begin_longitude[3], int end_latitude[3], int end_longitude[3], float &distance, float &bearing){
    int begin_latitude[3];
    int begin_longitude[3];
    int end_latitude[3];
    int end_longitude[3];

    //Вычисление ПИ
    double pi=4.0f*atan(1.0f);
    //Коэффициент перевода из градусов в радианы
    double dr=pi/180.0f;
    //Коэффициент перевода из радиан в градусы
    double rd=180.0f/pi;
    //Радиус земли
    float r_earth=6372.795;
    double latitude0=begin_latitude[0]+begin_latitude[1]*1.0/60.0+begin_latitude[2]*1.0/3600.0;
    double longitude0=begin_longitude[0]+begin_longitude[1]*1.0/60.0+begin_longitude[2]*1/3600.0;
    double latitude1=end_latitude[0]+end_latitude[1]*1.0/60.0+end_latitude[2]*1.0/3600.0;
    double longitude1=end_longitude[0]+end_longitude[1]*1.0/60.0+end_longitude[2]*1/3600.0;
    latitude0*=dr;
    longitude0*=dr;
    latitude1*=dr;
    longitude1*=dr;
    double cl0 = cos(latitude0);
    double cl1 = cos(latitude1);
    double sl0 = sin(latitude0);
    double sl1 = sin(latitude1);
    double delta = longitude1 - longitude0;
    double cdelta = cos(delta);
    double sdelta = sin(delta);
    //вычисления длины большого круга
    double y = sqrt(pow(cl1*sdelta,2)+pow(cl0*sl1-sl0*cl1*cdelta,2));
    double x = sl0*sl1+cl0*cl1*cdelta;
    double ad = atan2(y,x);
    distance = ad*r_earth;

    //вычисление начального азимута
    x = (cl0*sl1) - (sl0*cl1*cdelta);
    y = sdelta*cl1;
    double z = atan(-y/x);
    z*=rd;
    if (x < 0)
        z = z+180;

    double z2 = fmod((z+180.), 360) - 180;
    z2 = - (z2)*dr;
    double anglerad2 = z2 - ((2*pi)*floor((z2/(2*pi))) );
    bearing = anglerad2*rd;
    return 0;
}
#endif
