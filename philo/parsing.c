/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aammisse <aammisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/22 13:46:46 by aammisse          #+#    #+#             */
/*   Updated: 2025/03/04 22:38:12 by aammisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

long long	ft_atol(char *str)
{
	int			i;
	long long	nb;
	long long	sign;

	nb = 0;
	sign = 1;
	i = 0;
	if (str[i] == '+')
		i++;
	else if (str[i] == '-')
	{
		sign = -1;
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		nb = (nb * 10) + (str[i] - '0');
		if (nb > INT_MAX || nb < INT_MIN)
			return (nb);
		i++;
	}
	return (nb * sign);
}

int	parseargs(char **av)
{
	int			i;
	long long	number;

	i = 0;
	while (av[i])
	{
		number = ft_atol(av[i]);
		if (number > INT_MAX || number < 0)
			return (0);
		i++;
	}
	return (1);
}

int	check_num(char **str)
{
	int	i;
	int	j;

	i = 1;
	while (str[i])
	{
		j = 0;
		while (str[i][j])
		{
			if (!ft_isdigit(str[i][j]))
				return (1);
			j++;
		}
		i++;
	}
	return (0);
}
