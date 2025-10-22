/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mathematical.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: francesca <francesca@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/03 20:14:33 by jcarnebi          #+#    #+#             */
/*   Updated: 2025/10/21 12:06:08 by francesca        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MATHRT_H
# define MATHRT_H

/* ================= Config ================= */

// CONVERSIONE DA GRADI A RADIANTI Tutte le funzioni trigonometriche in C (sin, cos) usano radianti
# define RAD_TO_DEG 57.2957795131
# define DEG_TO_RAD 0.01745329252

#define EPSILON 1e-6

#ifndef M_PI
#define M_PI 3.14159265358979323846
# endif

// FUNZIONI SUI VETTORI
// 1.vector_arithmetic.c
void	scale_vec(t_vector *res, const t_vector *v, double scale);               // Allunga o accorcia la freccia
void	sub_vec(t_vector *res, const t_vector *v1, const t_vector *v2);          // Sottrae due vettori → freccia da v2 a v1
void	negate_vec(t_vector *res, const t_vector *v);                            // Inverte la direzione del vettore
void	add_vec(t_vector *res, const t_vector *v1, const t_vector *v2);          // Somma due vettori → nuova freccia

void	reflect_vector(t_vector *res, t_vector *in_vector, t_vector *normal);    // Calcola il raggio riflesso di in_vector su una superficie con normale normal -- Serve per riflessi speculari

// 2.vector_operations.c
double	vec_magnitude(const t_vector *vec);                                      // Lunghezza della freccia
double	dot_product(const t_vector *v1, const t_vector *v2);                     // Quanto due frecce puntano nella stessa direzione -- Serve per illuminazione diffuse, angoli tra vettori, riflessi
void	normalize_vec(t_vector *vec);                                            // Rende il vettore lungo 1 unità -- Serve per direzione pura, senza considerare lunghezza
void	cross_product(t_vector *res, const t_vector *v1, const t_vector *v2);    // Crea una nuova freccia perpendicolare a due frecce date  -- Serve per calcolare normali delle superfici
double	vec_distance(const t_vector *v1, const t_vector *v2);                    // Distanza tra due punti nello spazio

// MATRICI 
typedef double			t_mat4[4][4];          // trasformazioni 3D (traslazioni, rotazioni, scaling)
typedef double			t_mat3[3][3];          // utile per determinanti, inversione parziale
typedef double			t_mat2[2][2];          // minor uso, solo per calcoli di supporto

// FUNZIONI SULLE MATRICI
// 3.transformation_matrices.c
void	translate_matrix(t_mat4 *mat, double x, double y, double z);              // Crea matrice per spostare un oggetto nello spazio
void	scaling_matrix(t_mat4 *mat, double x, double y, double z);                // Crea matrice per ingrandire/rimpicciolire un oggetto lungo gli assi
void	rotation_matrix_x(t_mat4 *mat, double r);                                 // Ruotano un oggetto attorno a X, Y o Z di un angolo r (in radianti)
void	rotation_matrix_y(t_mat4 *mat, double r);                                 // Ruotano un oggetto attorno a X, Y o Z di un angolo r (in radianti)
void	axis_angle(t_mat4 *rot_mat, const t_vector *ax, double angle);            // Ruota attorno a un asse qualsiasi, non solo X/Y/Z -- Utile per orientamenti personalizzati degli oggetti

// MOLTIPLICAZIONI
// 4.matrix_operations.c 
void	identity_matrix(t_mat4 *mat);                                             // non fa nulla quando moltiplicata per un vettore -- Serve come base per costruire trasformazioni
void	transpose_matrix(t_mat4 *mat);                                            // Scambia righe e colonne -- Serve per alcune operazioni inverse e prodotti
void	mat_vec_multiply(t_vector *res, const t_mat4 *mat,                        // Applica la matrice al vettore → sposta/ruota/scala il vettore -- Serve per trasformazioni camera e oggetti
			const t_vector *vec);
void	mat_multiply(t_mat4 *res, const t_mat4 *m1, const t_mat4 *m2);            // Combina due trasformazioni in una sola

// FUNZIONI AVANZATE MATRICI
// 5.matrix_inverse.c 
// 6.matrix_inverse2.c 
void	submat4(t_mat3 *res, const t_mat4 *mat, int row, int col);                // Prende una sottoparte 3x3 da una 4x4 (utile per determinante/inversione)
double	det3(const t_mat3 *mat3);                                                 // Determinante di una matrice 3x3 -- Serve per calcolare l’inversa
void	mat_inverse(t_mat4 *res, const t_mat4 *mat);                              // Trova matrice inversa -- Serve per portare i raggi nel sistema locale di un oggetto e tornare alla scena originale


// // FUNZIONI DI UTILITà POI DA TOGLIERE PERCHE DA LIBFT
// //utils.c
// void	*ft_memset(void *s, int c, size_t n);
// void	ft_bzero(void *s, size_t n);
// void	*ft_memcpy(void *dst, const void *src, size_t len);

// DEFINIZIONI
/*
Radianti = misura angoli in “cerchi” invece che in gradi.
Normali = frecce perpendicolari alle superfici.
Matrici = scatole di numeri che trasformano punti e direzioni. Una tabella di numeri (righe e colonne)
Traslazioni = spostamenti.
Determinanti = numeri che dicono se una matrice è invertibile.
Applicare matrice a vettore = cambiare posizione o direzione del punto. Significa: prendi un punto nello spazio e usi la matrice per trasformarlo.
Matrice inversa = la trasformazione che “annulla” quella originale.
usiamo double = numeri decimali molto precisi → perfetto per calcoli 3D e raytracing.
*/

/* ------- FUNZIONI MATH CAMERA ----------- */
void	view_transform(t_mat4 *res, const t_vector *from, const t_vector *up, // Calcola la matrice della camera (transform) partendo da tre vettori, from dove si trova la camera, up verso l’alto della scena,forward direzione in cui guarda
			const t_vector *forward);                                         // Produce una matrice 4x4 che serve a trasformare tutti i punti dal mondo allo spazio della camera

void	camera_init(t_camera *camera, t_scene *scene); // Inizializza tutti i valori della camera (pos, dir, fov, half_width/height, pixel_size)
void	calculate_camera_transform(t_scene *scene);  // Funzione di supporto per aggiornare la matrice della camera se cambia la posizione o la direzione


/* -------- FUNZIONI MATH COLOR ------------ */
void			add_colors(t_color *res, const t_color *c1, const t_color *c2);
void			mult_color(t_color *res, const t_color *color, double val);
void			sub_colors(t_color *res, const t_color *c1, const t_color *c2);
void			blend_colors(t_color *res, const t_color *c1,
					const t_color *c2);

t_color	get_shape_color(t_intersection *itx); 						//Ottiene il colore dell’oggetto che è stato colpito dal raggio.

typedef struct s_scene t_scene;
bool	get_specular_and_diffuse(t_scene *scene, int light_idx,		//Questa funzione calcola la luce diffusa e speculare per un punto colpito da un raggio
	t_intersection *itx, t_phong *phong);

/* ------- INTERSEZIONI ----------- */
//itnersection.c
bool	intersect(t_shape *shape, const t_ray *ray, t_intersections *xs);                   // funzione per testare un singolo shape con un singolo raggio, chaima funzione giusta
bool	intersect_sphere_fast(const t_ray *ray, t_intersections *xs, t_shape *sphere);      // Utili quando la sfera/piano sono già nelle loro posizioni “semplici” nel world-space
bool	intersect_plane_fast(const t_ray *ray, t_shape *plane, t_intersections *xs);        // non serve trasformare il raggio
//bool	intersect_cylinder(const t_ray *ray, t_shape *shape, t_intersections *xs);          // richiede raggio in local space e file dedicato da fare dopo

//intersection_utils.c
void	prepare_computations(t_intersection *intersection,
			t_ray *ray);
t_intersection	*hit(t_intersections *xs);													// Restituisce il punto di impatto più vicino (quello con t > 0 e più piccolo)
void			ray_position(t_vector *pos, const t_ray *ray, double time);					// Calcola il punto lungo un raggio dato un certo t - dove si trova il raggio quando t = 1.5?
//void	transform_ray(t_ray *dst, const t_ray *src, const t_shape *shape);                  // Funzione helper che trasforma un raggio usando la matrice inversa della shape
void	transform_ray(t_ray *transformed_ray, const t_ray *ray,								// Trasforma un raggio da world space a object space  
			const t_shape *shape);

//normal.c
//t_vector		plane_normal(const t_shape *shape, const t_vector *itx_point);				// Calcolano la normale geometrica nel punto di impatto
t_vector		plane_normal(const t_shape *shape);
t_vector		sphere_normal(t_vector *normal, const t_shape *shape,
				const t_vector *itx_point);
t_vector		normal_at(const t_shape *shape, const t_vector *itx_point);					//Calcola la normale finale, considerando shape + texture
//NOTE PER ME 
/*
TRASFORMAZIONE DEL RAGGIO
World space = sistema di coordinate globale: dove stanno gli oggetti nel mondo intero, rispetto alla scena
Object space = È il sistema di coordinate locale dell’oggetto
La sfera crede di essere sempre al centro, se la sfera è spostata, scalata o ruotata, le formule non funzionano più direttamente
Trasformo il raggio, non l’oggetto, così possiamo usare le formule come se la sfera fosse ancora al centro

INTERSEZIONI FAST
Se la sfera è già al centro
Non è ruotata né scalata
Sta già nel "mondo", così com’è
Uso le formule direttamente, senza trasformare il raggio ,evitando i calcoli extra di trasformazione
*/

/* -------- FUNZIONI MATH LUCI -------- */

//FUNZIONI
bool	is_shadowed(t_scene *scene, int light_idx, t_vector *itx_point);
			//double *angle);
void	calculate_specular(t_vector *reflect_v, t_intersection *itx,
			t_phong *phong, t_light *light);
t_color	cast_reflection_ray(t_scene *scene, t_intersection *intersection,
		int remaining, int light_idx);
t_color	phong(t_intersection *itx, t_scene *scene, int light_idx);

# endif