#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

void parseCommand(char *command, char *components[1000])
{   
    char *ptr = strtok(command, " :");
    int i = 0;

	while(ptr != NULL)
	{
        components[i++] = ptr;
        ptr = strtok(NULL, " :");
	}

}

int check(char *s, char c)
{
    int i, count = 0;

    for (i = 0; s[i]; i++)
    {
        if (s[i] == c)
            count++;
    }

    return count;
}

void removeChar(char *str, char c)
{
    int i, j;
    int len = strlen(str);

    for (i = 0; i < len; i++)
    {
        if (str[i] == c)
        {
            for (j = i; j < len; j++)
            {
                str[j] = str[j+1];
            }
            len--;
            i--;
        }
    }
}

void look (char *file, char *command, char* filepath, int *flag)
{
    struct dirent *dp;
    DIR *dir;

    dir = opendir(file);    
    
    if(dir != NULL)
    {
        dp = readdir(dir);
    }
    
    
    if (strcmp(file,".") == 0) {}
    
    else
    {
        strcat(filepath,"/");
    }
    

    for(;dp != NULL; dp = readdir(dir))
    {
        if(strcmp(command, dp -> d_name) == 0)
        {
            strcat(filepath,dp -> d_name);
            printf("%s\n", filepath);
            *flag = 1;
            break;
        }
    }

    closedir(dir);

    struct dirent *dp1;
    DIR *dir1;

    dir1 = opendir(file);

    if(dir1 != NULL)
    {
        dp1 = readdir(dir1);
    }    

    for(;dp1 != NULL; dp1 = readdir(dir1))
    {
        if(dp1 -> d_type == DT_DIR && strcmp(".vscode", dp1 -> d_name) != 0 && 
                            strcmp(".", dp1 -> d_name) != 0 && strcmp("..", dp1 -> d_name) != 0)
        {
            strcpy(filepath, file);
            strcat(filepath, "/");        
            strcat(filepath,dp1 -> d_name);
            strcpy(dp1 -> d_name, filepath);
            look(dp1 -> d_name, command, filepath, flag);
        }
    }

    closedir(dir1);

}

int main()
{
    char *commandComponent[1000] = {NULL};
    char tmp;
    char *userInput = (char*)malloc(10000);
    char *text = (char*)malloc(100);
    char *filepath = (char*)malloc(100);
    int flagQuit = 0;

    do
    {
        printf("\n\033[0;34m");
        printf("findstuff");
        printf("\033[0m");
        printf("$");
        
        scanf("%[^\n]", userInput);
        scanf("%c", &tmp);

        parseCommand(userInput, commandComponent);
        
        if (strcmp(commandComponent[0], "find") == 0)
        {
            //Look for text in files.
            if (strchr(commandComponent[1], '"') != NULL)
            {
                //check if the text has spaces in it. 
                if (check(commandComponent[1], '"') != 2) 
                {
                    int i;
                    for (i = 1; i < 1000; i++)
                    {                        
                        strcat(text, commandComponent[i]);
                        
                        if (check(text, '"') == 2)
                        {
                            removeChar(text, '"');
                            break;
                        }

                        strcat(text, " ");
                    }
                    
                }

                //text doesn't have spaces
                else
                {    
                    strcpy(text,commandComponent[1]);
                    removeChar(text, '"');
                }

                //flag s is set
                if ((commandComponent[2] != NULL && strcmp(commandComponent[2], "-s") == 0) 
                    ||  (commandComponent[4] != NULL && strcmp(commandComponent[4], "-s") == 0))
                {
                    
                }

                printf("Still working on the implementation...\n");
            
            }

            //Look for files. 
            else
            {
                //s flag is set. look for the file within the sub directories
                if (commandComponent[2] != NULL && strcmp(commandComponent[2], "-s") == 0)
                {   
                    int* flag;
                    int b;
                    flag = &b;
                    *flag = 0;

                    strcpy(filepath, "./");
                    look(".", commandComponent[1] , filepath, flag);
                    
                    if (*flag == 0)
                    {
                        printf("find: '%s': No such file or directory\n",commandComponent[1]);
                    }
                }

                //look for the file only in the current directory
                else
                {
                    int flag = 0;
                    DIR *dir;
                    struct dirent *dp;
                    dir = opendir(".");
                    
                    dp = readdir(dir);

                    for(;dp != NULL; dp = readdir(dir))
                    {
                        if(strcmp(commandComponent[1], dp -> d_name) == 0)
                        {
                            strcpy(filepath, "./"); 
                            strcat(filepath,commandComponent[1]);
                            flag = 1;
                            break;
                        }
                    }

                    if (flag == 1)
                    {
                        printf("%s\n",filepath);
                    }
                    
                    else 
                    {
                        printf("find: '%s': No such file or directory\n",commandComponent[1]);
                    }

                    closedir(dir); 
                                
                }
                
            }

            for (int i = 0; i < 1000; i++)
            {
                commandComponent[i] = 0;
            }
        }

        else if(strcmp(commandComponent[0], "q") == 0 || strcmp(commandComponent[0], "quit") == 0)     
        {
            flagQuit = 1;
        }

        else
        {
            printf("Still working on the implementation...\n");
        }
        
        
    } while (flagQuit != 1);
    
    free(filepath);
    free(text);
    free(userInput);
    return 0;
}