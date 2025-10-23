/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lst_function.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: francesca <francesca@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/10 11:59:53 by francesca         #+#    #+#             */
/*   Updated: 2025/10/22 19:31:28 by francesca        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "miniRT.h"

/* reset dei soli campi lista (non libera: quello lo fa destroy) */
void objlist_init(t_scene *scene)
{
    scene->obj_head = NULL;
    scene->obj_end = NULL;
     /* --- contatori logici per validazione (A/C/L una sola volta) --- */
    scene->n_ambient = 0;
	scene->n_camera = 0;
	scene->n_lights = 0;
	scene->n_shapes = 0;
	/* --- contatori oggetti geometrici --- */
	scene->n_spheres = 0;
	scene->n_planes = 0;
	scene->n_cylinders = 0;
    scene->object_count = 0;
}

/* libera tutta la lista (da chiamare alla chiusura del programma) */
void objlist_destroy(t_scene *scene)
{
    t_objnode *current;
    t_objnode *next;

    if (!scene) 
        return;
    current = scene->obj_head;
    while (current) 
    { 
        next = current->next; 
        free(current); 
        current = next; 
    }
    scene->obj_head = NULL;
    scene->obj_end = NULL;
     /* --- contatori logici per validazione (A/C/L una sola volta) --- */
    scene->n_ambient = 0;
	scene->n_camera = 0;
	scene->n_lights = 0;
	scene->n_shapes = 0;
	/* --- contatori oggetti geometrici --- */
	scene->n_spheres = 0;
	scene->n_planes = 0;
	scene->n_cylinders = 0;
    scene->object_count = 0;
}

/* Aggiunge un nuovo nodo */
t_objnode *objnode_new(t_shape_type type, t_figures fig)
{
    t_objnode *n;
    
    n = (t_objnode *)malloc(sizeof(t_objnode));
    if (!n) 
        return NULL;
    n->type = type;
    n->fig  = fig;   /* copia per valore del union */
    n->next = NULL;
    return n;
}
/*Serve a aggiungere (append) un nuovo oggetto alla lista concatenata della scena, in O(1), 
tenendo in ordine gli oggetti come compaiono nel file e aggiornando i contatori.*/
int object_list_append(t_scene *scene, t_shape_type object_type, t_figures object_payload)
{
    t_objnode *new_node;

    /* 1) Validazione dei parametri di ingresso */
    if (scene == NULL)
        return print_err_msg("scene nulla");

    /* 2) Creazione del nodo da inserire */
    new_node = objnode_new(object_type, object_payload);
    if (new_node == NULL)
        return (print_err_msg("malloc nodo oggetto"));
    if (scene->obj_head == NULL) {
        /* 3a) Lista vuota: il nuovo nodo è sia testa che coda */
        scene->obj_head = new_node;
        scene->obj_end = new_node;
    } else 
    {
        /* 3b) Lista NON vuota:
               - collega il nuovo nodo dopo l’attuale coda
               - aggiorna il puntatore alla coda sul nuovo nodo */
        scene->obj_end->next = new_node;
        scene->obj_end = new_node;
    }
    /* 4) Aggiornare i contatori di scena */
    scene->object_count += 1;

    /* 5) Tutto ok */
    return 0;
}



