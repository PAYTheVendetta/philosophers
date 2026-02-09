/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aialonso <aialonso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/14 17:47:23 by aialonso          #+#    #+#             */
/*   Updated: 2026/02/09 19:53:10 by aialonso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*live(void *arg)
{
	t_phio	*philo;
	int		n;

	philo = (t_phio *)arg;
	n = 0;
	philo->last_meal = gettimeinmil();
	while (philo->rules->finished == 0)
	{
		pthread_mutex_lock(philo->left_fork);
		pthread_mutex_lock(philo->rules->print_mutex);
		printf("%ld %d has taken a fork\n", gettimeinmil(), philo->id);
		pthread_mutex_unlock(philo->rules->print_mutex);
		if (philo->rules->nb_philos != 1)
		{
			pthread_mutex_lock(philo->right_fork);
			pthread_mutex_lock(philo->rules->print_mutex);
			printf("%ld %d has taken a fork\n", gettimeinmil(), philo->id);
			pthread_mutex_unlock(philo->rules->print_mutex);
			pthread_mutex_lock(philo->rules->print_mutex);
			printf("%ld %d is eating\n", gettimeinmil(), philo->id);
			pthread_mutex_unlock(philo->rules->print_mutex);
			usleep(philo->rules->t_to_eat * 1000);
			philo->last_meal = gettimeinmil();
			pthread_mutex_unlock(philo->right_fork);
		}
		pthread_mutex_unlock(philo->left_fork);
		if ((gettimeinmil() - philo->last_meal) >= philo->rules->t_to_dead)
			philo->rules->finished = 1;
		usleep(200000);
		n++;
	}
	return (NULL);
}

int	start_thread(t_phio **philos, t_rules *rules)
{
	int	n;
	int	i;

	i = -1;
	if (pthread_mutex_init(rules->print_mutex, NULL))
		return (freedi(philos, &rules, i, 0));
	while (++i < rules->nb_philos)
		if (pthread_mutex_init(&rules->forks[i], NULL))
			return (freedi(philos, &rules, (i + 1), 0));
	n = -1;
	while (++n < rules->nb_philos)
	{
		(*philos)[n].id = n + 1;
		(*philos)[n].left_fork = &rules->forks[n];
		(*philos)[n].right_fork = &rules->forks[(n + 1) % rules->nb_philos];
		(*philos)[n].rules = rules;
		(*philos)[n].last_meal = gettimeinmil();
		(*philos)[n].meals_eaten = 0;
		if (pthread_create(&(*philos)[n].thread, NULL, live, &(*philos)[n]))
			return (freedi(philos, &rules, n, i));
	}
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
	(*rules)->must_eat = -1;
	if (argc == 6)
		(*rules)->must_eat = ft_atoi(argv[4]);
	(*rules)->forks = malloc(sizeof(pthread_mutex_t) * (*rules)->nb_philos);
	if (!(*rules)->forks)
		return (freedi(philos, rules, 0, 0));
	(*rules)->print_mutex = malloc(sizeof(pthread_mutex_t));
	if (!(*rules)->print_mutex)
		return (freedi(philos, rules, 0, 0));
	*philos = malloc(sizeof(t_phio) * (*rules)->nb_philos);
	if (!*philos)
		return (freedi(philos, rules, 0, 0));
	memset(*philos, '\0', (sizeof(t_phio) * (*rules)->nb_philos));
	if (start_thread(philos, *rules) == -1)
		return (-1);
	return (0);
}

int	main(int argc, char *argv[])
{
	t_rules	*rules;
	t_phio	*philos;
	int		n;

	rules = malloc(sizeof(t_rules));
	if (!rules)
		return (freedi(NULL, NULL, 0, 0));
	if (argc != 5 && argc != 6)
	{
		printf("Please enter valid values");
		return (freedi(NULL, &rules, 0, 0));
	}
	if (validate(argv, argc))
		return (0);
	if (init(&argv[1], &rules, argc, &philos) == -1)
		return (0);
	n = rules->nb_philos;
	while (n >= 0)
	{
		pthread_join(philos[(n - 1)].thread, NULL);
		n--;
	}
	return (0);
}
