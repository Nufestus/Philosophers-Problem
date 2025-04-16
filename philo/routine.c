/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aammisse <aammisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/01 19:32:13 by aammisse          #+#    #+#             */
/*   Updated: 2025/03/06 19:34:01 by aammisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	printstate(t_philosopher *philo, char *str)
{
	size_t	time;

	pthread_mutex_lock(&philo->info->write);
	time = get_time() - philo->starttime;
	pthread_mutex_lock(&philo->info->death);
	if (philo->info->isdead == 0)
		printf("%zu %d %s\n", get_time() - philo->starttime, philo->index, str);
	pthread_mutex_unlock(&philo->info->death);
	pthread_mutex_unlock(&philo->info->write);
}

void	takefork(t_philosopher *philo)
{
	pthread_mutex_lock(philo->left_f);
	printstate(philo, "has taken a fork");
	pthread_mutex_lock(philo->right_f);
	printstate(philo, "has taken a fork");
}

void singlephilo(t_philosopher *philo)
{
	pthread_mutex_lock(philo->right_f);
	printstate(philo, "has taken a fork");
	ft_usleep(philo->dietime);
}

void	ft_eat(t_philosopher *philo)
{
	if (philo->index % 2 == 0)
		takefork(philo);
	else
		takefork2(philo);
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
