#include <bits/stdc++.h>
#include <algorithm>
#include <iomanip>
#include <string.h>
using namespace std;

struct scheduling{

    int pid;
    int arriv_time;
    int burst_time;
    int start_time;
    int comp_time;
    int tat;
    int wait_time;
    int resp_time;

};

bool solve1(scheduling p1, scheduling p2)
{
    return p1.arriv_time < p2.arriv_time;
}

bool solve2(scheduling p1, scheduling p2)
{
    return p1.pid < p2.pid;
}

bool solve3(scheduling p1, scheduling p2)
{
    return p1.arriv_time < p2.arriv_time;
}

bool solve4(scheduling p1, scheduling p2)
{
    return p1.pid < p2.pid;
}


int main() {


    struct scheduling p[20];
    float avg_turnaround_time;
    float avg_waiting_time;
    float avg_response_time;
    int total_turnaround_time = 0;
    int total_waiting_time = 0;
    int total_response_time = 0;
    int total_idle_time = 0;
    float throughput;
    int burst_remaining[20];
    int is_completed[20];
    memset(is_completed,0,sizeof(is_completed));

    int n=0;



    cout << setprecision(2) << fixed;

    ifstream fil("os.txt");

    if(fil.is_open()){


        int k1,k2,c=0;

        int i=0,j=0;

        while(fil>>k1>>k2){
                p[i].arriv_time=k1;
                p[i].pid=i+1;
                p[i].burst_time=k2;
                burst_remaining[i] = p[i].burst_time;
                i++;
                n++;
        }

        cout<<"\nData saved successfully in file named os.txt\n\n";
    }

    int choice;

    cout<<"\n\n\nFind average Turn Around Time and average Waiting Time for:\n";

    cout<<"1.First Come First Serve Scheduling\n";
    cout<<"2.Shortest Remaining Time First Scheduling\n";
    cout<<"3.Shortest Job First Scheduling\n";
    cout<<"4.Round Robin Scheduling\n";

    cin>>choice;
    cout<<endl<<endl;

    switch(choice){

      case 1:{

      sort(p,p+n,solve1);

      int m=0;


      cout<<"------------------GANTT CHART---------------\n";

      for(int i = 0; i < n; i++) {

        p[i].start_time = (i == 0)?p[i].arriv_time:max(p[i-1].comp_time,p[i].arriv_time);
        p[i].comp_time = p[i].start_time + p[i].burst_time;
        p[i].tat = p[i].comp_time - p[i].arriv_time;
        p[i].wait_time = p[i].tat - p[i].burst_time;
        p[i].resp_time = p[i].start_time - p[i].arriv_time;

        total_turnaround_time += p[i].tat;
        total_waiting_time += p[i].wait_time;
        total_response_time += p[i].resp_time;
        total_idle_time += (i == 0)?(p[i].arriv_time):(p[i].start_time - p[i-1].comp_time);

        cout<<"       P"<<p[i].pid<<" ";

    }


    cout<<"\n   0 ";

    for(int i = 0; i < n; i++) {

        m+=p[i].burst_time;

        cout<<"       "<<m<<" ";
    }

    cout<<"\n----------------------------------------------\n";

    avg_turnaround_time = (double) total_turnaround_time / n;
    avg_waiting_time = (double)total_waiting_time / n;
    avg_response_time = (double) total_response_time / n;
    throughput = (double)(n) / (p[n-1].comp_time - p[0].arriv_time);

    sort(p,p+n,solve2);

    cout<<endl;
    cout<<"\nFirst Come First Serve Scheduling:\n\n";
    cout<<"  P_ID\t"<<"AT\t"<<"BT\t"<<"CT\t"<<"TAT\t"<<"WT\t"<<"RT\t"<<endl;
    cout<<"--------------------------------------------------------\n";

    for(int i = 0; i < n; i++) {
        cout<<"  "<<p[i].pid<<"\t"<<p[i].arriv_time<<"\t"<<p[i].burst_time<<"\t"<<p[i].start_time<<"\t"<<p[i].comp_time<<"\t"<<p[i].tat<<"\t"<<p[i].wait_time<<"\t"<<p[i].resp_time<<"\t"<<"\n"<<endl;
    }
    cout<<"Average Turnaround Time = "<<avg_turnaround_time<<endl;
    cout<<"Average Waiting Time = "<<avg_waiting_time<<endl;
    cout<<"Average Response Time = "<<avg_response_time<<endl;
    cout<<"Throughput = "<<throughput<<" process/unit time"<<endl;

    break;

    }

    case 2:{

    int current_time = 0;
    int completed = 0;
    int prev = 0,x=0;

    int ff[n];

    int ss=0;

    cout<<"------------------GANTT CHART---------------\n";

    while(completed != n) {
        int idx = -1;
        int mn = INT_MAX;
        for(int i = 0; i < n; i++) {
            if(p[i].arriv_time <= current_time && is_completed[i] == 0) {
                if(burst_remaining[i] < mn) {
                    mn = burst_remaining[i];
                    idx = i;
                }
                if(burst_remaining[i] == mn) {
                    if(p[i].arriv_time < p[idx].arriv_time) {
                        mn = burst_remaining[i];
                        idx = i;
                    }
                }
            }
        }

        if(idx != -1) {
            if(burst_remaining[idx] == p[idx].burst_time) {
                p[idx].start_time = current_time;
                total_idle_time += p[idx].start_time - prev;
            }
            burst_remaining[idx] -= 1;
            current_time++;
            prev = current_time;

            if(burst_remaining[idx] == 0) {
                p[idx].comp_time = current_time;
                p[idx].tat = p[idx].comp_time - p[idx].arriv_time;
                p[idx].wait_time = p[idx].tat - p[idx].burst_time;
                p[idx].resp_time = p[idx].start_time - p[idx].arriv_time;

                total_turnaround_time += p[idx].tat;
                total_waiting_time += p[idx].wait_time;
                total_response_time += p[idx].resp_time;

                is_completed[idx] = 1;
                completed++;
            }
        }
        else {
             current_time++;
        }

        cout<<"  P"<<p[idx].pid<<" |";
        ss++;
    }

    cout<<endl;

    cout<<"\n----------------------------------------------\n";

    int min_arrival_time = INT_MAX;
    int max_completion_time = -1;

    for(int i = 0; i < n; i++){
        min_arrival_time = min(min_arrival_time,p[i].arriv_time);
        max_completion_time = max(max_completion_time,p[i].comp_time);
    }

    avg_turnaround_time = (double) total_turnaround_time / n;
    avg_waiting_time = (double) total_waiting_time / n;
    avg_response_time = (double) total_response_time / n;
    throughput = (double)(n) / (max_completion_time - min_arrival_time);

    cout<<endl<<endl;
    cout<<"\nShortest Remaining Time First Scheduling:\n\n";

    cout<<"  P_ID\t"<<"AT\t"<<"BT\t"<<"CT\t"<<"TAT\t"<<"WT\t"<<"RT\t"<<endl;
    cout<<"--------------------------------------------------------\n";

    for(int i = 0; i < n; i++) {
         cout<<"  "<<p[i].pid<<"\t"<<p[i].arriv_time<<"\t"<<p[i].burst_time<<"\t"<<p[i].comp_time<<"\t"<<p[i].tat<<"\t"<<p[i].wait_time<<"\t"<<p[i].resp_time<<"\t"<<"\n"<<endl;
    }
    cout<<"Average Turnaround Time = "<<avg_turnaround_time<<endl;
    cout<<"Average Waiting Time = "<<avg_waiting_time<<endl;
    cout<<"Average Response Time = "<<avg_response_time<<endl;
    cout<<"Throughput = "<<throughput<<" process/unit time"<<endl;

    break;

   }

    case 3:{

    int current_time = 0;
    int completed = 0;
    int prev = 0,x=0;

    int ff[n];

    int ss=0;

    cout<<"------------------GANTT CHART---------------\n";

    while(completed != n) {
        int idx = -1;
        int mn = INT_MAX;
        for(int i = 0; i < n; i++) {
            if(p[i].arriv_time <= current_time && is_completed[i] == 0) {
                if(p[i].burst_time < mn) {
                    mn = p[i].burst_time;
                    idx = i;
                }
                if(p[i].burst_time == mn) {
                    if(p[i].arriv_time < p[idx].arriv_time) {
                        mn = p[i].burst_time;
                        idx = i;
                    }
                }
            }

        }
        if(idx != -1) {
            p[idx].start_time = current_time;
            p[idx].comp_time = p[idx].start_time + p[idx].burst_time;
            p[idx].tat = p[idx].comp_time - p[idx].arriv_time;
            p[idx].wait_time = p[idx].tat - p[idx].burst_time;
            p[idx].resp_time = p[idx].start_time - p[idx].arriv_time;

            total_turnaround_time += p[idx].tat;
            total_waiting_time += p[idx].wait_time;
            total_response_time += p[idx].resp_time;
            total_idle_time += p[idx].start_time - prev;

            is_completed[idx] = 1;
            completed++;
            current_time = p[idx].comp_time;
            prev = current_time;
        }
        else {
            current_time++;
        }


        ss+=p[idx].burst_time;
        ff[x]=ss;
        x++;
        cout<<"       P"<<p[idx].pid;

    }

    cout<<endl;

    cout<<"    0";

    for(int y=0;y<n;y++){
        cout<<"       "<<ff[y];
    }

    cout<<"\n----------------------------------------------\n";
    int min_arrival_time = INT_MAX;
    int max_completion_time = -1;

    for(int i = 0; i < n; i++) {
        min_arrival_time = min(min_arrival_time,p[i].arriv_time);
        max_completion_time = max(max_completion_time,p[i].comp_time);
    }

    avg_turnaround_time = (double) total_turnaround_time / n;
    avg_waiting_time =(double)total_waiting_time / n;
    avg_response_time = (double)total_response_time / n;
    throughput = (double)(n) / (max_completion_time - min_arrival_time);

    cout<<endl<<endl;
    cout<<"\nShortest Job First Scheduling:\n\n";

    cout<<"  P_ID\t"<<"AT\t"<<"BT\t"<<"CT\t"<<"TAT\t"<<"WT\t"<<"RT\t"<<endl;
    cout<<"--------------------------------------------------------\n";

    for(int i = 0; i < n; i++) {
         cout<<"  "<<p[i].pid<<"\t"<<p[i].arriv_time<<"\t"<<p[i].burst_time<<"\t"<<p[i].comp_time<<"\t"<<p[i].tat<<"\t"<<p[i].wait_time<<"\t"<<p[i].resp_time<<"\t"<<"\n"<<endl;
    }
    cout<<"Average Turnaround Time = "<<avg_turnaround_time<<endl;
    cout<<"Average Waiting Time = "<<avg_waiting_time<<endl;
    cout<<"Average Response Time = "<<avg_response_time<<endl;
    cout<<"Throughput = "<<throughput<<" process/unit time"<<endl;

    break;

    }
    case 4:{

    sort(p,p+n,solve3);
    int idx;
    int tq;
    cout<<"Enter time quanta for Round Robin\n";
    cin>>tq;

    queue<int> q;
    int current_time = 0;
    q.push(0);
    int completed = 0;
    int mark[20];
    memset(mark,0,sizeof(mark));
    mark[0] = 1;

    int arr[n]={0};

    cout<<"------------------GANTT CHART---------------\n";

    while(completed != n) {

        idx = q.front();
        q.pop();


        if(burst_remaining[idx] == p[idx].burst_time) {
            p[idx].start_time = max(current_time,p[idx].arriv_time);
            total_idle_time += p[idx].start_time - current_time;
            current_time = p[idx].start_time;
        }
        if(burst_remaining[idx]-tq > 0) {
            burst_remaining[idx] -= tq;
            current_time += tq;
        }
        else {
            current_time += burst_remaining[idx];
            burst_remaining[idx] = 0;
            completed++;

            p[idx].comp_time = current_time;
            p[idx].tat = p[idx].comp_time - p[idx].arriv_time;
            p[idx].wait_time = p[idx].tat - p[idx].burst_time;
            p[idx].resp_time = p[idx].start_time - p[idx].arriv_time;

            total_turnaround_time += p[idx].tat;
            total_waiting_time += p[idx].wait_time ;
            total_response_time += p[idx].resp_time;

        }



        for(int i = 1; i < n; i++) {
            if(burst_remaining[i] > 0 && p[i].arriv_time <= current_time && mark[i] == 0) {
                q.push(i);
                mark[i] = 1;
            }
        }
        if(burst_remaining[idx] > 0) {
            q.push(idx);
        }

        if(q.empty()) {
            for(int i = 1; i < n; i++) {
                if(burst_remaining[i] > 0) {
                    q.push(i);
                    mark[i] = 1;
                    break;
                }
            }
        }

       int kp=burst_remaining[idx];

       if(kp<=tq){
         while(kp--){
           cout<<" P"<<p[idx].pid<<"|";
         }
       }else{
         int kk=tq;

         while(kk--){
            cout<<" P"<<p[idx].pid<<"|";

         }

       }


    }


  cout<<"\n----------------------------------------------\n";

    avg_turnaround_time = (double) total_turnaround_time / n;
    avg_waiting_time = (double) total_waiting_time / n;
    avg_response_time = (double) total_response_time / n;
    throughput = (double)(n) / (p[n-1].comp_time - p[0].arriv_time);

    sort(p,p+n,solve4);

    cout<<endl;

    cout<<"Round Robin Scheduling:\n\n";
    cout<<"  P_ID\t"<<"AT\t"<<"BT\t"<<"CT\t"<<"TAT\t"<<"WT\t"<<"RT\t"<<endl;
    cout<<"--------------------------------------------------------\n";
    for(int i = 0; i < n; i++) {
        cout<<"  "<<p[i].pid<<"\t"<<p[i].arriv_time<<"\t"<<p[i].burst_time<<"\t"<<p[i].comp_time<<"\t"<<p[i].tat<<"\t"<<p[i].wait_time<<"\t"<<p[i].resp_time<<"\t"<<"\n"<<endl;
    }


    cout<<"Average Turnaround Time = "<<avg_turnaround_time<<endl;
    cout<<"Average Waiting Time = "<<avg_waiting_time<<endl;
    cout<<"Average Response Time = "<<avg_response_time<<endl;
    cout<<"Throughput = "<<throughput<<" process/unit time"<<endl;

    break;

    }

   }

}
