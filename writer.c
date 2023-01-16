#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <syslog.h>

int main (int argc, char *argv[]) {
  openlog("writer.c log", LOG_PID, LOG_USER);
  int exit_status;

  if (argc < 3 || argc >= 4) {
    exit_status = 1;
    syslog(LOG_ERR, "Invalid number of arguments");
    return exit_status;
  }

  int file_desc = open(argv[1], O_CREAT | O_WRONLY, 0664);

  ssize_t write_result = write(file_desc, argv[2], strlen(argv[2]));
  if (write_result == -1 || write_result !=strlen(argv[2])) {
    syslog(LOG_ERR, "Error writing to file");
    exit_status = 1;
  } else {
    exit_status = 0;
    syslog(LOG_DEBUG, "Writing %s to %s",argv[2],argv[1]);
  }

  int close_result = close(file_desc);
  if (close_result == -1) {
    syslog(LOG_ERR, "Error clsoing file");
    exit_status = 1;
  } else {
    exit_status = 0;
  }

  closelog();
  return exit_status;
}
