#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/sysmacros.h>
#include <errno.h>


char* device_type(struct stat* sb) {
    char* s;
    switch (sb->st_mode & S_IFMT) {
        case S_IFBLK: s = "блочное устройство";     break;
        case S_IFCHR: s = "символьное устройство";  break;
        case S_IFDIR: s = "каталог";                break;
        case S_IFIFO: s = "FIFO/канал";             break;
        case S_IFLNK: s = "символьная ссылка";      break;
        case S_IFREG: s = "обычный файл";           break;
        case S_IFSOCK: s = "сокет";                 break;
        default: s = "неизвестно(";                 break;
    }
    return s;
}


void lstat_error() {
    // ошибки с человеческим описанием
    switch (errno) {
        case EACCES: printf("Permission denied!\n");                    break;
        case EBADFD: printf("File descriptor in bad state!\n");         break;
        case EFAULT: printf("Bad address!\n");                          break;
        case ELOOP: printf("Too many symbolic links encountered!\n");   break;
        case ENAMETOOLONG: printf("File name too long!\n");             break;
        case ENOENT: printf("No such file or directory!\n");            break;
        case ENOMEM: printf("Out of memory!\n");                        break;
        case ENOTDIR: printf("Not a directory!\n");                     break;
        case EOVERFLOW: printf("Value too large for defined data type!\n"); break;
        case EINVAL: printf("Invalid argument!\n");                     break;
    }
}



int main(int argc, char* argv[]) {
    if(argc - 2) {
        fprintf(stderr, "Incorrect argc: %d\n", argc);
        exit(1);
    }

    struct stat sb;

    if(lstat(argv[1], &sb) == -1) {
        lstat_error();
        exit(1);
    }

    printf("Тип файла:              %s\n", device_type(&sb));

    printf("инода:                  %ld\n", (long)sb.st_ino);
    printf("Режим доступа:          %lo (octal)\n", (unsigned long)sb.st_mode);
    printf("Кол-во ссылок:          %ld\n", (long)sb.st_nlink);
    printf("владелец:               UID=%ld\tGID=%ld\n", (long)sb.st_uid, (long)sb.st_gid);
    printf("Предпочт. размер блока: %ld байт\n", (long)sb.st_blksize);
    printf("Размер файла:           %lld байт\n", (long long)sb.st_size);
    printf("Выделено блоков:        %lld\n", (long long)sb.st_blocks);

    printf("C_TIME:                 %s", ctime(&sb.st_ctime));
    printf("A_TIME:                 %s", ctime(&sb.st_atime));
    printf("M_TIME:                 %s", ctime(&sb.st_mtime));
    return 0;
}
