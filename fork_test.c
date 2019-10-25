/*Условие ИЗ№2:
Сравните и выведите в консоль время работы последовательного и параллельного
с использованием нескольких процессов алгоритмов, каждый из которых выделяет 
в динамической памяти символьный массив размером 100 Мб и, рассматривая его 
содержимое как абстрактную переписку, определяет эмоциональную окраску последней. 
Переписка считается оптимистичной, если диграфов :) в ней больше, чем диграфов :(; 
в противном случае переписка признается пессимистичной.
*/

#define SIZE_T_LINE_LENGTH 128
#define CHAR_LINE_LENGTH "127"
#define CORE_COUNT 4

#include <assert.h>
#include <errno.h>
#include <stdbool.h> 
#include <stdio.h>
#include <stdlib.h> 
#include <string.h>

#include "fork_patterns.h"
#include "test.h"



size_t read_array(char ***array, const char *file_name){
	
	if(file_name == NULL){
		exit(EINVAL);
	}

	FILE *fp;
	size_t array_size = 1;
	size_t line_size = SIZE_T_LINE_LENGTH;

	*array = (char**) malloc (array_size * sizeof(char*));

	if(*array == NULL){
		exit(ENOMEM);
	}

	for(size_t c = 0; c < array_size; ++c){
		(*array)[c] = (char*) malloc (line_size * sizeof(char));

		if((*array)[c] == NULL){
			for(size_t index = 0; index < c; ++index){
				free(array[index]);
			}
			free(*array);

			exit(ENOMEM);
		}	
	}
	
	if ((fp = fopen(file_name, "r")) == NULL){
		for(size_t c = 0; c < array_size; ++c){
			free((*array)[c]);
		}
		free(*array);

	 	exit(EAGAIN);
	}

	for(size_t c = 0; fscanf(fp, "%"CHAR_LINE_LENGTH"s", (*array)[c]) != EOF;){
		c++;

		if(c == array_size){
			char **buff = (char **) realloc (*array, array_size * 2 * sizeof(char*));
			if(buff == NULL){
				for(size_t index = 0; index < array_size; ++index){
					free((*array)[index]);
				}
				free(*array);

				exit(ENOMEM);
			}

			*array = buff;

			for(size_t g = c; g < array_size * 2; ++g){
				(*array)[g] = (char*) malloc (line_size * sizeof(char));
				
				if((*array)[g] == NULL){
					for(size_t index = 0; index < g; ++index){
						free((*array)[index]);
					}
					free(*array);

					exit(ENOMEM);
				}	
			}

			array_size *= 2;
		}
	}
	
	return(array_size);
}

bool fork_emotion_test(const char** array, const size_t array_size,
const char *good_pattern, const char *bad_pattern){

	return(((long long int)(fork_pattern_entity_in_array(CORE_COUNT, array, array_size, good_pattern) -
		    (long long int)fork_pattern_entity_in_array(CORE_COUNT, array, array_size, bad_pattern))) > 0);
}

int main(){

	char **array;
	size_t array_size = 0;

	array_size = read_array(&array, "test1.txt");
	assert(fork_emotion_test((const char**)array, array_size, ":)", ":("));
	printf("Test 1 passed SUCCESS\n");
	for(size_t c = 0; c < array_size; ++c){
		free(array[c]);
	}
	free(array);

	array_size = read_array(&array, "test2.txt");
	assert(!fork_emotion_test((const char**)array, array_size, ":)", ":("));
	printf("Test 2 passed SUCCESS\n");
	for(size_t c = 0; c < array_size; ++c){
		free(array[c]);
	}
	free(array);
}
