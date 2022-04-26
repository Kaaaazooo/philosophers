/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sabrugie <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/29 14:26:42 by sabrugie          #+#    #+#             */
/*   Updated: 2021/07/30 16:33:56 by sabrugie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int64_t	gettime_t(void)
{
	struct timeval	t;

	gettimeofday(&t, NULL);
	return (t.tv_sec * 1000 + t.tv_usec / 1000);
}

int64_t	gettimestamp(struct timeval start)
{
	struct timeval	t;
	int64_t			milli;

	gettimeofday(&t, NULL);
	milli = t.tv_sec * 1000 + t.tv_usec / 1000;
	milli -= start.tv_sec * 1000 + start.tv_usec / 1000;
	return (milli);
}

int	is_num(char *str)
{
	while (*str)
	{
		if (*str < '0' || *str > '9')
			return (0);
		++str;
	}
	return (1);
}

void	ft_itoa(char *str, int64_t nb)
{
	int		len;
	int64_t	tmp;

	if (nb == 0)
		len = 1;
	else
	{
		len = 0;
		tmp = nb;
		while (tmp)
		{
			tmp /= 10;
			++len;
		}
	}
	while (len--)
	{
		str[len] = nb % 10 + '0';
		nb /= 10;
	}
}

int	ft_atoi(char *str)
{
	int		sign;
	int64_t	res;

	sign = 1;
	res = 0;
	while ((*str >= 9 && *str <= 13) || *str == 32)
		++str;
	if (*str == '-')
	{
		sign = -1;
		++str;
	}
	while (*str && *str >= '0' && *str <= '9')
	{
		res *= 10;
		res += (int)(*str) - (int)('0');
		if (res < INT_MIN || res > INT_MAX)
			return (-1);
		++str;
	}
	return (((int)res) * sign);
}
