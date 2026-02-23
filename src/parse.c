/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aialonso <aialonso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/17 17:49:36 by aialonso          #+#    #+#             */
/*   Updated: 2026/02/17 17:50:09 by aialonso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	ft_isdigit(int c)
{
	if (c >= '0' && c <= '9')
		return (1);
	return (0);
}

long	ft_atoi(char *str)
{
	long	n[4];

	n[0] = 0;
	n[1] = 1;
	n[2] = 0;
	while ((str[n[0]] >= 9 && str[n[0]] <= 13) || str[n[0]] == 32)
		n[0]++;
	if (str[n[0]] == '-' || str[n[0]] == '+')
	{
		if (str[n[0]] == ('-'))
			n[1] = n[1] * -1;
		n[0]++;
	}
	while (ft_isdigit(str[n[0]]))
	{
		if (!ft_isdigit(str[n[0] + 1]))
		{
			n[2] = n[2] * 10 + (str[n[0]] - '0');
			return (n[1] * n[2]);
		}
		else
			n[2] = n[2] * 10 + (str[n[0]] - '0');
		n[0]++;
	}
	return (0);
}

int	validate(char **argv, int argc)
{
	int		num_arg;
	char	*to_validate;

	num_arg = 0;
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
