/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aialonso <aialonso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/17 17:28:31 by aialonso          #+#    #+#             */
/*   Updated: 2026/03/02 18:20:39 by aialonso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	main(int argc, char *argv[])
{
	t_rules	*rules;
	t_phio	*philos;
	int		n;

	if (argc < 5 || argc > 6)
	{
		printf("Please enter valid values");
		return (0);
	}
	rules = malloc(sizeof(t_rules));
	if (!rules)
		return (freedi(NULL, NULL, 0, NULL));
	memset(rules, '\0', sizeof(t_rules));
	if (init(&argv[1], &rules, argc, &philos) == -1)
		return (0);
	n = rules->nb_philos - 1;
	monitor(philos);
	while (n >= 0)
	{
		pthread_join(philos[n].thread, NULL);
		n--;
	}
	freedi(&philos, &rules, 0, philos->clear);
	return (0);
}
