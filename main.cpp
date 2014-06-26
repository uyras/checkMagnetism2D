#include <iostream>
#include <ctime>
#include "config.h"
#include "PartArray.h"

using namespace std;

int main()
{
    config::Instance()->srand(time(NULL));
    config::Instance()->set2D();


    PartArray *sys, *example;

    int n=3,m=5;//количество частиц в линейке
    double space = config::Instance()->partR*4.;//расстояние между центрами частиц

    example = new PartArray((double)n*space,(double)m*space,1);//размер образца 4 радиуса, или 2 диаметра

    //бросаем частицы в шахматном порядке на линию
    example->dropChain(space);
    StateMachineFree oldState(example->state);

    double dMax = space/2.-config::Instance()->partR;
    for (double d=0; d<=dMax; d+=dMax/100.){
        int anomCount=0;
        for (int j=0;j<100;j++){
            sys = example->copy();

            //разносим их до определенного уровня в любую сторону. Максимум: (расстояние между частицами/2)-радиус
            Vect dir; //направление, в которое двигать частицу.
            for(int i=0;i<sys->count();i++){
                double longitude = ((double)config::Instance()->rand()/(double)config::Instance()->rand_max) * 2. * M_PI;
                dir.x = d * cos(longitude);
                dir.y = d * sin(longitude);
                dir.z = 0;
                sys->parts[i]->pos += dir;
            }

            double EBeforeGS=sys->calcEnergy1();
            sys->setToGroundState();
            if (!(oldState==sys->state)){
                anomCount++;
            }
            delete sys;
        }
        cout<<"d = "<<d<<"; c = "<<anomCount<<endl;
    }


    cout << "finish!" << endl;
    return 0;
}

