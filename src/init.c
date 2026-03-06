/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aialonso <aialonso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/17 17:28:43 by aialonso          #+#    #+#             */
/*   Updated: 2026/03/06 18:22:03 by aialonso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

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

static int	init_rules_values(t_rules **rules, int argc, char **argv)
{
	if (validate(argv, (argc - 1)))
		return (freedi(NULL, rules, 0, NULL));
	(*rules)->nb_philos = ft_atoi(argv[0]);
	if ((*rules)->nb_philos <= 0)
		return (freedi(NULL, rules, 0 ,NULL));
	(*rules)->t_to_dead = ft_atoi(argv[1]);
	if ((*rules)->t_to_dead <= 0)
		return (freedi(NULL, rules, 0 ,NULL));
	(*rules)->t_to_eat = ft_atoi(argv[2]);
	if ((*rules)->t_to_eat <= 0)
		return (freedi(NULL, rules, 0 ,NULL));
	(*rules)->t_to_sleep = ft_atoi(argv[3]);
	if ((*rules)->t_to_sleep <= 0)
		return (freedi(NULL, rules, 0 ,NULL));
	return (0);
}

int	init(char **argv, t_rules **rules, int argc, t_phio	**philos)
{
	if (init_rules_values(rules, argc, argv) == -1)
		return (-1);
	(*rules)->finished = 0;
	(*rules)->in_for_phase = 0;
	(*rules)->must_eat = -1;
	(*rules)->list = malloc(sizeof(int) * ((*rules)->nb_philos + 1));
	if (!(*rules)->list)
		return (freedi(NULL, rules, 0 ,NULL));
	memset((*rules)->list, 0, (sizeof(int) * ((*rules)->nb_philos + 1)));
	if (argc == 6)
		(*rules)->must_eat = ft_atoi(argv[4]);
	if (argc == 6 && (*rules)->must_eat <= 0)
		return (freedi(NULL, rules, 0 ,NULL));
	if (memory_reserve(philos, rules) == -1)
		return (-1);
	if (start_thread(philos, *rules) == -1)
		return (-1);
	return (0);
}
