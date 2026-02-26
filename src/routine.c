/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aialonso <aialonso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/17 17:29:01 by aialonso          #+#    #+#             */
/*   Updated: 2026/02/26 19:03:39 by aialonso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	eat(t_phio *philo)
{
	waiter(philo, 1);
	if (take_forks(philo, 1))
	{
		waiter(philo, 0);
		return ;
	}
	pthread_mutex_lock(philo->rules->dead_mutex);
	philo->last_meal = gettimeinmil();
	pthread_mutex_unlock(philo->rules->dead_mutex);
	print_state(philo, MSG_EAT);
	usleep(philo->rules->t_to_eat * 1000);
	pthread_mutex_lock(philo->rules->dead_mutex);
	philo->meals_eaten++;
	pthread_mutex_unlock(philo->rules->dead_mutex);
	take_forks(philo, 0);
	waiter(philo, 0);
	print_state(philo, MSG_SLEEP);
	usleep(philo->rules->t_to_sleep * 1000);
}

void	*live(void *arg)
{
	t_phio	*philo;
	int		n;

	philo = (t_phio *)arg;
	n = 0;
	if (philo->id % 2 == 0)
		usleep(3000);
	pthread_mutex_lock(philo->rules->dead_mutex);
	philo->last_meal = gettimeinmil();
	pthread_mutex_unlock(philo->rules->dead_mutex);
	pthread_mutex_lock(philo->rules->dead_mutex);
	while (philo->rules->finished == 0)
	{
		if (philo->meals_eaten == philo->rules->must_eat)
		{
			pthread_mutex_unlock(philo->rules->dead_mutex);
			return (NULL);
		}
		pthread_mutex_unlock(philo->rules->dead_mutex);
		eat(philo);
		n++;
		pthread_mutex_lock(philo->rules->dead_mutex);
	}
	pthread_mutex_unlock(philo->rules->dead_mutex);
	return (NULL);
}
