// Client en C++
#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fstream>

#define PORT 8080

int main()
{
    int sock = 0;
    struct sockaddr_in serv_addr;
    char bufferSend[1024] = {0};
    char bufferRead[1024] = {0};

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        std::cout << "\n Socket creation error \n";
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Convertit les adresses IPv4 et IPv6 de texte à binaire
    if (inet_pton(AF_INET, "192.168.1.77", &serv_addr.sin_addr) <= 0)
    {
        std::cout << "\nInvalid address/ Address not supported \n";
        return -1;
    }

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        std::cout << "\nConnection Failed \n";
        return -1;
    }

    std::string sms = "bonjour c le client\n"; // test
    send(sock, sms.c_str(), sms.size(), 0);    // test

    //------------reception message du serveur-------------------------
    // int valread = read(sock, bufferSend, 1024);
    // std::cout << bufferSend << std::endl;
    //------------reception message du serveur-------------------------
    int valread = read(sock, bufferRead, 1024); // lecture du socket et mettre valeur dans bufferRead
    std::ofstream DATA("data.json");            // creation du fichier data.json
    DATA << bufferRead;                         // mettre le contenu de bufferRead dans data
    return 0;
}