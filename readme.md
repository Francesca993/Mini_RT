Allora mi sono basata sul subject e ho creato per ogni elemento una struttura, e su ogni struttura ho messo ogni parametro che dovrebbe avere secondo il subject.
All'inizio del mio main c'è una funzione di check sul file, che sia .rt. vorrei poi implementare una funzione unica che comprenderà anche quella, a cui si passerà l'fd e il file e lo controllerà prima di aprirlo. ma per il momento l'ho messa li. 
ora sto stilando una lista completa di tutti i check da fare mentre carico il file e di tutto cio che devo controllare secondo il subject.

**Fatto e da fare:**
-> ho fatto una funzione che legge il file con get_next_line, ne prende il primo frammento e a seconda dell id partira una funzione che farà il check su i parametri e se corretti creerà la struttura dati. Allo stesso tempo tiene il conto nella t_scene delle presenze. 

Estensione .rt già verificata per il momento nel main ✅

Righe vuote e spazi: puoi ignorare righe vuote o solo spazi/tab. Gli elementi sono separati da uno o più spazi/tabs. -> fare una funzione che ignora righe e spazi nelle utils


Identificatori validi: solo questi token a inizio riga sono permessi nel mandatory: A, C, L, sp, pl, cy. Qualsiasi altro identificatore → errore di parsing.

✅ **Presenze obbligatorie e unicità:**
A una sola volta.
C una sola volta.
L una sola volta nel mandatory (nel bonus potranno essere più luci).

✅ **Almeno un oggetto (sp/pl/cy)**
 deve essere presente per avere qualcosa da renderizzare (se non c’è nulla → errore logico di scena vuota).

Formato dei gruppi con virgole: 
- coordinate/orientazioni/colori sono sempre nel formato x,y,z e R,G,B 
- senza spazi dentro; 
- devono avere esattamente 3 componenti. 
- Se ci sono spazi, componenti in più/meno, o virgole doppie → ERRORE.

Numero di parametri: 
ogni riga deve avere esattamente i parametri previsti, né uno in più né uno in meno. 
In eccesso/mancanza → errore.

Tipi numerici: 
coordinate e dimensioni -> sono floating (accetta segno e punto decimale). 
Colori -> sono interi base 10.
Valori NaN/Inf -> rifiutati.



Vincoli di range e coerenza (per tipo di valore)
Colori R,G,B: interi 0–255. Qualsiasi valore fuori range → errore.

Ratio/Intensità:
A.ratio: 0.0–1.0.
L.brightness (intensità): 0.0–1.0.
FOV (camera): in gradi, 0 < FOV < 180. 0 o 180 o più → errore.

Orientazioni/vettori direzione (C dir, pl normal, cy axis):
Ogni componente deve stare in [-1, 1].
Il vettore non può essere (0,0,0).
Deve essere normalizzato (norma ≈ 1 entro una tolleranza). Se non lo è, nel mandatory si rifiuta la riga (niente auto-normalizzazione silenziosa).

Dimensioni positive:
sp diameter > 0.
cy diameter > 0 e height > 0.
Ordine dei valori per ogni elemento (mandatory)
L’ordine deve essere esattamente questo. Se l’ordine non è rispettato o manca/un extra parametro → errore di parsing.

**Ambient (A)**
A ratio R,G,B
ratio ∈ [0,1]
R,G,B interi 0–255

**Camera (C)**
C x,y,z nx,ny,nz FOV
x,y,z posizione
nx,ny,nz direzione (norm. in [-1,1], ≠(0,0,0))
FOV in gradi, 0<FOV<180

**Light (L)**
L x,y,z brightness R,G,B
x,y,z posizione
brightness ∈ [0,1]
R,G,B interi 0–255

**Sphere (sp)**
sp x,y,z diameter R,G,B
x,y,z centro
diameter > 0
R,G,B interi 0–255

**Plane (pl)**
pl x,y,z nx,ny,nz R,G,B
x,y,z un punto sul piano
nx,ny,nz normale (norm. in [-1,1], ≠(0,0,0))
R,G,B interi 0–255

**Cylinder (cy)**
cy x,y,z nx,ny,nz diameter height R,G,B
x,y,z punto base (o centro, dipende dalla tua convenzione, ma deve essere consistente)
nx,ny,nz asse (norm. in [-1,1], ≠(0,0,0))
diameter > 0
height > 0
R,G,B interi 0–255

Cosa succede se…
Ordine sbagliato: es. C FOV x,y,z nx,ny,nz → errore (riga rifiutata).

Parametro mancante o extra: es. sp x,y,z diameter (manca colore) o A ratio R,G,B extra → errore.

Orientazione non normalizzata / fuori [-1,1] / zero vector: errore (non normalizzare tacitamente nel mandatory).

Valori fuori range (colori, ratio, FOV, dimensioni non positive): errore.

Duplicati A/C/L: se trovi una seconda istanza → errore. 

# SPIEGAZIONE T_UNION

- come definire le struct,
- come costruire gli oggetti nel parsing,
- come userarli nel rendering.

### 🧩 1. Strutture dati principali
Queste vanno nel tuo miniRT_struct.h o in un file tipo scene.h.

```
typedef enum e_object_type
{
	OBJ_SPHERE,
	OBJ_PLANE,
	OBJ_CYLINDER
}	t_object_type;

/* ───────────── Oggetti geometrici ───────────── */

typedef struct s_sphere
{
	t_vec3	center;
	double	diameter;
	t_color	color;
}	t_sphere;

typedef struct s_plane
{
	t_vec3	point;
	t_vec3	normal;
	t_color	color;
}	t_plane;

typedef struct s_cylinder
{
	t_vec3	center;
	t_vec3	axis;
	double	diameter;
	double	height;
	t_color	color;
}	t_cylinder;

/* ───────────── Contenitore generico ───────────── */

typedef struct s_object
{
	t_object_type	type;   // serve per capire cosa contiene il union
	union
	{
		t_sphere	sphere;
		t_plane		plane;
		t_cylinder	cylinder;
	}	as;                  // "as" = as.sphere / as.plane / as.cylinder
}	t_object;

/* ───────────── Scena completa ───────────── */

typedef struct s_scene
{
	t_camera	camera;
	t_light		light;
	t_ambient	ambient;

	t_object	*objects;     // array dinamico di oggetti
	size_t		object_count; // quanti oggetti totali
	size_t		object_cap;   // capacità allocata
}	t_scene;

```
### ⚙️ 2. Funzioni per gestire la scena (array dinamico)
Queste funzioni servono al parser per aggiungere oggetti man mano che li legge dal file .rt.

```
#include <stdlib.h>

static int	scene_reserve_capacity(t_scene *scene, size_t new_count)
{
	if (new_count <= scene->object_cap)
		return (1);
	size_t	new_capacity = scene->object_cap ? scene->object_cap * 2 : 8;
	while (new_capacity < new_count)
		new_capacity *= 2;
	void	*new_data = realloc(scene->objects, new_capacity * sizeof(t_object));
	if (!new_data)
		return (0);
	scene->objects = new_data;
	scene->object_cap = new_capacity;
	return (1);
}

int	scene_add_object(t_scene *scene, t_object new_object)
{
	if (!scene_reserve_capacity(scene, scene->object_count + 1))
		return (0);
	scene->objects[scene->object_count++] = new_object;
	return (1);
}

void	scene_free(t_scene *scene)
{
	free(scene->objects);
	scene->objects = NULL;
	scene->object_count = 0;
	scene->object_cap = 0;
}
```
**scene_reserve_capacity**
Garantisce che scene->objects abbia spazio sufficiente per contenere almeno new_count elementi (capacità). Se non basta, rialloca (con realloc) aumentando la capacità.

- Controllo sufficienza: Se la capacità attuale (object_cap) è già ≥ new_count, non serve fare nulla.
- Crescita esponenziale:
    - Se l’array è vuoto, parte da 8 (scelta comune come capacità iniziale).
    - Altrimenti raddoppia finché non raggiunge almeno new_count.
    - Il raddoppio è una strategia classica per avere push amortizzato O(1) (cioè, molto veloce in media).
- Riallocazione:
    - realloc ridimensiona il blocco: se può allargarlo “in place”, lo fa; altrimenti copia i vecchi dati in un nuovo blocco e libera il precedente.
    - Se fallisce (NULL), non tocchiamo scene->objects (perché stiamo usando una variabile temporanea new_data), quindi niente leak e l’array rimane valido com’era.
    - Se va bene, aggiorniamo il puntatore e la nuova capacità.

Esempio pratico: come “cresce” la capacità
Partenza: object_cap=0, object_count=0.
1° push → new_count=1
new_capacity passa da 0 a 8 → realloc(8 * sizeof(t_object)) → object_cap=8, object_count=1.
Push fino a 8 → nessuna riallocazione (cap=8).
9° push → new_count=9
cap 8 non basta: raddoppia → 16 → realloc(16 * sizeof(...)) → object_cap=16, object_count=9.
E così via: 32, 64, 128…
Questo pattern limita il numero di riallocazioni e rende le scene_add_object molto economiche.

**scene_free**

- Libera la memoria del vettore.
- Azzera puntatore, count e cap per lasciare la struttura in stato consistente e riutilizzabile (o sicura da free-are di nuovo).
Nota:
free(NULL) è sicuro, quindi puoi chiamarla anche se non hai mai aggiunto oggetti.
Se in futuro i tuoi t_object contenessero risorse allocate internamente, dovresti prima iterare sugli oggetti e rilasciare le risorse interne, poi liberare l’array. Con il layout attuale (solo valori), non serve.


### 📄 3. Parsing di un oggetto (esempio concreto)
Quando il parser legge una riga del file .rt, riconosce il tipo (sp, pl, cy) e costruisce il t_object corrispondente.

**Esempio: riga di sfera**

```
sp 0,0,20.6 12.6 255,0,0


void	parse_sphere_line(t_scene *scene, const char *line)
{
	t_object	new_object;

	new_object.type = OBJ_SPHERE;

	// supponiamo che tu abbia già funzioni parse_vec3 e parse_color
	const char *cursor = line;

	parse_vec3(&cursor, &new_object.as.sphere.center);
	parse_double(&cursor, &new_object.as.sphere.diameter);
	parse_color(&cursor, &new_object.as.sphere.color);

	scene_add_object(scene, new_object);
}
```

**Esempio: riga di piano**

```
pl 0,0,-10 0,1,0 0,255,0

void	parse_plane_line(t_scene *scene, const char *line)
{
	t_object	new_object;

	new_object.type = OBJ_PLANE;
	const char *cursor = line;

	parse_vec3(&cursor, &new_object.as.plane.point);
	parse_vec3(&cursor, &new_object.as.plane.normal);
	parse_color(&cursor, &new_object.as.plane.color);

	scene_add_object(scene, new_object);
}
```
**Esempio: riga di cilindro**

```
cy 50,0,20 0,0,1 14.2 21.42 10,0,255

void	parse_cylinder_line(t_scene *scene, const char *line)
{
	t_object	new_object;

	new_object.type = OBJ_CYLINDER;
	const char *cursor = line;

	parse_vec3(&cursor, &new_object.as.cylinder.center);
	parse_vec3(&cursor, &new_object.as.cylinder.axis);
	parse_double(&cursor, &new_object.as.cylinder.diameter);
	parse_double(&cursor, &new_object.as.cylinder.height);
	parse_color(&cursor, &new_object.as.cylinder.color);

	scene_add_object(scene, new_object);
}
```

### 🌈 4. Uso nel rendering
Durante il ray tracing, scorrerai l’array scene->objects per trovare l’intersezione più vicina.
```
t_hit_record	find_nearest_intersection(t_ray ray, const t_scene *scene)
{
	t_hit_record	best_hit = { .t = INFINITY };
	for (size_t i = 0; i < scene->object_count; i++)
	{
		const t_object *obj = &scene->objects[i];
		t_hit_record	hit;

		if (obj->type == OBJ_SPHERE)
			hit = intersect_sphere(ray, &obj->as.sphere);
		else if (obj->type == OBJ_PLANE)
			hit = intersect_plane(ray, &obj->as.plane);
		else if (obj->type == OBJ_CYLINDER)
			hit = intersect_cylinder(ray, &obj->as.cylinder);
		else
			continue;

		if (hit.t > 0 && hit.t < best_hit.t)
			best_hit = hit;
	}
	return (best_hit);
}
```

# Flusso di azione:

**Dal main:**
-> Alloco memoria per la t_scene
-> Controllo che il file abbuia effettivamente l'estensione .rt (Valuterò dove spostarla in seguito).
-> parte **parse_file** che prende il puntatore al fd e la t_scene.

### Parse_file
-> parse_file inizializza subito la t_scene.
-> Apre il file con Open.
-> Legge la riga con GNL:
    - se è vuota la salta.
    - Altrimenti toglie il '\n' finale e mette '\0', e la pass a **lex_scan_check_and_count**.  

### NB: Assicurarmi di aggiungere al punto giusto credo qui, la funzione che in caso di errore libera correttamente tutta la memoria allocata.

### lex_scan_check_and_count

-> Prende la t_scene e la riga appena letta.
-> Prende la rioga e fa il primo controllo sul primo pezzo di token, ovvero l'ID, quello che dovrebbe identificare iul parametro con **check_first_token**, che fa due cose importanti:
    - Salta eventuali spazi iniziali e prende il primi caratteri.
    - prende i primi max 10 caratteri fino al prossimio spazio, e verifica che siano identificatori validi per i requisiti con **is_valid_identifier**.
        - Se lo sono: restituisce l'ID, e nel rest_out il puntatore al continuo della line.
        - Se NON lo sono NULL e quindi errore: IOdentificatorenon valido.
-> Se **check_first_token** va a buon fine a questo punto, sto implementando le funzioni che a seconda dell'id riconosciuto allocheranno il resto della roga mentre faranno gia anche i check che i valori siano corretti e nei range per ogni elemento, che non ci siano piu elementi la dove non dovessero essercene.

se con ft_strcmp dovesse trovare una "A":
### parse_ambient_line;