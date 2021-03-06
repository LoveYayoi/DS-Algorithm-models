#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <algorithm>
#include <string>
#include <cstring>
#include <cmath>
#include <map>
#include <stack>
#include <set>
#include <vector>
#include <queue>
#define eps 10e-7
#define INF 0x3f3f3f3f
#define MOD 1000000007
#define rep0(j,n) for(int j=0;j<n;++j)
#define rep1(j,n) for(int j=1;j<=n;++j)
#define pb push_back
#define set0(n) memset(n,0,sizeof(n))
#define ll long long
#define OJ codevs
using namespace std;
int read(){
    char c=getchar();int f=1,x=0;
    while(!isdigit(c)){if(c=='-') f=-1;c=getchar();}
    while(isdigit(c)){x=x*10+c-'0';c=getchar();}
    return f*x;
}
char get_ch(){
    char c=getchar();
    while(!isalpha(c)) c=getchar();
    return c;
}
const int MAXINT = 200010;
struct node{
    int size;
    bool isReverse;
    node *child[2],*fa;
    int checkfa(){
        if(this==fa->child[0]) return 0;
        if(this==fa->child[1]) return 1;
        return -1;
    }
    void pushup(){
        size=child[0]->size+child[1]->size+1;
        //calculate something else like max or min
    }
    void pushdown(){
        if(checkfa()!=-1) fa->pushdown();
        //do something like lazy tags
        if(isReverse){
            swap(child[0],child[1]);
            child[0]->isReverse^=1;
            child[1]->isReverse^=1;
            isReverse=0;
        }
    }
    node(){}
};

struct LCT{
    node mempool[MAXINT];
    int cnt;
    node nil,*NIL;
    LCT(){
        cnt=0;
        NIL=&nil;
        NIL->size=0;
        NIL->child[0]=NIL->child[1]=NIL->fa=NIL;
        rep0(i,MAXINT){
            mempool[i].fa=mempool[i].child[0]=mempool[i].child[1]=NIL;
            mempool[i].size=1;
        }
    }
    node *newnode(node *fa){
        node *now=&mempool[cnt++];
        now->fa=fa;
        now->child[0]=now->child[1]=NIL;
        now->pushup();
        return now;
    }
    void rotate(node *p,int d){//0 for left,1 for right
        node *pfa=p->fa;
        p->fa=pfa->fa;
        pfa->child[d^1]=p->child[d];
        pfa->child[d^1]->fa=pfa;
        p->child[d]=pfa;
        if(p->fa->child[0]==pfa) p->fa->child[0]=p;
        if(p->fa->child[1]==pfa) p->fa->child[1]=p;
        pfa->fa=p;
        //exchange children&father
        pfa->pushup();
        //pushup information
    }
    void splay(node *p){ //splay to the root of its auxiliary tree
        p->pushdown();
        while(p->checkfa()!=-1){
            if(p->checkfa()==1){
                if(p->fa->checkfa()==1)rotate(p->fa,0);
                rotate(p,0);
            }else{
                if(p->fa->checkfa()==0)rotate(p->fa,1);
                rotate(p,1);
            }
        }
        p->pushup();
    }
    void access(node *p){
        node *tmp=NIL;
        while(p!=NIL){
            splay(p);
            p->child[1]=tmp;
            p->pushup();
            tmp=p,p=p->fa;//跳到上一棵splay
        }
    }//真TMD简单
    void makeroot(node *p){
        access(p);
        splay(p);
        p->isReverse=1;
    }//?????
    node *find_root(node *p){
        while(p->fa!=NIL) p=p->fa;
        return p;
    }
    void link_edge(node *p,node *pfa){
        makeroot(p);
        p->fa=pfa;
    }
    void cut_edge(node *p,node *pfa){
        makeroot(p);
        access(pfa);
        splay(pfa);
        p->fa=NIL;
        pfa->child[0]=NIL;
        pfa->pushup();
    }
    void modify(node *x,node *y){//对x到y的链进行修改
        makeroot(x);
        access(y);
        splay(y);
        //do something to node y
    }
}lct;
int b[MAXINT],n,q,u,v;
node* nd[MAXINT];
int op;
int main() {
    #ifndef OJ
    freopen("2333.in","r",stdin);
    freopen("2333.out","w",stdout);
    #endif // OJ
    n=read();
    rep0(i,n+1) nd[i]=&lct.mempool[i];
    rep0(i,n) b[i]=read();
    rep0(i,n){
        lct.link_edge(nd[i],nd[n]);
    }
    rep0(i,n){
        if(b[i]&&i+b[i]<n){
            lct.cut_edge(nd[i],nd[n]);
            lct.link_edge(nd[i],nd[i+b[i]]);
        }
    }
    q=read();
    while(q--){
        op=read();
        if(op==1){
            u=read();
            lct.access(nd[u]);
            lct.splay(nd[u]);
            printf("%d\n",nd[u]->child[0]->size);
        }
        if(op==2){
            u=read();v=read();
            if(b[u]+u<n) lct.cut_edge(nd[u],nd[u+b[u]]); else lct.cut_edge(nd[u],nd[n]);
            b[u]=v;
            if(v+u<n) lct.link_edge(nd[u],nd[u+v]); else lct.link_edge(nd[u],nd[n]);
        }
        lct.makeroot(nd[n]);
    }
    return 0;
}

