/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sabrugie <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/29 14:26:42 by sabrugie          #+#    #+#             */
/*   Updated: 2021/08/01 21:10:06 by sabrugie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	get_args(t_args *args, int ac, char **av)
{
	int	i;

	i = 1;
	while (av[i])
		if (!is_num(av[i++]))
			return (-1);
	args->nb_of_philo = ft_atoi(av[1]);
	args->t_to_die = ft_atoi(av[2]);
	args->t_to_eat = ft_atoi(av[3]);
	args->t_to_sleep = ft_atoi(av[4]);
	if (args->nb_of_philo < 1 || args->t_to_die < 1
		|| args->t_to_eat < 1 || args->t_to_sleep < 1)
		return (-2);
	if (ac == 5)
		args->nb_of_meal = -1;
	else
	{
		args->nb_of_meal = ft_atoi(av[5]);
		if (args->nb_of_meal < 1)
			return (-3);
	}
	return (0);
}

t_philo	*create_philo(t_args args, uint8_t *global_state,
		pthread_mutex_t *forks, pthread_mutex_t *print)
{
	t_philo	*philo;
	int		i;

	philo = malloc(sizeof(t_philo) * args.nb_of_philo);
	if (philo == NULL)
	{
		free(forks);
		return (NULL);
	}
	i = 0;
	*global_state = ALL_ALIVE;
	memset(philo, 0, sizeof(t_philo) * args.nb_of_philo);
	while (i < args.nb_of_philo)
	{
		philo[i].id = i + 1;
		philo[i].global_state = global_state;
		philo[i].last_meal = args.start;
		philo[i].left = &forks[i];
		philo[i].right = &forks[(args.nb_of_philo + i - 1)
			% args.nb_of_philo];
		philo[i].print = print;
		philo[i++].args = args;
	}
	return (philo);
}

void	check_meals(t_philo *philo)
{
	int	i;
	int	ate_enough;

	i = 0;
	ate_enough = 1;
	while (i < philo->args.nb_of_philo && ate_enough)
		ate_enough = philo[i++].state & ATE_ENOUGH;
	if (ate_enough)
		*(philo->global_state) &= ATE_ENOUGH;
}

void	create_and_join(int32_t nb_of_philo, t_philo *p)
{
	int	i;

	i = -1;
	while (++i < nb_of_philo)
		pthread_create(&p[i].thread, NULL, &routine, (void *)&p[i]);
	while (*p->global_state & ALL_ALIVE
		&& *p->global_state ^ ATE_ENOUGH)
	{
		i = -1;
		check_meals(p);
		while (++i < p->args.nb_of_philo)
		{
			if (p[i].state & TAKING_FORKS && p->args.t_to_die
				< gettimestamp(p[i].last_meal))
			{
				*p->global_state -= ALL_ALIVE;
				timestamp_msg(&p[i], "died");
				break ;
			}
		}
	}
	while (--i >= 0)
		pthread_join(p[i].thread, NULL);
}

int	main(int ac, char **av)
{
	t_args			args;
	uint8_t			state;
	t_philo			*philo;
	pthread_mutex_t	*forks;
	pthread_mutex_t	print;

	if (ac < 5 || ac > 6)
		return (-1);
	if (get_args(&args, ac, av))
		return (-2);
	forks = malloc(sizeof(pthread_mutex_t) * args.nb_of_philo);
	if (forks == NULL)
		return (-3);
	while (--(args.nb_of_philo) > -1)
		pthread_mutex_init(&forks[args.nb_of_philo], NULL);
	args.nb_of_philo = ft_atoi(av[1]);
	pthread_mutex_init(&print, NULL);
	gettimeofday(&args.start, NULL);
	philo = create_philo(args, &state, forks, &print);
	if (philo == NULL)
		return (-3);
	create_and_join(args.nb_of_philo, philo);
	free(forks);
	free(philo);
	return (0);
}
