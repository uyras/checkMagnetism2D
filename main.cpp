#include <iostream>
#include <ctime>
#include "config.h"
#include "PartArray.h"

using namespace std;

int main()
{
    config::Instance()->srand(time(NULL));
    PartArray *sys;

    for (int i=0;i<100;i++){
        int n=20;//количество частиц в линейке
        double space = config::Instance()->partR*15.;//расстояние между центрами частиц
        sys = new PartArray((double)n*space,1,1);//размер образца 4 радиуса, или 2 диаметра

        //бросаем частицы в шахматном порядке на линию
        sys->dropChain(space);
        StateMachineFree oldState(sys->state);

        //разносим их до определенного уровня влево либо вправо. Максимум: (расстояние между частицами/2)-радиус
        double d=space/2.-config::Instance()->partR;
        d/=2.;
        for(int i=0;i<sys->count();i++){
            if ((double)config::Instance()->rand() / (double)config::Instance()->rand_max>0.5){
                sys->parts[i]->pos.x+=d;
            } else {
                sys->parts[i]->pos.x-=d;
            }
        }

        sys->setToGroundState();
        if (!(oldState==sys->state))
            cout<<"found anomaly!"<<endl;

        //замеряем уровень намагниченности, при аномалиях должен быть не 0 (для четногочисла частиц) и не 1 (для нечетного числа частиц)
        cout<<sys->M().length()<<endl;
    }


    cout << "finish!" << endl;
    return 0;
}

