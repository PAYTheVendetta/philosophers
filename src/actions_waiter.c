/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions_waiter.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aialonso <aialonso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/06 18:38:50 by aialonso          #+#    #+#             */
/*   Updated: 2026/03/06 18:38:50 by aialonso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	shift_wait_list(t_phio *philo, int iter)
{
	while (iter < (philo->rules->nb_philos - 1))
	{
		philo->rules->list[iter] = philo->rules->list[iter + 1];
		iter++;
	}
	philo->rules->list[philo->rules->nb_philos - 1] = 0;
}

static void	add_list(t_phio *philo, int id)
{
	int	iter;

	iter = 0;
	while (iter < philo->rules->nb_philos)
	{
		pthread_mutex_lock(philo->rules->waiter_mutex);
		if (philo->rules->list[iter] == 0)
		{
			philo->rules->list[iter] = id;
			pthread_mutex_unlock(philo->rules->waiter_mutex);
			return ;
		}
		pthread_mutex_unlock(philo->rules->waiter_mutex);
		iter++;
	}
}

static int	check_list(t_phio *philo)
{
	pthread_mutex_lock(philo->rules->waiter_mutex);
	if (philo->rules->list[0] == philo->id)
	{
		philo->rules->in_for_phase++;
		shift_wait_list(philo, 0);
		pthread_mutex_unlock(philo->rules->waiter_mutex);
		return (0);
	}
	pthread_mutex_unlock(philo->rules->waiter_mutex);
	return (-1);
}

void	waiter(t_phio *philo, int acction)
{
	pthread_mutex_lock(philo->rules->waiter_mutex);
	if (acction == 1)
	{
		pthread_mutex_unlock(philo->rules->waiter_mutex);
		add_list(philo, philo->id);
		while (1)
		{
			pthread_mutex_lock(philo->rules->waiter_mutex);
			if (philo->rules->in_for_phase < philo->rules->nb_philos / 2)
			{
				pthread_mutex_unlock(philo->rules->waiter_mutex);
				if (!check_list(philo))
					return ;
				usleep(100);
				continue ;
			}
			pthread_mutex_unlock(philo->rules->waiter_mutex);
			usleep(100);
		}
	}
	else
		philo->rules->in_for_phase--;
	pthread_mutex_unlock(philo->rules->waiter_mutex);
}
