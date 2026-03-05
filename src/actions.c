/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aialonso <aialonso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/17 17:47:01 by aialonso          #+#    #+#             */
/*   Updated: 2026/03/05 15:54:00 by aialonso         ###   ########.fr       */
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
	static long time;
	
	if(philo->id == philo->rules->list[0])
	{
		time = philo->last_meal;
		return (0);	
	}
	if(philo->id == ((philo->rules->list[0] % philo->rules->nb_philos) + 1)
		|| philo->id == ((philo->rules->list[0] - 2 +philo->rules->nb_philos) % philo->rules->nb_philos + 1))
		if (time - gettimeinmil() >= philo->rules->t_to_eat)
			return (-1);
	return (0);
}

int	check_list(t_phio *philo)
{
	static int	chec_next_list;
	static int	chec_stoped_ferst;
	
	pthread_mutex_lock(philo->rules->waiter_mutex);
	if ((philo->rules->list[chec_next_list] == philo->id && chec_stoped_ferst < 2)
		|| (philo->rules->list[0] == philo->id && chec_stoped_ferst > 1))
	{
		if (*philo->id_left_fork == 0 && *philo->id_right_fork == 0 && dont_annoy_ferst(philo))
		{
			*philo->id_left_fork = 1;
			*philo->id_right_fork = 1;
			philo->rules->in_for_phase++;
			if (philo->rules->list[0] == philo->id)
				chec_stoped_ferst = 0;
			if (philo->rules->list[0] != philo->id)
				chec_stoped_ferst++;
			modo_list(philo, chec_next_list);
			chec_next_list = 0;
			pthread_mutex_unlock(philo->rules->waiter_mutex);
			return (0);
		}
		if (chec_next_list == (philo->rules->nb_philos - 1)
			|| philo->rules->in_for_phase == philo->rules->nb_philos / 2)
			chec_next_list = -1;
		chec_next_list++;
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
