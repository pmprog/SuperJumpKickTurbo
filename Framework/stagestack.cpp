
#include "stagestack.h"

StageStack::StageStack()
{
  StackIndex = -1;
}

int StageStack::Push(Stage* newStage)
{
  if( StackIndex == MAX_STACK_SIZE - 1 )
    return -1;

  // Pause any current stage
  if( StackIndex != -1 )
    this->Current()->Pause();

  StackIndex++;
  Stack[StackIndex] = newStage;
  Stack[StackIndex]->Begin();

  return 0;
}

Stage* StageStack::Pop()
{
  Stage* result;

  // Remove stage from stack
  result = this->Current();
  result->Finish();
  Stack[StackIndex] = 0;
  StackIndex--;

  // If there's still an item on the stack, resume it
  if( StackIndex != -1 )
    this->Current()->Resume();

  return result;
}

Stage* StageStack::Current()
{
  if( StackIndex == -1 )
    return 0;

  return Stack[StackIndex];
}

int StageStack::GetStackIndex()
{
  return StackIndex;
}

Stage* StageStack::Item(int index)
{
  return Stack[index];
}

Stage* StageStack::Previous( Stage* checkStage )
{
  if( StackIndex < 0 )
    return 0;

	for( int i = 0; i <= StackIndex; i++ )
	{
		if( Stack[i] == checkStage && i != 0 )
			return Stack[i-1];
	}

  return 0;
}

bool StageStack::IsEmpty()
{
	return (StackIndex < 0);
}
