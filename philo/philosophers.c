/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aammisse <aammisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 17:02:38 by aammisse          #+#    #+#             */
/*   Updated: 2025/04/11 00:07:54 by aammisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void takefork2(t_philosopher *philo)
{
	pthread_mutex_lock(philo->right_f);
	printstate(philo, "has taken a fork");
	pthread_mutex_lock(philo->left_f);
	printstate(philo, "has taken a fork");
}

int	checkdeath(t_philosopher *philo)
{
	pthread_mutex_lock(&philo->info->death);
	if (philo->info->isdead == 1)
		return (pthread_mutex_unlock(&philo->info->death), 1);
	pthread_mutex_unlock(&philo->info->death);
	return (0);
}

void	*philoroutine(void *arg)
{
	t_philosopher	*philo;

	philo = (t_philosopher *)arg;
	if (philo->philo_number == 1)
	{
		singlephilo(philo);
		pthread_mutex_unlock(philo->right_f);
		return (NULL);
	}
	while (!checkdeath(philo))
	{
		ft_eat(philo);
		if (philo->meals_counter == philo->meals || checkdeath(philo))
		{
			if (philo->meals_counter == philo->meals)
			{
				pthread_mutex_lock(&philo->info->stop);
				philo->iseating = false;
				pthread_mutex_unlock(&philo->info->stop);
			}
			break ;
		}
	}
	return (NULL);
}

int checkeat(t_thread *info)
{
	int i;
	int res;

	i = -1;
	res = 0;
	while(++i < info->philo_number)
	{
		pthread_mutex_lock(&info->stop);
		if (info->philos[i].iseating == false)
			res++;
		pthread_mutex_unlock(&info->stop);
	}
	if (res == info->philo_number)
		return (1);
	return (0);
}

void *monitoring(void *arg)
{
	int	i;
	t_thread *info;

	info = (t_thread *)arg;
	while (1)
	{
		i = -1;
		while(++i < info->philo_number)
		{
			pthread_mutex_lock(&info->lastmeal);
				if (get_time() - info->philos[i].lastmeal > info->dietime && info->philos[i].iseating == true)
				{
					pthread_mutex_unlock(&info->lastmeal);
					printstate(&(info->philos[i]), "died");
					pthread_mutex_lock(&info->death);
					info->isdead = 1;
					info->philos[i].dead = 1;
					pthread_mutex_unlock(&info->death);
					return (NULL);
				}
			pthread_mutex_unlock(&info->lastmeal);
		}
		pthread_mutex_lock(&info->death);
		if (info->isdead == 1 || checkeat(info))
		{
			pthread_mutex_unlock(&info->death);
			break ;
		}
		pthread_mutex_unlock(&info->death);
		usleep(10);
	}
	return (NULL);
}

int startsim(t_thread *info)
{
	int	i;
	t_philosopher monitor;

	i = -1;
	if (pthread_create(&monitor.id, NULL, monitoring, info))
		return (0);
	while (++i < info->philo_number)
	{
		if (pthread_create(&info->philos[i].id, NULL,
				&philoroutine, &info->philos[i]))
			return (0);
	}
	i = -1;
	if (pthread_join(monitor.id, NULL))
		return (0);
	while (++i < info->philo_number)
		if (pthread_join(info->philos[i].id, NULL))
			return (0);
	return (1);
}

int	initphilo(t_thread *info)
{
	int	i;

	i = -1;
	info->starttime = get_time();
	while (++i < info->philo_number)
	{
		info->philos[i].dead = 0;
		info->philos[i].iseating = true;
		info->philos[i].deathpointer = &info->isdead;
		info->philos[i].starttime = info->starttime;
		info->philos[i].index = i + 1;
		info->philos[i].left_f = &info->forks[i];
		info->philos[i].right_f = &info->forks[(i + 1) % info->philo_number];
		info->philos[i].meals_counter = 0;
		info->philos[i].dietime = info->dietime;
		info->philos[i].sleeptime = info->sleeptime;
		info->philos[i].eattime = info->eattime;
		info->philos[i].meals = info->meals;
		info->philos[i].philo_number = info->philo_number;
		info->philos[i].lastmeal = get_time();
		info->philos[i].info = info;
	}
	return (1);
}

int	initdata(char **av, int ac, t_thread *info)
{
	int	i;

	info->isdead = 0;
	pthread_mutex_init(&info->write, NULL);
	pthread_mutex_init(&info->stop, NULL);
	pthread_mutex_init(&info->lastmeal, NULL);
	pthread_mutex_init(&info->death, NULL);
	info->shutdown = 0;
	info->philo_number = ft_atol(av[1]);
	info->dietime = ft_atol(av[2]);
	info->eattime = ft_atol(av[3]);
	info->sleeptime = ft_atol(av[4]);
	if (ac == 6)
		info->meals = ft_atol(av[5]);
	info->philos = malloc(sizeof(t_philosopher) * info->philo_number);
	if (!info->philos)
		return (0);
	info->forks = malloc(sizeof(pthread_mutex_t) * info->philo_number);
	if (!info->forks)
		return (free(info->philos), 0);
	i = -1;
	while (++i < info->philo_number)
		if (pthread_mutex_init(&info->forks[i], NULL))
			return (free(info->philos), free(info->forks), 0);
	return (1);
}

int	main(int ac, char **av)
{
	int			i;
	t_thread	info;

	if (av && ((ac != 5 && ac != 6) || !parseargs(av) || check_num(av)))
	{
		write(1, "Wrong Input!\n", 14);
		exit(0);
	}
	i = -1;
	info.ac = ac;
	info.av = av;
	info.meals = -1;
	info.philoeat = 0;
	if (!initdata(av, ac, &info))
		return (write(2, "Error\n", 7), 0);
	if (info.meals == 0)
		clear(&info);
	initphilo(&info);
	if (!startsim(&info))
		return (write(2, "Error\n", 7), 0);
	clear(&info);
	return (0);
}
