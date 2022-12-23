#include <stdio.h>
#include <stdlib.h> 
#include <unistd.h>
#include <string.h>

typedef unsigned char byte; 

typedef struct mypipe
{
    byte* pipebuffer;
    int buffersize;
    int start_occupied;
    int end_occupied;

} mypipe;


//initializes (malloc) the pipe with a size of "size" and sets start and end.
void init_pipe(mypipe *pipe,int size)
{
    pipe->pipebuffer = (byte*)malloc(size);
    int *p = (&pipe->buffersize);
    int *p1 = (&pipe->start_occupied);
    int *p2 = (&pipe->end_occupied);    
    *p = size;
    *p1 = 0;
    *p2 = 0;
}

//writes "size" bytes from buffer into the pipe, returns size
int mywrite(mypipe *pipe,byte *buffer,int size)
{   
    int i = 0;
    
    if (size <= (pipe->buffersize - pipe->end_occupied))
    {
        for (i = pipe->end_occupied; i < (size+pipe->end_occupied);)
        {
            for (int j = 0; j < size; j++)
            {
                pipe->pipebuffer[i] = buffer[j];
                i++;
            }

            pipe->pipebuffer[i-1] = '\0';
        }

        pipe->end_occupied += size;
    }

    else
    {
        int bytesWritten = pipe->buffersize - pipe->end_occupied;

        for (i = pipe->end_occupied; i < pipe->buffersize;)
        {
            for (int j = 0; j < (bytesWritten); j++)
            {
                pipe->pipebuffer[i] = buffer[j];
                i++;
            }
        }
        
        pipe->end_occupied = 0; 

        for (i = pipe->end_occupied; i < size- bytesWritten;)
        {
            for (int j = bytesWritten; j < size; j++)
            {
                pipe->pipebuffer[i] = buffer[j];
                i++;
            }

            pipe->pipebuffer[i-1] = '\0';
        }

        pipe->end_occupied += size-bytesWritten; 

    }
    
    return size;
}

//reads "size" bytes from pipe into buffer, returns how much it read (max size), 0 if pipe is empty
int myread(mypipe *pipe,byte *buffer,int size)
{
    if (size <= (pipe->buffersize - pipe->start_occupied))
    {
        for (int j = 0; j < size;)
        {
            for (int i = pipe -> start_occupied; i < (pipe -> start_occupied+size); i++)
            {
                buffer[j] = pipe->pipebuffer[i];
                j++;
            }
        }

        pipe->start_occupied += size;
    }

    else
    {
        int bytesRead = pipe->buffersize - pipe->start_occupied;

        for (int j = 0; j < bytesRead;)
        {
            for (int i = pipe -> start_occupied; i < pipe->buffersize; i++)
            {
                buffer[j] = pipe->pipebuffer[i];
                j++;
            }
        }

        pipe->start_occupied = 0;

        for (int j = bytesRead; j < size;)
        {
            for (int i = pipe -> start_occupied; i < (size-bytesRead); i++)
            {
                buffer[j] = pipe->pipebuffer[i];
                j++;
            }
        }

        pipe->start_occupied += size-bytesRead;

    }
    
    return size;
}


int main()
{
    char text[100];
    mypipe pipeA;
    
    init_pipe(&pipeA, 32);
    mywrite(&pipeA, "hello world", 12);
    mywrite(&pipeA, "it's a nice day", 16);

    myread(&pipeA, text, 12);
    printf("%s\n", text);
    myread(&pipeA, text, 16);
    printf("%s\n", text);
    
    mywrite(&pipeA, "and now we test the carryover", 30);
    myread(&pipeA, text, 30);
    printf("%s\n", text);

    return 0;
}