/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aammisse <aammisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/01 19:30:27 by aammisse          #+#    #+#             */
/*   Updated: 2025/03/06 20:03:50 by aammisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	ft_isdigit(int c)
{
	if (c >= '0' && c <= '9')
		return (1);
	return (0);
}

size_t	get_time(void)
{
	struct timeval	time;

	gettimeofday(&time, NULL);
	return ((time.tv_sec * 1000) + (time.tv_usec / 1000));
}

void	ft_usleep(size_t millisec)
{
	size_t	starttime;

	starttime = get_time();
	while ((get_time() - starttime) < millisec)
		usleep(200);
}

size_t	ft_strlen(const char *s)
{
	int	i;

	i = 0;
	while (s[i])
		++i;
	return (i);
}

void terminate(t_thread *info)
{
	int i;

	i = -1;
	while(++i < info->philo_number)
		pthread_mutex_destroy(&info->forks[i]);
	free(info->forks);
	free(info->philos);
	pthread_mutex_destroy(&info->write);
	pthread_mutex_destroy(&info->stop);
	pthread_mutex_destroy(&info->lastmeal);
	pthread_mutex_destroy(&info->death);
	exit(0);
}
