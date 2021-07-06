#include <stdio.h>
#include <stdlib.h>  // rand(), srand()
#include <string.h>
#include <time.h>    // time()

#define NUMBER_OF_STRING 4
#define MAX_STRING_SIZE 10
#define MAX_TENTATIVAS 6

int n_tentativas = 0;
int n_acertos = 0;

const char* palavras[] = 
{ 
	"informacao",
	"sistemas",
	"ciencia",
	"computacao"
};

const char* get_palavra() {
	int i;
	int k;
 
	i = NUMBER_OF_STRING;
	k = 0;
	srand((unsigned)time(NULL));
	k = rand() % i;
	return palavras[k];
}

char* tentativas = "";

void desenha_boneco(int erros){
	printf( "+----------+\n" );
	printf( "|          |\n" );
	printf( "|          %s\n", erros > 0 ? "O" : " " );
	printf( "|         %s%s%s\n", 
		erros > 2 ? "/" : " ",
		erros > 1 ? "|" : " ",
		erros > 3 ? "\\" : " "
	);
	printf( "|         %s %s\n",
		erros > 4 ? "/" : " ",
		erros > 5 ? "\\" : " "
	);
	printf( "|\n" );
	printf( "|\n" );
	printf( "+\n" );
	printf( "\n" );
}

void atualiza_acertos(char acertos[], char letra, const char *palavra ){
	int palavra_length = strlen(palavra);
	for(int i = 0; i < palavra_length; i++){
		if(palavra[i] == letra){
			acertos[i] = letra;
			n_acertos++;
		}
	}
}

int is_letra_in_palavra(char letra, const char *palavra){
	int i = 0;
	int ate = strlen(palavra);
	int exists = 0;
	for(i = 0; i < ate; i++){
		if(palavra[i] == letra){
			exists = 1;
		}
	}
		printf( "%d \n", exists);
	return exists;
}

int main()
{
    char letra;

	const char* palavra = get_palavra();
	int palavra_length = strlen(palavra);

	char acertos [palavra_length];
	char erros [palavra_length];
	strcpy(acertos, palavra); 

	// preenche os caracteres do array de acertos com _
	for(int i = 0; i < strlen(acertos); i++){
		acertos[i] = '_';
		erros[i] = '_';
	}

	while(
		// usuario nao extrapolou numero de tentativas
		n_tentativas <= MAX_TENTATIVAS &&
		// usuario ainda nao acertou todas as letras
		n_acertos < palavra_length
	){  
		printf(" Você possui %d tentativas \n", MAX_TENTATIVAS - n_tentativas);

		printf( "Please enter a letter: \n" );
		scanf( " %c", &letra);

		if(is_letra_in_palavra(letra, palavra) == 1){
			atualiza_acertos(acertos, letra, palavra);
		}else{
			erros[n_tentativas] = letra;	
			n_tentativas++;
		}

		desenha_boneco(n_tentativas);

		printf("Acertos: %s\n", acertos);
		printf("Erros: %s\n", erros);
		
	}  

	if(n_acertos == palavra_length){
		printf(" PARABENS! Você escapou da forca \n");
	}else{
		printf(" GAME OVER! Mais sorte na próxima tentativa! \n");
	}

    return 0;
}