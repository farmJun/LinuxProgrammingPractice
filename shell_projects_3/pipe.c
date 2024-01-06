#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

int main() {
    int pipe_fd[2];

    // 파이프 생성
    if (pipe(pipe_fd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    // O_NONBLOCK 플래그 설정
    fcntl(pipe_fd[1], F_SETFL, O_WRONLY);

    // 읽기 프로세스 생성
    pid_t child_pid = fork();

    if (child_pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (child_pid == 0) {  // Child process (reader)
        close(pipe_fd[1]);  // 쓰기 측 닫음
        sleep(5);           // 잠시 대기 후 종료
        exit(EXIT_SUCCESS);
    } else {  // Parent process (writer)
        close(pipe_fd[0]);  // 읽기 측 닫음

        // 쓰기 프로세스가 파이프가 가득 차도록 반복해서 데이터 쓰기
        while (write(pipe_fd[1], "Test", 4) != -1) {
            printf("Write successful.\n");
        }

        printf("hello");
        // 파이프가 가득 차서 더 이상 쓸 수 없는 경우 write 함수는 에러를 반환하고
        // 해당 에러를 확인하기 위해 perror를 사용합니다.
        //perror("write");

        // 쓰기 프로세스 종료 대기
        wait(NULL);

        // 파이프 닫기
        close(pipe_fd[1]);
    }

    return 0;
}
