#include <sys/types.h>
#include <sys/socket.h>
#include <sys/file.h>

#include <netinet/in.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>

//Taken from the prof's 

#define BUFFER_SIZE 1024

#define DEL_COM 0
#define STO_COM 1
#define RDD_COM 2

/* DEFINE MY ERRORS !!! */
#define FILE_NOT_FOUND 1967
#define FILE_EXISTS 2001
#define FILE_OPEN_FAILED_W 787
#define FILE_OPEN_FAILED_R 8832
#define FILE_ERROR_WHILE_WRITING 88188
#define LOCK_FAILED 9999
#define COULD_NOT_READ 808 
#define ERROR_DUR_READ 9990
#define ERROR_WRITING 99399
#define SIZE_ERROR_READING 73773
#define ERROR_ON_DELETE 666
#define COULD_NOT_READ_MSG 2222
#define INVALID_BYTE_RANGE 9121


#define ENTER_ERROR( string ) sprintf(error_discript, "ERROR: %s\n", string )

#define REPORT_ERROR( ERROR , string) case ERROR: ENTER_ERROR(string); break  

int error_lookup(int error_num , char * error_discript)
{
    
    switch(error_num)
    {
        case 458: 
            ENTER_ERROR("Ferrari Fuck up");
            break;
        case 111:
            ENTER_ERROR("1111 is badd son");
            break;
        REPORT_ERROR( FILE_NOT_FOUND, "NO SUCH FILE");
        REPORT_ERROR( FILE_EXISTS , "Requested file already exists");
        REPORT_ERROR( FILE_OPEN_FAILED_W ,"Attempt to open file for writing failed" );
        REPORT_ERROR( FILE_OPEN_FAILED_R , "Failed to open file for reading");
        REPORT_ERROR( LOCK_FAILED, "unable to obtain lock on file");
        REPORT_ERROR( COULD_NOT_READ_MSG , "Could not read all specified bytes from message");       
        REPORT_ERROR( ERROR_DUR_READ , "Error while reading message data");
        REPORT_ERROR( FILE_ERROR_WHILE_WRITING, "Could not write all bytes to file" );
        REPORT_ERROR( ERROR_WRITING , "Encoutered error while writing file");
        REPORT_ERROR( INVALID_BYTE_RANGE, "INVALID_BYTE_RANGE");
        REPORT_ERROR( SIZE_ERROR_READING, "Failed to read all of requested bytes");
        REPORT_ERROR( ERROR_ON_DELETE , "Sys error while deleting file");
        default:
            ENTER_ERROR("I don't even know");
            break;
    }
    return 0;
}



int write_from_socket( FILE * socket, char * filename, int size)
{
    // CHECK FOR FILENAME already existing

    if( access( filename, F_OK | R_OK | W_OK ) == -1)
    {
        // FILE EXISTS
        return FILE_EXISTS;
        // do I close the socket? no that's later
    }


    char * file_content = calloc(size , sizeof(char));

    

    //check for calloc sucess

    int n = (int)fread( file_content , sizeof(char), size ,socket);

    if( n != size)
    {
        // we have error
        if( n > 0 )
        {
            free (file_content);
            return COULD_NOT_READ;
        }
        else
        {
            free (file_content);
            return ERROR_DUR_READ;
        }
    }

    FILE * outFile = fopen( filename , "w");
    //check for fopen sucess

    if( outFile == NULL)
    {
        // FILE OPEN FAILED
        fprintf( stderr, "File open for write failed, %s \n", filename);
        return FILE_OPEN_FAILED_W;
    }
    /*
    int res = (int)flock( outFile , LOCK_EX);
    if(  res != 0 ) 
    {
        // flock failed for some reason
        return LOCK_FAILED;
    }
    */
    // lock file for safty resons
    n = (int) fwrite( file_content, sizeof(char) , size , outFile);
    if( n != size)
    {
        if( n < 0 )
        {
            // real error
            //Todo: Find the errors, or report error?
            free (file_content);
        }
        else
        {
            //not all bytes written
            free (file_content);
            //flock(outFile, LOCK_UN);
            fclose( outFile);
            return FILE_ERROR_WHILE_WRITING;
        }
    }

    //Remove file lock 
    free (file_content);
   // flock(outFile, LOCK_UN);
    fclose(outFile);
    //Todo: anything else?

    //exit with success
    return 0;
}

int read_to_socket( FILE * socket, char * filename , int start, int length)
{
    if( access( filename, F_OK | R_OK  ) != -1)
    {
        //FILE DOES NOT EXIST, THROW A TANTURNM
        return FILE_NOT_FOUND;
    }

    FILE* read_file = fopen( filename ,"r");

    if( read_file == NULL )
    {
        //file could not be opened;
        return FILE_OPEN_FAILED_R;
    }
    //get size of file, and check that start and length will work
    fseek(read_file, 0L , SEEK_END);
    int size = ftell(read_file);
    if( start + length > size)
    {
        fclose(read_file);
        return INVALID_BYTE_RANGE;
    }

    char * content = calloc( length , sizeof(char));

    fseek(read_file, (long)start , SEEK_SET);

    size = (int)fread( content, sizeof(char) ,length , read_file);

    if( size != length)
    {
        if( size > 0 )
        {
            free(content);
            fclose(read_file);
            return SIZE_ERROR_READING;
        }
        else 
        {
            // real error
            free (content);
            fclose (read_file);
            return 11;
        }
        
    }

    return 0;
}

int delete_file( FILE * socket , char * filename )
{
    if( access( filename, F_OK | R_OK  ) != -1)
    {
        //FILE DOES NOT EXIST, THROW A TANTURNM
        return FILE_NOT_FOUND;
    }
    int ret = remove(filename);
    if( ret != 0 )
    {
        //error while deleting file
        return ERROR_ON_DELETE;
    }
    return 0;
}

struct cmd_content
{
    char * filename;
    int command;
    int size;
    int start;
};

struct cmd_content get_command(int newsock , char start)
{
    struct cmd_content Ret; char * cmd_string; char test[1024]; 
    char BUFFER[1024]; char* a = BUFFER+1; int n = 2;
    BUFFER[0] = start;
    do
    {
        int s = recv( newsock, a , 1, 0 );
        n++;
        if( s< 0)
        {
            //Connetion closed
            exit(1);
        }

    }while( a != '\n');

    BUFFER[n] = '\0';

    //sscanf(BUFFER,"%s", cmd_string );

    if( strstr(BUFFER, "DELETE") == BUFFER )
    {
        sscanf(BUFFER, "DELETE %s\n", test);
        Ret.command = DEL_COM;
        sprintf(cmd_string, "DELETE %s", Ret.filename);
    }
    else if( strstr(BUFFER, "STORE") == BUFFER)
    {
        sscanf(BUFFER , "STORE %d\n" , test)
        Ret.command = STO_COM;
        strcpy( Ret.filename, test ,strlen(test));
        
        sprintf(cmd_string, "STORE %s %d", Ret.filename , Ret.size);
    }
    else if (strstr(BUFFER, "READ") == BUFFER)
    {
        sscanf(BUFFER, "READ %s %d %d\n", test , Ret.size , Ret.start);
        Ret.command = RDD_COM;
        strcpy( Ret.filename, test ,strlen(test));

        sprintf(cmd_string, "READ %s %d %d", Ret.filename ,Ret.start, Ret.size);
    }
    else
    {
        //COMMAND UNKOWN
        // CANNOT PARSE THIS SHIT
        Ret.command = -1;
        sprintf(BUFFER,"%s", cmd_string);
    }

    printf("[child %d] Rcvd: %s\n", getpid(),cmd_string);
    //printf("[child %d] Rcvd: %s\n", getpid(), BUFFER);

    return Ret;
}

int send_ACK( int newsock , int data_size)
{
    int n , size; char * msg, * extra;
    if( data_size == -1)
    {
        sprintf(msg,"ACK\n"); size = 4;
    }
    else
    {
        sprintf(extra , "%d\n" , data_size);
        size = strlen(extra)+4;
        sprintf(msg, "ACK %s", extra);
    }

    n = send( newsock, msg, size, 0 );
    fflush( NULL );
    if ( n != size)
      perror( "send() failed\n" );
    
    printf("[child %d] Sent: %s\n", getpid(), msg);
}

int send_Error(int newsock , char * error_msg)
{
    int n, size;
    size = strlen(error_msg);

    n = send( newsock, error_msg, size, 0 );
    fflush( NULL );
    if ( n != size)
      perror( "[child %d] send() failed\n" );
    else 
        printf("[child %d] Sent: %s\n", getpid(), error_msg);
}

void Read( int newsock , struct cmd_content cmd)
{
    int err, start, size;
    FILE * temp;
    
    err = read_to_socket(temp , cmd.filename, start, size);
    if( err != 0 )
    {
        char * error_msg;
        error_lookup(err , error_msg);
        send_Error( newsock, error_msg);        
    }
    else
    {
        printf("[child %d] Deleted '%s' from server\n", 
            getpid(), cmd.filename);

        send_ACK(newsock, size);
    }
}

void Write( int newsock ,struct cmd_content cmd)
{
    int err, size;
    FILE * temp;

    err = write_from_socket( temp, cmd.filename , size);
    if( err != 0 )
    {
        char * error_msg;
        error_lookup(err , error_msg);
        send_Error( newsock, error_msg);        
    }
    else
    {
        printf("[child %d] Stored file '%s' (%d bytes)\n", getpid() ,
         cmd.filename, size);

        send_ACK(newsock, -1);
    }
}

void Delete( int newsock , struct cmd_content cmd)
{
    int err; FILE * temp;

    err = delete_file(temp, cmd.filename );

    if( err != 0 )
    {
        char * error_msg;
        error_lookup(err , error_msg);
        send_Error( newsock, error_msg);        
    }
    else
    {
        printf("[child %d] Deleted '%s' from server\n", 
            getpid(), cmd.filename);

        send_ACK(newsock, -1);
    }
}


void Run_Connection(int newsock)
{
    //Stepping on the beach 
    char BUFFER[256]; int n; char start;
    do
    {
        n = recv(newsock , &start , 1 , 0);
        struct cmd_content cmd = get_command(newsock , start);
        if(cmd.command == STO_COM)
        {
            //store
            Write(newsock , cmd);
        }
        else if( cmd.command == RDD_COM)
        {
            Read(newsock, cmd);
        }
        else if( cmd.command == DEL_COM)
        {
            Delete( newsock, cmd);
        }
        else
        {
            //command not rec
            send_Error(newsock, "Error: Command not usable\n");
        }
        
    }while( n > 0);
}


int main(int argc, char  *argv[])
{
    /* I don't know */
    
    int sockfd, newsockfd, portno, clilen , pid , n;
    struct sockaddr_in serv_addr, cli_addr;
    
    sockfd = socket( PF_INET, SOCK_STREAM, 0 );


    if ( sockfd < 0 )
    {
        perror( "socket() failed" );
        exit( EXIT_FAILURE );
    }

    serv_addr.sin_family = PF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;

    int port = 9753;
    serv_addr.sin_port = htons(port);

    if ( bind( sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr) ) < 0 )
    {
        perror( "bind() failed" );
        exit( EXIT_FAILURE );
    }

    listen( sockfd , 5);

    clilen = sizeof(cli_addr);

    printf("Listening on port %d\n" , port);

    while(1)
    {
        int newsock = accept(sockfd, (struct sockaddr *)&cli_addr,
                              &clilen);

        printf("Received incoming connection from %s\n", 
            inet_ntoa( (struct in_addr)cli_addr.sin_addr));

        pid = fork();

        if( pid <0)
        {
            fprintf(stderr, "ERROR: forking failed\n");
            pid = send( newsock, "ERROR: FORK FAILED\n", 19, 0 );
            return EXIT_FAILURE;
        }
        if( pid ==0 )
        {
            Run_Connection( newsock);
        }
        else
        {
            close ( newsock );
        }
    }
    close(sockfd);
    return 0;
}