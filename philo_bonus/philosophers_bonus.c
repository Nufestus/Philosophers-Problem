/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers_bonus.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aammisse <aammisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/01 19:29:05 by aammisse          #+#    #+#             */
/*   Updated: 2025/03/05 18:04:21 by aammisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers_bonus.h"

void takefork2(t_philosopher *philo)
{
	pthread_mutex_lock(philo->right_f);
	printstate(philo, "has taken a fork");
	if (philo->philo_number == 1)
	{
		ft_usleep(philo->dietime * 2);
		return ;
	}
	pthread_mutex_lock(philo->left_f);
	printstate(philo, "has taken a fork");
}

void	*philoroutine(void *arg)
{
	t_philosopher	*philo;

	philo = (t_philosopher *)arg;
	while (!isdead(philo, 0))
	{
		pthread_create(&philo->bigbrother, NULL, check_death, philo);
		if (philo->index % 2 == 0)
			takefork(philo);
		else
			takefork2(philo);
		ft_eat(philo);
		pthread_detach(philo->bigbrother);
		if (philo->meals_counter == philo->meals)
		{
			pthread_mutex_lock(&philo->info->stop);
			if (++philo->info->philoeat == philo->info->philo_number)
			{
				pthread_mutex_unlock(&philo->info->stop);
				isdead(philo, 2);
			}
			pthread_mutex_unlock(&philo->info->stop);
			return (NULL);
		}
	}
	return (NULL);
}

int	initphilo(t_thread *info)
{
	int	i;

	i = -1;
	info->starttime = get_time();
	while (++i < info->philo_number)
	{
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
		info->philos[i].info = info;
		info->philos[i].id = fork();
		if (!info->philos[i].id)
			philoroutine(&info->philos[i]);
	}
	i = -1;
	return (1);
}

int	initdata(char **av, int ac, t_thread *info)
{
	int	i;

	sem_open(&info->write, O_CREAT | O_EXCL, 0644, 1);
	sem_open(&info->stop, O_CREAT | O_EXCL, 0644, 1);
	sem_open(&info->lastmeal, O_CREAT | O_EXCL, 0644, 1);
	sem_open(&info->death, O_CREAT | O_EXCL, 0644, 1);
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
	info->forks = malloc(sizeof(sem_t) * info->philo_number);
	if (!info->forks)
		return (free(info->philos), 0);
	i = -1;
	while (++i < info->philo_number)
		if (sem_open(&info->forks[i], O_CREAT | O_EXCL, 0644, 1))
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
		terminate(&info);
	if (!initphilo(&info))
		return (write(2, "Error\n", 7), 0);
	return (0);
}
