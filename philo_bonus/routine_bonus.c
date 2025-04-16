/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aammisse <aammisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 17:45:27 by aammisse          #+#    #+#             */
/*   Updated: 2025/03/05 17:45:43 by aammisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers_bonus.h"

void	printstate(t_philosopher *philo, char *str, int flag)
{
	size_t	time;

	if (flag)
	{
		sem_wait(philo->writephilo);
		time = get_time() - philo->starttime;
		printf("%zu %d %s\n", get_time() - philo->starttime, philo->index, str);
		sem_post(philo->writephilo);
	}
	else
	{
		sem_wait(philo->writephilo);
		time = get_time() - philo->starttime;
		if (!checkdeath(philo->info))
			printf("%zu %d %s\n", get_time() - philo->starttime, philo->index, str);
		sem_post(philo->writephilo);
	}
}

void	takefork(t_philosopher *philo)
{
	sem_wait(philo->forks);
	printstate(philo, "has taken a fork", 0);
	sem_wait(philo->forks);
	printstate(philo, "has taken a fork", 0);
}

void singlephilo(t_philosopher *philo)
{
	sem_wait(philo->forks);
	printstate(philo, "has taken a fork", 0);
	ft_usleep(philo->dietime + 3);
}

void	ft_eat(t_philosopher *philo)
{
	if (philo->index % 2 == 0)
		takefork(philo);
	else
		takefork2(philo);
	printstate(philo, "is eating", 0);
	sem_wait(philo->info->lastmeal);
	philo->lastmeal = get_time();
	philo->meals_counter++;
	sem_post(philo->info->lastmeal);
	ft_usleep(philo->eattime);
	sem_post(philo->forks);
	sem_post(philo->forks);
	printstate(philo, "is sleeping", 0);
	if (checkdeath(philo->info))
		return;
	ft_usleep(philo->sleeptime);
	printstate(philo, "is thinking", 0);
}
