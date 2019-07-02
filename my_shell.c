#include  <stdio.h>
#include  <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h> 
#include <fcntl.h>
#include <signal.h>

#define MAX_INPUT_SIZE 1024
#define MAX_TOKEN_SIZE 64
#define MAX_NUM_TOKENS 64

int FLAG_ = 0;
int PGID_ = 0;

int BGF_ = 0;
int BGID_ = 0;

int RACECDN = 0;
int tracker = 0;
int inccmd = 0;


char **tokenize(char *line)
{
  char **tokens = (char **)malloc(MAX_NUM_TOKENS * sizeof(char *));
  char *token = (char *)malloc(MAX_TOKEN_SIZE * sizeof(char));
  int i, tokenIndex = 0, tokenNo = 0;

  for(i =0; i < strlen(line); i++){

    char readChar = line[i];

    if (readChar == ' ' || readChar == '\n' || readChar == '\t'){
      token[tokenIndex] = '\0';
      if (tokenIndex != 0){
	tokens[tokenNo] = (char*)malloc(MAX_TOKEN_SIZE*sizeof(char));
	strcpy(tokens[tokenNo++], token);
	tokenIndex = 0; 
      }
    } else {
      token[tokenIndex++] = readChar;
    }
  }
 
  free(token);
  tokens[tokenNo] = NULL ;
  return tokens;
}

void main_sigint(int x){
    if(FLAG_==1)
    {
        kill(-(getpid()+10),14);
    }

    printf("AAA \n");

}



void racer(int x){
    RACECDN ++;
//    tracker++;
    return;
}

void tra_mod(int x){
    inccmd++;
//    tracker--;
}

int main(int argc, char* argv[]) {
	char  line[MAX_INPUT_SIZE];            
	char  **tokens;              
	int i;


    signal(SIGINT, main_sigint);
    signal(SIGUSR1, SIG_IGN);
    signal(SIGUSR2, racer);
    signal(SIGTSTP, tra_mod);


	FILE* fp;
	if(argc == 2) {
		fp = fopen(argv[1],"r");
		if(fp < 0) {
			printf("File doesn't exists.");
			return -1;
		}
	}

	while(1) {			
		
        int kpid,sta;
        kpid = waitpid(-1,&sta,WNOHANG);
        if((kpid!=0)&&(kpid!=-1)){printf("Shell: Background process finished\n");BGF_=0;} 


		bzero(line, sizeof(line));                                       
		if(argc == 2) { // batch mode
			if(fgets(line, sizeof(line), fp) == NULL) { // file reading finished
				break;	
			}
			line[strlen(line) - 1] = '\0';
		} else { // interactive mode
			printf("$ ");
			scanf("%[^\n]", line);
			getchar();
		}


		line[strlen(line)] = '\n'; //terminate with new line
		tokens = tokenize(line);
        
        int le = 0;
        int counter = 0;
        int as1 = 0;
        int as2 = 0;
        int as3 = 0;
        int pos[64];

        for(i=0;tokens[i]!=NULL;i++,le++){
			continue;
		}
        
        while(counter < le)
        {
            if(strcmp(tokens[counter],"&")==0){pos[as1]=counter;as1++;}
            else if(strcmp(tokens[counter],"&&")==0){pos[as2]=counter;as2++;}
            else if(strcmp(tokens[counter],"&&&")==0){pos[as3]=counter;as3++;}
            counter++;
        }

        if((as1==0)&&(as2==0)&&(as3==0))
        {
            if(strcmp(tokens[0],"cd")==0)
            {   
                if(chdir(tokens[1])!=0)
                {
                    printf("Shell: Incorrect command\n");
                }
                
            }

            else if(strcmp(tokens[0],"exit")==0)
            {   

                for(i=0;tokens[i]!=NULL;i++){
			        free(tokens[i]);
	        	}
		        free(tokens);  
                

                i=0;
                if(BGF_==1){kill(BGID_,15);}
                //kill(0,SIGUSR1);        ///// CHECK

                int kapid,sata;
                kapid = waitpid(-1,&sata,0);
                if((kapid!=0)&&(kapid!=-1)){printf("Shell: Background process finished\n");}
                printf("Shell: Goodbye.\n");
                exit(0);
            }

            else
            {
                int ret = fork();
                pid_t cpid;
                
                if(ret==0){
                    execvp(tokens[0],tokens);
                    printf("Shell: Incorrect command\n");
                    exit(0);

                }
                else{
                    int t1;
                    cpid = waitpid(ret,&t1,0);          
                }

            }
        }

        if(as1>0)               ////////////////////// ADD SUpoort for cd A &
        {
            
            if(strcmp(tokens[0],"cd")==0)
            {   
                if(chdir(tokens[1])!=0)
                {
                    printf("Shell: Incorrect command\n");
                }
            }
            
            else
            {
                signal(SIGINT, SIG_IGN);
                tokens[pos[0]]='\0';
                int ret = fork();
                
                if(ret>0){BGF_ = 1;BGID_ = ret;}
                //printf("%d \n",BGF_);
                //printf("%d \n",BGID_);
                
                int cpid;

                if(ret==0){
                    //printf("AA %d\n",getpid());
                    execvp(tokens[0],tokens);
                    printf("Shell: Incorrect command\n");
                    exit(0);
                }
                else{
                    signal(SIGINT,main_sigint);
                    int stat;
                    cpid = waitpid(-1,&stat,WNOHANG);
                    if(cpid == ret){printf("Shell: Background process finished\n");BGF_=0;} 

                }
            }
            
        }
        

        if(as2>0)
        {
            for(int i=0;i<=as2;i++)
            {
                if(i==0){tokens[pos[i]]='\0';}
                else if(i!=as2)
                {
                    int tesp = pos[i]-pos[i-1];
                    for(int j=0;j<tesp;j++)
                    {
                        tokens[j] = tokens[pos[i-1]+j+1];
                    }
                    tokens[tesp-1]='\0';
                }
                else 
                {
                    int j=0;
                    while(tokens[pos[as2-1]+j+1]!=NULL)
                    {
                        tokens[j] = tokens[pos[as2-1]+j+1];
                        j++;
                    }
                    tokens[j]='\0';
                }
                
                //// NOW FORK OR CD or exit cmd

                if(strcmp(tokens[0],"cd")==0)
                {
                    if(chdir(tokens[1])!=0)
                    {
                        printf("Shell: Incorrect command\n");
                    }
                
                }

                else if(strcmp(tokens[0],"exit")==0)
                {   

                    for(i=0;tokens[i]!=NULL;i++){
			        free(tokens[i]);
                    }
                    free(tokens);  
                    

                    i=0;
                    if(BGF_==1){kill(BGID_,15);}
                    //kill(0,SIGUSR1);        ///// CHECK

                    int kapid,sata;
                    kapid = waitpid(-1,&sata,0);
                    if((kapid!=0)&&(kapid!=-1)){printf("Shell: Background process finished\n");}
                    printf("Shell: Goodbye.\n");
                    exit(0);
                }

                else
                {
                    int ret = fork();
                    pid_t cpid;

                    if(ret==0){
                        //signal(SIGINT, front_sigint);
                        execvp(tokens[0],tokens);
                        printf("Shell: Incorrect command\n");
                        exit(0);

                    }
                    else{
                        int t1;
                        cpid = waitpid(ret,&t1,0);  
                        if(WTERMSIG(t1)){break;}       
                        }

                }
            }
        }

        if(as3>0)
        {   

            int * pid_arr = malloc((as3+1)*sizeof(int));
            tracker = 0;
            pid_arr[0]=-2;
            for(int i=0;i<=as3;i++)
            {
                if(i==0){tokens[pos[i]]='\0';}
                else if(i!=as3)
                {
                    int tesp = pos[i]-pos[i-1];
                    for(int j=0;j<tesp;j++)
                    {
                        tokens[j] = tokens[pos[i-1]+j+1];
                    }
                    tokens[tesp-1]='\0';
                }
                else 
                {
                    int j=0;
                    while(tokens[pos[as3-1]+j+1]!=NULL)
                    {
                        tokens[j] = tokens[pos[as3-1]+j+1];
                        j++;
                    }
                    tokens[j]='\0';
                }
                

                if(strcmp(tokens[0],"cd")==0)
                {
                    if(chdir(tokens[1])!=0)
                    {
                        printf("Shell: Incorrect command\n");
                    }
                }

                else if(strcmp(tokens[0],"exit")==0)
                {   

                    for(i=0;tokens[i]!=NULL;i++){
			        free(tokens[i]);
	        	    }
		             free(tokens);  
                
                sleep(5); // WAIT FOR OTHER CHILDs TO RUN ONCE FIRST   
            

                i=0;
                if(BGF_==1){kill(BGID_,15);}
                if(FLAG_==1){kill(-(getpid()+10),14);}
                printf("ASAsda %d\n",getpid()+10);
                int kapid,sata;
                kapid = waitpid(-1,&sata,0);
                if(WIFSIGNALED(sata)){if(WTERMSIG(sata)==14){printf("Shell: Goodbye.\n");exit(0);}}
                if((kapid!=0)&&(kapid!=-1)){printf("Shell: Background process finished\n");}
                printf("Shell: Goodbye.\n");
                exit(0);

                }

                else
                {
                    
                    FLAG_ = 1;

                    signal(SIGINT,main_sigint);

                    pid_arr[tracker] = fork();
                    
                    if(pid_arr[0]!=-2){PGID_ = pid_arr[0];}


                    if(pid_arr[tracker]==0)
                    {
                        //printf("A %d\n",getpid());
                        int y = getpgid(getpid());
                        kill(y,SIGUSR2);    //RACECND++
                        setpgid(getpid(),y+10);
                        printf("%d\n",y+10);
                        //PGID_ = pid_arr[0];     
                        execvp(tokens[0],tokens);
                        printf("Shell: Incorrect command\n");
                        kill(y,SIGTSTP);    //FAILED
                        
                        exit(0);

                    }
                    tracker++;
                }
            }
            /*
            printf("%d\n",tracker);
            printf("%d\n",RACECDN);
            printf("%d\n",inccmd);
            printf("%d\n",pid_arr[0]);
            */

            //printf("H");
            while((RACECDN!=tracker) && (tracker>0))
            {
                sleep(1); // WAIT FOR CHILD TO RUN ONCE FIRST   
            }
            /*
            printf("%d\n",tracker);
            printf("%d\n",RACECDN);
            printf("%d\n",inccmd);
            printf("%d\n",pid_arr[0]);
            */
            for(int i=0;i<tracker-inccmd;i++)
            {   
                int t2;
                waitpid(-(getpid()+10),&t2,0);
            }

            free(pid_arr);

            FLAG_ = 0;
            PGID_ = 0;
            RACECDN = 0;
            tracker = 0;
            inccmd = 0;
        }
	
        kpid = waitpid(-1,&sta,WNOHANG);
        if((kpid!=0)&&(kpid!=-1)){printf("Shell: Background process finished\n");BGF_=0;}  

		for(i=0;tokens[i]!=NULL;i++){
			free(tokens[i]);
		}
		free(tokens);

	}
	return 0;
}
