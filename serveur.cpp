// Serveur en C++
#include <iostream>
#include <string>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fstream>
#include <vector>

#define PORT 8080

int main()
{
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);

    // char bufferSend[1024] = {0};
    char bufferReceive[1024] = {0}; // test

    // Création du socket du serveur
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Attache le socket au port 8080
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 3) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    while (1)
    {
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0) // attacher l'adresse ip au nouveau socket crée pour le client
        {
            perror("accept");
            exit(EXIT_FAILURE);
        }

        // std::string hello = "Bonjour, vous êtes connecté au serveur !\n"; // envoie message vers le client
        // send(new_socket, hello.c_str(), hello.size(), 0);                 // envoie message vers le client
        /*
                std::ifstream dataFile("data.json");
                dataFile.read(bufferSend, sizeof(bufferSend));
                send(new_socket, bufferSend, dataFile.size());

        */

        //---------------------------------------envoie du ficher "data.json" au client-------------------
        std::ifstream dataFile("data.json", std::ios::binary | std::ios::ate);
        if (!dataFile) // ouvrir le fichier pour s'assurer que tout est bon
        {
            std::cerr << "Impossible d'ouvrir le fichier data.json\n";
            exit(EXIT_FAILURE);
        }

        std::streamsize size = dataFile.tellg(); // ces deux lignes servent à connaitre la taille du fichier
        dataFile.seekg(0, std::ios::beg);

        std::vector<char> buffer(size); // taille fichier dynamique
        if (dataFile.read(buffer.data(), size))
        {
            /* Le fichier a été lu avec succès */
            send(new_socket, buffer.data(), size, 0);
        }
        //---------------------------------------envoie du ficher "data.json" au client-------------------

        //---------------------------------------reception du message du client---------------------------
        int taille = read(new_socket, bufferReceive, 1024); // taille ne sera pas utilisé
        std::string requestType = bufferReceive;
        std::cout << taille << std::endl;
        if (requestType == "bonjour c le client\n")
        {
            std::cout << requestType << std::endl;
        }
        //---------------------------------------reception du message du client---------------------------
        std::cout << "Message envoyé au client\n";
        close(new_socket);
    }

    return 0;
}