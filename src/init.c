/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aialonso <aialonso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/17 17:28:43 by aialonso          #+#    #+#             */
/*   Updated: 2026/02/26 18:21:17 by aialonso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

t_clear	*init_clear(void)
{
	t_clear	*clear;

	clear = malloc(sizeof(t_clear));
	clear->dead_mutex_inited = 0;
	clear->forks_mutex_inited = 0;
	clear->print_mutex_inited = 0;
	clear->waiter_mutex_inited = 0;
	return (clear);
}

int	create_mutex(t_phio **philos, t_rules *rules, t_clear *clear)
{
	int	i;

	i = -1;
	if (pthread_mutex_init(rules->print_mutex, NULL))
		return (freedi(philos, &rules, 0, clear));
	clear->print_mutex_inited = 1;
	if (pthread_mutex_init(rules->waiter_mutex, NULL))
		return (freedi(philos, &rules, 0, clear));
	clear->waiter_mutex_inited = 1;
	if (pthread_mutex_init(rules->dead_mutex, NULL))
		return (freedi(philos, &rules, 0, clear));
	clear->dead_mutex_inited = 1;
	while (++i < rules->nb_philos)
	{
		if (pthread_mutex_init(&rules->forks[i], NULL))
			return (freedi(philos, &rules, 0, clear));
		clear->forks_mutex_inited = i;
	}
	return (0);
}

int	start_thread(t_phio **philos, t_rules *rules)
{
	int		n;
	t_clear	*clear;

	clear = init_clear();
	if (create_mutex(philos, rules, clear) == -1)
		return (-1);
	n = -1;
	while (++n < rules->nb_philos)
	{
		(*philos)[n].id = n + 1;
		(*philos)[n].left_fork = &rules->forks[n];
		(*philos)[n].right_fork = &rules->forks[(n + 1) % rules->nb_philos];
		(*philos)[n].rules = rules;
		(*philos)[n].last_meal = gettimeinmil();
		(*philos)[n].meals_eaten = 0;
		(*philos)[n].clear = clear;
		if (pthread_create(&(*philos)[n].thread, NULL, live, &(*philos)[n]))
			return (freedi(philos, &rules, n, clear));
	}
	return (0);
}

int	memory_reserve(t_phio	**philos, t_rules **rules)
{
	(*rules)->forks = malloc(sizeof(pthread_mutex_t) * (*rules)->nb_philos);
	if (!(*rules)->forks)
		return (freedi(philos, rules, 0, NULL));
	(*rules)->print_mutex = malloc(sizeof(pthread_mutex_t));
	if (!(*rules)->print_mutex)
		return (freedi(philos, rules, 0, NULL));
	(*rules)->waiter_mutex = malloc(sizeof(pthread_mutex_t));
	if (!(*rules)->waiter_mutex)
		return (freedi(philos, rules, 0, NULL));
	(*rules)->dead_mutex = malloc(sizeof(pthread_mutex_t));
	if (!(*rules)->dead_mutex)
		return (freedi(philos, rules, 0, NULL));
	*philos = malloc(sizeof(t_phio) * (*rules)->nb_philos);
	if (!*philos)
		return (freedi(philos, rules, 0, NULL));
	memset(*philos, '\0', (sizeof(t_phio) * (*rules)->nb_philos));
	return (0);
}

int	init(char **argv, t_rules **rules, int argc, t_phio	**philos)
{
	memset(*rules, '\0', sizeof(t_rules));
	(*rules)->nb_philos = ft_atoi(argv[0]);
	(*rules)->t_to_dead = ft_atoi(argv[1]);
	(*rules)->t_to_eat = ft_atoi(argv[2]);
	(*rules)->t_to_sleep = ft_atoi(argv[3]);
	(*rules)->finished = 0;
	(*rules)->in_for_phase = 0;
	(*rules)->must_eat = -1;
	(*rules)->list = malloc(sizeof(int) * (*rules)->nb_philos);
	if (!(*rules)->list)
		return (-1);
	memset((*rules)->list, 0, (sizeof(int) * (*rules)->nb_philos));
	if (argc == 6)
		(*rules)->must_eat = ft_atoi(argv[4]);
	if (memory_reserve(philos, rules) == -1)
		return (-1);
	if (start_thread(philos, *rules) == -1)
		return (-1);
	return (0);
}
