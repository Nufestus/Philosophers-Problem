/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aammisse <aammisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 17:02:38 by aammisse          #+#    #+#             */
/*   Updated: 2025/02/28 10:52:55 by aammisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

size_t	get_time(void)
{
	struct timeval	time;

	gettimeofday(&time, NULL);
	return ((time.tv_sec * 1000) + (time.tv_usec / 1000));
}

void ft_usleep(size_t millisec)
{
	size_t starttime;

	starttime = get_time();
	while((get_time() - starttime) < millisec)
		usleep(millisec / 10);
}

size_t	ft_strlen(const char *s)
{
	int	i;

	i = 0;
	while (s[i])
		++i;
	return (i);
}

void printstate(t_philosopher *philo, char *str)
{
	size_t time;

	pthread_mutex_lock(philo->write_lock);
	time = get_time() - philo->starttime;
	printf("%zu %d %s\n", time, philo->index, str);
	pthread_mutex_unlock(philo->write_lock);
}

void	error_message(char *text, int signal)
{
	if (text)
		write(2, text, ft_strlen(text) + 1);
	exit(signal);
}

void	destroy_all(t_thread *info, char *str, int count, int signal)
{
	while (--count >= 0)
		pthread_mutex_destroy(&info->forks[count]);
	pthread_mutex_destroy(&info->write);
	pthread_mutex_destroy(&info->lastmeal);
	error_message(str, signal);
}

void	philo_routine(t_philosopher *philo)
{
	pthread_mutex_lock(philo->left_f);
	printstate(philo, " has taken a fork");
	pthread_mutex_lock(philo->right_f);
	printstate(philo, " has taken a fork");
	pthread_mutex_lock(philo->lastmeal_lock);
	printstate(philo, " is eating");
	philo->lastmeal = get_time();
	philo->meals_counter += 1;
	pthread_mutex_unlock(philo->lastmeal_lock);
	ft_usleep(philo->eattime);
	pthread_mutex_unlock(philo->left_f);
	pthread_mutex_unlock(philo->right_f);
	printstate(philo, " is sleeping");
	ft_usleep(philo->sleeptime);
	printstate(philo, " is thinking");
}

void	*routine(void *ptr)
{
	t_philosopher	*philo;

	philo = (t_philosopher *)ptr;
	if (philo->id % 2 == 0)
		ft_usleep(1);
	pthread_mutex_lock(philo->lastmeal_lock);
	philo->starttime = get_time();
	philo->lastmeal = get_time();
	pthread_mutex_unlock(philo->lastmeal_lock);
	while (1)
		philo_routine(philo);
	return (NULL);
}

int mealscounter(t_thread *info)
{
	int i;
	int finish;

	i = -1;
	finish = 1;
	if (info->meals == -1)
		return (0);
	while (++i < info->philo_number)
	{
		pthread_mutex_lock(info->philos[i].lastmeal_lock);
		if (info->philos[i].meals_counter >= info->meals)
			finish++;
		pthread_mutex_unlock(info->philos[i].lastmeal_lock);
	}
	if (finish == info->philo_number)
	{
		pthread_mutex_lock(&info->write);
		return (1);
	}
	return (0);
}

void	*bigbrothersim(void *ptr)
{
	t_thread	*info;
	int		i;

	info = (t_thread *)ptr;
	while (1)
	{
		i = -1;
		while (++i < info->philo_number)
		{
			pthread_mutex_lock(info->philos[i].lastmeal_lock);
			if (get_time() - info->philos[i].lastmeal
				> info->dietime)
			{
				pthread_mutex_unlock(info->philos[i].lastmeal_lock);
				printstate(&info->philos[i], "has died");
				pthread_mutex_lock(info->philos[i].write_lock);
				return (NULL);
			}
			pthread_mutex_unlock(info->philos[i].lastmeal_lock);
		}
		if (mealscounter(info))
			return (NULL);
	}
	return (NULL);
}

int	startsimulation(t_thread *info)
{
	int i;
	pthread_t bigbrother;

	i = -1;
	while(++i < info->philo_number)
	{
		info->philos[i].lastmeal = get_time();
		if (pthread_create(&info->philos[i].id, NULL, routine, &info->philos[i]))
			destroy_all(info, "Thread Creation Failed\n", info->philo_number, 1);
	}
	if (pthread_create(&bigbrother, NULL, &bigbrothersim, &info))
		destroy_all(info, "Thread Creation Failed\n", info->philo_number, 1);
	i = -1;
	if (pthread_join(bigbrother, NULL))
		destroy_all(info, "Thread Creation Failed\n", info->philo_number, 1);
	while (++i < info->philo_number)
	{
		if (pthread_detach(info->philos[i].id) != 0)
			destroy_all(info, "[Thread Detach ERROR]\n", info->philo_number, 1);
	}
	return (1);
}

int	initphilo(t_thread *info)
{
	int i;
	
	i = 0;
	while(++i <= info->philo_number)
	{
		info->philos[i].starttime = get_time();
		info->philos[i].lastmeal = get_time();
		info->philos[i].index = i + 1;
		info->philos[i].deathcheck = &info->deathcheck;
		info->philos[i].left_f = &info->forks[i];
		info->philos[i].right_f = &info->forks[(i + 1) % info->philo_number];
		info->philos[i].meals_counter = 0;
		info->philos[i].dietime = info->dietime;
		info->philos[i].sleeptime = info->sleeptime;
		info->philos[i].eattime = info->eattime;
		info->philos[i].meals = info->meals;
		info->philos[i].philo_number = info->philo_number;
		info->philos[i].lastmeal_lock = &info->lastmeal;
		info->philos[i].write_lock = &info->write;
	}
	return (1);
}

int	initdata(char **av, int ac, t_thread *info)
{
	int i;

	info->deathcheck = 0;
	info->philo_number = ft_atol(av[1]);
	info->dietime = ft_atol(av[2]);
	info->eattime = ft_atol(av[3]);
	info->sleeptime = ft_atol(av[4]);
	if (ac == 6)
		info->meals = ft_atol(av[5]);
	else
		info->meals = -1;
	pthread_mutex_init(&info->lastmeal, NULL);
	pthread_mutex_init(&info->write, NULL);
	info->philos = malloc(sizeof(t_philosopher) * info->philo_number);
	if (!info->philos)
		return (0);
	info->forks = malloc(sizeof(pthread_mutex_t) * info->philo_number);
	if (!info->forks)
		return (free(info->philos), 0);
	pthread_mutex_init(&info->write, NULL);
	pthread_mutex_init(&info->lastmeal, NULL);
	i = -1;
	while(++i < info->philo_number)
		pthread_mutex_init(&info->forks[i], NULL);
	if (!initphilo(info))
		return (0);
	return (1);
}

int	main(int ac, char **av)
{
	t_thread info;
	
	if ((ac != 5 && ac != 6) || !parseargs(av))
	{
		write(1, "Wrong Input!\n", 14);
		exit(0);
	}
	info.ac = ac;
	info.av = av;
	if (!initdata(av, ac, &info))
		return (write(2, "Error\n", 7), 0);
	if (!startsimulation(&info))
		return (write(2, "Error\n", 7), 0);
	destroy_all(&info, NULL, info.philo_number, 0);
    return 0;
}