/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   miniRT_struct.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: francesca <francesca@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 15:29:54 by francesca         #+#    #+#             */
/*   Updated: 2025/10/22 21:03:40 by francesca        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINI_RT_STRUCT_H
# define MINI_RT_STRUCT_H

# include <stdbool.h>
 #include <stddef.h>

 # define MAX_LIGHTS 10 // ARRAY STATICI PER TESTARE PHONG SENZA MALLOCCARE
# define MAX_SHAPES 50 //

/* --- Coordinate --- */
// VETTORE 4D X,Y,Z SONO COORD NELLO SPAZIO W PER TRASFORMAZIONI OMOGENEE 
// usato per Posizioni degli oggetti, Direzioni dei raggi, Normali
typedef struct s_vector
{
	double	x;
	double	y;
	double	z;
	double	w;
}			t_vector;

/* --- Colori --- */
// Colori interni in spazio lineare [0..1]
/*
PERCHE' RGB IN DOUBLE
Quando fai i calcoli di luce, riflessione, ombreggiature, ecc.,
il programma deve moltiplicare e sommare i colori —
e deve farlo con precisione decimale, non con numeri interi.
Se hai un’illuminazione del 40% (ratio = 0.4)
e un colore rosso 255,0,0
allora il colore illuminato sarà: 255 * 0.4 = 102 
Ma se tu usassi solo interi (0 o 1), perderesti tutta la sfumatura.
*/
/*
Rappresenta un colore 
r = rosso
g = verde
b = blue
a = trasparenza
*/
typedef struct s_color
{
	double	r;
	double	g;
	double	b;
	double	a;
}			t_color;

typedef struct s_shape t_shape; //  AGGIUNGI QUESTA RIGA per compilazione
//CAMBIA ORDINE STRUTTURE
//contiene tutti i componenti della luce per un oggetto serve per calcolare quanto luce arriva su un punto di un oggetto
typedef struct s_phong		t_phong;
struct	s_phong
{
	t_color	effective_color;		//il colore dell’oggetto dopo essere stato "influenzato" dalla luce
	t_color	diffuse;				//la parte di luce diffusa, che colpisce la superficie e si "spalma"
	t_color	specular;				//la parte riflessa (i punti brillanti)
	t_color	ambient;				//la luce di base che illumina tutto (anche in ombra)
};

//RAY - un raggio ha un punto di partenza e una direzione
typedef struct s_ray t_ray;
struct s_ray
{
	t_vector	origin;    // punto di partenza del raggio (w = 1 se è posizione)
	t_vector	dir;       // direzione del raggio (w = 0), generalmente normalizzata
};

//INTERSEZIONI - intersezione è quando il raggio colpisce qualcosa
typedef struct s_intersect t_intersection;
struct s_intersect
{
	double		time;       // ci dice quando - parametro t lungo il raggio: punto = origin + t * dir 
	t_shape		*shape;     // ci dice cosa è stato colpito - puntatore alla shape colpita (non copia!)
	t_vector	point;      // punto di impatto , usato per illuminazione - punto di intersezione nello spazio mondo (può essere popolato più tardi)
	t_vector	normal;     // usato per illuminazione - normale della superficie al punto (già trasformata in world space)
	t_vector	eye;        // usato per illuminazione - vettore dalla superficie verso la camera (eye = -ray.dir) dal punto verso la camera
	t_vector	over_point; // punto leggermente spostato lungo la normale per evitare artefatti di ombre
	t_vector	reflect_vec;// vettore riflesso, utile per calcolare riflessioni
	bool		inside;     // true se il raggio partiva dall'interno dell'oggetto
};

// LISTA DI INTERSEZIONI  - Tiene traccia di tutti i punti colpiti 
typedef struct s_intersections t_intersections;
struct s_intersections
{
	t_intersection	arr[200];  // array statico per le intersezioni trovate (sufficiente per scene semplici) - una sfera ha 2 punti di intersezione entrata e uscita
	int				count;     // quanti elementi validi ci sono in arr (inizializzare a 0 prima dell'uso) quanti impatti 
};

/* --- Ambiente --- */
// Ambient lighting (A ratio [0..1], RGB [0..255] -> normalizzati)
typedef struct s_ambient
{
	double	intensity; // Valore da 0.0 a 1.0 (quanto è forte la luce ambiente)
	t_color color; // normalizzato 0..1
	bool present;  // per validare che A compaia una sola volta
}			t_ambient;

// MATRICI 
typedef double			t_mat4[4][4];          // trasformazioni 3D (traslazioni, rotazioni, scaling)
typedef double			t_mat3[3][3];          // utile per determinanti, inversione parziale
typedef double			t_mat2[2][2];          // minor uso, solo per calcoli di supporto

/* --- Camera --- */
// Camera (C pos, dir normalizzata, FOV in gradi [0..180])
// come un occhio nello spazio 3D
// Tutto quello che serve per generare i raggi lo metto dentro questa struttura
typedef struct s_camera
{
	t_vector	pos;
	t_vector 	dir;     // normalizzata
	int    		fov_deg;    // come da file .rt (per debug/log) DA TOGLIERE  e lasciare solo double fov
	double		fov;            // Field of view, campo visivo della camera, in radianti quanto zoommato
	bool		present;
	double		pixel_size;     // Dimensione di un pixel nello spazio 3D proiettato sulla scena, per sapere quanto spostarsi nello spazio per passare da un pixel all’altro
	double		half_width;     // Metà larghezza e altezza del viewport (la “finestra” della camera) in unità della scena, per centrare i raggi
	double		half_height;
	t_mat4		transform;      // Matrice che trasforma i punti dalla camera allo spazio mondo, quando vogliamo spostare o ruotare la camera
	t_mat4		inv_trans;      // Matrice inversa di transform, Serve per trasformare i raggi dal mondo nello spazio locale della camera, fondamentale
}			t_camera;

/* --- Luce --- */

/*
Tipo di luce
 */
typedef enum e_light_type	t_light_type;
enum e_light_type
{
	POINT,                                          //luce puntiforme (una lampadina), emette luce in tutte le direzioni.
	//SPOT                                            //luce direzionale con angolo di apertura (tipo torcia o faretto), direzionale e con cutoff
};

//Fonte di luce 
// Light (L pos, intensità [0..1], colore)
typedef struct s_light
{
	t_light_type	type;            // POINT o SPOT
	t_vector		position;        // Dove si trova la luce - Serve per calcolare distanza, direzione, ombre
	//t_vector		direction;       // (solo per SPOT) direzione della luce
	//t_vector		init_direction;  // (per rotazioni iniziali, SPOT)
	//t_mat4			added_rots;  // Rotazioni accumulate (SPOT)
	double intensity; // [0..1]
	t_color color;    // normalizzato 0..1
	bool	present;
}			t_light;
/*
Luce ambientale 
La luce ambientale simula la luce diffusa che colpisce tutto, 
anche le zone non direttamente illuminate.
 */

/* --- Oggetti (solo struct, niente logica) --- */
//NOTA:
/*
Questo è molto più flessibile rispetto all’approccio “unione”,
perché tutte le shape condividono una struttura comune, 
e l’enum decide solo quale logica usare, non quale union leggere
*/


/*
identifica il tipo geometrico della forma
quando ho un array generico di t_shape devo capire quale funzione di intersezione chiamare
SPHERE → intersezione raggio-sfera
PLANE → intersezione raggio-piano
CYLINDER → intersezione raggio-cilindro
 */
typedef enum e_shape_type {
    SPHERE,
    PLANE,
    CYLINDER							// Da implementare
}	t_shape_type;

// DIVERSE PROPRIETà DEGLI OGGETI 
typedef struct s_shape_props
{
	double		radius;					// quanto è grande 
	double		radius_squared;    		// serve per evitare di fare sqrt() inutili
	//double		height;				// quanto è alto
	t_color		color;         		    
	double		diffuse;           	//  come appare alla luce 
	double		specular;          	//  come appare alla luce
	double		shininess;         	
	double		reflectiveness;		// é riflettente
	double		distance_from_origin;   // aggiunto per intersection Joy
}	t_shape_props;

typedef struct s_shape {
    t_shape_type type;      			//dice che tipo di oggetto è (sphere, plane, ecc.)
	int id;   							// ???? identificativo univoco (utile per debug, picking, editor)
	t_shape_props props;        		// tutte le proprietà fisiche/estetiche definite sopra
    t_vector origin;        			// centro, base o punto sul piano , la posizione
    t_vector orientation;   			// la direzione principale (es. normale di un piano, asse di un cilindro)

	// Questi sopra mi bastano per calcolare intersezioni ray–shape

     t_mat4 transf;						//matrice trasformazione oggetto → mondo
     t_mat4 inv_transf;					//inversa per trasformare i raggi mondo → spazio locale dell’oggetto (importantissima!)
	 t_mat4 norm_transform;  			// per trasformare normali

} t_shape;

/* --- Sphere --- */
typedef struct s_sphere
{
	t_vector center; // centro
	double radius; // diametro/2, precomputato dal parser
	t_color color; // 0..1
}			t_sphere;

/* --- Plane --- */
typedef struct s_plane
{
	t_vector point;  // un punto sul piano
	t_vector normal; // normalizzata
	t_color color; // 0..1
}			t_plane;

/* --- Cylinder --- */
typedef struct s_cylinder
{
	t_vector base;   // punto base lungo l'asse (convenz.
	t_vector axis;   // direzione normalizzata
	double radius; // diametro/2
	double height; // altezza finita
	t_color color; // 0..1
}			t_cylinder;

/* ───────────── Contenitore generico ───────────── */

typedef union u_figures
{
	t_sphere			sphere;
	t_plane				plane;
	t_cylinder			cylinder;
}						t_figures;

/* Vector di oggetti con due array paralleli */
typedef struct s_objnode 
{
   	t_shape_type      type;   /* OBJ_SPHERE / OBJ_PLANE / OBJ_CYLINDER */
    t_figures         fig;    /* union con i dati dell’oggetto */
    struct s_objnode *next;
} t_objnode;

typedef struct s_settings
{
	int render_w;   // larghezza finale dell'immagine renderizzata
	int render_h;   // altezza finale dell'immagine renderizzata
	int edit_w;     // larghezza in modalità "edit" risoluzione ridotta, usata per debug
	int edit_h;     // altezza in modalità "edit" risoluzione ridotta, usata per debug
	bool edit_mode; // se true → usa edit_w/edit_h “render veloce” invece che render_w/render_h quello finale (false)
					// flag letto dentro camera_init per scegliere quale risoluzione usare

	int	reflection_depth;
} t_settings;

/* ───────────── Scena ───────────── */
typedef struct s_scene
{
	// istanze reali (da riempire via parser)
	t_ambient ambient; // A (una sola nel mandatory)
	t_camera cam;  // C (una sola nel mandatory)
	//t_light lights; // L (una sola nel mandatory)
	// contatori utili durante il parsing/validazione
	int n_ambient; // per rilevare doppie dichiarazioni di A
	int n_camera;  // per C
	int n_lights;  // per L (bonus: servirà un array)
	int n_shapes;  // aggiunto Joy
	// oggetti (per ora solo contatori; poi aggiungerai array/vettore)
	int		n_spheres;
	int		n_planes;
	int		n_cylinders;
	size_t		object_count; // quanti oggetti totali
	t_objnode *obj_head; /* testa */
    t_objnode *obj_end; /* coda per append O(1) */

	t_light *lights;
	t_shape shapes[MAX_SHAPES];

	// Aggiunta temporanea per la camera J
	t_settings settings;
}			t_scene;

#endif