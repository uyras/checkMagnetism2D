#include <iostream>
#include <ctime>
#include "config.h"
#include "PartArray.h"
#include "mpi.h"

using namespace std;

void moveSystemPosRandomly(PartArray* sys, double d){
    Vect dir; //направление, в которое двигать частицу.
    for(int i=0;i<sys->count();i++){
        double longitude = ((double)config::Instance()->rand()/(double)config::Instance()->rand_max) * 2. * M_PI;
        dir.x = d * cos(longitude);
        dir.y = d * sin(longitude);
        dir.z = 0;
        sys->parts[i]->pos += dir;
    }
}

void moveSystemMRandomly(PartArray* sys, double fi){
    double side = 1.;
    double oldFi;

    for(int i=0;i<sys->count();i++){
        double oldLen = sys->parts[i]->m.length();
        if ((double)config::Instance()->rand()/(double)config::Instance()->rand_max>0.5)
            side = -1.;
        else
            side = 1.;
        oldFi = sys->parts[i]->m.angle();
        double longitude = oldFi+(fi*side);
        sys->parts[i]->m.x = oldLen * cos(longitude);
        sys->parts[i]->m.y = oldLen * sin(longitude);
        sys->parts[i]->m.z = 0;
    }
}

int main(int argc, char** argv)
{
    config::Instance()->srand(time(NULL));
    config::Instance()->set2D();
    int rank, size;

    MPI_Init (&argc, &argv);	/* starts MPI */
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    if (size==1 || (size-1)%100!=0){
        cout<<"invalid process count"<<endl;
        return;
    }

    config::Instance()->srand(time(NULL)+rank);

    int a[100];
    MPI_Win window;
    MPI_Win_create(*a,sizeof(int)*100,sizeof(int),MPI_INFO_NULL,MPI_COMM_WORLD,*win);

    if (rank==0){
        int i=100;
        MPI_Put(*i,1,MPI_INT,)
    } else {

    }

    MPI_Win_free(*window);
    /*PartArray *sys, *example;

    int n=3,m=5;//количество частиц в линейке
    double space = config::Instance()->partR*4.;//расстояние между центрами частиц

    example = new PartArray((double)n*space,(double)m*space,1);//размер образца 4 радиуса, или 2 диаметра

    //бросаем частицы в шахматном порядке на линию
    example->dropChain(space);
    StateMachineFree oldState(example->state);

    //в первом потоке распределяем по подпроцессам задачи. В задаче2 параметра: d и fi.
    if (rank==0){
        for (double d=0.; d<=dMax; d+=dMax/100.){
            MPI_
        }
    }

    double dMax = space/2.-config::Instance()->partR;
    for (double d=0.; d<=dMax; d+=dMax/100.){
        int anomCount=0;
        for (int j=0;j<100;j++){
            sys = example->copy();

            moveSystemMRandomly(sys,0.3);

            sys->setToGroundState();
            if (!(oldState==sys->state)){
                anomCount++;
            }
            delete sys;
        }
        cout<<"d = "<<d<<"; c = "<<anomCount<<endl;
    }

    cout << "finish!" << endl;*/
    MPI_Finalize();
    return 0;
}

