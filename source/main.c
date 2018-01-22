#include "ps4.h"
#define debug(sock, ...) \
	do { \
		char buffer[512]; \
		int size = sprintf(buffer, ##__VA_ARGS__); \
		sceNetSend(sock, buffer, size, 0); \
	} while(0)

int _main(void) {
	// Init and resolve libraries
	initKernel();
	
	initLibc();
	initNetwork();
	
	
	// Connect to server
	char socketName[] = "debug";
	
	struct sockaddr_in server;
	
	server.sin_len = sizeof(server);
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = IP(192, 168, 1, 41);
	server.sin_port = sceNetHtons(9023);
	memset(server.sin_zero, 0, sizeof(server.sin_zero));
	
	int sock = sceNetSocket(socketName, AF_INET, SOCK_STREAM, 0);
	sceNetConnect(sock, (struct sockaddr *)&server, sizeof(server));
		
	debug(sock,"Copying PFS FILE");
	copyFile("/mnt/sandbox/pfsmnt/CUSA01615-app0-nest/pfs_image.dat","/mnt/usb0/pfs_image.dat");
	debug(sock,"Copying pkg FILE");
	copyFile("/user/app/CUSA01615/app.pkg","/mnt/usb0/app.pkg");
	debug(sock,"All done");
	// Return to browser
	return 0;
}
void copyFile(char *sourcefile, char* destfile)
{
	FILE *src = fopen(sourcefile, "r");
	FILE *out = fopen(destfile,"w");
	char buffer[2048];
	size_t bytes;
	while (0 < (bytes = fread(buffer, 1, sizeof(buffer), src)))
    		fwrite(buffer, 1, bytes, out);
	fclose(src);
	fclose(out);
}
