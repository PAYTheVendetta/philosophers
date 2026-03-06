/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aialonso <aialonso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/17 17:47:01 by aialonso          #+#    #+#             */
/*   Updated: 2026/03/06 13:18:45 by aialonso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	modo_list(t_phio *philo, int iter)
{
	while (iter < (philo->rules->nb_philos - 1))
	{
		if (philo->rules->list[iter + 1] != 0)
			philo->rules->list[iter] = philo->rules->list[iter + 1];
		else
		{
			philo->rules->list[iter] = philo->rules->list[iter + 1];
			philo->rules->list[philo->rules->nb_philos - 1] = 0;
			return ;
		}
		iter++;
	}
}

void	add_list(t_phio *philo, int id)
{
	int iter;

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

int	dont_annoy_ferst(t_phio *philo)
{
	static long mod_strict;
	static long id_traker;
	
	if (id_traker != philo->rules->list[0])
		mod_strict = 0;
	if(philo->id == philo->rules->list[0])
	{
		id_traker = philo->id;
		pthread_mutex_lock(philo->rules->dead_mutex);
		if ((philo->rules->t_to_dead - (gettimeinmil() - philo->last_meal)) > philo->rules->t_to_eat + 10)
			mod_strict = 1;
		pthread_mutex_unlock(philo->rules->dead_mutex);
		return (0);
	}
	if(philo->id == ((philo->rules->list[0] % philo->rules->nb_philos) + 1)
		|| philo->id == ((philo->rules->list[0] - 2 +philo->rules->nb_philos) % philo->rules->nb_philos + 1))
		if (mod_strict == 1)
			return (-1);
	return (0);
}

int	check_list(t_phio *philo)
{
	pthread_mutex_lock(philo->rules->waiter_mutex);
	if (philo->rules->list[philo->rules->chec_next_list] == philo->id)
	{
		if (*philo->id_left_fork == 0 && *philo->id_right_fork == 0 && dont_annoy_ferst(philo) == 0)
		{
			*philo->id_left_fork = 1;
			*philo->id_right_fork = 1;
			philo->rules->in_for_phase++;
			modo_list(philo, philo->rules->chec_next_list);
			philo->rules->chec_next_list = 0;
			pthread_mutex_unlock(philo->rules->waiter_mutex);
			return (0);
		}
		if (philo->rules->chec_next_list == (philo->rules->nb_philos - 1)
			|| philo->rules->in_for_phase == philo->rules->nb_philos / 2)
			philo->rules->chec_next_list = -1;
		philo->rules->chec_next_list++;
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
	{
		*philo->id_left_fork = 0;
		*philo->id_right_fork = 0;
		philo->rules->in_for_phase--;
	}
	pthread_mutex_unlock(philo->rules->waiter_mutex);
}
