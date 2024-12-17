#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>

int main(){
    int num;
    FILE*fpointer;
    fpointer = fopen("file1.dat","r");
    int array[1000];
    for(int i = 0; i<1000;i++){
        fscanf(fpointer, "%d",&array[i]);
    }
    fclose(fpointer);
    int numOfLines = sizeof(array)/sizeof(int);
    int count = 0;
    int fd[2];
    int begin;
    int end;
    int total;
    int childProcesses;
    printf("Enter the number of child processes: \n");
    scanf("%d", &childProcesses);
    if(childProcesses==1){
        if(pipe(fd)==-1){
            return 1;
        }
        int id = fork();
        if(id==-1){
            return 2;
        }

        if(id==0){
            begin=0;
            end = numOfLines;
            for(int i = begin; i<end; i++){
                count = count + array[i];
            }   
            printf("calculated sum : %d\n",count); 
        
        }
        if(id==0){
            close(fd[0]);
            write(fd[1],&count,sizeof(count));
            close(fd[1]);
        }
        else {
            close(fd[1]);
            read(fd[0], &count, sizeof(count));
            close(fd[0]);


            printf("Total sum: %d \n", count);
            wait(NULL);
            exit(0);
        }
    
    }
    if(childProcesses==2){
        if(pipe(fd)==-1){
            return 1;
        }
        int id = fork();
        int id2 = fork();
        if(id==-1){
            return 2;
        }
        if(id==0){
            if(id2==0){
                begin= numOfLines/2;
                end = numOfLines;
                for(int i = begin; i<end; i++){
                    count = count + array[i];
                }   
            } 
            else{
                if(id==0){
                    begin= 0;
                    end = numOfLines/2;
                    for(int i = begin; i<end; i++){
                        count = count + array[i];
                    }   
                    if(id==0){
                        close(fd[0]);
                        write(fd[1],&count,sizeof(count));
                        close(fd[1]);
                     }
                    else{
                        int childSum;
                        close(fd[1]);
                        read(fd[0], &childSum, sizeof(childSum));
                        close(fd[0]);
        
                        int total = count + childSum;
                        //printf("%d \n", total);
                        wait(NULL);
                        exit(0);
                    }
                }
            } 
        }    
        else{
            if(id==0){
                close(fd[0]);
                write(fd[1],&count,sizeof(count));
                close(fd[1]);
            }
            else{
                int childSum;
                close(fd[1]);
                read(fd[0], &childSum, sizeof(childSum));
                close(fd[0]);

                int total = count + childSum;
                //printf("%d \n", total);
                wait(NULL);
                exit(0);
            }
        }
    printf("calculated partial sum: %d\n",count);
    }
    if(childProcesses==4){
        int count2 = 0;
        if(pipe(fd)==-1){
            return 1;
        }
        int id = fork();
        int id2 = fork();

        if(id==-1){
            return 2;
        }
        if(id==0){
            if(id2==0){
                begin = numOfLines/2;
                end = numOfLines-(numOfLines/4);
                for(int i = begin; i<end; i++){
                    count = count + array[i];
                } 

            } 
            else {
                if(id==0){
                    begin=0;
                    end = numOfLines/4;
                    for(int i = begin; i<end; i++){
                        count = count + array[i];
                    } 
                }
            }
        }
        else {
                if(id2==0){
                    begin = numOfLines-(numOfLines/4);
                    end = numOfLines;
                    for(int i = begin; i<end; i++){
                        count = count + array[i];
                    }
                    close(fd[0]);
                    write(fd[1],&count,sizeof(count));
                    close(fd[1]);
                }
                else {
                    int childSum2;
                    int id3 = fork();
                    if (id3 == 0){
                        // This is child 4
                        begin = numOfLines/4;
                        end = numOfLines/2;
                        for(int i = begin; i<end; i++){
                            count = count + array[i];
                        }
                        close(fd[0]);
                        write(fd[1],&count,sizeof(count));
                        close(fd[1]);
                    }
                    else {
                        int childSum;
                        close(fd[1]);
                        read(fd[0], &childSum, sizeof(childSum));
                        close(fd[0]);

                        int total1 = count + childSum;
                        wait(NULL);
                        printf("total: %d\n",total1);
                        exit(0);
                    }
                }
        } 
        printf("calculated partial sum: %d\n",count);
    }
}
