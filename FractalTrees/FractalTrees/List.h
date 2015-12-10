#pragma once
#include"Node.h"
using namespace std;
template <typename listType>

class List
{
private:
	int length;
public:
	Node<listType> *head;
	Node<listType> *tail;
	List();
	~List();
	void insert(Node<listType>* prevNode, Node<listType>* newNode);
	void display();
	void displayEdges();
	Node<listType>* getNode(int x);
	Node<listType>* getNodeFromID(int ID);
	void deleteNode(int nodeNum);
	int getLength();
};

template <typename listType>
List<listType>::List()
{
	//cout << "List Constructor" << endl;
	this->head = 0;
	this->tail = 0;
	length = 0;
}

template <typename listType>
List<listType>::~List()
{
	//cout << "List destructor" << endl;
	//delete nodes
}

template <typename listType>
int::List<listType>::getLength()
{
	return length;
}

template <typename listType>
void::List<listType>::insert(Node<listType>* prevNode, Node<listType>* newNode)
{
	//prevNode is the node before where you will insert the new node, newNode is the node to add

	//if n is 0 then it the beginning of the list 
	//so n != 0 is therefore in the middle of the list
	if (prevNode != 0)
	{
		//set newNodes "next node" to the prevNodes "next node"
		//set prevNodes "next node" to newNode
		//set newNodes "previous node" to prevNode
		newNode->next = prevNode->next;
		prevNode->next = newNode;
		newNode->prev = prevNode;
		if (newNode->next != 0)
			//set the prevNodes old "nextNode"s "prevNode" to the newNode
			newNode->next->prev = newNode;
	}
	//if inserting the first item in the list
	if (this->head == 0)
	{
		//cout << "ADDING FIRST NODE" << endl;
		this->head = newNode;
		this->tail = newNode;
		newNode->next = 0;
		newNode->prev = 0;
	}
	//if not then set the tail to the newNode
	else if (this->tail == prevNode)
	{
		this->tail = newNode;
	}
	length += 1;
}


template <typename listType>
Node<listType>*::List<listType>::getNode(int nodeNum)
{
	Node<listType>* node;
	node = this->head;
	if (nodeNum != 0)
	{
		for (int i = 0; i != nodeNum; i++)
		{
			node = node->next;
		}
	}
	return node;
}

template <typename listType>
Node<listType>*::List<listType>::getNodeFromID(int ID)
{
	if (this->head == NULL)
		return NULL;
	//cout << "GETTING NODE" << endl;
	Node<listType>* node;
	node = this->head;
	if (node->getID() == 0 && ID == 0)
	{
		//cout << "RETURNING NODE WITH ID 0" << endl;
		return node;
	}
	if (node->getID() != ID)
	{
		//cout << "NODE NOT NODE 0" << endl;
		int i = 1;
		while (i != length)
		{

			//cout << "LOOP PASS: " << i << endl;
			if (node->getID() == ID)
			{
				//cout << "RETURNING NODE" << endl;
				return node;
			}
			node = node->next;
			i++;

		}
		//cout << "RETURNING NULL" << endl;
		//return NULL;
	}
	if (node->getID() == ID)
		return node;
	//cout << "RETURNING NODE 0" << endl;
	return NULL;
}

template <typename listType>
void::List<listType>::deleteNode(int nodeNum)
{
	Node<listType>* delNode = getNodeFromID(nodeNum);
	//cout << "DELETING NODE: " << nodeNum << " WITH ID: " << delNode->getBrickID() << endl;
	//3 cases, first node, last node, middle node
	//if first node
	length -= 1;
	if (length > 0)
	{
		if (delNode->prev == 0)
		{
			//cout << "this is the head" << endl;
			this->head = delNode->next;
			delNode->next->prev = 0;
			delete delNode;
		}
		//if last node
		else if (delNode->next == 0)
		{
			//cout << "this is the tail" << endl;
			this->tail = delNode->prev;
			delNode->prev->next = 0;
			delete delNode;
		}
		//if middle node
		else
		{
			//cout << "this is the middle" << endl;
			delNode->next->prev = delNode->prev;
			delNode->prev->next = delNode->next;
			delete delNode;
			//cout << "node deleted" << endl;
		}
	}

}

template <typename listType>
void::List<listType>::display()
{
	Node<listType>* i = this->head;
	cout << "List: ";
	while (i != 0)
	{
		cout << i->value << ",";
		i = i->next;
	}
	cout << endl;
}

template <typename listType>
void::List<listType>::displayEdges()
{
	Node<listType>* i = this->head;
	cout << "List: ";
	while (i != 0)
	{
		cout << "{" << i->A << "," << i->B << "}" << ",";
		i = i->next;
	}
	cout << endl;
}