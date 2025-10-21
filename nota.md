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

**int	parse_double(const char **line, double *out)**  -> Helper che estrae il double dalla stringa
**parse_rgb** -> Helper che estrae l'RGB dalla stringa

# NORMALIZZAZIONE:

Immagina un vettore 3D come una freccia che parte dall’origine (0,0,0) e punta in qualche direzione.

Nel file .rt, quando scrivi la direzione della camera, ad esempio:
C  10,0,0   0,0,1   70
il vettore 0,0,1 rappresenta dove guarda la camera.
🧮 Cos’è la normalizzazione
Immagina un vettore come una freccia che indica una direzione nello spazio.
👉 La lunghezza di quella freccia si chiama modulo o norma.
E si calcola con Pitagora:
```
(|v| sarebbe lunghezza)
|v| = √(x² + y² + z²)

-------

in c:

lunghezza = sqrt(x² + y² + z²)

```
E “normalizzato” significa: “stessa direzione, ma la freccia lunga esattamente 1 unità”.
💡 Perché lo controlliamo nel parser
Perché se la persona che scrive il file .rt sbaglia, allora la direzione non è normalizzata (|n| = 5),
e i tuoi calcoli di proiezione verrebbero sballati (il raggio della camera sarebbe troppo lungo).
Quindi il parser deve bloccare l’esecuzione e dire:
“Errore: la direzione non è normalizzata”.

# NB: Perchè senza sqrt?

la formula vera sarebbe
```
len = sqrt(x*x + y*y + z*z);
if (fabs(len - 1.0) <= epsilon) OK;
```

Ma la radice quadrata (sqrt):
è più lenta,
richiede **#include <math.h>, che non puoi usare nel progetto (vietato dal subject)**.
```
sqrt(x * x + y * y + z * z)
```
è proprio la radice quadrata di x² + y² + z²,
e serve a calcolare la lunghezza (o “modulo”) di un vettore 3D, cioè di una freccia che parte dall’origine e arriva a (x, y, z).
🎯 Cos’è quel risultato: la lunghezza del vettore
Se tu prendi un vettore (x, y, z), il risultato di:

```
sqrt(x*x + y*y + z*z)

```
ti dice quanto è lunga quella freccia.
È come usare il teorema di Pitagora in tre dimensioni:
x → **quanto si muove sul lato orizzontale**
y → **quanto si muove in verticale**
z → **quanto si muove in profondità**
e la radice quadrata ti dà la **distanza “diretta” dalla partenza (0,0,0) al punto (x,y,z)**

| Vettore               | Calcolo                           | Lunghezza      |
| --------------------- | --------------------------------- | -------------- |
| (1, 0, 0)             | √(1² + 0² + 0²) = 1               | ✅ unitario     |
| (0, 0, 5)             | √(0² + 0² + 5²) = 5               | ❌ troppo lungo |
| (1, 2, 2)             | √(1 + 4 + 4) = √9 = 3             | ❌ lungo 3      |
| (0.577, 0.577, 0.577) | √(0.333 + 0.333 + 0.333) = √1 = 1 | ✅ normalizzato |

🧠 Cosa dovresti capire dal risultato
Risultato	Cosa significa
= 1	il vettore è normalizzato → freccia lunga esattamente 1
> 1	è troppo lungo → bisogna accorciarlo (dividi per la lunghezza)
< 1	è troppo corto → non normalizzato (devi allungarlo)
= 0	è il vettore nullo (tutte componenti zero) → errore, non ha direzione

### 💡 A cosa serve nel tuo progetto (miniRT)
Nel ray tracer:
la direzione della camera, o la normale di una superficie,
deve essere solo una direzione, non una distanza;
per questo deve avere lunghezza 1.
Perché se fosse lunga 5, significherebbe che la direzione “pesa 5 volte di più” nei calcoli.
E i raggi o le luci verrebbero spostati o deformati.
✏️ In pratica
Quando nel parser leggi 0,0,5 come direzione:
```
len = sqrt(0*0 + 0*0 + 5*5); // len = 5
```
❌ non va bene: la direzione è lunga 5.
Per renderla corretta:
```
x = 0 / 5;
y = 0 / 5;
z = 5 / 5; // dividi ogni componente per la lunghezza
```
➡️ ottieni 0,0,1, che è normalizzato ✅

