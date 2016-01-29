#include <stdio.h>
#include <stdlib.h>
#include <string.h>



int main(int argc, char const *argv[])
{
	char meh[200] = "am";

	char * pch  = strstr( meh , argv[1]);

	if(pch == NULL) printf("IT WORKS MARTY\n");
	else printf(" MARTY, IT's ALIVE\n");

	printf("How does strlen work? %d\n", (int)strlen(meh));

	char* aa = (char*) malloc( (strlen(meh)*sizeof(char))+1);

	strcpy( aa , meh);

	printf("How does Copy work? %s\n", aa);

	return 0;
}