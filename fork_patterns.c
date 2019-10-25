/*Условие ИЗ№2:
Сравните и выведите в консоль время работы последовательного и параллельного
с использованием нескольких процессов алгоритмов, каждый из которых выделяет 
в динамической памяти символьный массив размером 100 Мб и, рассматривая его 
содержимое как абстрактную переписку, определяет эмоциональную окраску последней. 
Переписка считается оптимистичной, если диграфов :) в ней больше, чем диграфов :(; 
в противном случае переписка признается пессимистичной.
*/

#include <errno.h>
#include <stdbool.h> 
#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "fork_patterns.h"
#include "patterns.h"

unsigned long long int fork_pattern_entity_in_array(
const size_t core_count, const char **array, const size_t array_size, const char *pattern){

	if((array == NULL)||(pattern == NULL)){
		exit(EINVAL);
	}

	for(size_t c = 0; c < array_size; ++c){
		if(array[c] == NULL){
			exit(EINVAL);
		}
	}

	pid_t pid[core_count];
	int status = 0;
	int fork_id = -1;
	int fd[2];
	pipe(fd);

	const size_t pattern_length = strlen(pattern);
	unsigned long long int pattern_counter = 0;

	for(int c = 0; c < core_count; ++c){
		pid[c] = fork();
		if(pid[c] == -1){
			exit(EAGAIN);
		}

		if(pid[c] == 0){
			close(fd[0]);

			for(size_t index = c; index < array_size; index += core_count){
				pattern_counter += pattern_entity(array[index], pattern, pattern_length);
			}

			for (size_t index = c; index < array_size - 1; index += core_count){
				pattern_counter += pattern_entity_in_slice(array[index], array[index + 1], pattern, pattern_length);
			}

			write(fd[1], &pattern_counter, sizeof(unsigned long long int));
			exit(EXIT_SUCCESS);
		}
	}

	for(size_t c = 0; c < core_count; ++c){
		waitpid(pid[c], &status, 0);
	}
	//close(fd[1]);

	unsigned long long int *buff = (unsigned long long int *) malloc (core_count * sizeof(unsigned long long int));

	if(buff == NULL){
		exit(ENOMEM);
	}

	if(read(fd[0], buff, core_count * sizeof(unsigned long long int)) == -1){
		exit(EPERM);
	}

	for(size_t c = 0; c < core_count; ++c){
		pattern_counter += buff[c];
	}

	free(buff);
	return(pattern_counter);
}