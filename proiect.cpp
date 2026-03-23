#include <iostream>
#include <fstream>
#include <cstring>
#include <crypt.h>
#define LEN_MAX 256

/*
Proiectul rezolva problema menajarii fisierelor de mai multi utilizatori simultani
Utilizatorii pot apartine mai multor grupuri simultan, astfel trebuie sa verificam toate grupurile
*/


class Calculator
{
	static constexpr const char fisier_grup[50] = "grup.in",
		fisier_utilizatori[50] = "users.in", fisier_fisiere[50] = "file.in";
	// clasa asistoare pentru a menaja grupuri si utilizatori cu id-uri
	class ID_manager
	{
		bool taken_ids[1<<16];
	public:
		ID_manager()
		{
			memset(taken_ids, 0, sizeof(taken_ids));
		}
		int get_valid_id()
		{
			int i;
			for (i = 0; i < (1<<16) && taken_ids[i]; i++);
			return i;
		}
		void take_id(const int i) { taken_ids[i] = 1; }
	}grup_id, user_id;

	// clasa utilizator care asista la menajarea utilizatorilor
	class Utilizator
	{
		static constexpr const char salt[50] = "$5$bpKU3bUSQLwX87z/$\0";
		char password[LEN_MAX];
		int id;
		char nume[LEN_MAX];
		struct nod
		{
			int grup_id;
			nod* next;
			nod(const int g) : grup_id(g), next(NULL) {}
		};
		nod* Lista = NULL, *tail = NULL;
		int lista_len = 0;
	public:
		Utilizator(){}
		Utilizator(const char* nume, const char* parola, const int id) : id(id)
		{
			strcpy(this->nume, nume);
			char* buff = crypt(parola, salt);
			strcpy(password, buff);
		}
		int get_id() const { return id; }
		char* get_nume() const
		{
			char* ret = new char[1+strlen(nume)];
			strcpy(ret, nume);
			return ret;
		}
		Utilizator& change_nume(const char* nume)
		{
			strcpy(this->nume, nume);
			return *this;
		}
		void add_grup(int grup_id)
		{
			lista_len++;
			if (Lista == NULL)
			{
				Lista = new nod(grup_id);
				tail = Lista;
				return;
			}
			tail->next = new nod(grup_id);
			tail = tail->next;
		}
		void remove_grup(int grup_id)
		{
			if (Lista == NULL) return;
			if (Lista->grup_id == grup_id)
			{
				nod* p = Lista->next;
				if (tail == Lista) tail = NULL;
				delete Lista;
				Lista = p;
				lista_len--;
			}
			for (nod* it = Lista; it != NULL; it = it->next)
				if (it->next && it->next->grup_id == grup_id)
				{
					nod* p = it->next;
					it->next = p->next;
					if (tail == p) tail = it;
					delete p;
					lista_len--;
					return;
				}
		}
		friend std::istream& operator>>(std::istream& fin, Utilizator& u)
		{
			if (!(fin >> u.id)) return fin;
			fin.getline(u.nume, LEN_MAX);
			fin.getline(u.password, LEN_MAX);
			int n, x;
			fin >> n;
			while(n--)
			{
				fin >> x;
				u.add_grup(x);
			}
			fin.get(); fin.get();
			return fin;
		}
		friend std::ostream& operator<<(std::ostream& fout, Utilizator& u)
		{
			fout << u.id << "\n";
			fout << u.nume << "\n";
			fout << u.password << "\n" << u.lista_len << "\n";
			for (nod* p = u.Lista; p != NULL; p = p->next)
				fout << p->grup_id << "\n";
			return fout;
		}
		bool validate(const char* nume, const char* parola)
		{
			return strcmp(this->nume, nume) == 0 && strcmp(crypt(parola, salt), this->password) == 0;
		}
		bool operator==(const Utilizator& u)
		{
			return id == u.id;
		}
		bool operator!=(const Utilizator& u)
		{
			return id != u.id;
		}
		~Utilizator()
		{
			nod* p = Lista, *q;
			while (p)
			{
				q = p->next;
				delete p;
				p = q;
			}
		}
	};

	// clasa grup care asista la menajarea grupurilor:
	class Grup
	{
		int id;
		char nume[LEN_MAX];
		struct nod
		{
			int user_id;
			nod* next;
			nod(const int u) : user_id(u), next(NULL) {}
		};
		nod* Lista = NULL, *tail = NULL;
		int lista_len = 0;
	public:
		Grup(){}
		Grup(const char* nume, const int id)
		{
			strcpy(this->nume, nume);
			this->id = id;
		}
		int get_id() const { return id; }
		char* get_nume() const
		{
			char* ret = new char[1+strlen(nume)];
			strcpy(ret, nume);
			return ret;
		}
		Grup& change_nume(const char* nume)
		{
			strcpy(this->nume, nume);
			return *this;
		}
		bool operator==(const Grup& g)
		{
			return id == g.id;
		}
		bool operator!=(const Grup& g)
		{
			return id != g.id;
		}
		void add_user(int user_id)
		{
			lista_len++;
			if (Lista == NULL)
			{
				Lista = new nod(user_id);
				tail = Lista;
				return;
			}
			tail->next = new nod(user_id);
			tail = tail->next;
		}
		void remove_user(int user_id)
		{
			if (Lista == NULL) return;
			if (Lista->user_id == user_id)
			{
				nod* p = Lista->next;
				if (tail == Lista) tail = NULL;
				delete Lista;
				Lista = p;
				lista_len--;
			}
			for (nod* it = Lista; it != NULL; it = it->next)
				if (it->next->user_id == user_id)
				{
					nod* p = it->next;
					it->next = p->next;
					if (tail == p) tail = it;
					delete p;
					lista_len--;
					return;
				}
		}
		friend std::istream& operator>>(std::istream& fin, Grup& g)
		{
			if (!(fin >> g.id)) return fin;
			fin.get();
			fin.getline(g.nume, LEN_MAX);
			int n, x;
			fin >> n;
			while(n--)
			{
				fin >> x;
				g.add_user(x);
			}
			return fin;
		}
		friend std::ostream& operator<<(std::ostream& fout, Grup& g)
		{
			fout << g.id << "\n";
			fout << g.nume << "\n" << g.lista_len << "\n";
			for (nod* p = g.Lista; p != NULL; p = p->next)
				fout << p->user_id << "\n";
			return fout;
		}
		~Grup()
		{
			nod* p = Lista, *q;
			while (p)
			{
				q = p->next;
				delete p;
				p = q;
			}
		}
	};

	// menajarea fisierelor:
	class Fisier
	{
		char nume[LEN_MAX];
		struct nod_grup
		{
			nod_grup* next;
			int grup_id, permisiuni;
			nod_grup(int g, int perm) : grup_id(g), next(NULL), permisiuni(perm) {}
		};
		nod_grup* Lista_g, *tail_g;
		struct nod_u
		{
			nod_u* next;
			int user_id, permisiuni;
			nod_u(int u, int  perm) : user_id(u), next(NULL), permisiuni(perm) {}
		};
		nod_u* Lista_u, *tail_u;
		int lista_g_len;
		int lista_u_len;
		int generic_perm;
	public:
		Fisier() : Lista_u(NULL), tail_u(NULL), Lista_g(NULL), tail_g(NULL), generic_perm(0),
			lista_g_len(0), lista_u_len(0) {strcpy(nume, "");}
		Fisier(const char* nume) : Lista_u(NULL), tail_u(NULL), Lista_g(NULL), tail_g(NULL), generic_perm(0),
			lista_g_len(0), lista_u_len(0) 
		{
			strcpy(this->nume, nume);
		}
		Fisier(const char* nume, int user_id) : Lista_u(NULL), tail_u(NULL), Lista_g(NULL), tail_g(NULL), generic_perm(0),
			lista_g_len(0), lista_u_len(0) 
		{
			strcpy(this->nume, nume);
			change_user_perm(user_id, 3);
		}
		Fisier(const Fisier& ob): generic_perm(ob.generic_perm), lista_g_len(ob.lista_g_len), lista_u_len(ob.lista_u_len)
				,Lista_g(NULL), tail_g(NULL), Lista_u(NULL), tail_u(NULL)
		{
			strcpy(nume, ob.nume);
			for (nod_u* p = ob.Lista_u; p != NULL; p = p->next)
				change_user_perm(p->user_id, p->permisiuni);
			for (nod_grup* p = ob.Lista_g; p != NULL; p = p->next)
				change_grup_perm(p->grup_id, p->permisiuni);
		}
		void change_grup_perm(int grup_id, int perm)
		{
			nod_grup* p;
			for (p = Lista_g; p != NULL && p->grup_id != grup_id; p = p->next);
			if (p != NULL) {
				p->permisiuni = perm;
				return;
			}
			if (!Lista_g) {
			{ Lista_g = tail_g = new nod_grup(grup_id, perm); lista_g_len = 1;}
			} else {
				tail_g->next = new nod_grup(grup_id, perm);
				tail_g = tail_g->next;
				lista_g_len++;
			}
		}
		void change_user_perm(int user_id, int perm)
		{
			nod_u* p;
			for (p = Lista_u; p != NULL && p->user_id != user_id; p = p->next);
			if (p != NULL) {
				p->permisiuni = perm;
				return;
			}
			if (!Lista_u) {
			{ Lista_u = tail_u = new nod_u(user_id, perm); lista_u_len = 1; }
			} else {
				tail_u->next = new nod_u(user_id, perm);
				tail_u = tail_u->next;
				lista_u_len++;
			}
		}
		Fisier& operator=(const Fisier& ob)
		{
			if (this == &ob) return *this;
			nod_u* pu = Lista_u;
			while (pu)
			{
				nod_u* q = pu;
				pu = pu->next;
				delete q;
			}
			Lista_u = tail_u = NULL;

			nod_grup* pg = Lista_g;
			while (pg)
			{
				nod_grup* q = pg;
				pg = pg->next;
				delete q;
			}
			Lista_g = tail_g = NULL;

			strcpy(nume, ob.nume);
			generic_perm = ob.generic_perm;
			lista_g_len = 0;
			lista_u_len = 0;

			for (nod_u* p = ob.Lista_u; p != NULL; p = p->next)
				change_user_perm(p->user_id, p->permisiuni);

			for (nod_grup* p = ob.Lista_g; p != NULL; p = p->next)
				change_grup_perm(p->grup_id, p->permisiuni);

			return *this;
		}
		bool operator==(const Fisier& f)
		{
			return (strcmp(nume, f.nume) == 0);
		}
		bool operator!=(const Fisier& f)
		{
			return (strcmp(nume, f.nume) != 0);
		}
		friend std::istream& operator>> (std::istream& fin, Fisier& f)
		{
			fin.getline(f.nume, 256);
			fin >> f.generic_perm;
			int n, m, x, p;
			fin >> n >> m;
			while (n--)
			{
				fin >> x >> p;
				f.change_grup_perm(x, p);
			}
			while (m--)
			{
				fin >> x >> p;
				f.change_user_perm(x, p);
			}
			fin.get();
			return fin;
		}
		friend std::ostream& operator<<(std::ostream& fout, Fisier& f)
		{
			fout << f.nume;
			fout << "\n" << f.lista_g_len << "\n";
			for (nod_grup* p = f.Lista_g; p != NULL; p = p->next)
				fout << p->grup_id << " " << p->permisiuni << "\n";
			fout << f.lista_u_len << "\n";
			for (nod_u* p = f.Lista_u; p != NULL; p = p->next)
				fout << p->user_id << " " << p->permisiuni << "\n";
			return fout;
		}
		~Fisier()
		{
			nod_u*p = Lista_u;
			while (p)
			{
				nod_u* q = p;
				p = p->next;
				delete q;
			}
			nod_grup* g = Lista_g;
			while (g)
			{
				nod_grup* q = g;
				g = g->next;
				delete q;
			}
		}
	};
	class Lista_fisiere
	{
		struct nod
		{
			Fisier f;
			nod* next;
			nod(Fisier& file) : f(file), next(NULL){}
		};
		nod* Lista, *tail;
	public:
		Lista_fisiere() : Lista(NULL), tail(NULL) {}
		void add_fisier(Fisier f)
		{
			if (Lista == NULL)
			{
				Lista = new nod(f);
				tail = Lista;
				return;
			}
			tail->next = new nod(f);
			tail = tail->next;
		}
		void remove_fisier(Fisier& f)
		{
			if (Lista == NULL) return;
			if (Lista->f == f)
			{
				nod* p = Lista->next;
				delete Lista;
				Lista = p;
				if (Lista == NULL) tail = NULL;
			}
			nod* p;
			for (nod* p; p->next != NULL && p->next->f != f; p = p->next);
			if (p->next == NULL) return;
			if (p->next == tail) tail = p;
			nod* q = p->next;
			p->next = q->next;
			delete q;
		}
		friend std::ostream& operator<<(std::ostream& fout, Lista_fisiere &l)
		{
			for (nod*p = l.Lista; p != NULL; p = p->next)
				fout << (p->f) << "\n";
			return fout;
		}
		~Lista_fisiere()
		{
			nod* p = Lista;
			while(p)
			{
				nod*q = p;
				p = p->next;
				delete q;
			}
		}
	};
	Grup* grupuri[1<<16];
	Utilizator* utilizatori[1<<16];
	int user_logged_in;
	Lista_fisiere list;
public:

	// interfata:
	Calculator()
	{
		memset(grupuri, 0, sizeof(grupuri));
		memset(utilizatori, 0, sizeof(utilizatori));
		user_logged_in = -1;
		std::ifstream f_user(fisier_utilizatori);
		while (true) {
			Utilizator* u = new Utilizator;
			if (!(f_user >> *u)){ delete u; break; }
			utilizatori[u->get_id()] = u;
		}
		f_user.close();
		std::ifstream f_grup(fisier_grup);
		while (true) {
			Grup* g = new Grup;
			if (!(f_grup >> *g)){ delete g; break; }
			grupuri[g->get_id()] = g;
		}
		std::ifstream f_fisier(fisier_fisiere);
		while (true) {
			Fisier f;
			if (!(f_fisier >> f)) break; 
			list.add_fisier(f);
		}
	}
	void Add_user(const char* nume, const char* parola)
	{
		int id = user_id.get_valid_id();
		user_id.take_id(id);
		utilizatori[id] = new Utilizator(nume, parola, id);
	}
	void Add_grup(const char* nume)
	{
		int id = grup_id.get_valid_id();
		grup_id.take_id(id);
		grupuri[id] = new Grup(nume, id);
	}
	void make_file(const char* nume)
	{
		list.add_fisier(Fisier(nume, user_logged_in));
	}
	bool exista_utilizatori()
	{
		int i;
		for (i = 0; i < (1<<16) && utilizatori[i] == NULL; i++);
		return i < (1<<16);
	}
	void login()
	{
		char nume[LEN_MAX], parola[LEN_MAX];
		while(user_logged_in == -1)
		{
			std::cout << "Introduce numele: ";
			std::cin >> nume;
			std::cout << "Introduce parola: ";
			std::cin >> parola;
			for (int i = 0; i < (1<<16); i++)
				if (utilizatori[i] != NULL)	
				{
					if (utilizatori[i]->validate(nume, parola)) 
					{ user_logged_in = i; return; }
			}
			std::cout << "Logare esuata\n";
		}
	}

	~Calculator()
	{
		std::ofstream f_u(fisier_utilizatori), f_g(fisier_grup), f_f(fisier_fisiere);
		f_f << list;
		for (int i = 0; i < (1<<16); i++)
		{
			if (utilizatori[i] != NULL)
			{
				f_u << *utilizatori[i]; 
				delete utilizatori[i];
			}
			if (grupuri[i] != NULL)
			{
				f_g << *grupuri[i];
				delete grupuri[i];
			}
		}
	}
};

int main()
{
	char nume[LEN_MAX], parola[LEN_MAX];
	Calculator c;
	if (c.exista_utilizatori())
		c.login();
	else
	{
		std::cout << "Introduce numele: ";
		std::cin >> nume;
		std::cout << "Introduce parola: ";
		std::cin >> parola;
		c.Add_user(nume, parola);
		std::cout << "\n";
		c.login();
	}
	int x;
	do {
		std::cout << "Actiunea dorita:\n0 - exit\n1 - adaugare utilizator\n2 - adaugare grup\n3- creare fisier vid\n";
		std::cin >> x;
		switch(x)
		{
		case 0: return 0; break;
		case 1:
			std::cout << "Introduce numele: ";
			std::cin >> nume;
			std::cout << "\nIntroduce parola: ";
			std::cin >> parola;
			c.Add_user(nume, parola);
			break;
		case 2:
			std::cout << "Introduce numele: ";
			std::cin >> nume;
			c.Add_grup(nume);
			break;
		case 3:
			std::cout << "Introduce numele: ";
			std::cin >> nume;
			c.make_file(nume);
			break;
		}
	} while (x > 0 && x <= 3);
	return 0;
}
