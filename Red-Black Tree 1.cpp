#include <bits/stdc++.h>

#define pb push_back
#define pf push_front

#define F first
#define S second

#define IOS ios_base::sync_with_stdio(false), cin.tie(0), cout.tie(0);

using namespace std;
typedef long long ll;
typedef long double ld;

//cout<<fixed<<setprecision(x)<<d;
// 107, 109, 211, 223, 307, 401, 409, 503, 509, 601, 607, 701, 709, 809, 907, 1009

const int MAX_N = 1e5 + 100, LOG = 23;
const int mod = 1e9 + 7, MOD = 1e9 + 9;
const ll  inf = 1e9, INF = 1e18, super_inf = ~0ull / 4;

class node{
    public:
        int key;
        bool color;
        node *left, *right, *par;
        node(int val = 0, bool c = 0, node *p = NULL){
            key = val;
            color = c;
            par = p;
            left = NULL;
            right = NULL;
        }
} fake;

node *getpar(node *n){
    if(n == NULL){
        return NULL;
    }
    return n -> par;
}

node *getgpar(node *n){
    return getpar(getpar(n));
}

node *getsib(node* n){
    node *p = getpar(n);
    if(p == NULL){
        return NULL;
    }
    if(n == p -> left){
        return p -> right;
    }
    else{
        return p -> left;
    }
}

node *getun(node *n){
    node *p = getpar(n);
    return getsib(p);
}

void lrotate(node *n){
    node *r = n -> right;
    node *p = getpar(n);
    n -> right = r -> left;
    r -> left = n;
    n -> par = r;
    if(n -> right != NULL){
        n -> right -> par = n;
    }
    if(p != NULL){
        if(n == p -> left){
            p -> left = r;
        }
        else{
            p -> right = r;
        }
    }
    r -> par = p;
}

void rrotate(node *n){
    node *l = n -> left;
    node *p = getpar(n);
    n -> left = l -> right;
    l -> right = n;
    n -> par = l;
    if(n -> left != NULL){
        n -> left -> par = n;
    }
    if(p != NULL){
        if(n == p -> left){
            p -> left = l;
        }
        else{
            p -> right = l;
        }
    }
    l -> par = p;
}

void placeintree(node *r, node *n){
    if(r != NULL){
        if(n -> key < r -> key){
            if(r -> left != NULL){
                placeintree(r -> left, n);
                return;
            }
            else{
                r -> left = n;
            }
        }
        else{
            if(r -> right != NULL){
                placeintree(r -> right, n);
                return;
            }
            else{
                r -> right = n;
            }
        }
    }
    n -> par = r;
}

void repairtree(node *n){
    if(n -> par == NULL){
        n -> color = 1;
    }
    else if(n -> par -> color == 1){

    }
    else if(getun(n) != NULL && getun(n) -> color == 0){
        getpar(n) -> color = 1;
        getun(n) -> color = 1;
        getgpar(n) -> color = 0;
        repairtree(getgpar(n));
    }
    else{
        node *p = getpar(n);
        node *g = getgpar(n);
        if(n == p -> right && p == g -> left){
            lrotate(p);
            p = n;
            n = n -> left;
        }
        else if(n == p -> left && p == g -> right){
            rrotate(p);
            p = n;
            n = n -> right;
        }
        if(n == p -> left){
            rrotate(g);
        }
        else{
            lrotate(g);
        }
        p -> color = 1;
        g -> color = 0;
    }
}

node * insert(node *r, node *n){
    placeintree(r, n);
    repairtree(n);
    r = n;
    while(r -> par != NULL){
        r = r -> par;
    }
    return r;
}

bool search(node *r, int x){
    if(x == r -> key){
        return 1;
    }
    else if(x < r -> key){
        if(r -> left != NULL){
            return search(r -> left, x);
        }
        else{
            return 0;
        }
    }
    else{
        if(r -> right != NULL){
            return search(r -> right, x);
        }
        else{
            return 0;
        }
    }
}

void print(node *r, int x = 0){
    int t = 0;
    queue<node *> q, qh;
    q.push(r);
    while(!q.empty()){
        for(int i = 0; i < t; i++){
            cout<<"/\\ ";
        }
        cout<<endl;
        t = 0;
        while(!q.empty()){
            node *n = q.front();
            q.pop();
            if(n != NULL){
                t++;
                cout<<n -> key;
                if(n -> color == 0){
                    cout<<"R";
                }
                else{
                    cout<<"B";
                }
                if(n -> key == x){
                    cout<<"*";
                }
                cout<<" ";
                qh.push(n -> left);
                qh.push(n -> right);
            }
            else{
                cout<<"N ";
            }
        }
        cout<<endl;
        while(!qh.empty()){
            q.push(qh.front());
            qh.pop();
        }
    }
}
void print2(node *r, int x = 0){
    node *p[MAX_N];
    for(int i = 0; i < MAX_N; i++){
            p[i] = &fake;
    }
    bool next = 1;
    int i = 0;
    p[1] = r;
    while(next){
        next = 0;
        int t2 = pow(2, i);
        if(i != 0){
            for(int j = 0; j < pow(2, i); j++){
                if(p[t2 + j] != &fake){
                    if(j % 2 == 0){
                        cout<<"/ ";
                    }
                    else{
                        cout<<"\\ ";
                    }
                }
                else{
                    cout<<" ";
                }
            }
            cout<<endl;
        }
        for(int j = 0; j < pow(2, i); j++){
            if(p[t2 + j] != &fake){
                if(p[t2 + j] == NULL){
                    cout<<"N ";
                }
                else{
                    cout<<p[t2 + j] -> key;
                    if(p[t2 + j] -> color == 0){
                    cout<<"R";
                    }
                    else{
                        cout<<"B";
                    }
                    if(p[t2 + j] -> key == x){
                        cout<<"*";
                    }
                    cout<<" ";
                    p[2 * t2 + j * 2] = p[t2 + j] -> left;
                    p[2 * t2 + j * 2 + 1] = p[t2 + j] -> right;
                    next = 1;
                }
            }
            else{
                cout<<" ";
            }
        }
        cout<<endl;
        i++;
    }
}

int main(){



    //in barname delete ra handle nemikonad




    node * root = NULL;
    while(true){
        char c;
        int a;
        cin>>c;
        if(c == 'q'){
            break;
        }
        cin>>a;
        if(c == 'i'){
            root = insert(root, new node(a));
            print2(root);
        }
        else if(c == 's'){
            if(search(root, a)){
                print2(root, a);
            }
            else{
                cout<<"INVALID"<<endl;
            }
        }
    }
}
