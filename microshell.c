#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>

#include <stdio.h>

int main(int ac, char **av, char **env)
{
	int pipefd[2];
	int pid;

	pipe(pipefd);
	if (ac > 1)
	{
		pid = fork();
		if (!pid) {
			av[2] = 0;
			dup2(pipefd[1], 1);
			close(pipefd[1]);
			close(pipefd[0]);
			execve(av[1], av + 1, env);
		}
		waitpid(pid, NULL, 0);
		pid = fork();
		if (!pid) {
			dup2(pipefd[0], 0);
			close(pipefd[1]);
			close(pipefd[0]);
			execve(av[2], av + 2, env);
		}
		close(pipefd[1]);
		close(pipefd[0]);
		waitpid(pid, NULL, 0);
	}
	return 0;
}