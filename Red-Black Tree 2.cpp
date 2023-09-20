#include <iostream>
#include <queue>
using namespace std;

#define mp make_pair
#define F first
#define S second

class node {
    public :
        int val;
        node *right, *left, *par;
        bool color;     //0 for red and 1 for black.
        bool is_left_child, is_right_child;
        bool flag;
        node (int x = 0, bool c = 0, node *p = NULL) {
            val = x;
            par = p;
            color = c;
            is_left_child = 0;
            is_right_child = 0;
            right = NULL;
            left = NULL;
            flag = 0;
        }
};

class RBtree {
    public :
        node* root;
        int sz;
        RBtree () {
            sz = 0;
            root = NULL;
        }
        void rotate_right (node*);
        void rotate_left (node*);
        void insert (int);
        void show_tree (node*);
        node* grandpar (node* v) {
            if (v->par == NULL or v->par->par == NULL) return NULL;
            return v->par->par;
        }
        node* uncle (node *v) {
            if (v->par == NULL or v->par->par == NULL) return NULL;
            node *p = v->par;
            node *gp = p->par;
            if (p->is_left_child) return gp->right;
            else return gp->left;
        }
        node* sibling (node *v) {
            if (v->par == NULL) return NULL;
            node *p = v->par;
            if (v->is_left_child) return p->right;
            if (v->is_right_child) return p->left;
            return NULL;
        }
        void insert_relax (node*);
        node* search (int);
        void erase (node *v) {
            if (v->left and v->right) {
                node *suc = successor(v);
                v->val = suc->val;
                v = suc;
            }
            node* u = NULL;
            if (v->left) u = v->left;
            if (v->right) u = v->right;
            erase_relax(v, u);
        }
        void erase_relax (node*, node*);
        node* successor (node *v) {
            return successor_help (v->right);
        }
        node* successor_help (node*);
        void double_black (node*, node*);
};

void RBtree::rotate_right (node *v) {
    if (v->par == NULL) return;
    node *u = v->par;
    if (u->par == NULL) root = v;
    else if (u->is_left_child) {
        u->par->left = v;
        v->is_left_child = 1;
    }
    else if (u->is_right_child) {
        u->par->right = v;
        v->is_right_child = 1;
    }

    v->par = u->par;
    u->par = v;

    u->is_left_child = 0;
    u->is_right_child = 1;

    u->left = v->right;
    if (u->left != NULL) {
        u->left->par = u;
        u->left->is_left_child = 1;
        u->left->is_right_child = 0;
    }

    v->right = u;
}

void RBtree::rotate_left (node *v) {
    if (v->par == NULL) return;
    node *u = v->par;
    //cerr<<v->val<<" "<<u->val<<endl;
    if (u->par == NULL) root = v;
    else if (u->is_left_child) {
        u->par->left = v;
        v->is_left_child = 1;
    }
    else if (u->is_right_child) {
        u->par->right = v;
        v->is_right_child = 1;
    }

    v->par = u->par;
    u->par = v;

    u->is_left_child = 1;
    u->is_right_child = 0;

    u->right = v->left;
    if (u->right != NULL) {
        u->right->par = u;
        u->right->is_left_child = 0;
        u->right->is_right_child = 1;
    }

    v->left = u;
}

void RBtree::insert (int x) {
    sz++;
    if (root == NULL) {
        root = new node (x, 1);
        return;
    }
    node *cur = root, *v;
    while (true) {
        if (x <= cur->val) {
            if (cur->left == NULL) {
                cur->left = new node(x, 0, cur);
                cur->left->is_left_child = 1;
                v = cur->left;
                break;
            }
            cur = cur->left;
        }
        else {
            if (cur->right == NULL) {
                cur->right = new node (x, 0, cur);
                cur->right->is_right_child = 1;
                v = cur->right;
                break;
            }
            cur = cur->right;
        }
    }
    if (v->par->color == 1) return;
    //parent of v is red so grandparent exists and it's black
    insert_relax(v);
}

void RBtree::insert_relax (node *v) {
    if (v->par == NULL) {
        v->color = 1;
        return;
    }
    node *p = v->par;
    if (p->color == 1) return;
    if (p->color == 1 or p->par == NULL) {
        return;
    }
    //grandparent exist
    node *u = uncle(v);
    node *gp = p->par;
    if (u != NULL and u->color == 0) {
        p->color = 1;
        u->color = 1;
        gp->color = 0;
        insert_relax (gp);
        return;
    }
    else if (u == NULL or u->color == 1) {
        if (p->is_left_child) {
            if (v->is_right_child) {
                rotate_left(v);
                swap(v, p);
            }
            rotate_right(p);
            gp->color = 0;
            p->color = 1;
            return;
        }
        else if (p->is_right_child) {
            if (v->is_left_child) {
                rotate_right(v);
                swap(v, p);
            }
            rotate_left(p);
            gp->color = 0;
            p->color = 1;
            return;
        }
    }
}

void RBtree::show_tree (node *cur) {
    queue< pair<node*, int> > Q;
    node *nll = new node();
    nll->is_left_child = 1;
    nll->is_right_child = 1;
    int cnt = 0;
    Q.push(mp(cur, 0));
    int last_hight = 0;
    char color_map[2] = {'R', 'B'};
    while (Q.size()) {
        pair<node*, int> v = Q.front();
        Q.pop();
        if (v.S != last_hight) {
            cout<<endl;
            for (int i = 0; i < cnt; i++) cout<<"/\\";
            cout<<endl;
            cnt = 0;
            last_hight = v.S;
        }
        
        if ( (v.F->is_left_child and v.F->is_right_child) ) cout<<'N';
        else {
            cout<<v.F->val<<color_map[v.F->color];
            cnt++;
        }

        if (v.F->flag) cout<<'*';

        cout<<" ";

        if (v.F->left) Q.push(mp (v.F->left, v.S+1));
        else if ( !(v.F->is_left_child and v.F->is_right_child) ) Q.push(mp(nll, v.S+1));

        if (v.F->right) Q.push(mp (v.F->right, v.S+1));
        else if ( !(v.F->is_left_child and v.F->is_right_child) ) Q.push(mp(nll, v.S+1));
    }
    cout<<endl;
}

node* RBtree::search (int x) {
    node *cur = root;
    while (cur != NULL) {
        if (x == cur->val) return cur;
        else if (x < cur->val) cur = cur->left;
        else if (x > cur->val) cur = cur->right;
    }
    return cur;
}

node* RBtree::successor_help (node *v) {
    if (v->left) return successor_help (v->left);
    else if (v->right) return successor_help (v->right);
    else return v;
}

void RBtree::erase_relax (node *v, node *u) {
    if (v == root) {
        root = u;
        if (u != NULL) {
            u->is_left_child = v->is_left_child;
            u->is_right_child = v->is_right_child;
            u->par = v->par;
            u->color = 1;
        }
        return;
    }
    if (v->color == 0 or (u != NULL and u->color == 0)) {
        if (v->is_right_child) v->par->right = u;
        if (v->is_left_child) v->par->left = u;
        if (u != NULL) {
            u->is_left_child = v->is_left_child;
            u->is_right_child = v->is_right_child;
            u->par = v->par;
            u->color = 1;
        }
        return;
    }
    if (v->is_left_child) v->par->left = u;
    else if (v->is_right_child) v->par->right = u;
    if (u != NULL) {
        u->par = v->par;
        u->is_right_child = v->is_right_child;
        u->is_left_child = v->is_left_child;
        u->color = 1;
    }
    double_black (u, v->par);
}

void RBtree::double_black (node *u, node *p) {
    if (p == NULL) {
        if (u) u->color = 1;
        root = u;
        return;
    }
    node *s = NULL;
    if (p->left == u) s = p->right;
    else s = p->left;

    if (s == NULL) {
        double_black (p, p->par);
        return;
    }
    if (s and s->color == 1) {
        node *r = NULL;
        if (s->left and s->left->color == 0) r = s->left;
        if (s->right and s->right->color == 0) r = s->right;
        if (r) {
            if (s->is_right_child) {
                if (r->is_left_child) {
                    rotate_right(r);
                    s->color = 0;
                    r->color = 1;
                    swap(s, r);
                }
                rotate_left(s);
                r->color = 1;
            }
            if (s->is_left_child) {
                if (r->is_right_child) {
                    rotate_left(r);
                    s->color = 0;
                    r->color = 1;
                    swap(s, r);
                }
                rotate_right(s);
                r->color = 1;
            }
        }
        if (!r) {
            s->color = 0;
            double_black(p, p->par);
        }
    }
    if (s and s->color == 0) {
        if (s->is_right_child) {
            rotate_left (s);
            s->color = 1;
            //p->color = 0 then p->color = 1 so we do nothing.
            if (p->right) p->right->color = 0;
        }
        if (s->is_left_child) {
            rotate_right (s);
            s->color = 1;
            if (p->left) p->left->color = 0;
        }
    }
}

int main() {
    RBtree s;
    while (true) {
        int x;
        char c;
        cin>>c;
        if (c == 'q') break;
        cin>>x;
        if (c == 'i') {
            s.insert(x);
            s.show_tree(s.root);
        }
        else if (c == 's') {
            node *v = s.search(x);
            if (v == NULL) {
                cout<<"INVALID"<<endl;
                continue;
            }
            v->flag = 1;
            s.show_tree(s.root);
            v->flag = 0;
        }
        else if (c == 'd') {
            node *v = s.search(x);
            if (v == NULL) {
                cout<<"INVALID"<<endl;
                continue;
            }
            s.erase(v);
            s.show_tree(s.root);
        }
    }
}