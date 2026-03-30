#include <iostream>
#define Max(a, b) ((a<b) ? (b) : (a))
template<class T>
class AVL
{
private:
    struct nod
    {
        T info;
        nod* st, * dr, * t;
        int n;
        short inaltime;
        nod(T x)
        {
            info = x;
            st = dr = t = NULL;
            n = 1;
            inaltime = 1;
        }
        nod(nod& ob)
        {
            info = ob.info;
            st = ob.st;
            dr = ob.dr;
            t = ob.t;
            n = ob.n;
            inaltime = ob.inaltime;
        }
    };
    nod* rad;
    short Inaltime(nod* p)
    {
        return p == NULL ? 0 : p->inaltime;
    }
    int NrNoduri(nod* p)
    {
        return p == NULL ? 0 : p->n;
    }
    int BF(nod* p)
    {
        if (p == NULL) return 0;
        return Inaltime(p->st) - Inaltime(p->dr);
    }
    void LeftRotate(nod** a)
    {
        nod* p, * q, * k;
        p = *a;
        q = p->dr;
        k = q->st;
        if (rad == p)
            rad = q;
        else if (p->t->st == p)
            p->t->st = q;
        else p->t->dr = q;
        q->st = p;
        q->t = p->t;
        p->t = q;
        p->dr = k;
        if (k != NULL) k->t = p;
        /// recalcularea inaltimilor
        p->inaltime = 1 + Max(Inaltime(p->st), Inaltime(p->dr));
        q->inaltime = 1 + Max(Inaltime(q->st), Inaltime(q->dr));
        /// recalcularea numarului de noduri din subarbore
        p->n = 1 + NrNoduri(p->st) + NrNoduri(p->dr);
        q->n = 1 + NrNoduri(q->st) + NrNoduri(q->dr);
        /// punem pointerul p la noua radacina din subarbore
        *a = q;
    }
    void RightRotate(nod** a)
    {
        nod* p, * q, * k;
        p = *a;
        q = p->st;
        k = q->dr;
        if (rad == p)
            rad = q;
        else if (p->t->st == p)
            p->t->st = q;
        else p->t->dr = q;
        q->dr = p;
        q->t = p->t;
        p->t = q;
        p->st = k;
        if (k != NULL) k->t = p;
        /// recalcularea inaltimilor
        p->inaltime = 1 + Max(Inaltime(p->st), Inaltime(p->dr));
        q->inaltime = 1 + Max(Inaltime(q->st), Inaltime(q->dr));
        /// recalcularea numarului de noduri din subarbore
        p->n = 1 + NrNoduri(p->st) + NrNoduri(p->dr);
        q->n = 1 + NrNoduri(q->st) + NrNoduri(q->dr);
        /// punem pointerul p la noua radacina din subarbore
        *a = q;
    }
    void Fix(nod* p)
    {
        while (p != NULL)
        {
            p->n = 1 + NrNoduri(p->st) + NrNoduri(p->dr);
            p->inaltime = 1 + Max(Inaltime(p->st), Inaltime(p->dr));
            int bf = BF(p);
            if (bf > 1)
            {
                if (BF(p->st) < 0)
                    LeftRotate(&(p->st));
                RightRotate(&p);
            }
            else if (bf < -1)
            {
                if (BF(p->dr) > 0)
                    RightRotate(&(p->dr));
                LeftRotate(&p);
            }
            p = p->t;
        }
    }
    void cpy(nod* a, nod* b)
    {
        if (b == NULL) return;
        a = new nod(*b);
        cpy(a->st, b->st);
        cpy(a->dr, b->dr);
    }
    void Clear(nod* p)
    {
        if (p == NULL) return;
        Clear(p->st);
        Clear(p->dr);
        delete p;
    }
public:
    /// constructorii
    AVL()
    {
        rad = NULL;
    }
    AVL(AVL& ob)
    {
        cpy(rad, ob.rad);
    }
    /// clasa iterator
    class iterator
    {
    private:
        nod* pt;
        iterator(nod* p) { pt = p; }
        friend class AVL;
    public:
        iterator() { pt = NULL; }
        iterator(const iterator& it) { pt = it.pt; }
        iterator& operator++()
        {
            /// ne ducem la urmatorul nod
            if (pt == NULL) return *this;
            if (pt->dr)
            {
                pt = pt->dr;
                while (pt->st != NULL)
                    pt = pt->st;
                return *this;
            }
            while (pt->t != NULL && pt->t->dr == pt)
                pt = pt->t;
            pt = pt->t;
            return *this;
        }
        iterator operator++(int)
        {
            iterator cpy = *this;
            /// ne ducem la urmatorul nod
            if (pt == NULL) return cpy;
            if (pt->dr)
            {
                pt = pt->dr;
                while (pt->st != NULL)
                    pt = pt->st;
                return cpy;
            }
            while (pt->t != NULL && pt->t->dr == pt)
                pt = pt->t;
            pt = pt->t;
            return cpy;
        }
        T operator*()
        {
            return pt->info;
        }
        bool operator==(iterator it)
        {
            return pt == it.pt;
        }
        bool operator!=(iterator it)
        {
            return pt != it.pt;
        }
    };
    /// metode
    iterator begin()
    {
        nod* p = rad;
        while (p->st != NULL)
            p = p->st;
        iterator it(p);
        return it;
    }
    iterator end()
    {
        iterator it(NULL);
        return it;
    }

    int size()
    {
        return NrNoduri(rad);
    }
    bool empty()
    {
        return rad == NULL;
    }

    void insert(T x)
    {
        if (rad == NULL)
        {
            rad = new nod(x);
            return;
        }
        nod* q = rad;
        nod* p = rad;
        while (p != NULL)
        {
            /// x este deja in arbore
            if (p->info == x) return;
            q = p;
            if (x < p->info)
                p = p->st;
            else p = p->dr;
        }
        p = new nod(x);
        if (x < q->info) q->st = p;
        else q->dr = p;
        p->t = q;
        Fix(p);
    }
    void clear()
    {
        Clear(rad);
    }
    void erase(iterator it)
    {
        if (it.pt == NULL) return;
        nod* p = it.pt;
        if (p->st == NULL && p->dr == NULL)
        {
            if (p == rad) rad = NULL;
            else if (p->t->st == p) p->t->st = NULL;
            else p->t->dr = NULL;
            Fix(p->t);
            delete p;
            return;
        }
        if (p->st != NULL && p->dr != NULL)
        {
            nod* q = p->dr;
            while (q->st != NULL) q = q->st;
            p->info = q->info;
            iterator it(q);
            erase(q);
            return;
        }
        nod* q = ((p->st != NULL) ? (p->st) : (p->dr));
        q->t = p->t;
        if (p == rad) rad = q;
        else if (p == p->t->st)
            p->t->st = q;
        else p->t->dr = q;
        Fix(q);
        delete p;
        return;
    }

    iterator operator[] (int i)
    {
        if (i < 1 || i > rad->n) return end();
        /// ca sa transformam indexarea de la 0 la 1
        i--;
        nod* p = rad;
        while (p)
        {
            if (i < NrNoduri(p->st)) p = p->st;
            else if (i == NrNoduri(p->st)) { iterator it(p); return it; }
            else
            {
                i -= NrNoduri(p->st) + 1;
                p = p->dr;
            }
        }
        return end();
    }

    iterator find(T x)
    {
        nod* p = rad;
        while (p != NULL && p->info != x)
            p = (x < p->info) ? p->st : p->dr;
        iterator it(p);
        return it;
    }

    iterator lower_bound(const T& x) {
        nod* p = rad;
        nod* res = NULL;

        while (p != NULL)
        {
            if (!(p->info < x))
            {
                res = p;
                p = p->st;
            }
            else
                p = p->dr;
        }
        iterator it(res);
        return it;
    }
    iterator upper_bound(const T& x) {
        nod* p = rad;
        nod* res = NULL;

        while (p != NULL)
        {
            if (!(p->info < x || p->info == x))
            {
                res = p;
                p = p->st;
            }
            else
                p = p->dr;
        }
        iterator it(res);
        return it;
    }
    /// destructor

    ~AVL()
    {
        clear();
    }
};

int main()
{
    return 0;
}