//dont use control+c 

#include<iostream>
#include <functional>
#include <stdio.h>
#include <stdlib.h>
#include<fcntl.h> 
#include<vector>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include<unordered_map>
using namespace std;
//new
class file_info{
    public:
    string filename;
    string filesize;
    //string owner;
    string noofparts;
    string sha;
    unordered_map<int,vector<string>> partinfo;
    file_info(){}
};
class group_cls{
    public:
    string gid;
    string owner;
    vector<string> files;
    vector<string> pendreq;
    vector<string> members;
    unordered_map<string,file_info> infooffile;
    group_cls(){}
    group_cls(string id,string own){
        gid=id;
        owner=own;
        members.push_back(own);
    }
};
unordered_map<string,int> status;
unordered_map<string,group_cls> group_ds;
unordered_map<string,string> login_ds;

void error(const char *msg)
{   
    perror(msg);
    exit(1);
}
vector<string> linetoword(string str)
{
    vector<string> temp;
    string word = "";
    for (auto x : str) 
    {
        if (x == ' ')
        {
            temp.push_back(word);
            word = "";
        }
        else {
            word = word + x;
        }
    }
    temp.push_back(word);
    return temp;
}
int login(string a , string b){
    if (login_ds.find(a) == login_ds.end()){
        return 0;
    }
    if(login_ds[a]!=b){
        return 0;
    }
status[a]=1; //new1
return 1;
}
int createuser(string a,string b){
    if (login_ds.find(a) != login_ds.end()){
        return 0;
    }
    else{
        login_ds[a]=b;
    }
    status[a]=0; //new1
    return 1;

}
int loggedin(int sock,string user){
     string si="logged in";
    int ne = si.length();
    char buff[ne+ 1];
    strcpy(buff, si.c_str());
    int ret = write(sock,buff,sizeof(buff));
    if (ret < 0) error("ERROR writing to socket");
    while(1){
    char msg[500];
    bzero(msg,500);
     int n = read(sock,msg,500);
     if (n < 0) error("ERROR reading from socket");
     cout<<"message received:"<<" "<<msg<<endl;
    
     string stmsg(msg);
     vector<string> stvec;
     stvec=linetoword(stmsg);
     if(stvec[0]=="create_group"){
         string s;
         if (group_ds.find(stvec[1]) != group_ds.end()){
             s="group already exists";
         }
         else{
         group_ds[stvec[1]]=group_cls(stvec[1],user);
         s="group created";
         }
        int ne = s.length();
        char buff[ne+ 1];
        strcpy(buff, s.c_str());
        int ret = write(sock,buff,sizeof(buff));
        if (ret < 0) error("ERROR writing to socket");

        }
     else if(stvec[0]=="join_group"){
         string s;
         if (group_ds.find(stvec[1]) == group_ds.end()){
             s="group not found";
         }
         else{
             vector<string> mem=group_ds[stvec[1]].members;
             int flg=0;
             for(int i=0;i<mem.size();i++)
             {
                 if(mem[i]==user){
                     flg=1;
                     break;
                 }
             }
             if(flg==0){
                group_ds[stvec[1]].pendreq.push_back(user);
                s="request sent";
             }
             else{
                s="you are already in the group";
             }
         }

         int ne = s.length();
        char buff[ne+ 1];
        strcpy(buff, s.c_str());
        int ret = write(sock,buff,sizeof(buff));
        if (ret < 0) error("ERROR writing to socket");

        }
    else if(stvec[0]=="leave_group"){
        string s;
        if (group_ds.find(stvec[1]) == group_ds.end()){
             s="group not found";
         }
         else if(group_ds[stvec[1]].owner==user){
             s="owner cant leave the group";
         }
         else{
             int ind=-1;
             vector<string> allmem=group_ds[stvec[1]].members;
            int flg=0;
            for(int i=0;i<allmem.size();i++){
                if(allmem[i]==user){
                    ind=i;
                    flg=1;
                    break;
                }
            }
            if(flg==0){
                s="you are not in that group";
            }
            else{
                group_ds[stvec[1]].members.erase(ind+group_ds[stvec[1]].members.begin());
                s="you left the group";
            }
             
         }
         int ne = s.length();
        char buff[ne+ 1];
        strcpy(buff, s.c_str());
        int ret = write(sock,buff,sizeof(buff));
        if (ret < 0) error("ERROR writing to socket");
        }
    else if(stvec[0]=="requests"){
        string s;
        if (group_ds.find(stvec[2]) == group_ds.end()){

                s="no group found";

        }
        else{
            if(group_ds[stvec[2]].owner!=user){
                s="you are not owner of that group";
            }
            else{
                s="";
                vector<string> allpendreq=group_ds[stvec[2]].pendreq;
                if(allpendreq.size()==0){
                    s="no pending requests";
                }
                for(int i=0;i<allpendreq.size();i++){
                    s+=allpendreq[i]+" ";
                }
            }
        
        }
        int ne = s.length();
        char buff[ne+ 1];
        strcpy(buff, s.c_str());
        int ret = write(sock,buff,sizeof(buff));
        if (ret < 0) error("ERROR writing to socket");
        
        }
    else if(stvec[0]=="accept_request"){
        string s;
        if (group_ds.find(stvec[1]) == group_ds.end())
        {
            s="group doesnt exist";
        }
        else{
            if(group_ds[stvec[1]].owner==user){
                vector<string> pend=group_ds[stvec[1]].pendreq;
                int flg=0,ind;
                for(int i=0;i<pend.size();i++){
                    if(pend[i]==stvec[2]){
                        flg=1;
                        ind=i;
                        break;
                    }
                }
                if(flg==0){
                    s="no pending request with that username";
                }
                else{
                    //delete pend request and move to member stvec[2]
                    group_ds[stvec[1]].pendreq.erase(ind+group_ds[stvec[1]].pendreq.begin());
                    group_ds[stvec[1]].members.push_back(stvec[2]);
                    s="request accepted";
                }


            }
            else{
                s="you are not the group owner";
            }
        }
        int ne = s.length();
        char buff[ne+ 1];
        strcpy(buff, s.c_str());
        int ret = write(sock,buff,sizeof(buff));
        if (ret < 0) error("ERROR writing to socket");

    }
    else if(stvec[0]=="list_groups"){
        int si=group_ds.size();
        string s;
        if(si==0){
            s="no groups found";
        }
        else{
            s="";
            for (auto i = group_ds.begin(); i != group_ds.end(); i++){
                s+=i->first+" ";
            }
        

        }
        int ne = s.length();
        char buff[ne+ 1];
        strcpy(buff, s.c_str());
        int ret = write(sock,buff,sizeof(buff));
        if (ret < 0) error("ERROR writing to socket");

    }
    else if(stvec[0]=="list_files"){
        string grpn=stvec[1];
        string s;
        if (group_ds.find(stvec[1]) == group_ds.end())
        {
            s="group doesnt exist";
        }
        else{
            vector<string> mem=group_ds[stvec[1]].members;
            int flg=0;
            for(int i=0;i<mem.size();i++){
                if(user==mem[i]){
                    flg=1;
                    break;
                }
            }
            if(flg==0){
                s="you are not group member";
            }
            else{
                vector<string> fil=group_ds[stvec[1]].files;
                if(fil.size()==0){
                    s="no files";
                }
                else{
                    s="";
                    vector<string> fil=group_ds[stvec[1]].files;
                    for(int i=0;i<fil.size();i++){
                        s+=fil[i]+" ";
                    }

                }
            }
        }

        int ne = s.length();
        char buff[ne+ 1];
        strcpy(buff, s.c_str());
        int ret = write(sock,buff,sizeof(buff));
        if (ret < 0) error("ERROR writing to socket");



    }
    else if(stvec[0]=="upload_file"){
        string s;
        if (group_ds.find(stvec[2]) == group_ds.end())
        {
            s="group doesnt exist";
        }
        else{
        vector<string> mem=group_ds[stvec[2]].members;
            int flg=0;
            for(int i=0;i<mem.size();i++){
                if(user==mem[i]){
                    flg=1;
                    break;
                }
            }
            if(flg==0){
                s="you are not group member";
            }
            else{
                group_ds[stvec[2]].files.push_back(stvec[1]);
                group_ds[stvec[2]].infooffile[stvec[1]]=file_info();
                group_ds[stvec[2]].infooffile[stvec[1]].filename=stvec[1];
                group_ds[stvec[2]].infooffile[stvec[1]].filesize=stvec[5];
                group_ds[stvec[2]].infooffile[stvec[1]].noofparts=stvec[6];
                group_ds[stvec[2]].infooffile[stvec[1]].sha=stvec[7];
                string temp;
                temp=stvec[3]+" "+stvec[4]+" "+user;
                group_ds[stvec[2]].infooffile[stvec[1]].partinfo[0].push_back(temp);
                s="upload successfull";

            }

        }

        int ne = s.length();
        char buff[ne+ 1];
        strcpy(buff, s.c_str());
        int ret = write(sock,buff,sizeof(buff));
        if (ret < 0) error("ERROR writing to socket");
    }
    else if(stvec[0]=="download_file"){
        /*cout<<"recieved"<<endl;
        for(int j=0;j<stvec.size();j++){
            cout<<stvec[j]<<endl;
        }*/
        string s;
        if (group_ds.find(stvec[1]) == group_ds.end())
        {
            s="-1 group doesnt exist";
        }
        else{
        vector<string> mem=group_ds[stvec[1]].members;
            int flg=0;
            for(int i=0;i<mem.size();i++){
                if(user==mem[i]){
                    flg=1;
                    break;
                }
            }
            if(flg==0){
                s="-1 you are not group member";
            }
            else{
                int flgo=0;
                //cout<<"inside func"<<endl;
                unordered_map<string,file_info> fil=group_ds[stvec[1]].infooffile;
                if (fil.find(stvec[2])== fil.end())
                {       
                        s="-1 file not found in group";
                }
                else{
                    

                    cout<<"started sending"<<endl;
                    string s1;
                    s1=fil[stvec[2]].filename +" "+ fil[stvec[2]].noofparts;
                    int ne1 = s1.length();
                    char buff[ne1+ 1];


    
                    // check status and send
                    unordered_map<int,vector<string>> xy=group_ds[stvec[1]].infooffile[stvec[2]].partinfo;
                    vector<string> zer=xy[0];
                    //cout<<"func"<<endl;
                    /*for(int j=0;j<zer.size();j++){
                        cout<<zer[j]<<endl;
                    }*/
                    int sentflag=0;
                    for(int j=0;j<zer.size();j++){
                        vector<string> temp=linetoword(zer[j]);
                        if(status[temp[2]]==1){
                        cout<<"sent "<<zer[j]<<endl;
                            sentflag=1;
                            
                           s1=s1+" "+zer[j]+" "+group_ds[stvec[1]].infooffile[stvec[2]].sha;
                            break;
                        s="sent successfully";
                        }
                        
                    }
                    if(sentflag==0)
                        {s="-1 no user to send data";}
                    else{
                        s=s1;
                        // new change 1
                        //jnjnjjn
                        //jbhbb
                        string temp;
                        temp=stvec[4]+" "+stvec[5]+" "+user;
                        group_ds[stvec[1]].infooffile[stvec[2]].partinfo[0].push_back(temp);

                    }
                
                   
                  //s="sending";


            }

        }
        }
        int ne = s.length();
        char buff[ne+ 1];
        strcpy(buff, s.c_str());
        int ret = write(sock,buff,sizeof(buff));
        if (ret < 0) error("ERROR writing to socket");

        

    }
    else if(stvec[0]=="stop_share"){

                string s;
        if (group_ds.find(stvec[1]) == group_ds.end())
        {
            s="group doesnt exist";
        }
        else{
        vector<string> mem=group_ds[stvec[1]].members;
            int flg=0;
            for(int i=0;i<mem.size();i++){
                if(user==mem[i]){
                    flg=1;
                    break;
                }
            }
            if(flg==0){
                s="you are not group member";
            }
            else{
                int flgo=0;
                //cout<<"inside func"<<endl;
                unordered_map<string,file_info> fil=group_ds[stvec[1]].infooffile;
                if (fil.find(stvec[2])== fil.end())
                {       
                        s="file not found in group";
                }
                else{
                    vector<string> te=group_ds[stvec[1]].infooffile[stvec[2]].partinfo[0];
                    /* vector<string> pend=group_ds[stvec[1]].pendreq;
                int flg=0,ind;
                for(int i=0;i<pend.size();i++){
                    if(pend[i]==stvec[2]){
                        flg=1;
                        ind=i;
                        break;
                    }
                }
                if(flg==0){
                    s="no pending request with that username";
                }
                else{
                    //delete pend request and move to member stvec[2]
                    group_ds[stvec[1]].pendreq.erase(ind+group_ds[stvec[1]].pendreq.begin());
                    group_ds[stvec[1]].members.push_back(stvec[2]);
                    s="request accepted";
                } */
                int flg=0,ind;
                for(int i=0;i<te.size();i++){
                    vector<string> ltow=linetoword(te[i]);
                    if(ltow[2]==user){
                        flg=1;
                        ind=i;
                        break;
                    }
                }
                if(flg==0){
                    s="You are not sharing that file";
                }
                else{
                    s="file stopped sharing";
                    group_ds[stvec[1]].infooffile[stvec[2]].partinfo[0].erase(ind+group_ds[stvec[1]].infooffile[stvec[2]].partinfo[0].begin());
                }


                    


            }

        }
        }
        int ne = s.length();
        char buff[ne+ 1];
        strcpy(buff, s.c_str());
        int ret = write(sock,buff,sizeof(buff));
        if (ret < 0) error("ERROR writing to socket");


    }
    else if(stvec[0]=="thankyou"){
        string s="welcome";
        int ne = s.length();
        char buff[ne+ 1];
        strcpy(buff, s.c_str());
        int ret = write(sock,buff,sizeof(buff));
        if (ret < 0) error("ERROR writing to socket");
        

    }
    else if(stvec[0]=="logout"){

        // set status;
        status[user]=0;
        string s="bye";
        int ne = s.length();
        char buff[ne+ 1];
        strcpy(buff, s.c_str());
        n = write(sock,buff,sizeof(buff));
      
    
        if (n < 0) error("ERROR writing to socket");
        
        return 1;

        
    }
    else{
        string s="Invalid command";
        int ne = s.length();
        char buff[ne+ 1];
        strcpy(buff, s.c_str());
        int ret = write(sock,buff,sizeof(buff));
        if (ret < 0) error("ERROR writing to socket");

    }
    }

    return 0;
    

}
void* serve(void* para){
    int sock=*((int *)para);
    free(para);
    while(1){
    char msg[500];
    bzero(msg,500);
     int n = read(sock,msg,500);
     if (n < 0) error("ERROR reading from socket");
     cout<<"message received:"<<" "<<msg<<endl;
    
     string stmsg(msg);
     vector<string> stvec;
     stvec=linetoword(stmsg);
    if(stvec[0]=="logout"){
         string s="bye";
        int ne = s.length();
        char buff[ne+ 1];
        strcpy(buff, s.c_str());
        n = write(sock,buff,sizeof(buff));
      
    
        if (n < 0) error("ERROR writing to socket");
          //return NULL;
      }
      else if(stvec[0]=="login"){
            int ret=login(stvec[1],stvec[2]);
            string s;
            if(ret==1){
                
                int rv=loggedin(sock,stvec[1]);
                if(rv==1){
                    //return NULL;
                }
            }
            else{
                s="invalid user";
                int ne = s.length();
                char buff[ne+ 1];
                strcpy(buff, s.c_str());
                n = write(sock,buff,sizeof(buff));
                if (n < 0) error("ERROR writing to socket");
            }
            
            

      }
      else if(stvec[0]=="create_user"){

        int ret=createuser(stvec[1],stvec[2]);
        string s;
        if(ret==1){
                s="register successfull";
            }
            else{
                s="user exists with that id";
            }
        int ne = s.length();
        char buff[ne+ 1];
        strcpy(buff, s.c_str());
        n = write(sock,buff,sizeof(buff));
        if (n < 0) error("ERROR writing to socket");
      }
      else{
          string s="Invalid command";
        int ne = s.length();
        char buff[ne+ 1];
        strcpy(buff, s.c_str());
        n = write(sock,buff,sizeof(buff));
        if (n < 0) error("ERROR writing to socket");
      }
    

     }
     return NULL;
}
void * quitfun(void * para){
    string str;
    cin>>str;
    if(str=="quit"){
        exit(1);
    }
    return NULL;

}
int main(int argc, char *argv[])
{

    pthread_t thr;
    
    pthread_create(&thr,NULL,quitfun,NULL);

    string thiposi="after_quit_func";
     int sockfd, newsockfd, portno;
     bool workornot=false;
     socklen_t clilen;
     char *buffofcarpoi;
     char buffer[500];
     int noofconforser;
     struct sockaddr_in serv_addr, cli_addr;
     string statofcurspd="reading var";
     int n;
     char onbyres[1];
     if (argc < 3) {
         string msgoferror="less parameters are given";
         fprintf(stderr,"no ip or no port provided\n");
         exit(1);
         string stattolev="exiting the program";
     }
     int zerbefseradd=0;
     bzero((char *) &serv_addr, sizeof(serv_addr));
     int zeraftsecs=0;
     sockfd = socket(AF_INET, SOCK_STREAM, 0);
     int noosconnsd;
     if (sockfd < 0) 
        error("ERROR opening socket");
    string statofafsock;
     bzero((char *) &serv_addr, sizeof(serv_addr));
     string statafterred="reading para";
     portno = atoi(argv[2]);
     int paraofportno;
     serv_addr.sin_family = AF_INET;
     string statbefipsas;
     serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
     string aftbefpot;
     serv_addr.sin_port = htons(portno);
     if (bind(sockfd, (struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
              error("ERROR on binding");
    
     int lis=listen(sockfd,5); //change
     if(lis<0){
         error("error in listen");
     }
     while(1){
         
     clilen = sizeof(cli_addr);
     
     newsockfd = accept(sockfd,(struct sockaddr *) &cli_addr,&clilen);
     if (newsockfd < 0) 
          error("ERROR on accept");
   
    pthread_t th;
    
    //serve(sock);
    int *tem=(int *)malloc(sizeof(int));
    *tem=newsockfd;
    pthread_create(&th,NULL,serve,tem);
    //pthread_join(th,NULL);
    
     }
    
     close(newsockfd);
     close(sockfd);
     return 0; 
}
