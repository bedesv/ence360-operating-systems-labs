#include <stdio.h>    
#include <stdlib.h>    
#include <unistd.h>   
#include <ctype.h>    
                      

void translator(int input_pipe[], int output_pipe[]) 
{ 
    int c;     
    char ch;   
    int rc;    

    /* first, close unnecessary file descriptors */ 
	close(input_pipe[1]); // Close unneeded pipe input
    close(output_pipe[0]);// Close unneeded pipe output

    /* enter a loop of reading from the user_handler's pipe, translating */ 
    /* the character, and writing back to the user handler.              */ 
    while (read(input_pipe[0], &ch, 1) > 0) { 
        c = ch;
        if (isascii(c) && isupper(c)) // if character is ascii and upper case
            c = tolower(c); // Convert character to lowercase
        ch = c; 

        /* write translated character back to user_handler. */ 
        rc = write(output_pipe[1], &ch, sizeof(char));
		
		if (rc == -1) { /* write failed - notify the user and exit. */
            perror("translator: write"); 
            close(input_pipe[0]); 
            close(output_pipe[1]); 
            exit(1); 
        } 
    } 

    // Close open ends of pipes and exit program
    close(input_pipe[0]); 
    close(output_pipe[1]); 
    exit(0); 
} 



void user_handler(int input_pipe[], int output_pipe[]) 
{ 
    int c;    /* user input - must be 'int', to recognize EOF (= -1). */ 
    char ch;  /* the same - as a char. */ 
    int rc;   /* return values of functions. */ 

    /* first, close unnecessary file descriptors */
    close(input_pipe[1]); // Close unneeded pipe input
    close(output_pipe[0]);// Close unneeded pipe output

	printf("Enter text to translate:\n");
    /* loop: read input from user, send via one pipe to the translator, */ 
    /* read via other pipe what the translator returned, and write to   */ 
    /* stdout. exit on EOF from user.                                   */ 
    while ((c = getchar()) > 0) { 
        ch = (char)c; 

        /* write to translator */ 
		rc = write(output_pipe[1], &ch, sizeof(char));

        if (rc == -1) { /* write failed - notify the user and exit. */ 
            perror("user_handler: write"); 
            close(input_pipe[0]); 
            close(output_pipe[1]); 
            exit(1); 
        } 

        /* read back from translator */ 
		rc = read(input_pipe[0], &ch, 1);

        c = (int)ch; 
        if (rc <= 0) {  /* read failed - notify the user and exit. */
            perror("user_handler: read"); 
            close(input_pipe[0]); 
            close(output_pipe[1]); 
            exit(1); 
        } 

		putchar(c); // Write character to standard output
		if (c=='\n' || c==EOF) break; // Exit loop if newline character or EOF reached
    }
    // Close open ends of pipes and exit program
    close(input_pipe[0]); 
    close(output_pipe[1]); 
    exit(0); 
} 




int main(int argc, char* argv[]) 
{
    // Initialise pipes, process id and rc
    int user_to_translator[2]; 
    int translator_to_user[2]; 
    int pid;        
    int rc;         

    rc = pipe(user_to_translator); // Initialise user to translator pipe
    if (rc == -1) { /* pipe creation failed - notify the user and exit. */
        perror("main: pipe user_to_translator"); 
        exit(1); 
    } 

	rc = pipe(translator_to_user); // Initialise translator to user pipe
    if (rc == -1) { /* pipe creation failed - notify the user and exit. */
        perror("main: pipe translator_to_user"); 
        exit(1); 
    } 

    pid = fork(); // Fork another process for translator

    switch (pid) { 
        case -1: // Error forking process, print error and exit
            perror("main: fork"); 
            exit(1); 
        case 0: // Child code, run the translator code
            translator(user_to_translator, translator_to_user); /* line 'A' */ 
			exit(0);
        default: // Parent code, run the user handler
            user_handler(translator_to_user, user_to_translator); /* line 'B' */ 
    } 

    return 0;   
}



/* Show below the output for the following input: I wish YOU a HAPPY new YEAR

    i wish you a happy new year
*/