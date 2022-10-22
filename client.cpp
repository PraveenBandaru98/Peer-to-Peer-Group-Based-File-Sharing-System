#include<iostream>
#include <functional>
#include <string.h>
#include <unistd.h>
#include<fcntl.h> 
#include <pthread.h>
#include<unordered_map>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>



#include <openssl/sha.h>
#include<sstream>
#include <iomanip>

#include <ios>
#include<string>




#include <unistd.h>
#include <arpa/inet.h>
#include<thread>
#include <string.h>
#include<fstream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include<vector>
#include <netdb.h> 
#include<cmath>

using namespace std;
class downinfoclass{
    public:
    string prog;
    string group;
    string fname;
    string sha;
    downinfoclass(){

    }

};
string optimus;
unordered_map<string,downinfoclass>  infoofdown;
vector<string> arg;
void error(const char *msg)
{   
    perror(msg);
    exit(0);
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
void* serve(void* para){
int newsockfd=*((int *)para);
    free(para);
    /*while(1){
    char msg[500];
    bzero(msg,500);
     int n = read(sock,msg,500);
     if (n < 0) error("ERROR reading from socket");
     cout<<"message from other"<<" "<<msg<<endl;

          string s="got it";
        int ne = s.length();
        char buff[ne+ 1];
        strcpy(buff, s.c_str());
        n = write(sock,buff,sizeof(buff));
        if (n < 0) error("ERROR writing to socket");
      
    

     }*/
     int n;
    char buffer[256];
          bzero(buffer,256);

// phase 1

    
     n = read(newsockfd,buffer,255);
     if (n < 0) error("ERROR reading from socket");
     //printf("Here is the message: %s\n",buffer);
     string te(buffer);
     //cout<<te<<endl;
    ifstream in_file(te, ios::binary);   // change
   in_file.seekg(0, ios::end);
   int file_size = in_file.tellg();
   //cout<<"Size of the file is"<<" "<< file_size<<" "<<"bytes"<<endl;
string si=to_string(file_size);

    int ni = si.length();
    char sen[ni + 1];
    strcpy(sen, si.c_str());

     n = write(newsockfd,sen,sizeof(sen));
     if (n < 0) error("ERROR writing to socket");



     //phase 2


//cout<<"phase2"<<endl;

      bzero(buffer,256);
     n = read(newsockfd,buffer,255);
     //cout<<buffer<<endl;
     if (n < 0) error("ERROR reading from socket");
     //printf("Here is the message: %s\n",buffer);

 string leni(buffer);
   string te4=leni;
int partno=stoi(te4);



    string xx="got it";

    int ni1 = xx.length();
    char sen1[ni1 + 1];
    strcpy(sen1, xx.c_str());
    
     ifstream f(te);

    //part
    int noofparts=ceil(((float)file_size)/524288);
    int retpartsize;
    if(partno==noofparts){
        retpartsize=file_size-(524288*(partno-1));

    }
    else{
            retpartsize=524288;
    }





    //part

     si=to_string(retpartsize);

    ni = si.length();
    char sen5[ni + 1];
    strcpy(sen5, si.c_str());

     n = write(newsockfd,sen5,sizeof(sen5));
     if (n < 0) error("ERROR writing to socket");


//phase 3


bzero(buffer,256);
     n = read(newsockfd,buffer,255);
     if (n < 0) error("ERROR reading from socket");
     //printf("Here is the message: %s\n",buffer);
int by=0;
f.seekg((partno-1)*524288, ios::beg);
    while(by<retpartsize){
        char bi[5]="";
         
        f.read(bi, 1);
     n = write(newsockfd,bi,1);
     if (n < 0) error("ERROR writing to socket");
     //cout<<bi[0];
     by++;
    }
    f.close();
    close(newsockfd);     //doubt
     return NULL;

}
void filhandle(string para){
    
    string pass=para;
    vector<string> pasv=linetoword(pass);
    int sockfd=stoi(pasv[0]);
    int n;
    char buffer[255];
    string filname="";
    // phase 1
   //printf("Please enter file name: ");
    bzero(buffer,255);
    char buff[255];
    string msg;
    //getline(cin,msg);  //>>ws to remove white spaces
    msg=pasv[1];
    bzero(buff,255);
    for (int i = 0; i < msg.size(); i++) {
        buff[i] = msg[i];
    }

    //make file name

    string parta,partb;
    string word = "";
    for (auto x : msg) 
    {
        if (x == '.')
        {
           parta=word;
            word = "";
        }
        else {
            word = word + x;
        }
    }
    partb=word;




    
    n = write(sockfd,buff,sizeof(buff));
    if (n < 0) 
         error("ERROR writing to socket");
    bzero(buffer,255);
    //cout<<"check"<<endl;
    //cout<<"check"<<endl;
    n = read(sockfd,buffer,255);
    if (n < 0) 
         error("ERROR reading from socket");
    //printf("total size is %s\n",buffer);
    string tots(buffer);
    int totsi=stoi(tots);
    int cpart=ceil(((float)totsi)/524288);
    //cout<<"no of parts:"<<cpart<<endl;
        
      //printf("Please enter the part number: ");
    bzero(buffer,255);
    
    //getline(cin,msg);  //>>ws to remove white spaces
    //msg="1";
    //cin>>msg;
    //cout<<"1"<<endl;
    msg=pasv[2];
    filname=parta+"*"+msg+"."+partb;
    bzero(buff,255);
    for (int i = 0; i < msg.size(); i++) {
        buff[i] = msg[i];
    }
     n = write(sockfd,buff,sizeof(buff));
    if (n < 0) 
         error("ERROR writing to socket");
    bzero(buffer,255);
    int by=0;
    fstream  afile;
//cout<<"2"<<endl;

     bzero(buffer,255);
    n = read(sockfd,buffer,255);
    if (n < 0) 
         error("ERROR reading from socket");
    string leni(buffer);

//cout<<"3"<<endl;

// phase 3

int silen=stoi(leni);


//cout<<"z4"<<endl;
    string xx="recieved";

    int ni1 = xx.length();
    char sen1[ni1 + 1]="";
    strcpy(sen1, xx.c_str());
       n = write(sockfd,sen1,sizeof(sen1));
    if (n < 0) 
         error("ERROR writing to socket");


//cout<<"HERE"<<endl;


afile.open(filname, ios::out | ios::in | ios::app);
    while(by<silen )
    {   char bh[5]="";
        n = read(sockfd,bh,1);
        //cout<<"z6z"<<endl;
    if (n < 0) 
         error("ERROR reading from socket");
          afile<<bh[0];
    //cout<<bh[0];
    by++;
}
//cout<<"z7z"<<endl;
afile.close();          //doubt
close(sockfd);  //new   //doubt
   
}
void * listen_func(void * para){


    int sockfd, newsockfd, portno;
     socklen_t clilen;
     char buffer[500];
     struct sockaddr_in serv_addr, cli_addr;
     int n;
     
     bzero((char *) &serv_addr, sizeof(serv_addr));
     sockfd = socket(AF_INET, SOCK_STREAM, 0);
     if (sockfd < 0) 
        error("ERROR opening socket");
     bzero((char *) &serv_addr, sizeof(serv_addr));
     string s = arg[2];
    int ni = s.length();
    char te[ni+ 1];
    strcpy(te, s.c_str());
     portno = atoi(te);
     serv_addr.sin_family = AF_INET;
     string s1 =arg[1];
    int ni1 = s1.length();
    char te1[ni1+ 1];
    strcpy(te1, s1.c_str());
     serv_addr.sin_addr.s_addr = inet_addr(te1);
     
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
void * downhandle(void * para){

    char *pas;
    pas=(char *)para;
    string pass(pas);






    
    string pqz=optimus;
    //ddgf
    //fhfh
    //fgfhg
    //fdfd
    //cout<<"downhandle "<<pqz<<endl;
    
    vector<string> chk=linetoword(pqz);



            string ip=chk[2];
        string po=chk[3];


        char cip[ip.length()+ 1];
        strcpy(cip, ip.c_str());

        
        char cpor[po.length()+ 1];
        strcpy(cpor, po.c_str());


        int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;

    vector<thread> zzx;

    for(int i=1;i<stoi(chk[1])+1;i++){
 char buffer[255];
    
    portno = atoi(cpor);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
        error("ERROR opening socket");
    server = gethostbyname(cip);
    int returnty;
    if (server == NULL) {
        string t2empy;
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }
    bool retypeofze;
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    char copretsoc;
    bcopy((char *)server->h_addr, 
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
    int stsreturxz;
    serv_addr.sin_port = htons(portno);
    int noofconecofconn;
    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
        error("ERROR connecting 1");
    

string name;
string part;
name=chk[0]; //change


part=to_string(i);// change
//cout<<"file name:";
//cin>>name;
//cout<<"part name:";
//cin>>part;
string tosen;
tosen= to_string(sockfd)+" "+name+" "+part;



zzx.push_back(thread(filhandle,tosen));


//cout<<i;
//filhandle((void*) tosenc);




//sknk
//gfdf
//fgfg
//dffdgdf
}
for(int i=0;i<zzx.size();i++){
    zzx[i].join();
}
//erwetfertretg
//rfdsgfdgd
//rftgyed
string name=chk[0];

//dfe
//dsfdsf
//efsddsf
//sgfr
//fstream  afile;
//afile.open(name, ios::out | ios::in | ios::app);
char tm[name.size()+1];
strcpy(tm,name.c_str());
int fdz = open(tm, O_WRONLY | O_CREAT,0777);
if (fdz < 0) 
{ 
    perror("cannot open file"); 
    exit(1); 
}  

    string parta,partb;
    string word = "";
    for (auto x : name) 
    {
        if (x == '.')
        {
           parta=word;
            word = "";
        }
        else {
            word = word + x;
        }
    }
    partb=word;
stringstream ssha;
SHA_CTX comp;


char b[524288+1]; // xx
int status;
    SHA1_Init(&comp);
    for(int i=1;i<stoi(chk[1]);i++){// xx

    bzero(b,524288+1);// xx








    string te=parta+"*"+to_string(i)+"."+partb;

    ifstream fx(te);
    fx.read(b, 524288);

    fx.close();


    SHA1_Update(&comp,b,524288);
    char filec[500];
    strcpy(filec,te.c_str());

    status = remove(filec);
    if(status==0)
        {}
    else
        {cout<<"Error Occurred while deletion !"<<endl;}




    write(fdz, b, 524288); 
    
   //afile<<b;
   
    }
    
    bzero(b,524288+1);// xx
    string te=parta+"*"+chk[1]+"."+partb;

    ifstream fx(te);

    fx.seekg(0, ios::end);
   int size = fx.tellg();
    fx.seekg(0);
    char buffer[size+1];

bzero(buffer,size+1);
    fx.read(buffer, size);
    SHA1_Update(&comp,buffer,size);

    write(fdz, buffer, size); 

    //afile<<buffer;
  unsigned char out[SHA_DIGEST_LENGTH];
    fx.close();


    char filec[500];
    strcpy(filec,te.c_str());
SHA1_Final(out,&comp);
    status = remove(filec);
    if(status==0)
        {}
    else
       { cout<<"Error Occurred while deletion!"<<endl;}


  for(int tev=0;tev<SHA_DIGEST_LENGTH;tev++){
      string temxsh;
      ssha<<hex<<setw(2)<<setfill('0')<<(int)out[tev];
  }



    //afile.close();
    close(fdz);
    /*cout<<ssha.str()<<endl;
    cout<<"len "<<chk.size()<<endl;
    for(int i=0;i<chk.size();i++)
{
cout<<chk[i]<<endl;
} */
    if(chk[5]==ssha.str()){
        infoofdown[chk[0]].sha="sha_Matched";
    }
    else{
        infoofdown[chk[0]].sha="sha_not_Matched";
    }

infoofdown[chk[0]].prog="C";






close(sockfd);
return NULL;
}

int main(int argc, char *argv[])
{


    for(int i=0;i<argc;i++){
        arg.push_back(argv[i]);
    }
    pthread_t th;
    
    pthread_create(&th,NULL,listen_func,NULL);
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;

    //char buffer[256];
    if (argc < 5) {
       fprintf(stderr,"invalid no of arguments\n");
       exit(0);
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    portno = atoi(argv[4]);
    int retofsoc;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    string ststpdsoc;
    if (sockfd < 0) 
        error("ERROR opening socket");
    string hostnabyip;
    server = gethostbyname(argv[3]);
    int strofsevre;
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }
    char zerorets;
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bool cpyretval;
    bcopy((char *)server->h_addr, 
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
    string servporet;
    serv_addr.sin_port = htons(portno);
    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
        error("ERROR connecting");
    string statvalofco="successfull";
    int flag=1;
    char buff[500];
    while(flag){
    string msg;
    printf("Please enter the message: ");
    //cin>>msg;
    
    getline(cin,msg);  //>>ws to remove white spaces
    
    
    bzero(buff,500);
    for (int i = 0; i < msg.size(); i++) {
        buff[i] = msg[i]; // recent change
    }
    //cout<<"rechce "<<buff<<" "<<endl;
    vector<string> wordmsg=linetoword(msg);
    if(wordmsg[0]=="logout"){
        //flag=0;
        bzero(buff,500);
        for (int i = 0; i < msg.size(); i++) {
        buff[i] = msg[i]; // recent change
        }
        
    }
    else if(wordmsg[0]=="show_downloads"){
        for (auto i : infoofdown){
        cout << i.second.fname<<" "<<i.second.group<<" "<<i.second.prog<<" "<<i.second.sha<<endl;
        }
        msg="thankyou";
        bzero(buff,500);
        for (int i = 0; i < msg.size(); i++) {
        buff[i] = msg[i]; // recent change
        }

    }
    else if(wordmsg[0]=="upload_file"){
           string str=wordmsg[1];
         char xz[str.size()+1];
         strcpy(xz,str.c_str());


        int zz=open(xz,  O_RDONLY);
        
        if(zz<0){
            cout<<"No such file"<<endl;
            msg="thankyou";
            bzero(buff,500);
            for (int i = 0; i < msg.size(); i++) {
            buff[i] = msg[i]; // recent change
            }

        }
        else{
            close(zz);
        stringstream shas;
        unsigned char bun[524288+1];
            string temp;
            SHA_CTX com;
            ifstream in_file(wordmsg[1], ios::binary);   // change
            in_file.seekg(0, ios::end);
            SHA1_Init(&com);
            int readby;
            int filsi = in_file.tellg();
            in_file.close();
            //cout<<"Size of the file is"<<" "<< file_size<<" "<<"bytes"<<endl;
            int noofp=ceil(((float)filsi)/524288);
            //sdffg
            //fdhghg
            //fgfhfh
            
            vector<string> ltowo=linetoword(msg);
            string fname=ltowo[1];
            FILE *vfptr=fopen(fname.c_str(),"rb");
            while((readby=fread(bun,1,524288,vfptr))!=0){
                SHA1_Update(&com,bun,readby);
            }
            unsigned char fin[SHA224_DIGEST_LENGTH];
            int resultofsha;
            string finsha;
            SHA1_Final(fin,&com);
            int  totshalen=20;
            int tempinhas=0;
            for(int i=0;i<SHA_DIGEST_LENGTH;i++){
                tempinhas=1;
                shas<<hex<<setw(2)<<setfill('0')<<(int)fin[i];
            }
            string strhas;
            fclose(vfptr);
            temp=msg+" "+arg[1]+" "+arg[2]+" "+to_string(filsi)+" "+to_string(noofp)+" "+shas.str();
            
            msg=temp;
            bzero(buff,500);
            for (int i = 0; i < msg.size(); i++) {
                buff[i] = msg[i]; // recent change
            }
        }
    }
    else if(wordmsg[0]=="download_file"){
        // new change 
        //bjjjj
        //bhhy
        //hcfcf
        vector<string> temltow=linetoword(msg);
        infoofdown[temltow[2]]=downinfoclass();
        infoofdown[temltow[2]].fname=temltow[2];
        infoofdown[temltow[2]].group=temltow[1];

        infoofdown[temltow[2]].prog="D";
        infoofdown[temltow[2]].sha="-1";



        msg=msg+" "+arg[1]+" "+arg[2];
        bzero(buff,500);
        for (int i = 0; i < msg.size(); i++) {
        buff[i] = msg[i]; // recent change
        }
    
        n = write(sockfd,buff,strlen(buff));
        if (n < 0) 
            error("ERROR writing to socket");


    char buff2[500];
    bzero(buff2,500);
    n = read(sockfd,buff2,500);
    if (n < 0) 
         error("ERROR reading from socket");
    string zz(buff2);
    cout<<"read "<<zz<<endl;
    vector<string> chk=linetoword(zz);
    if(chk[0]!="-1"){

//cout<<"hi"<<endl;
//hubhb
//njbhvgf



   string tosen=zz;


pthread_t th7;



char tosenc[tosen.length() + 1]; 

strcpy(tosenc, tosen.c_str()); 

//filhandle((void*) tosenc);
//dfuisgu
//fdxjiiujfciujf
//gfkgn
optimus=tosen;
pthread_create(&th7,NULL,downhandle,(void*)tosenc);
//pthread_join(th7,NULL);



    

    }
    else{
        infoofdown[temltow[2]].prog="F";
    }
        
        



        msg="thankyou";
        bzero(buff,500);
        for (int i = 0; i < msg.size(); i++) {
        buff[i] = msg[i]; // recent change
        }

    }

    else{
        bzero(buff,500);
        for (int i = 0; i < msg.size(); i++) {
        buff[i] = msg[i]; // recent change
        }
    }
    n = write(sockfd,buff,strlen(buff));
    if (n < 0) 
         error("ERROR writing to socket");
    char buff2[500];
    bzero(buff2,500);
    n = read(sockfd,buff2,500);
    if (n < 0) 
         error("ERROR reading from socket");
    printf("%s\n",buff2);
    }
    close(sockfd);
    return 0;
}
