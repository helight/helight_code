#ifndef _BTREE_H 
#define _BTREE_H

#define MINDEGREE 3 // 定义BTree的最小度
#define MAXDEGREE (MINDEGREE*2)

// 定义BTree的数据结构
typedef void* NodeData;

typedef struct _targBTreeNode
{
	NodeData	data; // 
	_targBTreeNode*	cs[MAXDEGREE]; //孩子指针数组
	int	keys[MAXDEGREE-1]; //关键字数组
	int count; // 关键字个数
	int is_leaf; // 是否为叶子结点
} BTreeNode, *BTree;

#define BTREE_SIZE sizeof(BTreeNode)

void alloc_tree(BTree &tree); //分配内存 
void insert_keys_to_tree(BTree &tree, int keys[], int n); //将keys插入至tree中
void insert_key_to_tree(BTree &tree, int key); // 将key插入至根树tree中
void insert_key_to_unfull_tree(BTree &tree, int key); // 将key关键字插入至未满子树tree中 
void break_tree_child(BTree &tree, BTree &child, int i); // 将tree的第i个子孩子分裂
void disk_read(const BTree tree); // 代表读磁盘，未实现
void disk_write(const BTree tree); // 代表写磁盘，未实现
void display_tree(const BTree tree); // 显示BTree
int search_tree(const BTree tree, int key); // 搜索BTree中是否存在关键字
void delete_tree(BTree &tree, int key); // 删除BTree中的某一关键字，主要处理BTree为空的两种情况~
void delete_unless_tree(BTree &tree, int key); // 删除BTree中的某一子树~
int pre_succor_tree(const BTree tree, int i); // 找tree中第i位关键字的前驱~
int after_succor_tree(const BTree tree, int i); // 找Btree中第i位关键字的后继~

#endif
