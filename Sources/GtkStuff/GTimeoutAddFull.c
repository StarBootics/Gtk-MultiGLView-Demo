/*
 * GTimeoutAddFull.c
 * 
 * Copyright 2024 Guillaume Saumure <gsaumure@cgocable.ca>
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 * 
 * 
 */


#include "GTimeoutAddFull.h"

guint GTimeoutAddFull_GetID(GTimeoutAddFull* This)
{
	return This->ID;
}

void GTimeoutAddFull_Launch(GTimeoutAddFull* This)
{
	This->ID = g_timeout_add_full(This->Priority, This->Interval, This->Function, This->Data, NULL);
}

void GTimeoutAddFull_Init(GTimeoutAddFull* This, gint Priority, guint Interval, GSourceFunc Function, gpointer Data)
{
	This->GetID = GTimeoutAddFull_GetID;
	This->Launch = GTimeoutAddFull_Launch;
	
	This->ID = 0;
	This->Priority = Priority;
	This->Interval = Interval;
	This->Function = Function;
	This->Data = Data;
}

