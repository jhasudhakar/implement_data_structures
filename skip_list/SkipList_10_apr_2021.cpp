#include <limits.h>
#include<assert.h>
#include<vector>
#include <stdlib.h>
#include <iostream>
using namespace std;

struct node
{
	int val;
	struct node *next;
	struct node *down;
	node(int _val) { val = _val; next = down = NULL; }
	node (int _val, node* _next, node* _down)
	{
		val = _val;
		next = _next;
		down = _down;
	}
};

class SkipList
{
	private:
		const int H;
		const int MIN_INFINITY, MAX_INFINITY;
		struct node** slist;
	public:
		SkipList(const int _Height, const int MIN_VAL, const int MAX_VAL);
		int search(const int key, bool& is_found);
		void insert(const int key);
		void remove(const int key, bool& is_deleted);

		void unit_test();
};

SkipList::SkipList(const int _Height, const int MINVAL, const int MAXVAL)
	: H(_Height), MIN_INFINITY(MINVAL), MAX_INFINITY(MAXVAL)
{

	slist = new struct node* [H];
	for(int i = 0; i<H; i++)
	{
		slist[i] = new node(MIN_INFINITY);
		slist[i]->next = new node(MAX_INFINITY);

		if(i)
		{
			slist[i]->down = slist[i-1];
			slist[i]->next->down = slist[i-1]->next;
		}
	}
}

int SkipList::search(const int key, bool& is_found)
{
	if(key <= MIN_INFINITY || key >= MAX_INFINITY)
	{
		assert(false);
		is_found = false;
		return -1;
	}
	int height = H-1;

	vector<struct node*> level_path;
	struct node *prev = NULL, *cur = NULL;
	prev = cur = slist[H-1];
	while(height >0)
	{
		prev = cur = prev->down;
		while(cur->val <= key)
		{
			prev = cur;
			cur = cur->next;
		}
		level_path.push_back(prev);
		height--;	
	}
	if(prev->val == key)
	{
		is_found = true;
		return key;
	}
}

void SkipList::insert(const int key)
{
	if(key <= MIN_INFINITY || key >= MAX_INFINITY)
	{
		assert(false);
		return ;
	}
	int height = H-1;

	vector<struct node*> level_path;
	struct node *prev = NULL, *cur = NULL;
	prev = cur = slist[H-1];
	while(height >0)
	{
		prev = cur = prev->down;
		while(cur->val <= key)
		{
			prev = cur;
			cur = cur->next;
		}
		level_path.push_back(prev);
		height--;	
	}
	
	struct node* down_node = NULL;
	int prob = 1;
	while(level_path.size())
	{
		if(prob == 0)
			break;
		struct node* new_node = new struct node(key);
		prev = level_path[level_path.size()-1];
		new_node->next = prev->next;
		prev->next = new_node;
		new_node->down = down_node;

		down_node = new_node;
		level_path.pop_back();
		prob = random() % 2;
	}
}

void SkipList::remove(const int key, bool& is_deleted)
{
	is_deleted = false;
	if(key <= MIN_INFINITY || key >= MAX_INFINITY)
	{
		assert(false);
		return ;
	}
	int height = H-1;

	vector<struct node*> level_path;
	struct node *prev = NULL, *cur = NULL;
	prev = cur = slist[H-1];
	while(height >0)
	{
		prev = cur = prev->down;
		while(cur->val < key)
		{
			prev = cur;
			cur = cur->next;
		}
		level_path.push_back(prev);
		height--;	
	}
	
	while(level_path.size())
	{
		prev = level_path[level_path.size()-1];
		struct node* cur = prev->next;
		if(cur->val != key)
			break;

		is_deleted = true;
		prev->next = cur->next;
		delete cur;
		level_path.pop_back();
	}
}

void SkipList::unit_test()
{
	/*
	int search(const int key, bool& is_found);
	void insert(const int key);
	void remove(const int key, bool& is_deleted);
	*/
	unsigned int OP = 100;
	vector<int> vec;
	int val, index;
	bool is_deleted, is_found;
	const char* op_name [] = {"Insert", "Delete", "Search"};
	for(int i=0;i<OP; i++)
	{
		int code = random() % 3;
		cout << "\nRunning loop " << i+1 << " of " << OP <<": Operation = " << op_name[code]  << endl;

		switch (code)
		{
			case 0: /* insert*/
				val = random() % 1000;
				vec.push_back(val);
				this->insert(val);
				cout << " ==>  Inserting " << val << endl;
				break;

			case 1: /* delete */
				if(vec.size() == 0)
					continue;
				index = random() % (vec.size());
				val = vec[index];
				vec.erase(vec.begin() + index-1);
				is_deleted = false;
				this->remove(val, is_deleted);
				if(is_deleted == false)
					assert(false);
				cout << " ==>  Deleting " << val << endl;
				break;

			case 2:
				if(vec.size() == 0)
					continue;
				index = random() % (vec.size());
				val = vec[index];
				is_found = false;
				this->search(val, is_found);
				cout << " ==>  Searching  " << val << endl;
				if(is_found == false)
					assert(false);
				break;
			default:
				break;
		}
	}
}


int main()
{
	SkipList obj(10, INT_MIN/2, INT_MAX/2);
	obj.unit_test();
	return 0;
}
