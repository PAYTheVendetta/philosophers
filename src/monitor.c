/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aialonso <aialonso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/17 17:36:09 by aialonso          #+#    #+#             */
/*   Updated: 2026/03/02 19:22:15 by aialonso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	count_eating(t_phio	**philos, int philo)
{
	int	iter;
	int	cont_eting_finis;

	iter = -1;
	cont_eting_finis = 0;
	while (++iter < ((*philos)[0]).rules->nb_philos)
	{
		pthread_mutex_lock(((*philos)[iter]).rules->dead_mutex);
		if (((*philos)[iter]).rules->must_eat == ((*philos)[iter]).meals_eaten)
			cont_eting_finis++;
		if (cont_eting_finis == ((*philos)[iter]).rules->nb_philos)
		{
			((*philos)[iter]).rules->finished = 1;
		}
		pthread_mutex_unlock(((*philos)[iter]).rules->dead_mutex);
	}
	pthread_mutex_lock(((*philos)[philo]).rules->dead_mutex);
	if (((*philos)[philo]).meals_eaten == ((*philos)[philo]).rules->must_eat)
	{
		pthread_mutex_unlock(((*philos)[philo]).rules->dead_mutex);
		return (1);
	}
	pthread_mutex_unlock(((*philos)[philo]).rules->dead_mutex);
	return (0);
}

int	check_dead(t_phio	*philo)
{
	pthread_mutex_lock(philo->rules->dead_mutex);
	if ((gettimeinmil() - philo->last_meal) >= philo->rules->t_to_dead)
	{
		pthread_mutex_lock(philo->rules->print_mutex);
		philo->rules->finished = 1;
		printf("%ld %d %s\n", gettimeinmil(), philo->id, MSG_DIE);
		pthread_mutex_unlock(philo->rules->dead_mutex);
		pthread_mutex_unlock(philo->rules->print_mutex);
		return (1);
	}
	pthread_mutex_unlock(philo->rules->dead_mutex);
	return (0);
}

void	check_monitor(t_phio *philos)
{
	int	iter;

	iter = 0;
	while (philos->rules->nb_philos > iter)
	{
		pthread_mutex_lock(philos->rules->dead_mutex);
		if (philos->rules->finished == 1)
		{
			pthread_mutex_unlock(philos->rules->dead_mutex);
			return ;
		}
		pthread_mutex_unlock(philos->rules->dead_mutex);
		if (count_eating(&philos, iter) == 1)
		{
			iter++;
			pthread_mutex_lock(philos->rules->dead_mutex);
			continue ;
		}
		check_dead(&philos[iter]);
		iter++;
		usleep(10);
	}
}

void	monitor(t_phio *philos)
{
	while (1)
	{
		pthread_mutex_lock(philos->rules->dead_mutex);
		if (philos->rules->finished == 1)
		{
			pthread_mutex_unlock(philos->rules->dead_mutex);
			break ;
		}
		pthread_mutex_unlock(philos->rules->dead_mutex);
		check_monitor(philos);
	}
}
