#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

void show_help ();

const char *pass = "caolinye";

void encode (const char *filename, const char *password, int encodeall);

int main (int argc, char **argv) {
	if (argc <= 2) {
		show_help();
		return 0;
	}

	char *password = NULL;
	char *filename = NULL;
	int encodeall = 1;
	int i;

	for (i = 1; i < argc; ++i) {
		if (strcmp("-p", argv[i]) == 0) {
			password = argv[++i];
		}
		else if (strcmp("-f", argv[i]) == 0) {
			filename = argv[++i];
		}
		else if (strcmp("-n", argv[i]) == 0) {
			encodeall = 0;
		}
	}
	if (password == NULL) {
		show_help();
		return 0;
	}
	else if (filename == NULL) {
		show_help();
		return 0;
	}

	encode(filename, password, encodeall);
}

void show_help () {
	printf("help message:\n");
	printf("-p password 加密密码\n");
	printf("-f filename 加密文件\n");
	printf("-n not encode all data 视频文件只需要修改部分数据即不可播放\n");
	printf("-a 加密全部数据,默认（不可与-n同时使用）\n");
};

void encode (const char *filename, const char *password, int encodeall) {
	int f = open(filename, O_RDWR);
	char buff[1024];
	int read_len, pass_len = strlen(password);
	int i;

	read_len = read(f, buff, sizeof(buff));
	while (read_len) {
		for (i = 0; i < read_len; ++i) {
			buff[i] ^= password[i%pass_len];
		}
		lseek(f, -read_len, SEEK_CUR);
		write(f, buff, read_len);
		read_len = (encodeall)? read(f, buff, sizeof(buff)):0;
	}
	close(f);
}
