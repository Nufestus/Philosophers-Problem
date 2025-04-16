/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aammisse <aammisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/02 21:33:24 by aammisse          #+#    #+#             */
/*   Updated: 2025/03/05 17:44:33 by aammisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers_bonus.h"

size_t	ft_strlcat(char *dst, const char *src, size_t dstsize)
{
	size_t	i;
	size_t	j;
	size_t	dstlen;
	size_t	srclen;

	srclen = ft_strlen(src);
	if (dstsize == 0 && dst == NULL)
		return (srclen);
	dstlen = ft_strlen(dst);
	if (dstlen >= dstsize)
		return (dstsize + srclen);
	i = dstlen;
	j = 0;
	while (src[j] != '\0' && i < dstsize - 1)
	{
		dst[i] = src[j];
		i++;
		j++;
	}
	dst[i] = '\0';
	return (srclen + dstlen);
}

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

void clear(t_thread *info)
{
	sem_close(info->write);
	sem_close(info->stop);
	sem_close(info->death);
	sem_close(info->lastmeal);
	sem_close(info->forks);
	sem_unlink("/forks");
	sem_unlink("/write");
	sem_unlink("/death");
	sem_unlink("/stop");
	sem_unlink("/lastmeal");
	free(info->philos);
	exit(0);
}

static int	count_len(int n)
{
	int	len;

	len = 0;
	if (n <= 0)
		len = 1;
	while (n != 0)
	{
		len++;
		n /= 10;
	}
	return (len);
}

static char	*rtrn(int n, char *num, size_t len)
{
	unsigned int	abs;
	char			*index;

	index = "0123456789";
	num[len--] = '\0';
	if (n == 0)
	{
		num[0] = '0';
		return (num);
	}
	if (n < 0)
	{
		num[0] = '-';
		abs = -n;
	}
	else
		abs = n;
	while (abs > 0)
	{
		num[len--] = index[abs % 10];
		abs /= 10;
	}
	return (num);
}

char	*ft_strdup(const char *s1)
{
	size_t	i;
	char	*copy;
	char	*str;

	i = 0;
	str = (char *)s1;
	copy = (char *) malloc(ft_strlen(str) + 1);
	if (copy == NULL)
		return (NULL);
	while (s1[i] != '\0')
	{
		copy[i] = str[i];
		i++;
	}
	copy[i] = '\0';
	return (copy);
}

char	*ft_itoa(int n)
{
	char	*num;
	size_t	len;

	len = count_len(n);
	if (n == -2147483648)
		return (ft_strdup("-2147483648"));
	num = (char *) malloc (len + 1);
	if (num == NULL)
		return (NULL);
	num = rtrn(n, num, len);
	return (num);
}

static char	*handle(char *str, char *str1)
{
	char	*string;

	if (str1 != NULL && str == NULL)
	{
		string = malloc(ft_strlen(str1) + 1);
		if (!string)
			return (NULL);
		ft_strlcat(string, str1, ft_strlen(str1) + 1);
		return (string);
	}
	else if (str != NULL && str1 == NULL)
	{
		string = malloc(ft_strlen(str) + 1);
		if (!string)
			return (NULL);
		ft_strlcat(string, str, ft_strlen(str) + 1);
		return (string);
	}
	else
		return (NULL);
}

static void	fillfirst(const char *s1, char *join, int i)
{
	while (s1[i] != '\0')
	{
		join[i] = s1[i];
		i++;
	}
}

char	*ft_strjoin(const char *s1, const char *s2)
{
	size_t	i;
	size_t	j;
	char	*join;
	size_t	s1len;
	size_t	s2len;

	if (!s1 || !s2)
		return (handle((char *)s1, (char *)s2));
	s1len = ft_strlen(s1);
	s2len = ft_strlen(s2);
	i = 0;
	j = 0;
	join = (char *) malloc(s1len + s2len + 1);
	if (join == NULL)
		return (NULL);
	fillfirst(s1, join, i);
	i = s1len;
	while (s2[j] != '\0')
	{
		join[i] = s2[j];
		j++;
		i++;
	}
	join[i] = '\0';
	return (join);
}
