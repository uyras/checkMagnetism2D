#include <iostream>
#include <ctime>
#include "config.h"
#include "PartArray.h"

using namespace std;

int main()
{
    config::Instance()->srand(time(NULL));
    config::Instance()->set2D();


    int n=3;//количество частиц в линейке
    double space = config::Instance()->partR*4.;//расстояние между центрами частиц
    int anom=0;

    for (double d=0.4;d<=space/2.-config::Instance()->partR;d+=(space/2.-config::Instance()->partR)/100.) //разносим их до определенного уровня в любую сторону. Максимум: (расстояние между частицами/2)-радиус
    {
        cout<<"; anom="<<anom<<endl;
        cout<<"d="<<d;
        anom=0;
        for (int j=0;j<10000;j++){
            PartArray *sys;

            sys = new PartArray((double)n*space,(double)n*space,1);//размер образца 4 радиуса, или 2 диаметра

            //бросаем частицы в шахматном порядке на линию
            sys->dropChain(space);
            StateMachineFree oldState(sys->state);
            sys->save("1.dat");



            Vect dir; //направление, в которое двигать частицу.
            for(int i=0;i<sys->count();i++){
                double longitude = ((double)config::Instance()->rand()/(double)config::Instance()->rand_max) * 2. * M_PI;
                dir.x = d * cos(longitude);
                dir.y = d * sin(longitude);
                dir.z = 0;
                sys->parts[i]->pos += dir;
            }


            sys->setToGroundState();
            if (!(oldState==sys->state)){
                anom++;
            }
        }
    }
    cout<<"; anom="<<anom<<endl;

    cout << "finish!" << endl;
    return 0;
}

