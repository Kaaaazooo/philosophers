/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sabrugie <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/29 14:26:42 by sabrugie          #+#    #+#             */
/*   Updated: 2021/07/30 17:02:20 by sabrugie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	timestamp_msg(t_philo *philo, char *msg)
{
	char	str[2048];
	int		i;

	memset(str, 0, 2048);
	ft_itoa(str, gettimestamp(philo->args.start));
	i = 0;
	while (str[i])
		++i;
	str[i++] = ' ';
	ft_itoa(str + i, philo->id);
	while (str[i])
		++i;
	str[i++] = ' ';
	while (*msg)
		str[i++] = *msg++;
	str[i++] = '\n';
	pthread_mutex_lock(philo->print);
	write(1, str, i);
	pthread_mutex_unlock(philo->print);
}

int	ft_sleep(int64_t time, t_philo *philo)
{
	int64_t			t_d;
	struct timeval	t;

	gettimeofday(&t, NULL);
	t_d = (t.tv_sec - philo->last_meal.tv_sec) * 1000
		+ (t.tv_usec - philo->last_meal.tv_usec) / 1000;
	if (t_d + time > philo->args.t_to_die)
	{
		*(philo->global_state) = 0;
		t_d = gettime_t();
		while (gettime_t() - t_d < philo->args.t_to_sleep)
			usleep(500);
		timestamp_msg(philo, "died");
		return (-1);
	}
	t_d = gettime_t();
	while (gettime_t() - t_d < philo->args.t_to_sleep)
		usleep(500);
	return (0);
}

int	take_forks(t_philo *philo)
{
	pthread_mutex_lock(philo->left);
	if (*philo->global_state ^ ALL_ALIVE)
	{
		pthread_mutex_unlock(philo->left);
		philo->state -= TAKING_FORKS;
		return (-1);
	}
	timestamp_msg(philo, "has taken a fork");
	if (philo->left == philo->right)
		while (*philo->global_state & ALL_ALIVE)
			;
	pthread_mutex_lock(philo->right);
	if (*philo->global_state ^ ALL_ALIVE)
	{
		pthread_mutex_unlock(philo->left);
		pthread_mutex_unlock(philo->right);
		philo->state -= TAKING_FORKS;
		return (-1);
	}
	timestamp_msg(philo, "has taken a fork");
	return (0);
}

int	philo_eat(t_philo *philo)
{
	int64_t			t_meal;

	philo->state |= TAKING_FORKS;
	if (philo->meals == 0 && philo->id % 2)
		usleep(1000 * philo->args.t_to_eat / 4);
	if (take_forks(philo))
		return (-1);
	philo->state -= TAKING_FORKS;
	timestamp_msg(philo, "is eating");
	gettimeofday(&philo->last_meal, NULL);
	t_meal = philo->last_meal.tv_sec * 1000 + philo->last_meal.tv_usec / 1000;
	while (gettime_t() - t_meal < philo->args.t_to_eat)
		usleep(500);
	pthread_mutex_unlock(philo->left);
	pthread_mutex_unlock(philo->right);
	philo->meals++;
	return (0);
}

void	*routine(void *ptr)
{
	t_philo	*philo;

	philo = (t_philo *)ptr;
	while (*philo->global_state ^ ATE_ENOUGH
		&& *(philo->global_state) & ALL_ALIVE)
	{
		if (philo_eat(philo))
			break ;
		if (philo->meals == philo->args.nb_of_meal)
			philo->state += ATE_ENOUGH;
		if (*(philo->global_state) & ATE_ENOUGH
			|| *(philo->global_state) ^ ALL_ALIVE)
			break ;
		timestamp_msg(philo, "is sleeping");
		ft_sleep(philo->args.t_to_sleep, philo);
		if (*(philo->global_state) & ATE_ENOUGH
			|| *(philo->global_state) ^ ALL_ALIVE)
			break ;
		timestamp_msg(philo, "is thinking");
	}
	return (ptr);
}
