/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   s_array.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qperez <qperez42@gmail.com>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2013/10/02 12:59:24 by qperez            #+#    #+#             */
/*   Updated: 2014/12/02 11:50:08 by qperez           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
** <This file contains s_array function>
** < init, destroy, clear >
** Copyright (C) <2013>  Quentin Perez <qperez42@gmail.com>
** This file is part of 42-toolkit.
** 42-toolkit is free software: you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
** You should have received a copy of the GNU General Public License
** along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <f_secure/f_secure.h>
#include <array/s_array.h>
#include <f_error/m_error.h>
#include <f_memory/f_memory.h>

static inline void		uf_array_delete(void *ptr)
{
	(void)ptr;
}

static inline size_t	uf_array_realloc(size_t size)
{
	return (size << 1);
}

bool					f_array_init(t_array *v_this,
							size_t (*uf_realloc)(size_t size),
							void (*uf_delete)(void *ptr), size_t type_size)
{
	v_this->v_size = 0;
	v_this->v_capacity = 0;
	v_this->f_realloc = uf_realloc;
	if (uf_realloc == NULL)
		v_this->f_realloc = uf_array_realloc;
	v_this->f_delete = uf_delete;
	if (uf_delete == NULL)
		v_this->f_delete = uf_array_delete;
	if ((v_this->v_data = uf_malloc_s(2, type_size)) == NULL)
		return (M_ERROR(false, "Bad alloc"));
	uf_memset(v_this->v_data, 0, 2 * type_size);
	v_this->v_capacity = 2;
	v_this->v_type_size = type_size;
	return (true);
}

void					f_array_clear(t_array *v_this)
{
	size_t	i;
	size_t	size;
	char	*ptr;

	i = 0;
	ptr = (char *)v_this->v_data;
	size = v_this->v_size * v_this->v_type_size;
	while (i < size)
	{
		v_this->f_delete((void*)(ptr + i));
		i = i + v_this->v_type_size;
	}
	v_this->v_size = 0;
}

void					f_array_destroy(t_array *v_this)
{
	D_ARRAY(clear)(v_this);
	uf_free_s((void **)&v_this->v_data);
	v_this->v_size = 0;
	v_this->v_type_size = 0;
	v_this->v_capacity = 0;
	v_this->v_data = 0;
	v_this->f_delete = NULL;
	v_this->f_realloc = NULL;
}
