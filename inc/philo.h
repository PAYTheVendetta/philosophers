/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aialonso <aialonso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/02 12:29:23 by aialonso          #+#    #+#             */
/*   Updated: 2026/02/09 17:40:49 by aialonso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <pthread.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

//This is the struct for the philo 

typedef struct s_rules
{
	long			nb_philos;
	long			t_to_dead;
	long			t_to_eat;
	long			t_to_sleep;
	long			must_eat;
	int				finished;
	long			start_time;
	pthread_mutex_t	*forks;
	pthread_mutex_t	*print_mutex;
}	t_rules;


typedef struct s_phio
{
	int				id;
	pthread_t		thread;
	long			last_meal;
	int				meals_eaten;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
	t_rules			*rules;
}	t_phio;

/* Utils */

long	ft_atoi(char *str);
int		freedi(t_phio **philos, t_rules **rules, int n_for, int n_thread);
long	gettimeinmil(void);
int		validate(char **argv, int argc);