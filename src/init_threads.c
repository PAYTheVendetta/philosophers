/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_threads.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aialonso <aialonso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/06 18:39:04 by aialonso          #+#    #+#             */
/*   Updated: 2026/03/06 18:39:04 by aialonso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static t_clear	*init_clear(void)
{
	t_clear	*clear;

	clear = malloc(sizeof(t_clear));
	if (!clear)
		return (NULL);
	clear->dead_mutex_inited = 0;
	clear->forks_mutex_inited = -1;
	clear->print_mutex_inited = 0;
	clear->waiter_mutex_inited = 0;
	return (clear);
}

static int	create_mutex(t_phio **philos, t_rules *rules, t_clear *clear)
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
	if (!clear)
		return (freedi(philos, &rules, 0, clear));
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
