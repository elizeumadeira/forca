-- windows
gcc -o server server.c && server

-- linux
gcc -o client_socket_example client_socket_example.c & ./client_socket_example
gcc -o server_socket_example server_socket_example.c & ./server_socket_example

Executar o server antes do client