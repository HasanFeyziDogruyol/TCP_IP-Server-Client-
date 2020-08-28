#include <unistd.h> 
#include <netdb.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/socket.h>  

 
void func(int sockfd) 
{ 
    char metin[300]; 
    int n; 
    for (;;) { 
        bzero(metin, sizeof(metin)); 
        printf("Mesajınızı giriniz : "); 
        n = 0; 
        while ((metin[n++] = getchar()) != '\n') 
            ; 
        write(sockfd, metin, sizeof(metin)); 
        bzero(metin, sizeof(metin)); 
        read(sockfd, metin, sizeof(metin)); 
        printf("Sunucunun mesajı : %s", metin); 
        if ((strncmp(metin, "exit", 4)) == 0) { 
            printf("İstemciden cikildi...\n"); 
            break; 
        } 
    } 
} 
  
int main() 
{ 
    int sockfd; 
    struct sockaddr_in SunucuBilgileri; 
  
    // soket oluşturma ve doğrulama
    sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); 
    if (sockfd == -1) { 
        printf("Soket olusturmada hata olustu...\n"); 
        exit(0); 
    } 
    else
        printf("Soket basarili bi sekilde olusturuldu..\n"); 
    bzero(&SunucuBilgileri, sizeof(SunucuBilgileri)); 
  
    // IP, PORT atama
    SunucuBilgileri.sin_family = AF_INET; 
    SunucuBilgileri.sin_addr.s_addr = htonl(INADDR_ANY); 
    SunucuBilgileri.sin_port = htons(1234); 
  
    // istemci soketini sunucu soketine bağlayın
    if (connect(sockfd, (struct sockaddr*)&SunucuBilgileri, sizeof(SunucuBilgileri)) != 0) { 
        printf("connection with the server failed...\n"); 
        exit(0); 
    } 
    else
        printf("connected to the server..\n"); 
  
    // sohbet işlevi
    func(sockfd); 
  
    // soketi kapat
    close(sockfd); 
} 
