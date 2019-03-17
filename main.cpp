#include <bits/stdc++.h>

using namespace std;

/**
*
*   3 levels feedback scheduling algorithm
*               i. level 1 : using Round Robin algorithm
*              ii. level 2 : using Round Robin algorithm
*             iii. level 3 : using FCFS algorithm
*
*
**/

const int PROCESSES = 1000;

struct Process{
    int priority;
    int id;
    int burst_time;
    int arrival_time;

    int begin_time;
    int completion_time;

    int waiting_time;
    int turnaround_time;
    int remaining_time;
    //int response time;
};

struct Process Q1[PROCESSES] , Q2[PROCESSES] , Q3[PROCESSES],  Qt[PROCESSES];
int  i , j = 0 , d = 0 , idx = 0 , Qid = 0 , process_time = 0 , start_time = 0 , id , res = 0,
   first_index = 0 , second_index = 0 , third_index = 0 ,
   Qsize[3] = {0,0,0}, QUANTUM[2]={8,16} ;

vector<int> V1 , V2 , V3 , V , v;

/**
*          Consider Q1 of higher-priority  0
*                   Q2 of medium-priority  1
*                   Q3 of lower-priority   2
*
*          main function input : number of processes
*                                arrival time           burst time
*
*          at any arrival time for processes (ascending)
*
**/

void initialize( int processes){
    /** input burst_time and arrival_time of each process **/
    for(i=0;i<processes;++i){
        cin >> Q1[i].arrival_time>> Q1[i].burst_time;
    }
    /**initialize processes **/
    for(i = 0 ; i < processes ; ++i){
        Q1[i].id = i+1;
        Q1[i].remaining_time = Q1[i].burst_time;
        Q1[i].priority = 0;
        Q1[i].turnaround_time = 0;
        Q1[i].completion_time = 0;
    }
    Q1[0].begin_time = Q1[0].arrival_time;
    start_time = Q1[0].begin_time;
    Qsize[0] += processes;

    for(i = 0 ; i < processes ; ++i){
        Qt[i].id = i+1;
        Qt[i].begin_time = Q1[0].begin_time;
        Qt[i].arrival_time = Q1[i].arrival_time;
        Qt[i].turnaround_time = 0;
        Qt[i].waiting_time = 0;
    }

}
/**
*
*   Qid = priority of each queue and index(queue number) for each queue
*        Qid
*    Q1   0
*    Q2   1
*    Q3   2
*
*   Qsize[3] --> number of processes for each queue
*
**/

void threeLevelsFeedback(struct Process Q[] , int Qid ) {
    /** for priority 2 (third queue): FCFS algorithm **/
    if(Qid == 2){
        int i = third_index;
        while( i < Qsize[Qid]){

            Q[i].completion_time = start_time + Q[i].remaining_time;
            id = Q[i].id;
            Q[i].turnaround_time = Q[i].completion_time - Qt[id-1].arrival_time;
            Q[i].waiting_time = Q[i].turnaround_time - Q[i].burst_time;

            Qt[id-1].turnaround_time = Q[i].turnaround_time;
            Qt[id-1].waiting_time = Q[i].waiting_time;

            V3.push_back(Q[i].id);
            V3.push_back(start_time);
            V3.push_back(Q[i].completion_time);
            start_time = Q[i].completion_time;

            ++i;
            /** non-preemptive Priority scheduling between queues **/
            if( start_time > Qt[first_index].arrival_time){
                third_index = i;
                threeLevelsFeedback(Q1 , 0);
            }

        }
    }
    /** for Priority 0 or 1 (first 2 queues): Round Robin algorithm **/
    else if(Qid <= 1){
        int i;
        if(Qid == 0 && Qsize[0] > 0) {
            i = first_index;
            Q[i].begin_time = start_time;
        }
        else if(Qid == 1 && Qsize[1] > 0){
            i = second_index;
            Q[i].begin_time = start_time;
        }
        while(i < Qsize[Qid]){
          /** Round Robin 1st case **/
          if(Q[i].remaining_time <= QUANTUM[idx] ){

                if(Qid == 0){
                    Q[i].completion_time = Q[i].begin_time + Q[i].remaining_time;
                    id = Q[i].id;
                    Qt[id-1].begin_time = Q[i].begin_time;

                    Q[i].turnaround_time = Q[i].completion_time - Qt[id-1].arrival_time;
                    Q[i].waiting_time = Q[i].turnaround_time - Q[i].burst_time;
                    Q[i].remaining_time = 0;

                    id = Q[i].id;
                    Qt[id-1].turnaround_time = Q[i].turnaround_time;
                    Qt[id-1].waiting_time = Q[i].waiting_time;

                    V1.push_back(Q[i].id);
                    V1.push_back(Q[i].begin_time);
                    V1.push_back(Q[i].completion_time);

                    Q[i+1].begin_time = Q[i].completion_time;
                    start_time = Q[i].completion_time;

                    ++first_index;
                }
                if(Qid == 1){
                    Q[i].completion_time = start_time + Q[i].remaining_time;
                    id = Q[i].id;

                    Q[i].turnaround_time = Q[i].completion_time - Qt[id-1].arrival_time;
                    Q[i].waiting_time = Q[i].turnaround_time - Q[i].burst_time ;
                    Q[i].remaining_time = 0;


                    Qt[id-1].turnaround_time = Q[i].turnaround_time;
                    Qt[id-1].waiting_time = Q[i].waiting_time;

                    V2.push_back(Q[i].id);
                    V2.push_back(start_time);
                    V2.push_back(Q[i].completion_time);

                    start_time = Q[i].completion_time;

                    /** non-preemptive Priority scheduling between queues **/
                    ++second_index;
                    if(start_time > Qt[first_index].arrival_time){
                        threeLevelsFeedback(Q1 , 0);
                    }

                }
          }
          /** Round Robin 2nd case **/
          else if(Q[i].remaining_time > QUANTUM[idx]){
                if(Qid == 0 ){
                     process_time = Q[i].begin_time + QUANTUM[idx];
                     id = Q[i].id;
                     Qt[id-1].begin_time = Q[i].begin_time;

                     Q[i].remaining_time -= QUANTUM[idx];

                     V1.push_back(Q[i].id);
                     V1.push_back( Q[i].begin_time );
                     V1.push_back(process_time);

                     Q[i+1].begin_time = process_time;
                     start_time = process_time;
                     Q[i].priority += 1;

                     ++first_index;
                }
                if(Qid == 1){
                     process_time = start_time + QUANTUM[idx];
                     Q[i].remaining_time -= QUANTUM[idx];

                     V2.push_back(Q[i].id);
                     V2.push_back( start_time );
                     V2.push_back(process_time);

                     start_time = process_time;
                     Q[i].priority += 1;

                    /** non-preemptive Priority scheduling between queues **/
                     ++second_index;
                     if(start_time > Qt[first_index].arrival_time){
                        threeLevelsFeedback(Q1 , 0);
                    }
                }
                /** Assigning priority **/
                if(Q[i].priority == 1){
                    Q2[j] = Q[i];
                    ++j;
                    ++Qsize[1];
                }
                else if(Q[i].priority == 2){
                    Q3[d] = Q[i];
                    ++d;
                    ++Qsize[2];
                }
          }
          ++i;

          if(Qid == 0 && (i >= Qsize[Qid] || Qt[i].arrival_time > start_time)){
                first_index = i;
                threeLevelsFeedback(Q2 , 1);
          }
          else if(Qid == 1 &&( i >= Qsize[Qid] || Qt[i].arrival_time > start_time)){
              second_index = i;
              threeLevelsFeedback(Q3 , 2);
          }
        }
    }

}
int main()
{
    int processes_number;
    cin >> processes_number;
    initialize( processes_number);
    threeLevelsFeedback( Q1 , 0 );

    printf("First Queue processes :\nProcessID\tFrom\t\tTo\n");
    for(i = 0 ; i < V1.size()  ; i += 3){
        printf("P%d\t\t%d\t\t%d\n",V1[i],V1[i+1],V1[i+2]);
    }
    printf("Second Queue processes :\nProcessID\tFrom\t\tTo\n");
    for(i = 0 ; i < V2.size()   ; i += 3){
        printf("P%d\t\t%d\t\t%d\n",V2[i],V2[i+1],V2[i+2]);
    }
    printf("Third Queue processes :\nProcessID\tFrom\t\tTo\n");
    for(i = 0 ; i < V3.size()  ; i += 3){
        printf("P%d\t\t%d\t\t%d\n",V3[i],V3[i+1],V3[i+2]);
    }
    printf("\nProcessID\tTotal turnaround time\tWaiting time\n");
    for(i = 0 ; i < processes_number ; ++i){
        id = Qt[i].id;
        printf("P%d\t\t%d\t\t\t%d\n",id,Qt[id-1].turnaround_time,Qt[id-1].waiting_time);
    }


    return 0;
}
