#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[BUFFER_SIZE] = {0};
    
    // Resposta HTTP simples com uma página HTML
    char *response = 
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/html\r\n\r\n"
        "<html><body><h1>Olá, Mundo! Servidor Web em C</h1></body></html>";
    
    // Criando o socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Falha ao criar o socket");
        exit(EXIT_FAILURE);
    }
    
    // Definindo o endereço e a porta do servidor
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
    
    // Fazendo o bind do socket à porta 8080
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Falha no bind");
        close(server_fd);
        exit(EXIT_FAILURE);
    }
    
    // Escutando por conexões (até 3 conexões simultâneas)
    if (listen(server_fd, 3) < 0) {
        perror("Falha ao escutar");
        close(server_fd);
        exit(EXIT_FAILURE);
    }
    
    printf("Servidor rodando na porta %d...\n", PORT);
    
    while (1) {
        // Aceitando a conexão do cliente
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
            perror("Falha ao aceitar a conexão");
            close(server_fd);
            exit(EXIT_FAILURE);
        }
        
        // Lendo a requisição do cliente (não estamos analisando o conteúdo)
        read(new_socket, buffer, BUFFER_SIZE);
        printf("Requisição recebida:\n%s\n", buffer);
        
        // Enviando a resposta HTTP
        write(new_socket, response, strlen(response));
        
        // Fechando a conexão com o cliente
        close(new_socket);
    }
    
    return 0;
}
