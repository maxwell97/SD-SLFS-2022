**Nume: Andrei-Radu Manea**

**Grupă: 313CA**

## Simple Linux File System - Tema 3

### Descriere:

* Scurtă descriere a funcționalității temei
Tema data consta in implementarea conceptului de arbore generic utilizand liste inlantuite;

Pentru implementarea structurii arborelui, am creat un director 
radacina "root" cu tipul FileTree si am creat o lista simplu inlantuita
unde vor fi stocate elementele sub forma nodurilor, in care respectiv 
vor fi stocate alte noduri sau continutul fisierelor(in functie de tipul nodului)

* Comenzi posibile:

1. touch <filename> <filecontent>
 - creeaza un fisier
 - acesta va contine <filecontent> daca este dat ca parametru, daca nu va fi creeat fara content

2. ls <arg>
 - va lista fisierele si directoarele din directorul parinte(cel curent) daca nu este dar un argument
 - daca exista un argument, se va afisa continutul directorului, respectiv continutul fisierului dat ca parametru
 - daca nu exista <arg> se afiseaza o eroare

3. mkdir <dirname>
 - creeaza un director cu numele <dirname> in directorul in care ne aflam
 - daca exista deja un director cu acelasi nume, se va afisa o eroare

4. cd <path>
 - primeste ca parametrul calea catre un fisier / director
 - parcurge arborele, nod cu nod, pana cand gaseste path-ul cautat
 - intoarce directorul destinatie, daca este gasit

5. tree <path>
 - daca nu este dat niciun parametru, va afisa toata ierarhia de fisiere si directoare, incepand de la root
 - daca este dat un path, va afisa ierarhia pornind de la directorul dat

6. pwd
 - intoarce calea absoluta a directorului curent, relativa la root, in cazul dat

7. rmdir <dirname>
 - sterge un director, numai daca acesta este gol

8. rm <filename>
 - sterge continutul unui fisier si fisierul in sine
 - este cautat si respectiv gasit fisierul dat prin <filename>, apoi nodul acestuia este sters din lista de copii a directorului in care se afla si memoria sa este eliberata

9. rmrec <resourcename>
 - sterge fisierul / directorul primit ca parametru recursiv, reapelandu-se in interiorul sau
 - cand functia primeste ca parametru un director, este reapelata avand ca parametru nou, pe rand, elementele directorului respectiv
 - daca directorul dat este gol, se apeleaza rmdir
 - cand functia primeste ca parametru un fisier, apeleaza functia rm

10. cp <source> <destination>
 - primeste ca parametru numai fisier si il copiaza la destinatia data
 - daca exista deja un fisier cu acelasi nume la destinatie, continutul va fi inlocuit
 - daca este dat un fisier non-existent ca destinatie, va fi creat unul cu numele respectiv si cu continutul din <source>

11. mv <source> <destination>
 - va muta un fisier SAU director catre destinatia data
 - similara cu cp, dar functioneaza si cu directoare
 - dupa ce se "copiaza" elementul dat ca <source>, se sterge nodul <source>

### Resurse / Bibliografie:

1. (https://ocw.cs.pub.ro/courses/sd-ca)
2. (https://geeksforgeeks.org)
3. (https://www.ibm.com/docs/en/zos/2.4.0?topic=reference-library-functions)
