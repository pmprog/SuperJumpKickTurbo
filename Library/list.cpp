
/****************************************
| Simple C-based Linked List
|
| Code by Marq Watkin
|   Polymath Programming
|   http://www.pmprog.co.uk
| 
| Entered into the public domain - Feel free to do what you like with this
|
*/

#include <stdlib.h>
#include "list.h"

List::List()
{
	first = 0;
	last = 0;
	count = 0;
}

void List::AddToStart( void* newItem )
{
	ListItem* itm = (ListItem*)malloc( sizeof( ListItem ) );
	itm->nextItem = first;
	itm->previousItem = 0;
	itm->itemData = newItem;
	if( first != 0 )
		first->previousItem = itm;
	first = itm;
	if( last == 0 )
		last = first;
	count++;
}

void List::AddToEnd( void* newItem )
{
	ListItem* itm = (ListItem*)malloc( sizeof( ListItem ) );
	itm->previousItem = last;
	itm->nextItem = 0;
	itm->itemData = newItem;
	if( last != 0 )
		last->nextItem = itm;
	last = itm;
	if( first == 0 )
		first = last;
	count++;
}

void* List::ItemAt( int index )
{
	int curItemIdx = 0;
	ListItem* curItem = first;
	while( curItem != 0 && curItemIdx < index )
	{
		curItem = curItem->nextItem;
		curItemIdx++;

	}
	return curItem->itemData;
}

void List::RemoveFirst()
{
	ListItem* temp = first;
	if( first == last )
	{
		first = 0;
		last = 0;
	} else {
		temp->nextItem->previousItem = 0;
		first = temp->nextItem;
	}
	free( (void*)temp );
	count--;
}

void List::RemoveLast()
{
	ListItem* temp = last;
	if( first == last )
	{
		first = 0;
		last = 0;
	} else {
		temp->previousItem->nextItem = 0;
		last = temp->previousItem;
	}
	free( (void*)temp );
	count--;
}

void List::RemoveAt( int index )
{
	if( index == 0 )
		RemoveFirst();
	else if( index == count - 1 )
		RemoveLast();
	else {
		int curItemIdx = 0;
		ListItem* curItem = first;
		while( curItem != 0 && curItemIdx < index )
		{
			curItem = curItem->nextItem;
			curItemIdx++;

		}
		curItem->previousItem->nextItem = curItem->nextItem;
		curItem->nextItem->previousItem = curItem->previousItem;
		free( (void*)curItem );
		count--;
	}
}
