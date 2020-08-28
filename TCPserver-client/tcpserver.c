#include <stdio.h> 
#include <netdb.h> 
#include <netinet/in.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/socket.h> 
#include <sys/types.h> 
  
 
//İstemci ve sunucu arasında sohbet için tasarlanmış işlev.
void func(int sockfd) 
{ 
    char metin[80]; 
    int n; 

    // sohbet için sonsuz döngü
    for (;;) { 
        bzero(metin, 80); 
  
        // mesajı istemciden okuyun ve arabelleğe kopyalayın
        read(sockfd, metin, sizeof(metin)); 


        // İstemci içeriğini içeren yazdırma arabelleği
        printf("İstemciden gelen: %s\t İstemciye gonder : ", metin); 
        bzero(metin, 80); 
        n = 0; 

        // arabellekte sunucu mesajını kopyala
        while ((metin[n++] = getchar()) != '\n') 
            ; 
  
        //ve bu metni istemciye gönder
        write(sockfd, metin, sizeof(metin)); 
  
        // Mesaj "exit" içeriyorsa sunucudan çıkılır ve sohbet sona erer.
        if (strncmp("exit", metin, 4) == 0) { 
            printf("Sunucudan cikildi...\n"); 
            break; 
        } 
    } 
} 
  
// Sürücü işlevi
int main() 
{ 
    int sockfd, connfd, len; 
    struct sockaddr_in SunucuBilgileri,cli; 
  
    // soket oluşturma ve doğrulama
    sockfd = socket(AF_INET, SOCK_STREAM, 0); 
    if (sockfd == -1) { 
        printf("Soket olusturulamadı...\n"); 
        exit(0); 
    } 
    else
        printf("Soket olusturuldu... \n"); 
    bzero(&SunucuBilgileri, sizeof(SunucuBilgileri)); 
  
    // IP, PORT atama
	// Ipv4 ifadesine karşılık gelen ifade AF_INET.
    SunucuBilgileri.sin_family = AF_INET; 
    SunucuBilgileri.sin_addr.s_addr = htonl(INADDR_ANY); 
    SunucuBilgileri.sin_port = htons(1234); 
  
    // Binding newly created socket to given IP and verification 
    // Yeni oluşturulan soketi verilen IP'ye bağlama ve doğrulama
    if ((bind(sockfd, (struct sockaddr *)&SunucuBilgileri, sizeof(SunucuBilgileri))) != 0) { 
        printf("Soket, ip adresine baglanamadı...\n"); 
        exit(0); 
    } 
    else
        printf("Soket, ip adresine baglandı..\n"); 
  
    // Artık sunucu dinlemeye ve doğrulamaya hazır
    if ((listen(sockfd, 5)) != 0) { 
        printf("Sunucuyu dinleme işlemi başarısız oldu...\n"); 
        exit(0); 
    } 
    else
        printf("Sunucudan mesaj bekleniyor..\n"); 
		len = sizeof(cli); 
  
    // İstemciden gelen veri paketini kabul edin ve doğrulayın
    connfd = accept(sockfd, (struct sockaddr *)&cli, &len); 
    if (connfd < 0) { 
        printf("Sunucu ile baglanti basarisiz oldu...\n"); 
        exit(0); 
    } 
    else
        printf("Sunucu, istemci ile baglanti sagladi...\n"); 
  
    // İstemci ve sunucu arasında sohbet etme işlevi
    func(connfd); 
  
    // Sohbet ettikten sonra soketi kapat
    close(sockfd); 
} 
