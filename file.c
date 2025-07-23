#include<stdio.h>

int main()
{
    //if(gamestate==gameover)
    {
        FILE *fp;
        fp=fopen("Score.txt","w");
        if(fp==NULL)
        {
            perror("Error opening file");
            return 1;
        }
    
     char buffer[100]="GamerId:";
      char buffer2[100]="Score:";
        char buffer3[100]="High Score:";
    while (*buffer!='\0') 
    {
        printf("%s\n", buffer);
    }
    while (*buffer2!='\0') 
    {
        printf("%s\n", buffer);
    }
    while (*buffer3!='\0') 
    {
        printf("%s\n", buffer);
    }

    fclose(fp); 

    return 0;
}
}