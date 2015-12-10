#pragma once
#include<iostream>
template <typename valueType>

class Node
{
private:
	int ID;

public:
	valueType *value;
	Node<valueType>* next;
	Node<valueType>* prev;
	Node<valueType>(valueType *val);
	~Node<valueType>();
	//setters
	void setID(int x);
	//getters
	int getID();
};

template <typename valueType>
Node<valueType>::Node(valueType *val)
{
	//cout << "Node Constructor" << endl;
	this->value = val;
	this->next = (Node*)0;
	this->prev = (Node*)0;
}

template <typename valueType>
Node<valueType>::~Node()
{
	//cout << "Node Destrustor" << endl;
}

//setters
template <typename valueType>
void::Node<valueType>::setID(int x)
{
	ID = x;
}

//getters
template <typename valueType>
int::Node<valueType>::getID()
{
	return ID;
}




