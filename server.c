#include <stdio.h>
#include <stdlib.h>  // rand(), srand()
#include <string.h>
#include <time.h>    // time()
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <errno.h>

#define _GNU_SOURCE
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

int get_file_line_number(){
	FILE * fp;
    char * line = NULL;
    size_t len = 0;
    ssize_t read;
	int line_number;

    fp = fopen("./palavras.txt", "r");
    if (fp == NULL)
        exit(EXIT_FAILURE);


    while ((read = getline(&line, &len, fp)) != -1) {
		line_number++;
    }

	fclose(fp);
    if (line){
		free(line);
	}

	return line_number;
}

void get_palavra(char * palavra_escolhida) {
	int line_number = 0;
	int random_number = 0;

	int total_line_number = get_file_line_number();
	srand((unsigned)time(NULL));
	random_number = rand() % total_line_number;

	// printf("Numero sorteado: %d\n\n", random_number);

	FILE* filePointer;
	int bufferLength = 255;
	char buffer[bufferLength];

	filePointer = fopen("palavras.txt", "r");

	while(fgets(buffer, bufferLength, filePointer)) {
		// printf("%s\n", buffer);

		// remover quebra de linha no final da palavra
		int len = strlen(buffer);
		if (len > 0 && buffer[len-1] == '\n') buffer[len-1] = '\0';

		if(line_number == random_number){
			strcat(palavra_escolhida, buffer);
		}

		line_number++;
	}

	fclose(filePointer);
}

char* tentativas = "";

void desenha_boneco(char * boneco, int erros){
	char cabeca[] = " ";
	char besquerdo[] = " ";
	char tronco[] = " ";
	char bdireito[] = " ";
	char pesquerdo[] = " ";
	char pdireito[] = " ";

	if(erros >= 1){
		strcpy(cabeca, "O" );
	}
	if(erros >= 2){
		strcpy(besquerdo, "/" );
	}
	if(erros >= 3){
		strcpy(tronco, "|" );
	}
	if(erros >= 4){
		strcpy(bdireito, "\\" );
	}
	if(erros >= 5){
		strcpy(pesquerdo, "/" );
	}
	if(erros >= 6){
		strcpy(pdireito, "\\" );
	}

	strcat(boneco, "+----------|\n" );
	strcat(boneco, "|          |\n" );

	if(erros >= 6){
		strcat(boneco, "|          "  );
	}

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
	if(erros < 6){
		strcat(boneco, "|\n" );
	}
	
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
		}
	}
}

int is_letra_in_palavra(char letra, const char *palavra){
	int i = 0;
	int ate = strlen(palavra);
	int exists = 0;
	for(i = 0; i < ate; i++){
		if(palavra[i] == letra){
			exists++;
		}
	}

	printf( "%d \n", exists);
	return exists;
}

int main()
{
	
	// printf(" Palavra Escolhida: %s \n", word);

	// printf(" Finalizando \n");
	// return 0;
	
	// inicia as variaveis relativas ao socket
	int server_sockfd, client_sockfd;
	int bind_return;
    int server_len, client_len;
    struct sockaddr_un server_address;
    struct sockaddr_un client_address;
    char ch;
	
	// configura socket
	server_sockfd = socket(AF_UNIX, SOCK_STREAM, 0);
    server_address.sun_family = AF_UNIX;
	remove("server_socket");
    strcpy(server_address.sun_path, "server_socket");
    server_len = sizeof(server_address);
    bind_return = bind(server_sockfd, (struct sockaddr *)&server_address, server_len);
	
	if(bind_return == -1){
		printf(" erro bind No: %d %d \n", errno, server_sockfd);
		return 0;
	}

    listen(server_sockfd, 5);

	// inicia as variaveis referentes a regra de negócio do sistema
	char palavra[30] = ""; 
	get_palavra(palavra);
	int palavra_length = strlen(palavra);
	char acertos [palavra_length];
	char erros [palavra_length];
	strcpy(acertos, palavra); 

	printf("%s - %d", palavra, palavra_length);

	// preenche os caracteres do array de acertos com _
	for(int i = 0; i < strlen(acertos); i++){
		acertos[i] = '_';
		erros[i] = '_';
	}

	while(
		// usuario nao extrapolou numero de tentativas
		(n_tentativas < MAX_TENTATIVAS) == 1 &&
		// usuario ainda nao acertou todas as letras
		(n_acertos < palavra_length) == 1
		// 1
	){
		char boneco[1000] = "";

		sprintf(boneco, " Você possui %d tentativas \n", MAX_TENTATIVAS - n_tentativas);

		printf("Server waiting\n");
        client_len = sizeof(struct sockaddr_un);
        client_sockfd = accept(server_sockfd, (struct sockaddr *)&client_address, &client_len);
        read(client_sockfd, &ch, 1);

		int qtd_acertos_vez = is_letra_in_palavra(ch, palavra);
		if(qtd_acertos_vez > 0){
			strcat(boneco, "Voce acertou!! \n");
			atualiza_acertos(acertos, ch, palavra);
			n_acertos = n_acertos + qtd_acertos_vez;
		}else{
			strcat(boneco, "Voce errou... \n");
			erros[n_tentativas] = ch;	
			n_tentativas++;
		}

		desenha_boneco(boneco, n_tentativas);

		strcat(boneco, "Acertos: ");
		strcat(boneco, acertos);
		strcat(boneco, "\n");

		strcat(boneco, "Erros: ");
		strcat(boneco, erros);
		strcat(boneco, "\n");

		printf("n_acertos: %d\n", n_acertos);
		printf("palavra_length: %d\n", palavra_length);

		write(client_sockfd, &boneco, 1000);
        close(client_sockfd);
	}

	char mensagem_final[] = "* ";
	if(n_acertos == palavra_length){
		strcat(mensagem_final, "PARABENS! Você escapou da forca \n");
	}else{
		strcat(mensagem_final, "GAME OVER! Mais sorte na próxima tentativa! \n");
	}

	client_len = sizeof(struct sockaddr_un);
    client_sockfd = accept(server_sockfd, (struct sockaddr *)&client_address, &client_len);
	write(client_sockfd, &mensagem_final, 1000);
    close(client_sockfd);

    return 0;
}