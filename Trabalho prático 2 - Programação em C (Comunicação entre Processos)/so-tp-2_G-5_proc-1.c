#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/shm.h>
#include <signal.h>
#define MAX 30


// estrutura para a lista de mensagens
struct mesg_buffer {
	long mesg_type;
	char mesg_text[100];
} message;


// função para escrever uma mensagem
int escrever_mensagem() {
	key_t key;
	int msgid;
	// gera uma chave única através do ftok
	key = ftok("progfile", 65);
	// origina uma fila de mensagens e retorna o identificador
	msgid = msgget(key, 0666 | IPC_CREAT);
	message.mesg_type = 1;
    // pede ao utilizador para inserir uma mensagem
	printf("Escreva uma mensagem: ");
	fgets(message.mesg_text, MAX, stdin);
	// envia a mensagem para a fila de mensagens
	msgsnd(msgid, &message, sizeof(message), 0);
	return 0;
}


// função para ler uma mensagem
int ler_mensagem() {
	key_t key;
	int msgid;
	// gera uma chave única através do ftok
	key = ftok("progfile", 65);
	// origina uma fila de mensagens e retorna o identificador
	msgid = msgget(key, 0666 | IPC_CREAT);
	// recebe a mensagem
	msgrcv(msgid, &message, sizeof(message), 1, 0);
	// mostra a mensagem recebida
	printf("A menssagem recebida foi: %s \n",message.mesg_text);
	// apaga a fila de mensagens após a leitura
	msgctl(msgid, IPC_RMID, NULL);
	return 0;
}


// função para enviar um sinal "SIGUSR1" para um processo
int sinal_1() {
	// Ler o PID do programa guardado no ficheiro
	FILE *pid_programa_1 = fopen("pid_programa_1.txt", "r");
    pid_t pid_prog_1;
    fscanf(pid_programa_1, "%d", &pid_prog_1);
    fclose(pid_programa_1);
    // envia o sinal SIGUSR1 para um processo
    kill(pid_prog_1, SIGUSR1);
    printf("Sinal SIGUSR1 enviado para o processo %d.\n", (int)pid_prog_1);
    return 0;
}


// função para enviar um sinal "SIGUSR2" para um processo
int sinal_2() {
	// Ler o PID do programa guardado no ficheiro
	FILE *pid_programa_1 = fopen("pid_programa_1.txt", "r");
    pid_t pid_prog_1;
    fscanf(pid_programa_1, "%d", &pid_prog_1);
    fclose(pid_programa_1);
    // Envia o sinal SIGUSR2 para um processo
    kill(pid_prog_1, SIGUSR2);
    printf("Sinal SIGUSR2 enviado para o processo %d.\n", (int)pid_prog_1);
    return 0;
}


// função para exibir o menu de opções
int menu() {
    int escolha;
    while (1) {
        printf("\n1. Receber Mensagem\n2. Enviar Mensagem\nEscolha: ");
        scanf("%d", &escolha); // ler a mensagem inserida
        getchar();
        switch (escolha) {
            case 1:
                sinal_1();
                return 0;
            case 2:
                escrever_mensagem();
                sinal_2();
                return 0;
            default:
                printf("Escolha inválida. Tente novamente.\n");
        }
    }
}


// função para manipular os sinais
void handle_sigusr(int signo) {
    if (signo == SIGUSR1) {
        printf("Sinal recebido\n");
        escrever_mensagem();
        sinal_2();
    }
    else if (signo == SIGUSR2) {
        printf("Sinal recebido\n");
        ler_mensagem();
        menu();
    }
}


// função main
int main() {
    // Criar um ficheiro para armazenar o PID
    FILE *pid_programa_2 = fopen("pid_programa_2.txt", "w");
    // Escrever o PID no arquivo
    fprintf(pid_programa_2, "%d", getpid());
    fclose(pid_programa_2);
    // Configurar o manipulador de sinal para SIGUSR1 e SIGUSR2
    signal(SIGUSR1, handle_sigusr);
    signal(SIGUSR2, handle_sigusr);
    printf("Aguardando o sinal\n");
    // aguardar por um sinal
    while (1) {
        sleep(1);
    }
    return 0;
}
