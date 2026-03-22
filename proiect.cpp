#include <iostream>
#include <fstream>
#include <cstring>
#include <crypt.h>
#include <ostream>
#define LEN_MAX 256

// inspiratie:
// proiectul rezolva problemele legate cu menajarea fisierelor dintr-un calculator 
// cand mai multe persoane le vor modifica
// https://www.youtube.com/watch?v=dQw4w9WgXcQ

class Calculator
{
	static constexpr const char fisier_grup[50] = "grup.in",
		fisier_utilizatori[50] = "users.in", fisier_fisiere[50] = "file.in";
	class ID_manager
	{
		bool taken_ids[1<<16];
	public:
		int get_valid_id()
		{
			int i;
			for (i = 0; i < (1<<16) && !taken_ids[i]; i++);
			return i;
		}
		void take_id(const int i) { taken_ids[i] = 1; }
	}grup_id, user_id;
	class Grup;
	class Utilizator
	{
		static constexpr const char salt[50] = "$5$bpKU3bUSQLwX87z/$\0";
		char* password;
		int id;
		char* nume;
		struct nod
		{
			int grup_id;
			nod* next;
			nod(const int g) : grup_id(g), next(NULL) {}
		};
		nod* Lista = NULL, *tail = NULL;
	public:
		Utilizator(){}
		Utilizator(const char* nume, const char* parola, const int id) : id(id)
		{
			this->nume = new char[1+strlen(nume)];
			strcpy(this->nume, nume);
			char* buff = crypt(password, salt);
			password = new char[1 + strlen(buff)];
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
			delete[] this->nume;
			this->nume = new char[1+strlen(nume)];
			strcpy(this->nume, nume);
			return *this;
		}
		void add_grup(int grup_id)
		{
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
			}
			for (nod* it = Lista; it != NULL; it = it->next)
				if (it->next->grup_id == grup_id)
				{
					nod* p = it->next;
					it->next = p->next;
					if (tail == p) tail = it;
					delete p;
					return;
				}
		}
		friend std::istream& operator>>(std::istream& fin, Utilizator& u)
		{
			fin >> u.id;
			fin.get();
			fin.getline(u.nume, LEN_MAX);
			fin.getline(u.password, LEN_MAX);
			int n, x;
			fin >> n;
			while(n)
			{
				fin >> x;
				u.add_grup(x);
			}
			return fin;
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
			delete[] password;
			delete[] nume;
			nod* p = Lista, *q;
			while (p)
			{
				q = p->next;
				delete p;
				p = q;
			}
		}
	};
	class Grup
	{
		int id;
		char* nume;
		struct nod
		{
			int user_id;
			nod* next;
			nod(const int u) : user_id(u), next(NULL) {}
		};
		nod* Lista = NULL, *tail = NULL;
	public:
		Grup(){}
		Grup(const char* nume, const int id)
		{
			this->nume = new char[1+strlen(nume)];
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
			delete[] this->nume;
			this->nume = new char[1+strlen(nume)];
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
			}
			for (nod* it = Lista; it != NULL; it = it->next)
				if (it->next->user_id == user_id)
				{
					nod* p = it->next;
					it->next = p->next;
					if (tail == p) tail = it;
					delete p;
					return;
				}
		}
		friend std::istream& operator>>(std::istream& fin, Grup& g)
		{
			fin >> g.id;
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
		~Grup()
		{
			delete[] nume;
			nod* p = Lista, *q;
			while (p)
			{
				q = p->next;
				delete p;
				p = q;
			}
		}
	};
	class Fisier
	{
		char* nume;
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
		int generic_perm;
	public:
		Fisier() : nume(NULL), Lista_u(NULL), tail_u(NULL), Lista_g(NULL), tail_g(NULL), generic_perm(0) {}
		Fisier(const char* nume) : Lista_u(NULL), tail_u(NULL), Lista_g(NULL), tail_g(NULL), generic_perm(0)
		{
			this->nume = new char[1+strlen(nume)];
			strcpy(this->nume, nume);
		}
		void change_grup_perm(int grup_id, int perm)
		{
			nod_grup* p;
			for (p = Lista_g; p != NULL && p->grup_id != grup_id; p = p->next);
			if (p != NULL)
				p->permisiuni = perm;
			else {tail_g->next = new nod_grup(grup_id, perm); tail_g = tail_g->next; } 
		}
		void change_user_perm(int user_id, int perm)
		{
			nod_u* p;
			for (p = Lista_u; p != NULL && p->user_id != user_id; p = p->next);
			if (p != NULL)
				p->permisiuni = perm;
			else {tail_u->next = new nod_u(user_id, perm); tail_u = tail_u->next; } 
		}
		bool operator==(const Fisier& f)
		{
			return (strcmp(nume, f.nume) == 0);
		}
		bool operator!=(const Fisier& f)
		{
			return (strcmp(nume, f.nume) != 0);
		}
		friend std::istream& operator>> (std::istream& fin, Fisier f)
		{
			f.nume = new char[256];
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
			return fin;
		}
		~Fisier()
		{
			delete[] nume;
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
			nod(const Fisier& file) : f(file), next(NULL){}
		};
		nod* Lista, *tail;
	public:
		Lista_fisiere() : Lista(NULL), tail(NULL) {}
		void add_fisier(const Fisier& f)
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
		void remove_fisier(const Fisier& f)
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
			for (nod* p = Lista; p->next != NULL && p->next->f != f; p = p->next);
			if (p->next == NULL) return;
			if (p->next == tail) tail = p;
			nod* q = p->next;
			p->next = q->next;
			delete q;
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
	Calculator()
	{
		std::ifstream f_user(fisier_utilizatori);
		Utilizator u;
		while (f_user >> u)
		{
			utilizatori[u.get_id()] = &u;
		}
		f_user.close();
		std::ifstream f_grup(fisier_grup);
		Grup g;
		while (f_grup >> g)
		{
			grupuri[g.get_id()] = &g;
		}
		std::ifstream f_fisier(fisier_fisiere);
		Fisier f;
		while (f_fisier >> f)
		{
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
		list.add_fisier(Fisier(nume));
	}
	void login(const char* nume, const char* parola)
	{
		for (int i = 0; i < (1<<16); i++)
			if (utilizatori[i] != NULL)	
			{
				Utilizator u(nume, parola, i);
				if (u == (*utilizatori[i])) 
				{ user_logged_in = i; return; }
			}
	}
	~Calculator()
	{
		for (int i = 0; i < (1<<16); i++)
		{
			if (utilizatori[i] != NULL) delete utilizatori[i];
			if (grupuri[i] != NULL) delete grupuri[i];
		}
	}
};

int main()
{
	return 0;
}
