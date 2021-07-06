#include <stdio.h>
#include <stdlib.h>  // rand(), srand()
#include <string.h>
#include <time.h>    // time()
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

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

void desenha_boneco(char * boneco, int erros){
	char cabeca[] = "";
	char besquerdo[] = "";
	char tronco[] = "";
	char bdireito[] = "";
	char pesquerdo[] = "";
	char pdireito[] = "";

	if(erros > 0){
		strcat(cabeca, "O" );
	}
	if(erros > 1){
		strcat(besquerdo, "/" );
	}
	if(erros > 2){
		strcat(tronco, "|" );
	}
	if(erros > 3){
		strcat(bdireito, "b" );
	}
	if(erros > 4){
		strcat(pesquerdo, "/" );
	}
	if(erros > 5){
		strcat(pdireito, "p" );
	}

	strcat(boneco, "+----------|\n" );
	strcat(boneco, "|          |\n" );
	strcat(boneco, "|          "  );
	strcat(boneco, cabeca );
	strcat(boneco, "\n"  );
	
	strcat(boneco, "|         ");
	strcat(boneco, besquerdo);
	strcat(boneco, tronco);
	strcat(boneco, bdireito);
	strcat(boneco, "\n"  );
	
	strcat(boneco, "|         ");
	strcat(boneco, pesquerdo);
	strcat(boneco, "´");
	strcat(boneco, pdireito);
	strcat(boneco, "\n"  );
	
	strcat(boneco, "|\n" );
	strcat(boneco, "|\n" );
	strcat(boneco, "+\n" );
	strcat(boneco, "\n" );
	// for(int i = 0; i < 1000; i++){
	// 	boneco[i] = boneco2[i];
	// }
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
    // char letra;

	// inicia as variaveis relativas ao socket
	int server_sockfd, client_sockfd;
    int server_len, client_len;
    struct sockaddr_un server_address;
    struct sockaddr_un client_address;
    char ch;
	
	// configura socket
	server_sockfd = socket(AF_UNIX, SOCK_STREAM, 0);
    server_address.sun_family = AF_UNIX;
    strcpy(server_address.sun_path, "server_socket");
    server_len = sizeof(server_address);
    bind(server_sockfd, (struct sockaddr *)&server_address, server_len);
    listen(server_sockfd, 5);

	//inicia as variaveis referentes a regra de negócio do sistema
	// const char* palavra = get_palavra();
	// int palavra_length = strlen(palavra);
	// char acertos [palavra_length];
	// char erros [palavra_length];
	// strcpy(acertos, palavra); 

	// preenche os caracteres do array de acertos com _
	// for(int i = 0; i < strlen(acertos); i++){
	// 	acertos[i] = '_';
	// 	erros[i] = '_';
	// }

	while(
		// usuario nao extrapolou numero de tentativas
		// n_tentativas <= MAX_TENTATIVAS &&
		// usuario ainda nao acertou todas as letras
		// n_acertos < palavra_length
		1
	){  
		// printf(" Você possui %d tentativas \n", MAX_TENTATIVAS - n_tentativas);

		// printf( "Please enter a letter: \n" );
		// scanf( " %c", &letra);

		printf("Server waiting\n");
        client_len = sizeof(struct sockaddr_un);
        client_sockfd = accept(server_sockfd, (struct sockaddr *)&client_address, &client_len);
        read(client_sockfd, &ch, 1);
        ch++;
        
		// if(is_letra_in_palavra(ch, palavra) == 1){
		// 	atualiza_acertos(acertos, ch, palavra);
		// }else{
		// 	erros[n_tentativas] = ch;	
		// 	n_tentativas++;
		// }

		// char boneco[] = "";
		// // desenha_boneco(boneco, n_tentativas);

		// strcat(boneco, "Acertos: ");
		// strcat(boneco, acertos);
		// strcat(boneco, "\n");

		// strcat(boneco, "Erros: ");
		// strcat(boneco, erros);
		// strcat(boneco, "\n");

		write(client_sockfd, &ch, 1);
        close(client_sockfd);

		// printf("%s\n", boneco);
	}  

	// if(n_acertos == palavra_length){
	// 	printf(" PARABENS! Você escapou da forca \n");
	// }else{
	// 	printf(" GAME OVER! Mais sorte na próxima tentativa! \n");
	// }

    return 0;
}