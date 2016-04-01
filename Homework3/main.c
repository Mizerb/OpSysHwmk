#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>





int main(int argc, char const *argv[])
{
  DIR           *d;
  struct dirent *dir;
  d = opendir(".");
  if (d)
  {
    while ((dir = readdir(d)) != NULL)
    {
      printf("%s\n", dir->d_name);
    }

    closedir(d);
  }
}