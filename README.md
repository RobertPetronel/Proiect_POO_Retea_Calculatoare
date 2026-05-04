Proiectul creaza un envirement in care poti sa adaugi fisiere sau alte directoare, astfel fiind posibil crearea ierarhiilor de directoare. Pentru a testa programul, rulati scriptul compile.sh si pentru a rula codul, utilizati executabilil main.

Folderul "include" contine toate fisierele de tip .h, iar folderul "src" contine toate fisierele de tip .cpp. "proiect.cpp" este inceputul proiectului.

Partile proiectului:
- comenzile: Acestea sunt implementate prin mostenirea unei clase comune "Executabil". Fiecare comanda rescrie metoda "run".
- calculator: Unitatea care contine mai multe parti. Clasa "calculator" este responsabila pentru toate operatiile pe fisiere locale prin intermediul comenzilor.

Review 1: Mihai D. Laurentiu-Ionut

Documentatie pentru ceea ce a fost utilizat:
- https://en.cppreference.com/cpp/filesystem navigarea fisierelor si folderelor de pe disk
- https://cplusplus.com/reference/sstream/ parcurgerea stringurilor