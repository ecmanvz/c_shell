
#include <sys/wait.h> 
#include <unistd.h>   
#include <stdlib.h>   
#include <stdio.h>    
#include <string.h>   

#define DELIM_BOS " "
#define DELIM_NOKVIRGUL ";"

char *satirOku()
{
	
	char *line = (char *)malloc(sizeof(char) * 512); 
	char c;
	int pos = 0, bufsize = 512;
	if (!line) 
	{
		printf("\nBuffer Allocation Error.");
		exit(EXIT_FAILURE);
	}
	while(1)
	{
		c=getchar();
		if (c == EOF || c == '\n') 
		{
			line[pos] = '\0';
			return line;
		}
		else
		{
			line[pos] = c;
		}
		pos ++;
		
		if (pos >= bufsize)
		{
			bufsize += 1024;
			line = realloc(line, sizeof(char) * bufsize);
			if (!line) 
			{
			printf("\nBuffer Allocation Error.");
			exit(EXIT_FAILURE);
			}
		}
	}
}

void alt_satira_inmeyi_sil(char *str){

	   int count = 0;

		for(int i = 0; str[i]; i++)
			if(str[i] != '\n')
				str[count++] = str[i];

		str[count] = '\0';

}

char **bol_satir_noktalivirgul(char *line)
{
	char **tokens = (char **)malloc(sizeof(char *) * 64);
	char *token;
	int pos = 0, bufsize = 64;
	if (!tokens)
	{
		printf("\nBuffer Allocation Error.");
		exit(EXIT_FAILURE);
	}
	token = strtok(line, DELIM_NOKVIRGUL);
	while (token != NULL)
	{
		alt_satira_inmeyi_sil(token);
		tokens[pos] = token;
		pos ++;
		if (pos >= bufsize)
		{
			bufsize += 64;
			line = realloc(line, bufsize * sizeof(char *));
			if (!line) 
			{
			printf("\nBuffer Allocation Error.");
			exit(EXIT_FAILURE);
			}
		}
		token = strtok(NULL, DELIM_NOKVIRGUL);
	}
	tokens[pos] = NULL;
	return tokens;
}

char **bol_satir_bosluk(char *line)
{
	char **tokens = (char **)malloc(sizeof(char *) * 64);
	char *token;
	int pos = 0, bufsize = 64;
	if (!tokens)
	{
		printf("\nBuffer Allocation Error.");
		exit(EXIT_FAILURE);
	}
	token = strtok(line, DELIM_BOS);
	while (token != NULL)
	{
		alt_satira_inmeyi_sil(token);
		tokens[pos] = token;
		pos++;
		if (pos >= bufsize)
		{
			bufsize += 64;
			line = realloc(line, bufsize * sizeof(char *));
			if (!line) 
			{
			printf("\nBuffer Allocation Error.");
			exit(EXIT_FAILURE);
			}
		}
		token = strtok(NULL, DELIM_BOS);
	}
	tokens[pos] = NULL;
	return tokens;
}

int noktalivirgul_varmi(char **line){

    if (line == NULL)
    {
        fprintf(stderr, "Hata: satir bos\n");
        exit(0);
    }

    int temp = 0;
    while (line[temp] != NULL)
    {
        temp++;
    }
    
    return temp;
    
}

int karnele_gonder(char **args)
{

	pid_t pid, wpid;
	int status;
	pid = fork();
	if (pid == 0)
	{
		// The Child Process
		if (execvp(args[0], args) == -1)
		{
			perror("shell hatasi: ");
		}
	exit(EXIT_FAILURE);

	 execvp(args[0], args);
        fprintf(stderr, "Hata: hatalı kod girdiniz \n");
        printf("%s bu kod calistirilamadid \n", args[0]);
        exit(0);
	}
	else if (pid < 0)
	{
		//Forking Error
		perror("shell hatasi: ");
	}
	else
	{
		// The Parent Process
	do 
	{
      wpid = waitpid(pid, &status, WUNTRACED);
    } while (!WIFEXITED(status) && !WIFSIGNALED(status));
	}
	return 1;
}

int karnele_coklu_gonder(char **args){

    if (args == NULL)
    {
        fprintf(stderr, "Hata: arguman yok \n");
        exit(0);
    }

    int get_komut = noktalivirgul_varmi(args);
    char** tek_komut;

    for (int  i = 0; i < get_komut; i++)
    {
        tek_komut = bol_satir_bosluk(args[i]);

        karnele_gonder(tek_komut);
    }
    

}

int dosya_okuma(char *dosya_yolu)
{
	FILE *fptr;
	char line[512];
	char **args;

    char **vir_bol;
    char **bos_bol;
    int coklucalisma = 0, i=0;
	fptr = fopen(dosya_yolu, "r");

	if (fptr == NULL)
	{
		fprintf(stderr, "Hata: dosya yok yada acilmadi \n");
        return EXIT_FAILURE;
	}
	else
	{
		while(fgets(line, sizeof(line), fptr) != NULL)
		{
            printf("\n%s", line);
            vir_bol = bol_satir_noktalivirgul(line);
			coklucalisma = noktalivirgul_varmi(vir_bol);

			if (coklucalisma > 1)
			{
				karnele_coklu_gonder(vir_bol);
			}
			else 
			{
				while (vir_bol[i] != NULL)
				{
					bos_bol = bol_satir_bosluk(vir_bol[i++]);
					karnele_gonder(bos_bol);
				}i=0;
				
				
			}
		}
	}
	
	fclose(fptr);
	return 1;
}

int cikis_yap(char *kod){

    if (strcmp(kod, "quit") == 0)
    {
        return 0;
    }

    return 1;
}

int main(int argc, char* argv[]){

    char *satir;
    char **vir_bol;
    char **bos_bol;
    int durum = 1,coklucalisma,i = 0;

    if (argc > 2)
    {
        fprintf(stderr, "Hata: argumani fazla girdinizi");
        exit(1);
    }
    else if (argc == 2)
    {
        dosya_okuma(argv[1]);
    }

    do
    {
        printf("shell> ");
        satir = satirOku();
        vir_bol = bol_satir_noktalivirgul(satir);

        coklucalisma = noktalivirgul_varmi(vir_bol);

        if (coklucalisma > 1)
        {
            karnele_coklu_gonder(vir_bol);
        }
        else
        {

			while (vir_bol[i] != NULL)
			{
				bos_bol = bol_satir_bosluk(vir_bol[i++]);
				durum = cikis_yap(bos_bol[0]);
				if(!durum) break;
				karnele_gonder(bos_bol);   
			}
        }
        

    } while (durum);

	free(satir);
	free(bos_bol);
	free(vir_bol);
}