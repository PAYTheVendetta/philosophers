/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aialonso <aialonso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/03 16:17:09 by aialonso          #+#    #+#             */
/*   Updated: 2026/02/09 18:05:18 by aialonso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	ft_isdigit(int c)
{
	if (c >= '0' && c <= '9')
		return (1);
	return (0);
}

long	gettimeinmil(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

long	ft_atoi(char *str)
{
	long	n[4];

	n[0] = 0;
	n[1] = 1;
	n[2] = 0;
	while ((str[n[0]] >= 9 && str[n[0]] <= 13) || str[n[0]] == 32)
		n[0]++;
	if (str[n[0]] == '-' || str[n[0]] == '+')
	{
		if (str[n[0]] == ('-'))
			n[1] = n[1] * -1;
		n[0]++;
	}
	while (ft_isdigit(str[n[0]]))
	{
		if (!ft_isdigit(str[n[0] + 1]))
		{
			n[2] = n[2] * 10 + (str[n[0]] - '0');
			return (n[1] * n[2]);
		}
		else
			n[2] = n[2] * 10 + (str[n[0]] - '0');
		n[0]++;
	}
	return (0);
}

int	freedi(t_phio **philos, t_rules **rules, int n_for, int n_thread)
{
	if (n_thread != 0 && *rules != NULL)
		(*rules)->finished = 1;
	while (n_thread != 0)
		pthread_join((*philos)[n_thread-- - 1].thread, NULL);
	if (n_for-- == (*rules)->nb_philos + 1)
		pthread_mutex_destroy((*rules)->print_mutex);
	while (n_for != 0)
		pthread_mutex_destroy(&(*rules)->forks[(--n_for)]);
	if ((*rules)->print_mutex != NULL)
		free((*rules)->print_mutex);
	(*rules)->print_mutex = NULL;
	if ((*rules)->forks != NULL)
		free((*rules)->forks);
	(*rules)->forks = NULL;
	if (*rules != NULL)
		free(*rules);
	*rules = NULL;
	if (*philos != NULL)
		free(*philos);
	*philos = NULL;
	return (-1);
}

int	validate(char **argv, int argc)
{
	int		num_arg;
	char	*to_validate;

	num_arg = 0;
	while (++num_arg < argc)
	{
		to_validate = argv[num_arg];
		if (!*to_validate)
			return (-1);
		while (*to_validate)
		{
			if (*to_validate == '+')
				if (!ft_isdigit(*(++to_validate)))
					return (-1);
			if (!ft_isdigit(*to_validate))
				return (-1);
			to_validate++;
		}
	}
	return (0);
}
