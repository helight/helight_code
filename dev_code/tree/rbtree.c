#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#define RED 0
#define BLACK 1
#define NUM 20
#define MAX_NUM 300000

typedef struct RBTNode
{
		int key;
		struct RBTNode *parent,*left,*right;
		int color;
		int size;//扩张红黑树
}RBTNode,*RBTree;

RBTree nil,root;

int printnode=0;/*记录结点的高度,在PrintRBTree函数里使用*/

void InitLeafNode();/*构造叶子结点*/
RBTree CreateNode(int key,int color);/*创建一个新的树结点*/
RBTree TreeMinimum(RBTree x);/*找出树种最小的key并返回key的指针*/
RBTree TreeSuccessor(RBTree x);/*找出中序遍历顺序下它的后继并返回它的指针*/
void TreeInsert(RBTree &T, RBTree z);/*二叉查找树的插入操作*/
void LeftRotate(RBTree &T, RBTree x);/*左旋*/
void RightRotate(RBTree &T, RBTree x);/*右旋*/
void RBTreeInsert(RBTree &T, RBTree z);/*红黑树的插入操作*/
void RBTreeInsertFixup(RBTree &T, RBTree z);/*红黑树插入后调整颜色达到红黑树*/
void RBTreeDelete(RBTree &T, RBTree z);/*红黑树树的删除操作*/
void RBDeleteFixup(RBTree &T, RBTree z);/*红黑树删除后调整颜色达到红黑树*/
void PrintRBTree(RBTree T);/*中序打印树结点*/
/*查找树中与key值相同的结点,返回该结点的指针*/
RBTree TreeNodeSearch(RBTree T,int key,int &outcome);
/*红黑树的黑高度*/
int BlackHeight(RBTree T);
/*计算查询key=15000所用的时间*/
void PrintSearchTime(RBTree T,int times,int outcome,int rb);


void InitLeafNode()//构造叶子结点		
{
		nil = (RBTree)malloc(sizeof(RBTree));
		//root = (RBTree)malloc(sizeof(RBTree));
		nil->color=BLACK;
		nil->left=NULL;
		nil->right=NULL;
		nil->key=-1;
		nil->size=0;//红黑树扩张
		root=nil;
}
RBTree CreateNode(int key,int color)
{
		RBTree x = (RBTNode *)malloc(sizeof(RBTNode));
		x->color = color;
		x->key = key;
		x->left = nil;
		x->right = nil;
		x->parent = NULL;
		x->size=1;//红黑树扩张
		return x;
}
RBTree TreeMinimum(RBTree x)
{
		while(x->left != nil) {
				x = x->left;
		}
		return x;
}
RBTree TreeSuccessor(RBTree x)
{
		RBTree y;
		if (x->right != nil) {
				return TreeMinimum(x->right);
		}
		y = x->parent;
		while (y != nil && x == y->right) {
				x = y;
				y = y->parent;
		}
		return y;
}
void TreeInsert(RBTree &T, RBTree z)
{
		RBTree x,y;
		y = nil;
		x = root;
		while(x != nil) {
				y = x;
				if(z->key < x->key)	{
						x = x->left;
				} else {
						x = x->right;
				}
		}
		z->parent = y;
		if(y == nil) {
				root = z;
		} else {
				if(z->key < y->key) {
						y->left = z;
				} else {
						y->right = z;
				}
		}
		//红黑树扩张,设置size的值
		while(z != root) {
				z->parent->size = z->parent->right->size +
						z->parent->left->size + 1;
				z = z->parent;
		}
}
void LeftRotate(RBTree &T, RBTree x)
{
		RBTree y;
		y = x->right;
		x->right = y->left;
		if(y->left != nil) {
				y->left->parent = x;
		}
		y->parent = x->parent;
		if(x->parent == nil) {
				root = y;
		} else if(x == x->parent->left) {
				x->parent->left = y;
		} else {
				x->parent->right = y;
		}
		y->left = x;
		x->parent = y;
		//红黑树扩张,设置size值
		y->size = x->size;
		x->size = x->right->size + x->left->size + 1;
}
void RightRotate(RBTree &T, RBTree x)
{
		RBTree y;
		y = x->left;
		x->left = y->right;
		if(y->right != nil) {
				y->right->parent = x;
		}
		y->parent = x->parent;
		if(x->parent == nil) {
				root = y;
		} else if(x == x->parent->left) {
				x->parent->left = y;
		} else {
				x->parent->right = y;
		}
		y->right = x;
		x->parent = y;
		//红黑树扩张,设置size值
		y->size = x->size;
		x->size = x->right->size + x->left->size + 1;
}
void RBTreeInsert(RBTree &T, RBTree z)
{
		TreeInsert(T,z);
		z->left = nil;
		z->right = nil;
		z->color = RED;
		RBTreeInsertFixup(T,z);
}
void RBTreeInsertFixup(RBTree &T, RBTree z)
{
		RBTree y;
		while(z->parent->color == RED) {
				if(z->parent == z->parent->parent->left) {
						y = z->parent->parent->right;
						/*case 1*/
						if(y->color == RED) {
								z->parent->color = BLACK;
								y->color = BLACK;
								z->parent->parent->color = RED;
								z = z->parent->parent;
						} else {/*case 2*/ 
								if(z == z->parent->right) {
										z = z->parent;
										LeftRotate(T,z);
								}
								/*case 3*/
								z->parent->color = BLACK;
								z->parent->parent->color = RED;
								RightRotate(T,z->parent->parent);
						}
				} else {
						y = z->parent->parent->left;
						/*case 1*/
						if(y->color == RED) {
								z->parent->color = BLACK;
								y->color = BLACK;
								z->parent->parent->color = RED;
								z = z->parent->parent;
						} else {/*case 2*/ 
								if(z == z->parent->left) {
										z = z->parent;
										RightRotate(T,z);
								}
								/*case 3*/
								z->parent->color = BLACK;
								z->parent->parent->color = RED;
								LeftRotate(T,z->parent->parent);
						}
				}
		}
		root->color = BLACK;
}
void RBTreeDelete(RBTree &T, RBTree z)
{
		RBTree x,y;
		if(z->left == nil || z->right == nil) {
				y = z;
		} else {
				y = TreeSuccessor(z);
		} 
		if(y->left == nil) {
				x = y->left;
		} else {
				x = y->right;
		}
		x->parent = y->parent;
		if(y->parent == nil) {
				root = x;
		} else {
				if(y == y->parent->left) {
						y->parent->left = x;
				} else {
						y->parent->right = x;
				}
		}
		if(y != z)
				z->key = y->key;
		if(y->color == BLACK) {
				RBDeleteFixup(T,x);
		}
		free(y);
}
void RBDeleteFixup(RBTree &T,RBTree x)
{
		RBTree w;
		while(x != root && x->color == BLACK) {
				if(x == x->parent->left) {
						w = x->parent->right;
						if(w->color == RED) {/*case 1*/
								w->color = BLACK;
								x->parent->color = RED;
								LeftRotate(T,x->parent);
								w = x->parent->right;
						}
						if(w->left->color == BLACK && w->right->color == BLACK) { /*case 2*/
								w->color = RED;
								x = x->parent;
						} else {
								if(w->right->color == BLACK) {/*case 3*/
										w->left->color = BLACK;
										w->color = RED;
										RightRotate(T,w);
										w = x->parent->right;
								}						/*case 4*/
								w->color = x->parent->color;
								x->parent->color = BLACK;
								w->right->color = BLACK;
								LeftRotate(T,x->parent);
								x = root;
						}
				} else {
						w = x->parent->left;
						if(w->color == RED) {/*case 1*/
								w->color = BLACK;
								w->parent->color = RED;
								RightRotate(T,x->parent);
								w = x->parent->left;
						}
						if(w->left->color == BLACK && w->right->color == BLACK) { /*case 2*/
								w->color = RED;
								x = x->parent;
						} else {
								if(w->left->color == BLACK) {/*case 3*/
										w->right->color = BLACK;
										w->color = RED;
										LeftRotate(T,w);
										w = x->parent->left;
								}
								/*case 4*/
								w->color = x->parent->color;
								x->parent->color = BLACK;
								w->left->color = BLACK;
								RightRotate(T,x->parent);
								x = root;
						}
				}
		}/*while*/
}
RBTree TreeNodeSearch(RBTree T,int key,int &outcome)
{
		RBTree find = T;
		while(find != nil ) {
				if( key == find->key ) {
						outcome = 1;
						break;
				} else if(key < find->key) {
						find = find->left;
				} else {
						find = find->right;
				}
		}
		if(find == nil)
				outcome = 0;
		return find;
}
void PrintRBTree(RBTree T)
{
		if(T != nil) {
				for(int i = 0; i <= printnode;i++) {
						printf(" ");
				}
				printf("(%d",T->key);
				if(T->color == BLACK) {
						printf("B,\n");
				} else {
						printf("R,\n");
				}
				printnode++;
				PrintRBTree(T->left);
				PrintRBTree(T->right);
				printnode--;
				for(int j = 0; j <= printnode;j++) {
						printf(" ");
				}
				printf("),\n");
		} else {
				for(int i = 0; i <= printnode;i++) {
						printf(" ");
				}
				printf("Nil,\n");
		}
}
int BlackHeight(RBTree T)
{
		RBTree t = T->left;
		int height = 0;
		while(t != NULL) {
				if(t->color == BLACK)
						height++;
				t = t->left;
		}
		return height;
}
void PrintSearchTime(RBTree T,int times,int outcome,int rb)
{
		RBTree find;
		clock_t start, end;
		double duration;
		start = clock();
		/*需要计算时间的代码*/
		for(int i = 0; i < times; i++)
				find = T;
		end = clock();
		printf("start:%f\n",(double)start);printf("end:%f\n",(double)end);
		/*得到的数值是这段代码的执行时间,以秒为单位CLOCKS_PER_SEC=1000*/
		duration = (double)(end-start)/CLOCKS_PER_SEC;
		double average = duration/times;
		char *s;
		if(rb)
				s = "红黑树";
		else
				s = "二叉查找树";
		if(find != nil && outcome != 0) {
				printf("==============%s查找key=15000成功================\n",s)
						;
		} else {
				printf("==============%s查找key=15000不成功==============\n",s)
						;
		}
		printf("%s中查找%d次所用时间:%f\n",s,times,duration);
		printf("%s中查找%d次的平均时间:%f\n\n",s,times,duration);
}
/*递归确定一个元素的秩*/
int OS_KEY_RANK(RBTree T,int key)
{
		if(T != nil) {
				if(T->key > key) {
						return OS_KEY_RANK(T->left,key);
				} else if(T->key < key) {
						return OS_KEY_RANK(T->right,key) + T->left->size + 1;
				} else
						return T->left->size + 1;
		}
		return -100000;//如果没有找到相同的key值,返回一个负数
}
int main()
{
		RBTree node[NUM],find;
		int outcome = 0;//记录查询的结果,成功:1,失败:0
		int num = 0;//记录红黑树的结点数
		int height = 0;//记录树的黑高度
		InitLeafNode();
		printf("请输入你要建立红黑树的结点数目: ");
		scanf("%d", &num);
		int key = 0;
		printf("请依次输入%d个结点的值:",num);
		for(int i = 0; i < num; i++) {
				scanf("%d",&key);
				node[i] = CreateNode(key,RED);
				RBTreeInsert(root,node[i]);
		}
		/*输出插入结点后树的模型*/
		printf("**********************插入结点后树的模型**********************\n");
		PrintRBTree(root);
		height=BlackHeight(root);
		printf("**********************\n");
		printf("此红黑树的黑高度是: %d\n",height);
		/*输出删除key=15结点后树的模型*/
		printf("**********************删除key=15结点后树的模型**********************\n");
		find = TreeNodeSearch(root,15,outcome);
		if(find != nil && outcome != 0) {
				RBTreeDelete(root,find);
				PrintRBTree(root);
				printf("==============================\n");
				height=BlackHeight(root);
				printf("此红黑树的黑高度是: %d\n",height);
				printf("==============================\n");
		} else {
				printf("================查找不成功================\n\n");
		}
		static RBTree rb_randnode[MAX_NUM];//一定要用static,或者定义为全局变量(存放在全局静态区)
		srand( (unsigned)time( NULL ) );
		/*建立300000个结点的红黑二叉树*/
		free(root);
		InitLeafNode();
		for(int j = 0; j < MAX_NUM; j++) {
				key = rand()%MAX_NUM + 1;
				rb_randnode[j]=CreateNode(key,RED);
				RBTreeInsert(root,rb_randnode[j]);
		}
		int times=0;
		printf("请输入你要查询key关键字的次数:");
		scanf("%d",&times);
		PrintSearchTime(TreeNodeSearch(root,15000,outcome),times,outcome,
						1);
		/*建立300000个结点的二叉查找树*/
		free(root);
		InitLeafNode();
		static RBTree randnode[MAX_NUM];
		for(int m = 0; m < MAX_NUM; m++) {
				key = rand()%MAX_NUM + 1;
				randnode[m]=CreateNode(key,RED);
				TreeInsert(root,randnode[m]);
		}
		PrintSearchTime(TreeNodeSearch(root,15000,outcome),times,outcome,
						0);
		static RBTree os_key[NUM];//一定要用static,或者定义为全局变量(存放在全局静态区)
		/*建立扩张红黑树*/
		free(root);
		InitLeafNode();
		int num2=0;
		printf("请输入你要建立红黑树的结点数目: ");
		scanf("%d",&num2);
		int key2=0;
		printf("请依次输入%d个结点的值:",num2);
		for(int k = 0; k < num2; k++) {
				scanf("%d",&key2);
				node[k]=CreateNode(key2,RED);
				RBTreeInsert(root,node[k]);
		}
		int t=0,os=0;
		printf("请输入查找的key值: ");
		scanf("%d",&os);
		t=OS_KEY_RANK(root,os);
		PrintRBTree(root);
		if(t <= 0) {
				printf("key=%d在红黑树中找不到!\n",os);
		} else
				printf("key=%d在红黑树中的秩为:%d \n",os,t);
		return 1;
}
