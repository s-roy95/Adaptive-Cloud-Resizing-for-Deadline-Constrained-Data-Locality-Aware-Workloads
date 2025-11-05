#include <iostream>
#include<cstdlib>
#include<vector>
#include<list>
#include<algorithm>
#include<fstream>
#include<iomanip>
#include<string>
#include<unordered_map>
#include <bits/stdc++.h>

using namespace std;

#define num_job 2146
#define num_chunk 15000
#define min_chunk 150
#define max_chunk 10
#define capacity 200
#define ddline 5
#define CORE 50
#define no_of_times 5 // no. of iterations for saturation



/// here 1 task requires only 1 chunk . hence ( total_tasks == total_chunks)

//ofstream of("trace.txt", ios::out);

class ds{

 public:

    int core=CORE;
    vector<int>storage;
    //vector<int>ds_flag(storage.size(),1);
    //int exec_tasks[2][2];

};

class job{

   public:
       int id;
       int num_task;
       int art;
       int dl;
       vector<int>job_task;

};
map<int,vector<float>>chunk_alpha;
vector<int>ex_time(num_job,0);
vector<int>ex_time_1(num_job,0);
vector<int>ex_time_chunk;
vector<pair<int,int>>temp;
vector<vector<int>>table;
vector<job>job_list;
vector<job>job_list_1;
vector<job>job_list_2;
vector<job>job_list_3;
vector<float>alpha_1;
vector<float>alpha_2;
vector<float>alpha_3;
map<int,int>mp;
map<int,int>mp_1;
map<int,int>mp_2;
map<int,int>mp_3;
map<long int,int>google;
vector<ds>ds_list;
vector<ds>ds_list_1;
vector<ds>ds_list_2;
vector<ds>ds_list_3;
vector<int>Chunk;
vector<int>case1;
vector<int>case2;
vector<int>PST;
int reg=0;
int deep=0;
int unsat=0;

bool cmp(pair<int, int>& a,pair<int, int>& b){return a.second > b.second;}

bool compare_SJF( const job & j1, const job & j2) {             ///sorting jobs based on tasks
  if( j1.num_task != j2.num_task){return (j1.num_task < j2.num_task);}
  else{return (j1.id< j2.id);}
}

bool sort_bycondition(const pair<int,int>&a, const pair<int,int>&b){
       if(a.second!=b.second){return (a.second < b.second);}               ///sorting based on complete time
       else{return (a.first < b.first);}
}
///srching elements of one array into another
bool SRCH(vector<int>&v1,vector<int>&v2){
     vector<int>::iterator it;
     for(auto a:v1){
        it= find(v2.begin(),v2.end(),a);
        if(it==v2.end()){return 0;}
        }
     return 1;
}
bool deadline_satisfied(vector<int>&V){
  for(int i=0;i<V.size();i++){
    if(V[i] > ddline){return false;}
  }
  return true;
}

/// Check if a job is exhausted or not
bool JOB_FINISHED(vector<int>&v){

       for(int i=0;i<v.size();i++){
        if(v[i]!= -1){return false;}
       }
      return true;
}


///THIS IS THE TRACE GENERATOR
vector<job>Job_request(vector<job>&job_list){

    ofstream of("trace.txt", ios::out);

   srand((unsigned) time(NULL));

	for(int i=0;i< num_job;i++){

         int num_task= min_chunk +rand()%(max_chunk - min_chunk);
         int art=0;
         int dl=ddline;

	     of<< i<<","<<" "<< art << ","<<" "<< dl<< ","<< " "<<num_task<< ","<< " ";
         vector<int>tasks;
         for(int i=0;i<num_task;i++){tasks.push_back(( rand() % num_chunk ));}

        for(int j=0;j< tasks.size();j++){of << tasks[j] << "," << " ";}
        of<< "\n";}

    of.close();

    fstream f;
    ofstream fout;
    ifstream fin;
    fin.open("trace.txt");
    ofstream openfile("input.txt", ios::out);
    fin.seekg(0, ios::beg);
    string line;
    openfile << num_job << endl;

    while(getline(fin,line)){

         vector<int>buffer(line.size()/2-1,0);
         int j=0;
         for ( int i = 0;  i  < line.size()  ; i++){

                if(line[i]==','){ continue;}
                if(line[i]==' '){ j++;}
                else {buffer[j] = buffer[j] * 10 + (line[i] - 48);}
              }

            job newjob;
            newjob.id=buffer[0];
            newjob.art=buffer[1];
            newjob.dl=buffer[2];
            newjob.num_task=buffer[3];


            openfile << newjob.dl << " " << newjob.num_task << endl;

             for(int i=4;i<4+newjob.num_task;i++){
                 newjob.job_task.push_back(buffer[i]);
                 openfile<< buffer[i] << " ";
             }

             openfile << endl;
             job_list.push_back(newjob);
            }
            openfile << CORE << " " << capacity << endl;

            return job_list;
}

///This initialises the job list with pre generated data

void init_job_list()
{
    fstream f;
    ofstream fout;
    ifstream fin;
    fin.open("trace.txt", ios::in);
    //fout.open ("file2.txt",ios::app);
   // fin.seekg(0, ios::beg);
    string line;


    while(getline(fin,line))
    {

         vector<int>buffer(line.size()/2-1,0);
         int j=0;

         for ( int i = 0;  i  < line.size()  ; i++)
         {
                if(line[i]==','){ continue;}
                if(line[i]==' '){ j++;}
                else {buffer[j] = buffer[j] * 10 + (line[i] - 48);}
         }

         job newjob;
         newjob.id=buffer[0];
         newjob.art=buffer[1];
         newjob.dl=buffer[2];
         newjob.num_task=buffer[3];

         for(int i=4;i<4+newjob.num_task;i++)
         {
             newjob.job_task.push_back(buffer[i]);
         }

         job_list.push_back(newjob);
    }

    return;
}

map<long int,int> google_trace(){

    fstream f;
    ofstream fout;
    ifstream fin;
    fin.open("google.txt", ios::in);
    string line;
    

    while(getline(fin,line) && deep< 20527)
    {
         vector<int>buffer;
         int j=0;
         while(line[j]!=','){j++;}
         j++;
         while(line[j]!=','){
               buffer.push_back(line[j]);
               // {buffer[j] = buffer[j] * 10 + (line[i] - 48);}
                j++;
         }
         long int res=0;
         for(auto d:buffer){res=res*10 + d;}
            google[res]++;
            deep++;
    }
      google.clear();

    while(getline(fin,line) && deep< 150982)
    {
         vector<int>buffer;
         int j=0;
         while(line[j]!=','){j++;}
         j++;
         while(line[j]!=','){
               buffer.push_back(line[j]);
               // {buffer[j] = buffer[j] * 10 + (line[i] - 48);}
                j++;
         }
         long int res=0;
         for(auto d:buffer){res=res*10 + d;}
            google[res]++;
            deep++;
    }
    

  return google;
}

bool comp(pair<long int, int>& a, pair<long int, int>& b) {  return a.second < b.second; } 
  
void new_sort(map<long int, int>& M) 
{ 
    vector<pair<long int, int> > A; 
    for (auto& it : M) { 
        A.push_back(it); 
    } 
    sort(A.begin(), A.end(), comp); 
    cout<< A[0].second<<"\n";
    cout<< A[A.size()-1].second<< "\n";

 }

map<int,int>job_to_map(vector<job>&job_list, map<int,int>&mp){

     mp.clear();
     for(int i=0; i< job_list.size(); i++){
         for(auto itr: job_list[i].job_task){mp[itr]++;}
     }
   return mp;
 }

///THIS IS THE PLACEMENT FUNCTION

vector<ds>PLACEMENT(vector<ds>&ds_list,map<int,int>&mp){

         vector<pair<int,int>>v;
               for(auto& it : mp){
                v.push_back(make_pair(it.first,it.second));
                // v.push_back(it);
               }

    sort(v.begin(), v.end(), cmp);
    int tmp= v[0].second;

    if(tmp==1){

        ds newds;
        ds_list.push_back(newds);
        int j=0;
        for(int i=0;i<v.size();i++){

            if(ds_list[j].storage.size()==capacity){
                ds newds;
                ds_list.push_back(newds);
                j++;
                ds_list[j].storage.push_back(v[i].first);
            }

            else{ds_list[j].storage.push_back(v[i].first);}
        }
    }

   else if(tmp!=1){

        for(int i=0;i<tmp;i++){
        ds newds;
        newds.storage.push_back(v[0].first);
        ds_list.push_back(newds);
      }

      for(int it=1;it<v.size();it++){

        int ctr=v[it].second;
        int j=0;

        for(int i=j; i< ds_list.size() and ctr!=0 ;i++){

                 if(ds_list[i].storage.size()==capacity){

                       continue;
                 }

                if(ds_list[i].storage.size() < capacity and i!= ds_list.size()-1){

                    ds_list[i].storage.push_back( v[it].first);
                    ctr--;
                }

               else if(ds_list[i].storage.size() < capacity and i== ds_list.size()-1){

                     if(ds_list[i-1].storage.size() < capacity){

                        ds_list[i].storage.push_back( v[it].first);
                        ctr--;
                     }
                     else if(ds_list[i-1].storage.size()==capacity){

                        ///prev all <ds> are full . Create and add new <ds>

                                  for(int i=0;i<tmp;i++){
                                        ds newds;
                                        ds_list.push_back(newds);
                                 }
                                 ds_list[i].storage.push_back( v[it].first);
                                 ctr--;
                                 if(ctr==0){j=i;}else{j=i+1;}
                          }
                        }
                      }
                    }
                  }

            ///removing the empty ds from the last
            for(int i=ds_list.size()-1; i>=0;i--){
                if(ds_list[i].storage.size()==0 or ds_list[i].storage.size()==1 ){
                    ds_list.pop_back();
                }
            }

        return ds_list;
     }



     vector<ds>PLACEMENT_2(vector<ds>&ds_list,map<int,int>&mp){

             PLACEMENT(ds_list,mp);

              ///remving less than half filled redundant ds
            for(int i=ds_list.size()-1; i>0;i--){
                if(ds_list[i].storage.size() <= capacity/2){

                    if(SRCH(ds_list[i].storage,ds_list[i-1].storage)==1){ds_list.pop_back();}
                }
            }
             return ds_list;
     }

///This is SCHEDULING FUNCTION
vector<int>SJF(vector<ds>&ds_list,vector<job>&Job_list,vector<int>&ex_time){

       sort( Job_list.begin(), Job_list.end(),compare_SJF);
         for(int i=0;i< ds_list.size();i++){
                if(ds_list[i].core!=CORE){ds_list[i].core=CORE;}
            }
         vector<vector<int>>ds_flag(ds_list.size());
         for(int i=0; i< ds_list.size();i++){
            for(int j=0; j< ds_list[i].storage.size();j++){
                ds_flag[i].push_back(1);
            }
         }
         int time=1;
         int no_of_jobs= Job_list.size();
         vector<int>job_list_flag(Job_list.size(),1);

         while(no_of_jobs!=0){
                 
           /* cout<<"The availible cores of all the DS at time" <<time<<" ";
            for(int i=0;i< ds_list.size();i++){
              cout<< ds_list[i].core;
            }*/
            for(int i=0;i< ds_list.size();i++){
                if(ds_list[i].core!=CORE){ds_list[i].core=CORE;}
            }

           for(int i=0; i< ds_flag.size();i++){
            for(int j=0; j< ds_flag[i].size();j++){
                if(ds_flag[i][j]==0){ds_flag[i][j]=1;}
            }
         }

            for(int i=0;i< ds_list.size();i++){
                    ///loop for next ds

                int flag1=1;
                int ptr=0;

                while(ptr < Job_list.size()){
                  ///job=job_list[ptr];
                  if(job_list_flag[Job_list[ptr].id]== 1){

                             for(int j=0;j< ds_list[i].storage.size();j++){
                                    ///loop for ds chhunks
                                    int flag2=1;

                                  if(ds_flag[i][j]==1){

                                     for(int b=0;b < Job_list[ptr].job_task.size();b++){

                                       if(ds_list[i].storage[j] == Job_list[ptr].job_task[b]){

                                        ds_list[i].core--;
                                        if(ds_list[i].core==0){ flag1=0; }  ///GOTO NEXT DS

                                        ds_flag[i][j]=0;
                                        Job_list[ptr].num_task--;

                                        flag2=0;///GOTO NEXT DS CHUNK

                                        if(Job_list[ptr].num_task ==0){
                                            ex_time[Job_list[ptr].id]=time;
                                            no_of_jobs--;
                                            job_list_flag[Job_list[ptr].id]=0;

                                        } }

                                     if(flag1==0){break;}

                                     if(flag2==0){break;}
                                 } }

                                  if(flag1==0){break;}
                            } }

                           if(flag1==0){break;}

                           ptr++;
                       } }

                time++;
           }

     return ex_time;
    }

vector<int>FCFS(vector<ds>&ds_list,vector<job>&Job_list,vector<int>&ex_time){

         for(int i=0;i< ds_list.size();i++){
                if(ds_list[i].core!=CORE){ds_list[i].core=CORE;}
            }
         vector<vector<int>>ds_flag(ds_list.size());
         for(int i=0; i< ds_list.size();i++){
            for(int j=0; j< ds_list[i].storage.size();j++){
                ds_flag[i].push_back(1);
            }
         }
         int time=1;
         int no_of_jobs= Job_list.size();
         vector<int>ds_list_flag(ds_list.size(),1);
         vector<int>job_list_flag(Job_list.size(),1);

         while(no_of_jobs!=0){
                 cout<<endl;
           /* cout<<"The availible cores of all the DS at time" <<time<<" ";
            for(int i=0;i< ds_list.size();i++){
              cout<< ds_list[i].core;
            }*/
            for(int i=0;i< ds_list.size();i++){
                if(ds_list[i].core!=CORE){ds_list[i].core=CORE;}
            }

           for(int i=0; i< ds_flag.size();i++){
            for(int j=0; j< ds_flag[i].size();j++){
                if(ds_flag[i][j]==0){ds_flag[i][j]=1;}
            }
         }
                   int flag1=1;
                int ptr=0;
                while(ptr < Job_list.size()){
                    
                  if(job_list_flag[Job_list[ptr].id]== 1){

                     for(int b=0;b < Job_list[ptr].job_task.size();b++){
                        ///loop for ds chhunks
                                    int flag2=1;
                                    
                        int ptr2=0;            
                        while(ptr2< ds_list.size()){
                            if(ds_list_flag[ptr2]==1){
                             for(int j=0;j< ds_list[ptr2].storage.size();j++){

                                  if(ds_flag[ptr2][j]==1){
                                       
                                       if(ds_list[ptr2].storage[j] == Job_list[ptr].job_task[b]){

                                        ds_list[ptr2].core--;
                                        if(ds_list[ptr2].core==0){ ds_list_flag[ptr2]=0; }  ///DS EXHAUSTED

                                        ds_flag[ptr2][j]=0;
                                        Job_list[ptr].num_task--;
                                        Job_list[ptr].job_task[b]=-1;

                                        flag2=0;///GOTO NEXT TASK OF SAME JOB

                                        if(Job_list[ptr].num_task ==0){
                                            ex_time[Job_list[ptr].id]=time;
                                            no_of_jobs--;
                                            job_list_flag[Job_list[ptr].id]=0;

                                        } 

                                     if(flag2==0){break;}
                                 } }

                            }
                            } 
                         ptr2++;
                        }  if (Job_list[ptr].num_task !=0){flag1=0;}

                           if(flag1==0){break;}
                         }
                          } 
                          ptr++;
                          if(flag1==0){break;}
                        }

                time++;
           }

     return ex_time;
    }

///This is SCHEDULING FUNCTION 2
int SJF_2(vector<ds>&ds_list,vector<job>&Job_list,vector<int>&ex_time){

       sort( Job_list.begin(), Job_list.end(),compare_SJF);
         for(int i=0;i< ds_list.size();i++){
                if(ds_list[i].core!=CORE){ds_list[i].core=CORE;}
            }
         vector<vector<int>>ds_flag(ds_list.size());
         for(int i=0; i< ds_list.size();i++){
            for(int j=0; j< ds_list[i].storage.size();j++){
                ds_flag[i].push_back(1);
            }
         }
         int time=1;
         int no_of_jobs= Job_list.size();
         vector<int>job_list_flag(Job_list.size(),1);

         while(time <= ddline ){
                
           /* cout<<"The availible cores of all the DS at time" <<time<<" ";
            for(int i=0;i< ds_list.size();i++){
              cout<< ds_list[i].core;
            }*/
           // cout<<"\n"<<"DS req"<<ds_list.size()<<"\n";
            for(int i=0;i< ds_list.size();i++){
                if(ds_list[i].core!=CORE){ds_list[i].core=CORE;}
            }

           for(int i=0; i< ds_flag.size();i++){
            for(int j=0; j< ds_flag[i].size();j++){
                if(ds_flag[i][j]==0){ds_flag[i][j]=1;}
            }
         }

            for(int i=0;i< ds_list.size();i++){
                    ///loop for next ds

                int flag1=1;
                int ptr=0;

                while(ptr < Job_list.size()){
                  ///job=job_list[ptr];
                  if(job_list_flag[Job_list[ptr].id]== 1){

                             for(int j=0;j< ds_list[i].storage.size();j++){
                                    ///loop for ds chhunks
                                    int flag2=1;

                                  if(ds_flag[i][j]==1){

                                     for(int b=0;b < Job_list[ptr].job_task.size();b++){

                                       if(ds_list[i].storage[j] == Job_list[ptr].job_task[b]){

                                        ds_list[i].core--;
                                        if(ds_list[i].core==0){ flag1=0; }  ///GOTO NEXT DS

                                        ds_flag[i][j]=0;
                                        Job_list[ptr].num_task--;
                                        Job_list[ptr].job_task[b]=-1;


                                        flag2=0;///GOTO NEXT DS CHUNK

                                        if(Job_list[ptr].num_task ==0){
                                            ex_time[Job_list[ptr].id]=time;
                                            no_of_jobs--;
                                            job_list_flag[Job_list[ptr].id]=0;

                                        } }

                                     if(flag1==0){break;}

                                     if(flag2==0){break;}
                                 } }

                                  if(flag1==0){break;}
                            } }

                           if(flag1==0){break;}

                           ptr++;
                       } }

                time++;
           }

     return no_of_jobs;
    }

    vector<int>SJF_3(vector<int>&Chunk,vector<ds>&ds_list,vector<int>&ex_time_chunk){

         for(int i=0;i< ds_list.size();i++){
                if(ds_list[i].core!=CORE){ds_list[i].core=CORE;}
            }
         vector<vector<int>>ds_flag(ds_list.size());
         for(int i=0; i< ds_list.size();i++){
            for(int j=0; j< ds_list[i].storage.size();j++){
                ds_flag[i].push_back(1);
            }
         }
         int time=1;
         vector<int>chunk_flag(Chunk.size(),1);

         while(time <= ddline){
                
         /*   cout<<"The availible cores of all the DS at time" <<time<<" ";
            for(int i=0;i< ds_list.size();i++){
              cout<< ds_list[i].core;}
         */
            for(int i=0;i< ds_list.size();i++){
                if(ds_list[i].core!=CORE){ds_list[i].core=CORE;}
            }

           for(int i=0; i< ds_flag.size();i++){
            for(int j=0; j< ds_flag[i].size();j++){
                if(ds_flag[i][j]==0){ds_flag[i][j]=1;}
            }
         }

            for(int i=0;i< ds_list.size();i++){
                    ///loop for next ds

                int flag1=1;
                int ptr=0;

                while(ptr < Chunk.size()){

                  if(chunk_flag[ptr]== 1){ ///chunk not used

                             for(int j=0;j< ds_list[i].storage.size();j++){

                                  if(ds_flag[i][j]==1){

                                       if(ds_list[i].storage[j] == Chunk[ptr]){

         /*problem*/                     ds_list[i].core--;
                                        ds_flag[i][j]=0;
                                        chunk_flag[ptr]=0;
                                       // Chunk[ptr]=-1;
                                        ex_time_chunk[ptr]=time;
                                        if(ds_list[i].core==0){ flag1=0; }  ///GOTO NEXT DS

                                        if(flag1==0){break;}

                                        }

                                     if(flag1==0){break;}
                                 }

                                  if(flag1==0){break;}
                            }}
                      if(flag1==0){break;}
                      ptr++;
                      }

                       }
               time++; }
  return ex_time_chunk;  }



///THIS IS THE DISPLAY FUNCTION
void DISPLAY(vector<ds>&ds_list,vector<job>&job_list,vector<int>&ex_time){

         for(int i=0;i<ds_list.size();i++){

            cout<< "\n\n";
            cout<<"DS storage="<< ds_list[i].storage.size()<< endl;
            for(auto a: ds_list[i].storage){cout<< a << " ";}
            cout<<"\n";
         }
         cout<<"\n";

         cout<<"\n";
         cout<<"the jobs deadlines are"<<endl;

         for(auto it: job_list){cout<<it.dl<<" ";}
         cout<<"\n";
         cout<<"the jobs execution times are"<<endl;
         for(auto it:ex_time){cout<< it<< " ";}

}
 map<int,int>new_frequency_generator(map<int,vector<float>>&chunk_alpha,map<int,int>&Mp){

        Mp.clear();
        auto it=chunk_alpha.begin();
        while(it!=chunk_alpha.end()){
         float alpha = it->second[0];
         float old_freq = it->second[1];
         int x = int (old_freq);

         if(alpha >=1){
         while(float(alpha*x ) >= old_freq){x--;}
         int new_freq=x+1;
         Mp[it->first]= new_freq;
         it++;
        }
       else{
         while(float(alpha*x ) < old_freq){x++;}
         int new_freq=x;
         Mp[it->first]= new_freq;
         it++;
       }
     }
       cout<<endl;
      /* auto a=Mp.begin();
       while(a!=Mp.end()){
        cout<<a->first<<","<<a->second<<endl;
        a++;

       }*/

  return Mp;
 }
 map<int,int>new_frequency_generator_2(map<int,vector<float>>&chunk_alpha,map<int,int>&Mp){

       Mp.clear();
        auto it=chunk_alpha.begin();
        while(it!=chunk_alpha.end()){
         float alpha = it->second[0];
         float old_freq = it->second[1];
         int x = int (old_freq);

         if(alpha >=1){
            it++;
        }
       else{
         while(float(alpha*x ) < old_freq){x++;}
         int new_freq=x;
         Mp[it->first]= new_freq;
         it++;
       }
     }
       cout<<endl;
      /* auto a=Mp.begin();
       while(a!=Mp.end()){
        cout<<a->first<<","<<a->second<<endl;
        a++;
       }*/

  return Mp;
 }


 void calculation(vector<job>&j,vector<float>&alpha){
          chunk_alpha.clear();
             for(int k= j.size()-1;k>=0;k--){
                for(auto it: j[k].job_task){
                    if(chunk_alpha.find(it)==chunk_alpha.end()){ chunk_alpha[it]= {float(alpha[k]),float(1)};}
                    else{
                        chunk_alpha[it][0]+= float(alpha[k]);
                        chunk_alpha[it][1]++;
                    }}}

             auto itr=chunk_alpha.begin();
            /* while(itr!=chunk_alpha.end()){
                cout<< itr->first<<","<<itr->second[0]<<","<<itr->second[1]<<endl;
                itr++;
             }*/
          auto ita=chunk_alpha.begin();
             while(ita!=chunk_alpha.end()){
                ita->second[0]/= float(ita->second[1]);
                ita++;
            }
            cout<< endl;
             auto a=chunk_alpha.begin();
            /* while(a!=chunk_alpha.end()){
                cout<< a->first<<","<< a->second[0]<<","<< a->second[1]<<endl;
                a++;
             }*/
          return;
 }

 map<int,int>chunk_to_map(vector<job>&job_list_1,map<int,int>&mp_3){
    vector<int>v;
        for(int i=0;i<job_list_1.size();i++){
            if(job_list_1[i].num_task!=0){

            for (int j=0;j<job_list_1[i].job_task.size();j++){
            if(job_list_1[i].job_task[j]!=-1){
             v.push_back(job_list_1[i].job_task[j]);

            }
            }
            }

        }
           //cout<< "total chunks are"<<" "<<v.size();
           if(v.size()==0){reg=1;}
           Chunk.clear();
           for(auto it:v){
                Chunk.push_back(it);
                mp_3[it]++;
           }
           return mp_3;
     }

 map<int,int>rem_chunk_to_map(vector<int>&Chunk,map<int,int>&mp){

          // mp.clear();
           for(auto it:Chunk){
                mp[it]++;
           }
           return mp;
     }


bool chunk_sat(vector<int>&ex_time_chunk){
      for(auto it:ex_time_chunk){
        if(it==0){return false;}
      }
     return true;
}



int main(int argc, char** argv)
{
     cout<< "MY WORK STARTED";

     if(!strcmp(argv[1], "1"))
     {
         Job_request(job_list);///trace generate
         cout<<"\n"<< "there are"<<" "<< job_list.size()<<" "<<"jobs"<<endl;
     }

     else if(!strcmp(argv[1], "2"))
     {
        int ctr=1;
    do{ 
         job_list.clear();
         ds_list.clear();
         init_job_list();
         job_to_map(job_list,mp);
         PLACEMENT_2(ds_list,mp);///placement
         cout<<"\n";
         cout<<"Total DS required are "<< ds_list.size();
         SJF(ds_list,job_list,ex_time);///schedule
        // DISPLAY(ds_list,job_list,ex_time);///print
         cout<<"\n";
       
         job_list.clear();
         init_job_list();
         alpha_1.clear();
         job_list_1.clear();
         temp.clear();

         int i=0;
         while(i<ex_time.size()){
         temp.push_back(make_pair(i,ex_time[i]));
         i++;
         }
         sort(temp.begin(),temp.end(),sort_bycondition);
         for(int i=0;i<temp.size();i++){
                job_list_1.push_back(job_list[temp[i].first]);
                alpha_1.push_back(ddline/float(temp[i].second));
         }
         calculation(job_list_1,alpha_1);
         new_frequency_generator(chunk_alpha,mp_1);

         for(int i=0;i<ex_time.size();i++){ex_time[i]=0;}
         ds_list_1.clear();
         PLACEMENT_2(ds_list_1,mp_1);
         SJF_2(ds_list_1,job_list_1,ex_time);///schedule

         mp_3.clear();
         chunk_to_map(job_list_1,mp_3);  ///returns map_3 

         if(reg==1){
            case1.push_back(ds_list_1.size());
            cout<<"\\n\n"<<"case 1"<<" "<<case1[0]<<"\n";
            break;
         }   /// If no more chunks are left


         ds_list.clear();
         PLACEMENT_2(ds_list,mp_3);
       //ds_list.erase(ds_list.begin(),ds_list.begin()+8);    

         /*cout<<"\n";
         for(int j=0;j<ds_list.size();j++){
                cout<<" ";
            for(int i=0;i<capacity;i++){cout<< ds_list[j].storage[i]<<",";}
         }*/
        // cout<<"\n"<<"chunks are"<<"\n";
         sort(Chunk.begin(),Chunk.end());
        // for(auto it:Chunk){cout<<it<<",";}

         for(auto it:Chunk){ex_time_chunk.push_back(0);}
         SJF_3(Chunk,ds_list,ex_time_chunk);

         for(auto it:ex_time_chunk){
            if(it==0){unsat++;}
        }
         //ds newds;

        while(unsat==0){
           // newds= ds_list[0];
            ds_list.erase(ds_list.begin(),ds_list.begin()+1); 
            ex_time_chunk.clear();
            unsat=0;
            for(auto it:Chunk){ex_time_chunk.push_back(0);}
             SJF_3(Chunk,ds_list,ex_time_chunk);  
             for(auto it:ex_time_chunk){
             if(it==0){unsat++;}
            } 
           }
           //ds_list.push_back(newds);
         
        int rem=ds_list.size();
        /*if(unsat==0)*/{cout<<"\n\n"<<"case 2"<<" "<<ds_list.size() + ds_list_1.size()<<"\n"; }

                vector<int>V;
            for(int i=0;i<ex_time_chunk.size();i++){
                if(ex_time_chunk[i]==0){
                 V.push_back(Chunk[i]);
                }
            }
         if(V.size()!=0)  
            { Chunk.clear();
            for(auto it:V){Chunk.push_back(it);}
            ex_time_chunk.clear();
            for(auto it:Chunk){ex_time_chunk.push_back(0);}
            mp.clear();
            rem_chunk_to_map(Chunk,mp);
            ds_list.clear();
            PLACEMENT_2(ds_list,mp);
            mp.clear();
            rem+=ds_list.size();
         //   tmp+=rem;
            SJF_3(Chunk,ds_list,ex_time_chunk);

            int flag=0;
            for(auto it:ex_time_chunk){if(it==0)flag=1;}
            if(flag==0){cout<<"\n\n"<<"case 3"<<" "<<ds_list_1.size() + rem;}    
            ex_time_chunk.clear();
         }
          cout<<"\n"<<"Primary DS req are" <<" "<<ds_list_1.size();
            cout<<"\n"<<"Extra DS req are" <<" "<<rem;
              cout<<"\n"<<"Total DS req are" <<" "<<ds_list_1.size() + rem;   
              PST.push_back(ds_list_1.size() + rem);
          
         ctr++;

        } while(  ctr< no_of_times);
        cout<<"\n\n";
        //for(auto it:PST){cout<<it<<",";}
     }

    
   cout<<"\n";
   for (auto it : table){
    cout<< it[0] << "," << it[1];
    cout<< "\n";
   }

     cout<< "MY WORK ENDED";
     return 0;

}
