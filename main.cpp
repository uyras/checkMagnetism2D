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
    MPI_Init (&argc, &argv);	/* starts MPI */
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    if (size==1){
        cout<<"invalid process count"<<endl;
        MPI_Finalize();
        return 0;
    }
    cout<<rank<<": start MPI process"<<endl;

    config::Instance()->srand(time(NULL));
    config::Instance()->set2D();

    config::Instance()->srand(time(NULL)+rank*size);config::Instance()->rand();
    int exitCode = -1;

    if (rank!=0){
        int messageCount = (double)config::Instance()->rand()/(double)config::Instance()->rand_max * 10.;
        cout<<rank<<": it will sended "<<messageCount<<" messages"<<endl;
        //передаем случайное количество случайных сообщений
        for (int i=0;i<messageCount;i++){
            int message = (double)config::Instance()->rand()/(double)config::Instance()->rand_max * 900 + 100;
            MPI_Send(&message,1,MPI_INT,0,0,MPI_COMM_WORLD);
            cout<<rank<<": send code "<<message<<endl;
        }

        //выходим
        MPI_Send(&exitCode,1,MPI_INT,0,0,MPI_COMM_WORLD);
        cout<<rank<<": send code "<<exitCode<<endl;

    } else {
        int recv = 0;
        MPI_Status stat;
        int finished=0;
        do{
            MPI_Recv(&recv,1,MPI_INT,MPI_ANY_SOURCE,0,MPI_COMM_WORLD,&stat);
            cout<<rank<<": recieve code "<<recv<<" from "<<stat.MPI_SOURCE<<endl;
            if (recv==exitCode)
                finished++;
        } while (finished<(size-1));
        cout<<rank<<": "<<"program may finish"<<endl;
    }

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

