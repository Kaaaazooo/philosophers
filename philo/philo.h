/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sabrugie <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/29 14:26:42 by sabrugie          #+#    #+#             */
/*   Updated: 2021/07/30 16:11:55 by sabrugie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <limits.h>
# include <inttypes.h>
# include <pthread.h>
# include <sys/time.h>

# define ATE_ENOUGH 1
# define ALL_ALIVE 2
# define TAKING_FORKS 4

typedef struct s_arguments
{
	int32_t			nb_of_philo;
	int32_t			nb_of_meal;
	int32_t			t_to_eat;
	int32_t			t_to_sleep;
	int32_t			t_to_die;
	struct timeval	start;
}		t_args;

typedef struct s_philosopher
{
	int32_t			id;
	uint8_t			*global_state;
	uint8_t			state;
	int32_t			*ate_enough;
	int32_t			meals;
	struct timeval	last_meal;
	pthread_mutex_t	*left;
	pthread_mutex_t	*right;
	pthread_mutex_t	*print;
	t_args			args;
	pthread_t		thread;
}		t_philo;

void	timestamp_msg(t_philo *philo, char *str);
void	*routine(void *ptr);
void	*check_status(void *ptr);

int64_t	gettime_t(void);
int64_t	gettimestamp(struct timeval start);
int		is_num(char *str);
void	ft_itoa(char *str, int64_t nb);
int		ft_atoi(char *str);

#endif
