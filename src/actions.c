/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aialonso <aialonso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/17 17:47:01 by aialonso          #+#    #+#             */
/*   Updated: 2026/02/22 20:49:09 by aialonso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	print_state(t_phio *philo, char *print)
{
	pthread_mutex_lock(philo->rules->dead_mutex);
	if (philo->rules->finished != 1)
	{
		pthread_mutex_unlock(philo->rules->dead_mutex);
		pthread_mutex_lock(philo->rules->print_mutex);
		printf("%ld %d %s\n", gettimeinmil(), philo->id, print);
		pthread_mutex_unlock(philo->rules->print_mutex);
		return ;
	}
	pthread_mutex_unlock(philo->rules->dead_mutex);
}

int	take_forks(t_phio	*philo, int pick_or_drop)
{
	if (philo->rules->nb_philos == 1)
		return (1);
	if (pick_or_drop == 1)
	{
		if (philo->id % 2 == 0)
		{
			pthread_mutex_lock(philo->left_fork);
			print_state(philo, MSG_FORK);
			pthread_mutex_lock(philo->right_fork);
			print_state(philo, MSG_FORK);
		}
		else
		{
			pthread_mutex_lock(philo->right_fork);
			print_state(philo, MSG_FORK);
			pthread_mutex_lock(philo->left_fork);
			print_state(philo, MSG_FORK);
		}
	}
	else
	{
		pthread_mutex_unlock(philo->right_fork);
		pthread_mutex_unlock(philo->left_fork);
	}
	return (0);
}

void	waiter(t_phio *philo, int acction)
{
	if (acction == 1)
	{
		pthread_mutex_lock(philo->rules->waiter_mutex);
		while (philo->rules->in_for_phase == philo->rules->nb_philos - 1
			&& philo->rules->nb_philos != 1)
		{
			pthread_mutex_unlock(philo->rules->waiter_mutex);
			usleep(100);
			pthread_mutex_lock(philo->rules->waiter_mutex);
		}
		philo->rules->in_for_phase++;
		pthread_mutex_unlock(philo->rules->waiter_mutex);
	}
	else
	{
		pthread_mutex_lock(philo->rules->waiter_mutex);
		philo->rules->in_for_phase--;
		pthread_mutex_unlock(philo->rules->waiter_mutex);
	}
}
