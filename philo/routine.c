/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aammisse <aammisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/01 19:32:13 by aammisse          #+#    #+#             */
/*   Updated: 2025/03/01 19:36:07 by aammisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	isdead(t_philosopher *philo, int flag)
{
	pthread_mutex_lock(&philo->info->death);
	if (flag)
		philo->info->shutdown = 1;
	if (philo->info->shutdown)
	{
		pthread_mutex_unlock(&philo->info->death);
		exit(0);
		return (1);
	}
	pthread_mutex_unlock(&philo->info->death);
	return (0);
}

void	printstate(t_philosopher *philo, char *str)
{
	size_t	time;

	pthread_mutex_lock(&philo->info->write);
	time = get_time() - philo->starttime;
	if (time >= 0 && !isdead(philo, 0))
		printf("%zu %d %s\n", get_time() - philo->starttime, philo->index, str);
	pthread_mutex_unlock(&philo->info->write);
}

void	*check_death(void *arg)
{
	t_philosopher	*philo;

	philo = (t_philosopher *)arg;
	ft_usleep(philo->dietime + 1);
	pthread_mutex_lock(&philo->info->lastmeal);
	pthread_mutex_lock(&philo->info->stop);
	if (!isdead(philo, 0) && get_time() - philo->lastmeal >= philo->dietime)
	{
		pthread_mutex_unlock(&philo->info->lastmeal);
		pthread_mutex_unlock(&philo->info->stop);
		printstate(philo, "has died");
		isdead(philo, 1);
		return (NULL);
	}
	pthread_mutex_unlock(&philo->info->lastmeal);
	pthread_mutex_unlock(&philo->info->stop);
	return (NULL);
}

void	takefork(t_philosopher *philo)
{
	pthread_mutex_lock(philo->left_f);
	printstate(philo, "has taken a fork");
	if (philo->philo_number == 1)
	{
		ft_usleep(philo->dietime * 2);
		return ;
	}
	pthread_mutex_lock(philo->right_f);
	printstate(philo, "has taken a fork");
}

void	ft_eat(t_philosopher *philo)
{
	printstate(philo, "is eating");
	pthread_mutex_lock(&philo->info->lastmeal);
	philo->lastmeal = get_time();
	philo->meals_counter++;
	pthread_mutex_unlock(&philo->info->lastmeal);
	ft_usleep(philo->eattime);
	pthread_mutex_unlock(philo->right_f);
	pthread_mutex_unlock(philo->left_f);
	printstate(philo, "is sleeping");
	ft_usleep(philo->sleeptime);
	printstate(philo, "is thinking");
}
