#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include "../include/mathRT.h"
#include "../include/camera.h" 
#include "../include/scene.h"
#include "../include/intersections.h"

#include "../include/color.h"
#include "../include/shape.h"
#include "../include/light.h"

#include <string.h> // per memset

void	init_identity_matrices(t_shape *shape)
{
	identity_matrix(&shape->transf);
	mat_inverse(&shape->inv_transf, &shape->transf);
	ft_memcpy(&shape->norm_transform, &shape->inv_transf, sizeof(t_mat4));
	transpose_matrix(&shape->norm_transform);
    t_color cast_reflection_ray(t_scene *scene, t_intersection *intersection,
                            int remaining, int light_idx);
}

int	main(void)
{
	t_scene			scene;
	t_shape			sphere1, sphere2;
	t_ray			ray;
	t_intersections	itxs;
	//t_color			final_color;

	// 1. Inizializza scena
	memset(&scene, 0, sizeof(t_scene));
	scene.n_shapes = 2;  // Adesso due sfere
	scene.n_lights = 1;
	scene.settings.reflection_depth = 1;

	// 2. Sfera rossa (colpita dal raggio)
	memset(&sphere1, 0, sizeof(t_shape));
	sphere1.type = SPHERE;
	sphere1.props.color = (t_color){1, 0, 0, 0}; // Rosso
	sphere1.props.reflectiveness = 0.5;
	sphere1.props.diffuse = 0.9;
	sphere1.props.specular = 0.9;
	sphere1.props.shininess = 200.0;
	init_identity_matrices(&sphere1);
	scene.shapes[0] = sphere1;

	// 3. Sfera grigia (tra luce e sfera rossa)
	memset(&sphere2, 0, sizeof(t_shape));
	sphere2.type = SPHERE;
	sphere2.origin = (t_vector){2, 5, 5, 1}; // più vicino alla luce
    
    // Centro della seconda sfera tra punto impatto e luce
	sphere2.props.color = (t_color){0.5, 0.5, 0.5, 0}; // Grigia
	sphere2.props.reflectiveness = 0.0;
	sphere2.props.diffuse = 0.9;
	sphere2.props.specular = 0.1;
	sphere2.props.shininess = 50.0;
	init_identity_matrices(&sphere2);
	scene.shapes[1] = sphere2;

	// 4. Luce puntiforme
	scene.lights[0].type = POINT;
	scene.lights[0].position = (t_vector){-10, 10, -10, 1};
	scene.lights[0].color = (t_color){1, 1, 1, 0};
	scene.lights[0].intensity = 1.0;

	// 5. Luce ambientale
	scene.ambient.intensity = 0.1;
	scene.ambient.color = (t_color){1, 1, 1, 0};

	// 6. Raggio dalla camera verso la sfera rossa
	ray.origin = (t_vector){0, 0, -10, 1};
	ray.dir = (t_vector){0, 0, 1, 0};

	// 7. Fingi intersezione con sfera rossa (sphere1)
	itxs.count = 1;
	itxs.arr[0].time = 5.0;
	itxs.arr[0].shape = &scene.shapes[0];

	// 8. Prepara shading per la sfera rossa
	prepare_computations(&itxs.arr[0], &ray);

	// 9. Calcola colore con modello Phong per la sfera rossa
	t_color final_color_red = phong(&itxs.arr[0], &scene, 0);

	// -----------------------------
	// Ora simula anche intersezione con la sfera grigia (sphere2)
	// -----------------------------

	t_intersection second_hit;
	second_hit.time = 15.0;
	second_hit.shape = &scene.shapes[1];

	prepare_computations(&second_hit, &ray);

	t_color final_color_gray = phong(&second_hit, &scene, 0);

	// 10. Output
	printf("\n--- TEST ILLUMINAZIONE PHONG + OMBRA ---\n");
	printf("[SPHERE1] Colore finale (ROSSO): R=%.2f, G=%.2f, B=%.2f\n",
		final_color_red.r, final_color_red.g, final_color_red.b);
	printf("[SPHERE2] Colore finale (GRIGIO): R=%.2f, G=%.2f, B=%.2f\n",
		final_color_gray.r, final_color_gray.g, final_color_gray.b);


	return (0);
}


// TEST PER STAMPARE IL COLORE RISULTATNTE DAL MODELLO PHONG FUNZIONANTE 1 SFERA
/*void	init_identity_matrices(t_shape *shape)
{
	identity_matrix(&shape->transf);
	mat_inverse(&shape->inv_transf, &shape->transf);
	ft_memcpy(&shape->norm_transform, &shape->inv_transf, sizeof(t_mat4));
	transpose_matrix(&shape->norm_transform);
}

int	main(void)
{
	t_scene			scene;
	t_shape			sphere;
	t_ray			ray;
	t_intersections	itxs;
	t_color			final_color;

	// 1. Inizializza scena
	memset(&scene, 0, sizeof(t_scene));
	scene.n_shapes = 1;
	scene.n_lights = 1;
	scene.settings.reflection_depth = 1;

	// 2. Inizializza sfera
	memset(&sphere, 0, sizeof(t_shape));
	sphere.type = SPHERE;
	sphere.props.color = (t_color){1, 0, 0, 0}; // Rosso
	sphere.props.reflectiveness = 0.5;
	sphere.props.diffuse = 0.9;
	sphere.props.specular = 0.9;
	sphere.props.shininess = 200.0;
	init_identity_matrices(&sphere);

	scene.shapes[0] = sphere;

	// 3. Inizializza luce puntiforme
	scene.lights[0].type = POINT;
	scene.lights[0].position = (t_vector){-10, 10, -10, 1};
	scene.lights[0].color = (t_color){1, 1, 1, 0};
	scene.lights[0].intensity = 1.0;

	// 4. Inizializza luce ambientale
	scene.ambient.intensity = 0.1;
	scene.ambient.color = (t_color){1, 1, 1, 0};

	// 5. Crea un raggio che punta verso la sfera
	ray.origin = (t_vector){0, 0, -10, 1};     // Punto di partenza
	ray.dir = (t_vector){0, 0, 1, 0};          // Direzione verso la sfera

	// 6. Fingi un'intersezione manuale a t = 5
	itxs.count = 1;
	itxs.arr[0].time = 5.0;
	itxs.arr[0].shape = &scene.shapes[0];

	// Prepara i dati per il shading
	prepare_computations(&itxs.arr[0], &ray);

	// 7. Calcola il colore usando il modello Phong
	final_color = phong(&itxs.arr[0], &scene, 0); // usa luce 0

	// 8. Output
	printf("\n--- TEST ILLUMINAZIONE PHONG ---\n");
	printf("Colore finale: R=%.2f, G=%.2f, B=%.2f\n",
		final_color.r, final_color.g, final_color.b);

	return (0);
}*/


// TEST PER STAMPARE IL COLORE RISULTATNTE DAL MODELLO PHONG
/*t_color	get_ambient(t_scene *scene, t_color patter_color);
t_color phong(t_intersection *itx, t_scene *scene, int light_idx);
t_color shade_point(t_intersections *arr, t_scene *scene, t_ray *ray);
t_color cast_reflection_ray(t_scene *scene, t_intersection *intersection,
                            int remaining, int light_idx);

// Funzione di stampa colori
void print_color(const char *label, t_color color) {
    printf("%s → R: %.2f, G: %.2f, B: %.2f\n", label, color.r, color.g, color.b);
}

int main(void)
{
    t_scene scene;
    t_light light;
    t_intersection itx;
    t_intersections arr;
    t_ray ray;

    // Inizializza la scena
    scene.n_lights = 1;
    scene.n_shapes = 1;
    scene.settings.reflection_depth = 1;

    // Luce puntiforme
    light.type = POINT;
    light.position = (t_vector){10, 10, -10, 1};
    light.color = (t_color){1, 1, 1, 0};
    light.intensity = 1.0;
    scene.lights[0] = light;

    // Luce ambientale
    scene.ambient.intensity = 0.1;
    scene.ambient.color = (t_color){1, 1, 1, 0}; // bianca

    // Oggetto (shape)
    scene.shapes[0].props.color = (t_color){1, 0, 0, 0}; // rosso
    scene.shapes[0].props.diffuse = 0.9;
    scene.shapes[0].props.specular = 0.9;
    scene.shapes[0].props.shininess = 200;
    scene.shapes[0].props.reflectiveness = 0.5;

    // Intersezione simulata (come se il raggio colpisse l'oggetto)
    itx.shape = &scene.shapes[0];
    itx.point = (t_vector){0, 0, 0, 1};
    itx.over_point = (t_vector){0, 0, 0.0001, 1};
    itx.normal = (t_vector){0, 0, -1, 0};
    itx.eye = (t_vector){0, 0, -1, 0};
    itx.reflect_vec = (t_vector){0, 0, -1, 0};

    // Simula un raggio che arriva dalla camera
    ray.origin = (t_vector){0, 0, -5, 1};
    ray.dir = (t_vector){0, 0, 1, 0};

    // Imposta la lista di intersezioni
    arr.count = 1;
    arr.arr[0] = itx;

    // Calcola contributi
    t_color ambient = get_ambient(&scene, itx.shape->props.color);
    t_color phong_color = phong(&arr.arr[0], &scene, 0);
    t_color final_color = shade_point(&arr, &scene, &ray);

    // Output
    printf("\n--- TEST ILLUMINAZIONE PHONG ---\n");
    print_color("Luce ambientale", ambient);
    print_color("Modello Phong (diffuse + specular + ambient)", phong_color);
    print_color("Finale con riflessione", final_color);

    return 0;
}*/


//MAIN PER TESTARE INTERSEZIONI E HIT PIU VICINA
/*static void print_vec(const char *name, const t_vector *v)
{
	printf("%s = (%.4f, %.4f, %.4f, %.4f)\n",
		name, v->x, v->y, v->z, v->w);
}

int main(void)
{
	t_shape			sphere;
	t_ray			ray;
	t_intersections	xs;
	t_intersection	*closest;
	t_vector		point;
	t_vector		normal;
	t_vector		reflected;

	// --- inizializza la sfera --- 
	ft_bzero(&sphere, sizeof(t_shape)); // se hai ft_bzero/link nella libft 
	sphere.type = SPHERE;
	// centro al'origine (0,0,0), w = 1 per i punti 
	sphere.origin = (t_vector){0.0, 0.0, 0.0, 1.0};
	sphere.props.radius = 1.0;
	sphere.props.radius_squared = 1.0; //importante: intersect_sphere_fast lo usa 

	// --- raggio di test --- 
	ray.origin = (t_vector){0.0, 0.0, -5.0, 1.0}; // punto di partenza 
	ray.dir    = (t_vector){0.0, 0.0,  1.0, 0.0}; // direzione verso +Z 
	normalize_vec(&ray.dir); // importante: normalizziamo la direzione 

	// --- struttura intersezioni --- 
	xs.count = 0;

	printf("Test: raggio da (0,0,-5) verso (0,0,1) su sfera r=1 in origine\n");

	//chiama la funzione dispatcher (che per SPHERE usa intersect_sphere_fast) 
	if (intersect(&sphere, &ray, &xs) == false)
	{
		printf("intersect() ha restituito false — nessuna intersezione\n");
		return 0;
	}

	// stampa risultati trovati 
	printf("Intersezioni trovate: %d\n", xs.count);
	for (int i = 0; i < xs.count; ++i)
		printf("  t[%d] = %.6f\n", i, xs.arr[i].time);

	// scegli la hit più vicina > 0 
	closest = hit(&xs);
	if (closest == NULL)
	{
		printf("hit() -> nessuna intersezione utile (t>0)\n");
		return 0;
	}
	printf("Hit piu' vicina: t = %.6f\n", closest->time);

	//calcola il punto lungo il raggio 
	ray_position(&point, &ray, closest->time);
	print_vec("Punto di intersezione", &point);

	// calcola la normale manualmente: (point - centro) normalizzato 
	sub_vec(&normal, &point, &sphere.origin); // normal = point - center 
	normalize_vec(&normal);
	print_vec("Normale calcolata (point - center)", &normal);

	//calcola il vettore riflesso 
	reflect_vector(&reflected, &ray.dir, &normal);
	print_vec("Vettore riflesso", &reflected);

	return 0;
}*/

//Main per testare la camera con traslazione e rotazione
/*void print_matrix(const t_mat4 *m)
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
            printf("%8.4f ", (*m)[i][j]);
        printf("\n");
    }
    printf("\n");
}

void print_vector(const char *label, const t_vector *v)
{
    printf("%s: (%.4f, %.4f, %.4f, %.4f)\n", label, v->x, v->y, v->z, v->w);
}

int main(void)
{
    t_scene scene;

    // Risoluzione fittizia per la camera
    scene.settings.render_w = 800;
    scene.settings.render_h = 600;
    scene.settings.edit_mode = false;

    // Posizione della camera TRASLATA e RUOTATA
    scene.cam.pos = (t_vector){0, 0, 5, 1};  // camera a (0, 0, 5)
    scene.cam.dir = (t_vector){0, 0, 1, 0};       // guarda lungo +Z → poi la view la invertirà
    scene.cam.fov = 60.0;

    camera_init(&scene.cam, &scene);
    calculate_camera_transform(&scene);

    printf("Camera transform matrix:\n");
    print_matrix(&scene.cam.transform);

    printf("Camera inverse transform matrix:\n");
    print_matrix(&scene.cam.inv_trans);

    // Punto nella scena
    t_vector point = {0, 0, -5, 1};
    t_vector transformed;
    mat_vec_multiply(&transformed, &scene.cam.transform, &point);

    print_vector("Original point", &point);
    print_vector("After transform", &transformed);

    return 0;
}*/

//Definizioni MINIME per permettere di testare la camera
//main per testare la camera configurazione base (nessuna rotazione, nessuna traslazione)

/*void print_matrix(const t_mat4 *m)
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
			printf("%8.4f ", (*m)[i][j]);
		printf("\n");
	}
	printf("\n");
}

void print_vector(const char *label, const t_vector *v)
{
	printf("%s: (%.4f, %.4f, %.4f, %.4f)\n",
		label, v->x, v->y, v->z, v->w);
}

int main(void)
{
	t_scene scene;

	scene.settings.render_w = 800;
	scene.settings.render_h = 600;
	scene.settings.edit_mode = false;

	scene.cam.pos = (t_vector){0, 0, 0, 1}; // la posizione della camera è l’origine (0,0,0) nello spazio, ed è un PUNTO perche w=1
	scene.cam.dir = (t_vector){0, 0, -1, 0}; // la direzione è lungo l’asse Z negativo, ed è un VETTORE quindi se traslo la scena, questo vettore resta uguale (non viene spostato) w=0
	scene.cam.fov = 60.0;

	camera_init(&scene.cam, &scene);
	printf("Camera initialized:\n");
	printf("  Half width:  %.4f\n", scene.cam.half_width);
	printf("  Half height: %.4f\n", scene.cam.half_height);
	printf("  Pixel size:  %.6f\n\n", scene.cam.pixel_size);

	calculate_camera_transform(&scene);

	printf("Camera transform matrix:\n");
	print_matrix(&scene.cam.transform);

	printf("Camera inverse transform matrix:\n");
	print_matrix(&scene.cam.inv_trans);

	t_vector point = {0, 0, -5, 1};
	t_vector transformed;

	mat_vec_multiply(&transformed, &scene.cam.transform, &point);

	print_vector("Original point", &point);
	print_vector("After transform", &transformed);

	return 0;
}*/

// main per testare matrice inversa
/*void print_mat(const char *name, const t_mat4 mat)
{
    printf("%s:\n", name);
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
            printf("%8.3f ", mat[i][j]);
        printf("\n");
    }
    printf("\n");
}

void print_vec(const char *name, const t_vector *v)
{
    printf("%s = (%.3f, %.3f, %.3f, %.3f)\n", name, v->x, v->y, v->z, v->w);
}

int main(void)
{
    t_mat4 mat;
    t_mat4 inv;
    t_vector P = {1, 2, 3, 1};   // punto originale
    t_vector P_trans, P_back;

    // Creiamo una matrice di trasformazione arbitraria
    identity_matrix(&mat);
    mat[0][0] = 2; mat[1][1] = 0.5; mat[2][2] = 3;  // scaling
    mat[0][3] = 2; mat[1][3] = 3; mat[2][3] = 4;    // traslazione

    print_mat("Matrice originale", mat);

    // Calcoliamo l'inversa
    mat_inverse(&inv, &mat);
    print_mat("Matrice inversa", inv);

    // Applichiamo la matrice originale al punto
    mat_vec_multiply(&P_trans, &mat, &P);
    print_vec("Punto trasformato", &P_trans);

    // Applichiamo la matrice inversa al punto trasformato
    mat_vec_multiply(&P_back, &inv, &P_trans);
    print_vec("Punto riportato indietro", &P_back);

    return 0;
}*/

//main per testare determinanti e cofattori per matrice inversa 
/*void print_mat3(const t_mat3 *m)
{
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
            printf("%7.3f ", (*m)[i][j]);
        printf("\n");
    }
    printf("\n");
}

int main(void)
{
    t_mat4 mat4 = {
        {1, 2, 3, 4},
        {5, 6, 7, 8},
        {9,10,11,12},
        {13,14,15,16}
    };

    t_mat3 sub;
    submat4(&sub, &mat4, 0, 0); // rimuove riga 0 e colonna 0

    printf("Sotto-matrice 3x3 di mat4 (senza riga 0, colonna 0):\n");
    print_mat3(&sub);

    double det = det3(&sub);
    printf("Determinante della sotto-matrice: %.3f\n", det);

    return 0;
}*/

// main per testare le operazioni delle matrici
// Funzione di utilità per stampare una matrice 4x4
/*void print_matrix(const t_mat4 *mat)
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
            printf("%7.2f ", (*mat)[i][j]);
        printf("\n");
    }
    printf("\n");
}
// Funzione di utilità per stampare un vettore
void print_vector(const t_vector *v)
{
    printf("(%.2f, %.2f, %.2f, %.2f)\n", v->x, v->y, v->z, v->w);
}
int main(void)
{
    t_mat4 m1;
    t_mat4 m2;
    t_mat4 res;
    t_vector v, vr;

    // Test identity_matrix se la applichi a un punto, non cambia niente
    printf("=== Test identity_matrix ===\n");
    identity_matrix(&m1);
    print_matrix(&m1);

    // Test mat_multiply con identity (m1 * m1) due trasformazioni “neutre” → ottieni ancora neutro
    printf("=== Test mat_multiply con identity ===\n");
    identity_matrix(&m2);
    mat_multiply(&res, &m1, &m2);
    print_matrix(&res);

    // Creiamo m2 manualmente per testare la moltiplicazione. la colonna di destra (2,3,4) → è proprio il vettore di traslazione
    identity_matrix(&m1);
    identity_matrix(&m2);
    m2[0][3] = 2;  // traslazione x=2
    m2[1][3] = 3;  // traslazione y=3
    m2[2][3] = 4;  // traslazione z=4

    printf("=== m2 = matrice traslazione (2,3,4) ===\n");
    print_matrix(&m2);

    // Moltiplichiamo m1 (identity) per m2 → dovrebbe dare m2
    printf("=== m1 * m2 ===\n");
    mat_multiply(&res, &m1, &m2);
    print_matrix(&res);

    // Test mat_vec_multiply (traslazione punto) 
    // Punto originale: (1,1,1,1) Moltiplicando per la matrice di traslazione →
    //(1+2, 1+3, 1+4, 1) = (3,4,5,1)
    v.x = 1;
    v.y = 1;
    v.z = 1;
    v.w = 1; // punto
    printf("=== Punto originale ===\n");
    print_vector(&v);

    mat_vec_multiply(&vr, &m2, &v);
    printf("=== Punto dopo traslazione (applicata m2) ===\n");
    print_vector(&vr);

    // Test transpose_matrix Le righe sono diventate colonne, e viceversa.
    //I numeri (2,3,4) che prima erano in alto a destra ora sono in basso a sinistra.
    printf("=== Trasposta di m2 ===\n");
    transpose_matrix(&m2);
    print_matrix(&m2);

    return 0;
}*/

//main per testare trasformazione delle matrici
// Funzione per stampare vettori 
/*void print_vec(const char *name, const t_vector *v)
{
    printf("%s = (%.2f, %.2f, %.2f, %.2f)\n", name, v->x, v->y, v->z, v->w);
}
// Funzione per stampare una matrice 4x4
void print_matrix(const char *name, t_mat4 *mat)
{
    printf("%s:\n", name);
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
            printf("%6.2f ", (*mat)[i][j]);
        printf("\n");
    }
    printf("\n");
}
int main(void)
{
    t_mat4 mat;

    // Test traslazione - La matrice di traslazione avrà x=2, y=3, z=4 nella quarta colonna
    // la matrice non mostra i nuovi punti, mostra solo come trasformarli.
    translate_matrix(&mat, 2, 3, 4);
    print_matrix("Matrice di traslazione (2,3,4)", &mat);

    // Test scaling - La matrice di scaling avrà 2, 0.5, 3 sulla diagonale
    scaling_matrix(&mat, 2, 0.5, 3);
    print_matrix("Matrice di scaling (2,0.5,3)", &mat);

    // Test rotazione X di 90° (pi/2 radianti) - Le matrici di rotazione avranno cos/sin nei posti giusti
    rotation_matrix_x(&mat, 3.14159265 / 2);
    print_matrix("Matrice rotazione X 90°", &mat);

    // Test rotazione Y di 90°
    rotation_matrix_y(&mat, 3.14159265 / 2);
    print_matrix("Matrice rotazione Y 90°", &mat);

    // Test axis-angle (asse Z = (0,0,1), 90°) - mostrerà la rotazione attorno a Z.
    t_vector axis = {0, 0, 1, 0};
    axis_angle(&mat, &axis, 3.14159265 / 2);
    print_matrix("Matrice axis-angle attorno a Z (90°)", &mat);

    return 0;
}*/

//main per testare riflessi speculari
/*int main(void)
{
    t_vector incident, normal, reflected;

    // ===== Caso 1: superficie orizzontale =====
    incident = (t_vector){1, -1, 0, 0};  // raggio verso il basso
    normal   = (t_vector){0, 1, 0, 0};   // normale verso l'alto
    reflect_vector(&reflected, &incident, &normal);
    print_vec("Incident 1", &incident);
    print_vec("Normal 1", &normal);
    print_vec("Reflected 1", &reflected);
    printf("\n");

    // ===== Caso 2: superficie inclinata 45° =====
    incident = (t_vector){1, -0.5, 0, 0}; // raggio obliquo
    normal   = (t_vector){0.70710678, 0.70710678, 0, 0};
    normalize_vec(&normal);
    reflect_vector(&reflected, &incident, &normal);
    print_vec("Incident 2", &incident);
    print_vec("Normal 2", &normal);
    print_vec("Reflected 2", &reflected);
    printf("\n");

    // ===== Caso 3: superficie verticale =====
    incident = (t_vector){-1, 0, 0, 0}; // raggio da sinistra
    normal   = (t_vector){1, 0, 0, 0};  // normale verso destra
    reflect_vector(&reflected, &incident, &normal);
    print_vec("Incident 3", &incident);
    print_vec("Normal 3", &normal);
    print_vec("Reflected 3", &reflected);

    return 0;
}*/
// main per testare aritmetica dei vettori
/*int main(void)
{
    t_vector a = {1, 2, 0, 1};  // Punto nello spazio
    t_vector b = {3, -1, 0, 0}; // Direzione
    t_vector res;

    add_vec(&res, &a, &b);
    print_vec("a + b", &res);   // Somma

    sub_vec(&res, &a, &b);
    print_vec("a - b", &res);   // Sottrazione

    scale_vec(&res, &b, 2.0);
    print_vec("b * 2", &res);   // Allunga la freccia

    negate_vec(&res, &b);
    print_vec("-b", &res);      // Direzione opposta

    return 0;
}*/

// main per testare le operazioni dei vettori 
/*int main(void)
{
    t_vector a = {3, 4, 0, 0};
    t_vector b = {1, 0, 0, 0};
    t_vector c = {0, 1, 0, 0};
    t_vector res;

    // Magnitude = lunghezza freccia si calcola così: radice di x2 + y2 + z2 = 3 + 4 + 0 = 9 + 16 = 25 radice 5
    printf("Lunghezza di a = %.2f\n", vec_magnitude(&a));

    //Normalize = freccia lunga 1. Basta dividere ogni componente per la lunghezza: 3:5, 4:5, 0:5 = 0.6, 0.8, 0
    normalize_vec(&a);
    printf("a normalizzato = (%.2f, %.2f, %.2f)\n", a.x, a.y, a.z);

    //Dot product = angolo tra vettori (0 → perpendicolari, 1 → paralleli)
    // Avevamo b = (1,0,0) e c = (0,1,0) 
    // b⋅c=(1)(0)+(0)(1)+(0)(0)=0
    // Qui facciamo b · b con b = (1,0,0)
    // b⋅b=1⋅1+0⋅0+0⋅0=1
    printf("Dot product b·c = %.2f\n", dot_product(&b, &c));
    printf("Dot product b·b = %.2f\n", dot_product(&b, &b));

    //Cross product = vettore perpendicolare
    //Il prodotto vettoriale crea un vettore perpendicolare ai due di partenza.
    //b = (1,0,0) → punta lungo X
    //c = (0,1,0) → punta lungo Y
    // Il risultato deve puntare lungo Z: Infatti il risultato stampato è (0,0,1)
    cross_product(&res, &b, &c);
    printf("Cross product b x c = (%.2f, %.2f, %.2f)\n", res.x, res.y, res.z);

    // Distanza = lunghezza del segmento tra due punti
    // Avevamo p1 = (1,2,3) e p2 = (4,6,3).
    // La distanza è come la lunghezza del vettore p2 - p1: è 5
    t_vector p1 = {1, 2, 3, 1};
    t_vector p2 = {4, 6, 3, 1};
    printf("Distanza tra p1 e p2 = %.2f\n", vec_distance(&p1, &p2));

    return 0;
}*/
