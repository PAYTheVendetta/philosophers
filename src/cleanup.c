/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aialonso <aialonso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/17 17:50:36 by aialonso          #+#    #+#             */
/*   Updated: 2026/02/22 19:51:44 by aialonso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	safe_free(void	**ptr)
{
	free(*ptr);
	*ptr = NULL;
}

void	destroy_mutex(t_rules **rules, t_clear *clear)
{
	int	n_forks;

	n_forks = clear->forks_mutex_inited;
	if (clear->print_mutex_inited == 1 && *rules != NULL)
		pthread_mutex_destroy((*rules)->print_mutex);
	if (clear->dead_mutex_inited == 1 && *rules != NULL)
		pthread_mutex_destroy((*rules)->dead_mutex);
	if (clear->waiter_mutex_inited == 1 && *rules != NULL)
		pthread_mutex_destroy((*rules)->waiter_mutex);
	while (n_forks > 0 && *rules != NULL)
		pthread_mutex_destroy(&(*rules)->forks[--n_forks]);
}

int	freedi(t_phio **philos, t_rules **rules, int n_thread, t_clear *clear)
{
	if (n_thread != 0 && *rules != NULL)
	{
		pthread_mutex_lock((*rules)->dead_mutex);
		(*rules)->finished = 1;
		pthread_mutex_unlock((*rules)->dead_mutex);
	}
	while (n_thread != 0 && *philos != NULL)
		pthread_join((*philos)[n_thread-- - 1].thread, NULL);
	if (clear != NULL)
		destroy_mutex(rules, clear);
	if ((*rules)->print_mutex != NULL)
		safe_free((void **)&(*rules)->print_mutex);
	if ((*rules)->dead_mutex != NULL)
		safe_free((void **)&(*rules)->dead_mutex);
	if ((*rules)->waiter_mutex != NULL)
		safe_free((void **)&(*rules)->waiter_mutex);
	if ((*rules)->forks != NULL)
		safe_free((void **)&(*rules)->forks);
	if (clear != NULL)
		safe_free((void **)&clear);
	if (*rules != NULL)
		safe_free((void **)rules);
	if (*philos != NULL)
		safe_free((void **)philos);
	return (-1);
}
