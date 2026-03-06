/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aialonso <aialonso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/02 12:29:23 by aialonso          #+#    #+#             */
/*   Updated: 2026/03/06 18:04:13 by aialonso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <sys/time.h>
# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <string.h>

# define MSG_EAT   "is eating"
# define MSG_SLEEP "is sleeping"
# define MSG_THINK "is thinking"
# define MSG_FORK  "has taken a fork"
# define MSG_DIE   "died"

//This is the struct for the philo

typedef struct s_rules
{
	long			nb_philos;
	long			t_to_dead;
	long			t_to_eat;
	long			t_to_sleep;
	long			must_eat;
	int				in_for_phase;
	int				finished;
	int				*list;
	pthread_mutex_t	*forks;
	pthread_mutex_t	*print_mutex;
	pthread_mutex_t	*waiter_mutex;
	pthread_mutex_t	*dead_mutex;
}	t_rules;

typedef struct s_clear
{
	int	print_mutex_inited;
	int	dead_mutex_inited;
	int	waiter_mutex_inited;
	int	forks_mutex_inited;
}	t_clear;

typedef struct s_phio
{
	int				id;
	pthread_t		thread;
	long			last_meal;
	int				meals_eaten;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
	t_rules			*rules;
	t_clear			*clear;
}	t_phio;

/* Parse */

long	ft_atoi(const char *nptr);
int		validate(char **argv, int argc);
int		ft_isdigit(int c);

/* Time */

long	gettimeinmil(void);

/* Init */

int		start_thread(t_phio **philos, t_rules *rules);
int		init(char **argv, t_rules **rules, int argc, t_phio	**philos);

/* Rutine */

void	eat(t_phio *philo);
void	*live(void *arg);

/* Actions */

void	print_state(t_phio *philo, char *print);
int		take_forks(t_phio	*philo, int pick_or_drop);
void	waiter(t_phio *philo, int acction);

/* Monitor */

int		count_eating(t_phio	**philos, int philo);
int		check_dead(t_phio	*philo);
void	check_monitor(t_phio *philos);
void	monitor(t_phio *philos);

/* Cleanup */

void	safe_free(void	**ptr);
int		freedi(t_phio **philos, t_rules **rules, int n_thread, t_clear *clear);

#endif