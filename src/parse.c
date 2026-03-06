/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aialonso <aialonso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/17 17:49:36 by aialonso          #+#    #+#             */
/*   Updated: 2026/03/06 18:03:36 by aialonso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	ft_isdigit(int c)
{
	if (c >= '0' && c <= '9')
		return (1);
	return (0);
}

long	ft_atoi(const char *nptr)
{
	int		signo;
	int		num;

	signo = 1;
	num = 0;
	while ((*nptr >= 9 && *nptr <= 13) || *nptr == 32)
		nptr++;
	if (*nptr == '-' || *nptr == '+')
	{
		if (*nptr == '-')
			signo = signo * -1;
		nptr++;
	}
	while (*nptr >= '0' && *nptr <= '9')
	{
		if (!(*(nptr + 1) >= '0' && *(nptr + 1) <= '9'))
		{
			num = num * 10 + (*nptr - '0');
			return (signo * num);
		}
		else
			num = num * 10 + (*nptr - '0');
		nptr++;
	}
	return (0);
}

int	validate(char **argv, int argc)
{
	int		num_arg;
	char	*to_validate;

	num_arg = -1;
	while (++num_arg < argc)
	{
		to_validate = argv[num_arg];
		if (!*to_validate)
			return (-1);
		while (*to_validate)
		{
			if (*to_validate == '+')
				if (!ft_isdigit(*(++to_validate)))
					return (-1);
			if (!ft_isdigit(*to_validate))
				return (-1);
			to_validate++;
		}
	}
	return (0);
}
